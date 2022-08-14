﻿// This examples shows how to jit cwerg ir to a64 assembly

#include "Base/ir.h"
#include "CodeGenX64/codegen.h"
#include "CodeGenX64/legalize.h"
#include "CpuX64/symbolic.h"
#include "Util/assert.h"
#include "Util/common.h"

#include <iostream>

using namespace cwerg;

#include "Examples/fib_ir.h"


using FunPtr = uint32_t (*)(uint32_t);

int main(int argc, char* argv[]) {
  // Set-up internal memory pools.
  InitStripes(1);
  code_gen_x64::InitCodeGenX64();

  // Generate IR and translate to A64
  std::ostream myout(std::cout.rdbuf());

  base::Unit unit = MakeFibonacci();
  code_gen_x64::LegalizeAll(unit, false, &myout);
  code_gen_x64::RegAllocGlobal(unit, false, &myout);
  code_gen_x64::RegAllocLocal(unit, false, &myout);
  x64::X64Unit x64unit = code_gen_x64::EmitUnitAsBinary(unit);
  std::cerr << std::hex;
  std::cerr
            << "RODATA SIZE: 0x" << x64unit.sec_rodata->data->size() << "\n"
            << "TEXT SIZE:   0x" << x64unit.sec_text->data->size() << "\n"
            << "DATA SIZE:   0x" << x64unit.sec_data->data->size() << "\n"
            << "BSS SIZE:    0x" << x64unit.sec_bss->data->size() << "\n";

  // Allocate space for Section data
  void* ctx[1];
  uint32_t* text_memory = (uint32_t*)p_mmap(ctx, 4096);

  ASSERT(text_memory != nullptr, "");
  x64unit.sec_text->shdr.sh_addr = (uint64_t)text_memory;
  std::cerr << "text addr: " << text_memory << "\n";

  // Compute Symbol Addresses and Relocate
  for (auto& sym : x64unit.symbols) {
    ASSERT(sym->sym.st_value != ~0U, "undefined symbol " << sym->name);
    if (sym->section != nullptr) {
      ASSERT(sym->section->shdr.sh_addr != ~0U,
             sym->name << "has bad sec " << *sym->section);
      sym->sym.st_value += sym->section->shdr.sh_addr;
      std::cerr << "sym [" << sym->name << "]: 0x" <<   sym->sym.st_value << "\n";
    }
  }

  for (auto& rel : x64unit.relocations) {
    x64::ApplyRelocation(rel);
  }

  // Copy Section data to their final destination
  // Note we only have a text section
  memcpy(text_memory, x64unit.sec_text->data->data(), x64unit.sec_text->data->size());

  // Invoke JITed code. This assumes that the calling convention of the
  // JITed code matches the platform ABI. This is only true for simple cases.
  FunPtr f = reinterpret_cast<FunPtr>(text_memory);
  for (unsigned i = 0; i < 10; ++i) {
    std::cout << std::dec << i << " " << f(i) << "\n";
  }
  p_ummap(ctx, text_memory);
  std::cout << " -- end --" << std::endl;
  return 0;
}
