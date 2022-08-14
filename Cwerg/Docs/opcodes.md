## Virtual ISA Description

<!--- NOTE: this file is PARTIALLY autogenerated via: Base/opcode_tab.py documentation --->

The instructions are modeled after a generic three register RISC ISA, 
with an infinite number of registers, e.g.
```
sub r0 r1 r2        # r0 = r1 - r2
sub r0 0x666 r2     # r0 = 0x666 - r2
sub r0 r1 0x666     # r0 = r1 - 0x666 
```

Many operands can be either registers or constants (subject to typing constraints).

The opcodes are overloaded and the type of the operands determines the actual behavior.
For example `div` can be used with signed and unsigned integers or even floating point 
operands.

Function calls are modelled with instruction sequences using 
`poparg` and `pusharg` instructions to pass parameters and return results.

The `cmpXX` opcode is an oddball accessing 5 registers.

Examples programs can be found in [../TestData](../TestData)

### Typing 

Registers and constants are typed. Where a type is a tuple of `flavor` and `width`.

Currently, the `width` is one of 8, 16, 32, 64 bits and the `flavor` is one of:
* **U** unsigned int (wrap around - no trap on overflow)
* **S** signed int (2's complement, wrap around - no trap on overflow)
* **F** floating point (ieee formats)
* **A** data address 
* **C** code address (only function but not bbl addresses are currently exposed)

Registers must be declared before use so that their type is known.
The declaration can happen in one of two ways: 
1. with  the `.reg` directive 
2. with a `colon-suffix` by the first instruction writing to it.

```
.reg F32 [a b c]   # declares registers a, b, c to be 32 bit wide and of flavor F
add a b c
```
is equivalent to
```
.reg F32 [b c]
add a:F32 b c
```

A constant must also be typed using the `colon-suffix` unless the type can be deduced
from the other operands which is usually the case.

```
.reg F32 [a b]
add a b 1.0:F32
```
is equivalent to 

```
.reg F32 [a b]
add a b 1.0
```

For constant that are stored (via `st`) or pushed (via `pusharg`) the types
cannot be deduced and must always be provided:

```
.reg A32 [a]
...
st a 16 = 1.0:f32   # stores a 32 bit wide floating point constant
st a 16 = 32:U8     # stores a 8 bit wide integer constant
st a 16 = 0:A64     # stores a 64 bit wide null pointer
push 0:C64          # pushes  a 64 bit wide code null pointer
```


* The type of operands must agree for most instructions
* The modified register, if any, is always the first operand.
* Target architectures may not support all width and types
* New types, e.g. for vector and SIMD operations, may be added in the future.


### Addressing Modes

Load and store instructions have one addressing mode: `base + offset`

The base is usually an address register but may also be a global memory
address or a location within the current stack frame.
The offset can be a register or a constant.

### Address Arithmetic

The `lea` (load effective address) family of opcodes allows for the 
addition of integers to addresses. This is only possible for data addresses
not code addresses.

Limited address arithmetic helps the optimizer to better reason about memory
conflicts. It can be worked around via bitcasts between 
integers of addresses of the same size.

There is no mechanism to define aggregate data types like C structs. All
size and offset computations have to occur in the frontend.

###  How to read the Opcode descriptions below

```
OPCODE OPERAND_1 [constraint_1]  ...  OPERAND_n [constraint_n] 
```

The `constraint` describes the kind of the operand as shown below:

| Operand Kind | Description |
| ----- | ---- |
| REG | Register |
| CONST | Constant |
| REG/CONST | Register or Constant |
| FUN | Function or Function Signature|
| BBL | Basic Block|
| JTB | Jump Table | 
| MEM | Global Memory Location |
| STK | Stack Location | 

For REG and CONST operands there is an additional  type constraint (seperated by a  ':'):

| Type Constraint | Description |
| ----- | ---- |
| ANY | no contraints |
| UINT |  flavor must be U |
| INT |  flavor must be one of S, U |
| NUM | flavor must be one of F, S, U |
| ADDR | flavor must be A |
| ADDR_NUM | flavor must be A, F, S, U |
| CODE | flavor must be C |
| OFFSET | TBD |
| SAME_AS_PREV | type must be same as previous REG or CONST | 


All kinds except for `CONST` must match the following regex: `[%_$a-zA-Z][%_$a-zA-Z0-9]*`

 `$` should be used for name-space management and 
 `%` to avoid name clashes with the original source language
   

<!--- @AUTOGEN-START@ --->
### Overview
| No | Format | Description |
| ---- | ---- | ---- |
| 0x10 | add dst src1 src2 | Addition: `dst := src1 + src2` |
| 0x11 | sub dst src1 src2 | Subtraction: `dst := src1 - src2` |
| 0x12 | mul dst src1 src2 | Multiplication: `dst := src1 \* src2` |
| 0x13 | div dst src1 src2 | Division: `dst := src1 / src2`  |
| 0x18 | xor dst src1 src2 | Bitwise exclusive or: `dst := src1 ^ src2` |
| 0x19 | and dst src1 src2 | Bitwise and: `dst := src1 & src2` |
| 0x1a | or dst src1 src2 | Bitwise or: `dst := src1 | src2` |
| 0x1b | shl dst src1 src2 | Shift left: `dst := src1 << src2` |
| 0x1c | shr dst src1 src2 | Shift right: `dst := src1 >> src2` |
| 0x1d | rem dst src1 src2 | Modulo: `dst := src1 % src2` |
| 0x1e | clmul dst src1 src2 | NYI: Carry-less multiplication |
| 0x20 | beq op1 op2 target_bbl | Conditional branch (if equal) |
| 0x21 | bne op1 op2 target_bbl | Conditional branch (if not equal) |
| 0x22 | blt op1 op2 target_bbl | Conditional branch (if greater than) |
| 0x23 | ble op1 op2 target_bbl | Conditional branch (if less or equal) |
| 0x28 | switch index table | Multi target computed jump |
| 0x29 | bra target_bbl | Unconditional branch |
| 0x2a | ret  | Return from fun |
| 0x2b | bsr target_fun | Call fun (branch to subroutine) |
| 0x2c | jsr target_fun_addr target_fun_sig | Call fun indirectly through reg (jump to subroutine) |
| 0x2d | syscall target_fun_sig syscall_no | Syscall to `syscall_no`  |
| 0x2e | trap  | Abort program |
| 0x30 | pusharg src | Push a call or return arg  |
| 0x31 | poparg dst | Pop a call or return arg  |
| 0x32 | conv dst src | Conversion of numerical regs |
| 0x33 | bitcast dst src | Cast between regs of same size |
| 0x34 | mov dst src | Move between registers |
| 0x35 | cmpeq dst src1 src2 cmp1 cmp2 | Conditional move (if equal): `dst := (cmp1 == cmp2) ? src1 : src2` |
| 0x36 | cmplt dst src1 src2 cmp1 cmp2 | Conditional move (if less than): `dst := (cmp1 < cmp2) ? src1 : src2`  |
| 0x38 | lea dst base offset | Load effective address: `dst  := base + offset`   |
| 0x39 | lea.mem dst base offset | Load effective mem address with offset: `dst := base + offset` |
| 0x3a | lea.stk dst base offset | Load effective stk address with offset: `dst := base + offset` |
| 0x3b | lea.fun dst base | Load effective fun address: `dst := base`  |
| 0x40 | ld dst base offset | Load from reg base with offset: `dst := RAM[base + offset]` |
| 0x41 | ld.mem dst base offset | Load from mem base with offset: `dst := RAM[base + offset]` |
| 0x42 | ld.stk dst base offset | Load from stk base with offset: `dst := RAM[base + offset]` |
| 0x44 | st base offset src | Store to reg base with offset: `RAM[base + offset] := src` |
| 0x45 | st.mem base offset src | Store to mem base with offset: `RAM[base + offset] := src` |
| 0x46 | st.stk base offset src | Store to stk base with offset: `RAM[base + offset] := src` |
| 0x48 | cas dst cmp src base offset | Atomic Compare and Swap   |
| 0x49 | cas.mem dst cmp src base offset | Atomic Compare and Swap   |
| 0x4a | cas.stk dst cmp src base offset | AtomicCompare and Swap   |
| 0x50 | ceil dst src | Round float to integral (toward positive infinity) |
| 0x51 | floor dst src | Round float to integral (toward negative infinity) |
| 0x52 | round dst src | Round float to integral (to nearest with ties to away) |
| 0x53 | trunc dst src |  |
| 0x54 | copysign dst src1 src2 | Set the sign of src1 to match src2 (floating point only) |
| 0x55 | sqrt dst src | Compute the sqrt of floating point value |
| 0x60 | cntlz dst src | Count leading zeros |
| 0x61 | cnttz dst src | Count trailing zeros |
| 0x62 | cntpop dst src | Count set bits (pop count) |
| 0x70 | nop  | nop - internal use |
| 0x71 | nop1 src_and_dst | nop with one reg - internal use |
| 0x77 | line file line | NYI: debug line number |
| 0x78 | inline target-asm-ins | Inject arbitrary target instructions bytes into instruction stream |
| 0x79 | getfp dst | Materialize the frame-pointer |
| 0x7a | getsp dst | Materialize the stack-pointer |
| 0x7b | gettp dst | Materialize the tls-pointer |
| 0x01 | .mem name alignment mem_kind | Add new mem region to unit |
| 0x02 | .data repeat data | Add content to current mem region: multiple bytes |
| 0x03 | .addr.fun width fun | Add content to current mem region: code address |
| 0x04 | .addr.mem width mem offset | Add content to current mem region: memory address with offset |
| 0x05 | .fun name fun_kind out_params in_params | Add new fun to unit |
| 0x06 | .bbl name | Add new bbl to current fun |
| 0x07 | .reg reg_kind names | Add new regs to current fun |
| 0x08 | .stk name alignment size | Add stk region to current fun |
| 0x09 | .jtb name size default_bbl map | Add jump table to current fun |
## Basic ALU

#### [10] add *dst* <sub>[REG:NUM]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Addition: `dst := src1 + src2`

#### [11] sub *dst* <sub>[REG:NUM]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Subtraction: `dst := src1 - src2`
             
             Note: `sub dst = 0 src` can be used to emulate `neg` for integers.
                    (for floating point use `dat = mul src -1.0`)
             

#### [12] mul *dst* <sub>[REG:NUM]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Multiplication: `dst := src1 \* src2`

#### [13] div *dst* <sub>[REG:NUM]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Division: `dst := src1 / src2` 
             
             Division by 0 and signed division of min_int by -1 are UB  
             

#### [18] xor *dst* <sub>[REG:INT]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Bitwise exclusive or: `dst := src1 ^ src2`
             
             Note: `xor dst = src1  0b111...1` can be used to emulate `not`

#### [19] and *dst* <sub>[REG:INT]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Bitwise and: `dst := src1 & src2`

#### [1a] or *dst* <sub>[REG:INT]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Bitwise or: `dst := src1 | src2`

#### [1b] shl *dst* <sub>[REG:INT]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Shift left: `dst := src1 << src2`
                           
             dst: = src1 << (src2 % bitwidth(src1))

#### [1c] shr *dst* <sub>[REG:INT]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Shift right: `dst := src1 >> src2`
                          
             dst: = src1 >> (src2 % bitwidth(src1))

#### [1d] rem *dst* <sub>[REG:INT]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Modulo: `dst := src1 % src2`
             
              Modulo by 0 and signed modulo of min_int by -1 are UB  
              

#### [1e] clmul *dst* <sub>[REG:INT]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
NYI: Carry-less multiplication
             
             def clmul(src1: int, src2: int) -> int:
                 dst = 0
                 for i in range(bitwidth(src1)):
                     if (1 << i) & src1: dst ^= src2 << i
                 return dst
             

## Conditional Branches

#### [20] beq *op1* <sub>[REG/CONST:ANY]</sub> *op2* <sub>[REG/CONST:SAME_AS_PREV]</sub> *target_bbl* <sub>[BBL]</sub>
Conditional branch (if equal)

#### [21] bne *op1* <sub>[REG/CONST:ANY]</sub> *op2* <sub>[REG/CONST:SAME_AS_PREV]</sub> *target_bbl* <sub>[BBL]</sub>
Conditional branch (if not equal)

#### [22] blt *op1* <sub>[REG/CONST:ADDR_NUM]</sub> *op2* <sub>[REG/CONST:SAME_AS_PREV]</sub> *target_bbl* <sub>[BBL]</sub>
Conditional branch (if greater than)

#### [23] ble *op1* <sub>[REG/CONST:ADDR_NUM]</sub> *op2* <sub>[REG/CONST:SAME_AS_PREV]</sub> *target_bbl* <sub>[BBL]</sub>
Conditional branch (if less or equal)

## Other Control Flow

#### [28] switch *index* <sub>[REG:UINT]</sub> *table* <sub>[JTB]</sub>
Multi target computed jump
                
                The reg argument must be less than the jtb `size`.
                
                The jtb symbol must have been previously defined with the `.jtb` directive.
                

#### [29] bra *target_bbl* <sub>[BBL]</sub>
Unconditional branch

#### [2a] ret 
Return from fun

#### [2b] bsr *target_fun* <sub>[FUN]</sub>
Call fun (branch to subroutine)

#### [2c] jsr *target_fun_addr* <sub>[REG:CODE]</sub> *target_fun_sig* <sub>[FUN]</sub>
Call fun indirectly through reg (jump to subroutine)
             
             Note: fun describes the signature which must have been previously defined with the `.fun` directive.

#### [2d] syscall *target_fun_sig* <sub>[FUN]</sub> *syscall_no* <sub>[CONST:UINT]</sub>
Syscall to `syscall_no` 
                 
                 Note: fun describes the signature which must have been previously defined with the `.fun` directive.

#### [2e] trap 
Abort program

## Move/Conversion

#### [30] pusharg *src* <sub>[REG/CONST:ANY]</sub>
Push a call or return arg 
                 
                 Note: must immediately precede bsr/jsr or ret.

#### [31] poparg *dst* <sub>[REG:ANY]</sub> =
Pop a call or return arg 
                
                Note: must immediately follow fun entry or bsr/jsr.

#### [32] conv *dst* <sub>[REG:NUM]</sub> = *src* <sub>[REG/CONST:NUM]</sub>
Conversion of numerical regs
              
              Note: regs do not have to be of same size. Bits may change.
              If the conversion involves both a widening and a change of type, the widening is performed
              first. 

#### [33] bitcast *dst* <sub>[REG:ANY]</sub> = *src* <sub>[REG/CONST:SAME_SIZE_AS_PREV]</sub>
Cast between regs of same size
                 
                 Note: Bits will be re-interpreted but do not change. 
                 This is useful for manipulating addresses in unusual ways or 
                 looking at the  binary representation of floats.

#### [34] mov *dst* <sub>[REG:ANY]</sub> = *src* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Move between registers
             
             While a mov can be emulated via a `add dst = src 0`, 
             having a dedicated instruction makes some optimizations easier to 
             implement when combined with a canonicalization.

#### [35] cmpeq *dst* <sub>[REG:ANY]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub> *cmp1* <sub>[REG/CONST:ANY]</sub> *cmp2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Conditional move (if equal): `dst := (cmp1 == cmp2) ? src1 : src2`
               
               Note: dst/cmp1/cmp2 may be of a different type than src1/src2.

#### [36] cmplt *dst* <sub>[REG:ANY]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub> *cmp1* <sub>[REG/CONST:ADDR_NUM]</sub> *cmp2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Conditional move (if less than): `dst := (cmp1 < cmp2) ? src1 : src2` 
               
               Note: dst/cmp1/cmp2 may be of a different type than src1/src2.

## Address Arithmetic

#### [38] lea *dst* <sub>[REG:ADDR]</sub> = *base* <sub>[REG/CONST:SAME_AS_PREV]</sub> *offset* <sub>[REG/CONST:OFFSET]</sub>
Load effective address: `dst  := base + offset`  
             
             Note: dst and base are addresses but offset is not.

#### [39] lea.mem *dst* <sub>[REG:ADDR]</sub> = *base* <sub>[MEM]</sub> *offset* <sub>[REG/CONST:OFFSET]</sub>
Load effective mem address with offset: `dst := base + offset`

#### [3a] lea.stk *dst* <sub>[REG:ADDR]</sub> = *base* <sub>[STK]</sub> *offset* <sub>[REG/CONST:OFFSET]</sub>
Load effective stk address with offset: `dst := base + offset`

#### [3b] lea.fun *dst* <sub>[REG:CODE]</sub> = *base* <sub>[FUN]</sub>
Load effective fun address: `dst := base` 
                 
                 Note: no offset

## Load

#### [40] ld *dst* <sub>[REG:ANY]</sub> = *base* <sub>[REG/CONST:ADDR]</sub> *offset* <sub>[REG/CONST:OFFSET]</sub>
Load from reg base with offset: `dst := RAM[base + offset]`

#### [41] ld.mem *dst* <sub>[REG:ANY]</sub> = *base* <sub>[MEM]</sub> *offset* <sub>[REG/CONST:OFFSET]</sub>
Load from mem base with offset: `dst := RAM[base + offset]`

#### [42] ld.stk *dst* <sub>[REG:ANY]</sub> = *base* <sub>[STK]</sub> *offset* <sub>[REG/CONST:OFFSET]</sub>
Load from stk base with offset: `dst := RAM[base + offset]`

#### [44] st *base* <sub>[REG:ADDR]</sub> *offset* <sub>[REG/CONST:OFFSET]</sub> = *src* <sub>[REG/CONST:ANY]</sub>
Store to reg base with offset: `RAM[base + offset] := src`

#### [45] st.mem *base* <sub>[MEM]</sub> *offset* <sub>[REG/CONST:OFFSET]</sub> = *src* <sub>[REG/CONST:ANY]</sub>
Store to mem base with offset: `RAM[base + offset] := src`

#### [46] st.stk *base* <sub>[STK]</sub> *offset* <sub>[REG/CONST:OFFSET]</sub> = *src* <sub>[REG/CONST:ANY]</sub>
Store to stk base with offset: `RAM[base + offset] := src`

## Store

#### [48] cas *dst* <sub>[REG:ANY]</sub> *cmp* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src* <sub>[REG/CONST:SAME_AS_PREV]</sub> *base* <sub>[REG:ADDR]</sub> *offset* <sub>[REG/CONST:OFFSET]</sub>
Atomic Compare and Swap  
                
                addr = base + offset 
                dst = RAM[addr] 
                if dst == cmp: RAM[addr] = src
             

#### [49] cas.mem *dst* <sub>[REG:ANY]</sub> *cmp* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src* <sub>[REG/CONST:SAME_AS_PREV]</sub> *base* <sub>[MEM]</sub> *offset* <sub>[REG/CONST:OFFSET]</sub>
Atomic Compare and Swap  
                    
                    addr = base + offset 
                    dst = RAM[addr] 
                    if dst == cmp: RAM[addr] = src
                 

#### [4a] cas.stk *dst* <sub>[REG:ANY]</sub> *cmp* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src* <sub>[REG/CONST:SAME_AS_PREV]</sub> *base* <sub>[STK]</sub> *offset* <sub>[REG/CONST:OFFSET]</sub>
AtomicCompare and Swap  

                    addr = base + offset 
                    dst = RAM[addr] 
                    if dst == cmp: RAM[addr] = src
             

## Float ALU

#### [50] ceil *dst* <sub>[REG:FLT]</sub> = *src* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Round float to integral (toward positive infinity)

#### [51] floor *dst* <sub>[REG:FLT]</sub> = *src* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Round float to integral (toward negative infinity)

#### [52] round *dst* <sub>[REG:FLT]</sub> = *src* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Round float to integral (to nearest with ties to away)

#### [53] trunc *dst* <sub>[REG:FLT]</sub> = *src* <sub>[REG/CONST:SAME_AS_PREV]</sub>

               Round float to integral (toward zero)

               Note, frac(val) = val - trunc(val)

#### [54] copysign *dst* <sub>[REG:FLT]</sub> = *src1* <sub>[REG/CONST:SAME_AS_PREV]</sub> *src2* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Set the sign of src1 to match src2 (floating point only)
                  
                  Note: `copysign dst src1 0.0` can be used to emulate `abs`

#### [55] sqrt *dst* <sub>[REG:FLT]</sub> = *src* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Compute the sqrt of floating point value

## Advanced ALU

#### [60] cntlz *dst* <sub>[REG:INT]</sub> = *src* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Count leading zeros

#### [61] cnttz *dst* <sub>[REG:INT]</sub> = *src* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Count trailing zeros

#### [62] cntpop *dst* <sub>[REG:INT]</sub> = *src* <sub>[REG/CONST:SAME_AS_PREV]</sub>
Count set bits (pop count)

## Annotation

#### [70] nop 
nop - internal use

#### [71] nop1 *src_and_dst* <sub>[REG:ANY]</sub> =
nop with one reg - internal use
              
              Note: Can be used to `reserve` a reg for code generation.

#### [77] line *file* <sub>[STRING]</sub> *line* <sub>[CONST:ANY]</sub>
NYI: debug line number

#### [78] inline *target-asm-ins* <sub>[BYTES]</sub>
Inject arbitrary target instructions bytes into instruction stream

#### [79] getfp *dst* <sub>[REG:ADDR]</sub>
Materialize the frame-pointer
             
             Get the stack-pointer's value before the call. Used mainly to interface with
             the Linux execution environment at program startup.

#### [7a] getsp *dst* <sub>[REG:ADDR]</sub>
Materialize the stack-pointer
             
             Get the current stack-pointer. Used mainly to interface with
             the Linux execution environment after a clone syscall.

#### [7b] gettp *dst* <sub>[REG:ADDR]</sub>
Materialize the tls-pointer

## Directives

#### [01] .mem *name* <sub>[NAME]</sub> *alignment* <sub>[INT]</sub> *mem_kind* <sub>[MEM_KIND]</sub>
Add new mem region to unit

#### [02] .data *repeat* <sub>[INT]</sub> *data* <sub>[BYTES]</sub>
Add content to current mem region: multiple bytes

#### [03] .addr.fun *width* <sub>[INT]</sub> *fun* <sub>[FUN]</sub>
Add content to current mem region: code address

#### [04] .addr.mem *width* <sub>[INT]</sub> *mem* <sub>[MEM]</sub> *offset* <sub>[INT]</sub>
Add content to current mem region: memory address with offset

#### [05] .fun *name* <sub>[NAME]</sub> *fun_kind* <sub>[FUN_KIND]</sub> *out_params* <sub>[TYPE_LIST]</sub> *in_params* <sub>[TYPE_LIST]</sub>
Add new fun to unit

#### [06] .bbl *name* <sub>[NAME]</sub>
Add new bbl to current fun

#### [07] .reg *reg_kind* <sub>[DATA_KIND]</sub> *names* <sub>[NAME_LIST]</sub>
Add new regs to current fun

#### [08] .stk *name* <sub>[NAME]</sub> *alignment* <sub>[INT]</sub> *size* <sub>[INT]</sub>
Add stk region to current fun

#### [09] .jtb *name* <sub>[NAME]</sub> *size* <sub>[INT]</sub> *default_bbl* <sub>[BBL]</sub> *map* <sub>[BBL_TAB]</sub>
Add jump table to current fun

<!--- @AUTOGEN-END@ --->

