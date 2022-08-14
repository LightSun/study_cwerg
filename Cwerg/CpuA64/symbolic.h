#pragma once
// (c) Robert Muth - see LICENSE for more info
// NOTE: this file is PARTIALLY autogenerated via: ./opcode_tab.py gen_h

#include "CpuA64/opcode_gen.h"
#include "Elf/enum_gen.h"  // for reloc types

#include <cstdint>
#include <string_view>
#include <vector>

namespace cwerg::a64 {

extern std::string_view InsSymbolize(const a64::Ins& ins,
                                     std::vector<std::string>* ops);

extern bool InsFromSymbolized(const std::vector<std::string_view>& token, Ins* ins);


}  // namespace cwerg::a64
