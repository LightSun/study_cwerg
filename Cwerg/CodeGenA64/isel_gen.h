#pragma once
// NOTE: this file is PARTIALLY autogenerated via: ./isel_tab.py gen_h
// (c) Robert Muth - see LICENSE for more info

#include <cstdint>

#include "Base/opcode_gen.h"
#include "Base/ir.h"
#include "CpuA64/opcode_gen.h"
#include "CodeGenA64/regs.h"

namespace cwerg::code_gen_a64 {
/* @AUTOGEN-START@ */

enum class IMM_CURB : uint8_t {
    INVALID = 0,
    ZERO = 1,
    ANY = 2,
    IMM_SHIFTED_10_21_22 = 3,
    IMM_10_15_16_22_W = 4,
    IMM_10_15_16_22_X = 5,
    IMM_SHIFTED_5_20_21_22 = 6,
    IMM_SHIFTED_5_20_21_22_NOT = 7,
    pos_stk_combo_shifted_10_21_22 = 8,
    pos_stk_combo_16_bits = 9,
    pos_stk_combo_32_bits = 10,
    pos_stk_combo_10_21 = 11,
    pos_stk_combo_10_21_times_2 = 12,
    pos_stk_combo_10_21_times_4 = 13,
    pos_stk_combo_10_21_times_8 = 14,
    IMM_POS_32 = 15,
};

enum class PARAM : uint8_t {
    invalid = 0,
    reg0 = 1,
    reg1 = 2,
    reg2 = 3,
    reg3 = 4,
    reg4 = 5,
    num0 = 6,
    num1 = 7,
    num2 = 8,
    num3 = 9,
    num4 = 10,
    num0_neg = 11,
    num1_neg = 12,
    num2_neg = 13,
    num3_neg = 14,
    num4_neg = 15,
    num0_not = 16,
    num1_not = 17,
    num2_not = 18,
    bbl0 = 19,
    bbl2 = 20,
    fun0 = 21,
    scratch_gpr = 22,
    scratch_flt = 23,
    num2_rsb_width = 24,
    num2_rsb_width_minus1 = 25,
    mem1_num2_prel_hi21 = 26,
    mem1_num2_lo12 = 27,
    stk1_offset2 = 28,
    stk1_offset2_lo = 29,
    stk1_offset2_hi = 30,
    stk0_offset1 = 31,
    stk0_offset1_lo = 32,
    stk0_offset1_hi = 33,
    fun1_prel_hi21 = 34,
    fun1_lo12 = 35,
    jtb1_prel_hi21 = 36,
    jtb1_lo12 = 37,
    num1_0_16 = 38,
    num1_16_32 = 39,
    num1_32_48 = 40,
    num1_48_64 = 41,
    frame_size = 42,
};
/* @AUTOGEN-END@ */


struct InsTmpl {
  // signed because some operands need that (e.g. offsets)
  // the values need to be processed with EncodeOperand
  int64_t operands[a64::MAX_OPERANDS];
  a64::OPC opcode;
  // bit i set: operands[i] is of enum type Param
  // bit i clr: operands[i] contains a number
  uint16_t template_mask;
};

struct Pattern {
  union {
    base::DK type_curbs[8];  // 8 >= MAX_OPERANDS
    uint64_t type_curbs_as_int;
  };
  IMM_CURB imm_curbs[8];    // 8 >= MAX_OPERANDS
  const InsTmpl* start;
  uint8_t length;
};

extern const Pattern* FindMatchingPattern(base::Ins ins);

const constexpr uint8_t MATCH_IMPOSSIBLE = 255;

extern uint8_t FindtImmediateMismatchesInBestMatchPattern(base::Ins ins,
                                                          bool assume_stk_op_matches);

extern a64::Ins MakeIns(a64::OPC opc_enum,
                 int64_t x0 = 0,
                 int64_t x1 = 0,
                 int64_t x2 = 0,
                 int64_t x3 = 0,
                 int64_t x4 = 0);

extern a64::Ins MakeInsFromTmpl(const InsTmpl& tmpl,
                 base::Ins ins,
                 const EmitContext& ctx);

extern void EmitFunProlog(const EmitContext& ctx, std::vector<a64::Ins>* output);
extern void EmitFunEpilog(const EmitContext& ctx, std::vector<a64::Ins>* output);


extern const char* EnumToString(IMM_CURB f);
extern const char* EnumToString(PARAM f);

}  // namespace cwerg::code_gen_a32
