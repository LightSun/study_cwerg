cmake_minimum_required(VERSION 3.10)
project(Cwerg)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS "-fno-exceptions -O3 -g -fPIC -Wall -Werror -pedantic -Wno-class-memaccess -Wno-sign-compare ${CWERG_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-static")

# with static linking, see
# https://gcc.gnu.org/bugzilla/show_bug.cgi?id=58909
# Everything here is super brittle:  the order of libs matter
set(CMAKE_CXX_STANDARD_LIBRARIES "${CWERG_LIBS} -Wl,--whole-archive -lpthread -Wl,--no-whole-archive ")

#========= HEAVEN7 ===============
SET(IS_WINDOWS FALSE)
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    message("current platform: Linux ")
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    message("current platform: Windows ")
    set(IS_WINDOWS TRUE)
else()
    message("current platform: unkonw ")
endif()
#========= end HEAVEN7 ===============

set(COMMON_LIBS)
set(COMMON_INCLUDE_DIR)
set(COMMON_LINK_DIR)
IF(IS_WINDOWS)
    set(COMMON_LINK_DIR "D:/study/github/mine/CppLibs/build-common-Desktop_Qt_5_12_9_MinGW_64_bit-Debug")
    set(COMMON_LIBS TH)
    SET(COMMON_INCLUDE_DIR "D:/study/github/mine/CppLibs/common")
    include_directories(${COMMON_INCLUDE_DIR})
    set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -lws2_32")
endif()

# The must come AFTER their use or be commented out to be picked up from the environment
set(CWERG_LIBS "")
set(CWERG_FLAGS "")

include_directories(./)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_library(UTIL_LIB STATIC
        Util/assert.cc
        Util/assert.h
        Util/breakpoint.cc
        Util/breakpoint.h
        Util/bitvec.cc
        Util/bitvec.h
        Util/bst.cc
        Util/bst.h
        Util/handle.h
        Util/handle.cc
        Util/handlevec.cc
        Util/handlevec.h
        Util/immutable.cc
        Util/immutable.h
        Util/list.h
        Util/mem_pool.h
        Util/parse.cc
        Util/parse.h
        Util/stripe.cc
        Util/stripe.h
        Util/switch.cc
        Util/switch.h
        Util/webserver.cc
        Util/webserver.h
        Util/common.h
)
IF(IS_WINDOWS)
    target_link_directories(UTIL_LIB PUBLIC ${COMMON_LINK_DIR})
    target_link_libraries(UTIL_LIB PUBLIC Ws2_32 ${COMMON_LIBS})
endif()

add_executable(immutable_test.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        Util/immutable_test.cc
)
target_link_libraries(immutable_test.exe PUBLIC UTIL_LIB)

add_executable(bitvec_test.exe
        #$<TARGET_OBJECTS:UTIL_LIB>
        Util/bitvec_test.cc
)
target_link_libraries(bitvec_test.exe PUBLIC UTIL_LIB)

add_executable(handle_test.exe
        #$<TARGET_OBJECTS:UTIL_LIB>
        Util/handle_test.cc
)
target_link_libraries(handle_test.exe PUBLIC UTIL_LIB)

add_executable(handlevec_test.exe
        #$<TARGET_OBJECTS:UTIL_LIB>
        Util/handlevec_test.cc
)
target_link_libraries(handlevec_test.exe PUBLIC UTIL_LIB)

add_executable(parse_test.exe
        #$<TARGET_OBJECTS:UTIL_LIB>
        Util/parse_test.cc
)
target_link_libraries(parse_test.exe PUBLIC UTIL_LIB)

add_executable(mem_pool_test.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        Util/mem_pool_test.cc
)
target_link_libraries(mem_pool_test.exe PUBLIC UTIL_LIB)

add_library(CPUA32_LIB OBJECT
        CpuA32/symbolic.cc
        CpuA32/symbolic.h
        CpuA32/assembler.cc
        CpuA32/assembler.h
        CpuA32/opcode_gen.cc
        CpuA32/opcode_gen.h
)

add_library(CPUA32_LITE_LIB OBJECT
        CpuA32/symbolic.cc
        CpuA32/symbolic.h
        CpuA32/opcode_gen.cc
        CpuA32/opcode_gen.h
)

add_library(CPUA64_LIB OBJECT
        CpuA64/symbolic.cc
        CpuA64/symbolic.h
        CpuA64/assembler.cc
        CpuA64/assembler.h
        CpuA64/opcode_gen.cc
        CpuA64/opcode_gen.h
)

add_library(CPUA64_LITE_LIB OBJECT
        CpuA64/symbolic.cc
        CpuA64/symbolic.h
        CpuA64/opcode_gen.cc
        CpuA64/opcode_gen.h
)

add_library(CPUX64_LIB OBJECT
        CpuX64/symbolic.cc
        CpuX64/symbolic.h
        CpuX64/assembler.cc
        CpuX64/assembler.h
        CpuX64/opcode_gen.cc
        CpuX64/opcode_gen.h
)

add_library(CPUX64_LITE_LIB OBJECT
        CpuX64/symbolic.cc
        CpuX64/symbolic.h
        CpuX64/opcode_gen.cc
        CpuX64/opcode_gen.h
)

add_executable(a64_opcode_test.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:CPUA64_LITE_LIB>
        CpuA64/opcode_test.cc
)
target_link_libraries(a64_opcode_test.exe PUBLIC UTIL_LIB)

add_executable(a64_disassembler_tool.exe
        #$<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:CPUA64_LITE_LIB>
        CpuA64/disassembler_tool.cc
)
target_link_libraries(a64_disassembler_tool.exe PUBLIC UTIL_LIB)

add_executable(a64_assembler_tool.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:CPUA64_LIB>
        $<TARGET_OBJECTS:ELF_LIB>
        CpuA64/assembler_tool.cc
)
target_link_libraries(a64_assembler_tool.exe PUBLIC UTIL_LIB)

add_executable(x64_disassembler_tool.exe
        #$<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:CPUX64_LITE_LIB>
        CpuX64/disassembler_tool.cc
)
target_link_libraries(x64_disassembler_tool.exe PUBLIC UTIL_LIB)

add_executable(x64_assembler_tool.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:CPUX64_LIB>
        $<TARGET_OBJECTS:ELF_LIB>
        CpuX64/assembler_tool.cc
)
target_link_libraries(x64_assembler_tool.exe PUBLIC UTIL_LIB)

add_library(ELF_LIB STATIC
        Elf/enum_gen.cc
        Elf/enum_gen.h
        Elf/elfhelper.cc
        Elf/elfhelper.h
        Elf/elf_unit.h)
#target_link_libraries(ELF_LIB PUBLIC ${COMMON_LIBS})

add_library(BASE_LIB OBJECT
        Base/canonicalize.cc
        Base/canonicalize.h
        Base/cfg.cc
        Base/cfg.h
        Base/eval.cc
        Base/eval.h
        Base/ir.cc
        Base/ir.h
        Base/liveness.cc
        Base/liveness.h
        Base/lowering.cc
        Base/lowering.h
        Base/opcode_gen.cc
        Base/opcode_gen.h
        Base/sanity.cc
        Base/sanity.h
        Base/serialize.cc
        Base/serialize.h
        Base/reaching_defs.cc
        Base/reaching_defs.h
        Base/reg_stats.cc
        Base/reg_stats.h
        Base/reg_alloc.cc
        Base/reg_alloc.h
        Base/optimize.cc
        Base/optimize.h
)

add_executable(optimize_tool.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:BASE_LIB>
        Base/optimize_tool.cc
)
target_link_libraries(optimize_tool.exe PUBLIC UTIL_LIB)

add_executable(elf_tool.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
       # $<TARGET_OBJECTS:ELF_LIB>
        Elf/elf_tool.cc
)
target_link_libraries(elf_tool.exe PUBLIC UTIL_LIB ELF_LIB)

add_executable(gen_elf_test.exe
        # $<TARGET_OBJECTS:UTIL_LIB>
        # $<TARGET_OBJECTS:ELF_LIB>
        Elf/gen_elf_test.cc
)
target_link_libraries(gen_elf_test.exe PUBLIC UTIL_LIB ELF_LIB)

add_executable(a32_assembler_tool.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:CPUA32_LIB>
       # $<TARGET_OBJECTS:ELF_LIB>
        CpuA32/assembler_tool.cc
)
target_link_libraries(a32_assembler_tool.exe PUBLIC UTIL_LIB ELF_LIB)

add_executable(a32_disassembler_tool.exe
      #  $<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:CPUA32_LIB>
      # $<TARGET_OBJECTS:ELF_LIB>
        CpuA32/disassembler_tool.cc
)
target_link_libraries(a32_disassembler_tool.exe PUBLIC UTIL_LIB ELF_LIB)

add_library(CODEGENA32_LIB OBJECT
        CodeGenA32/isel_gen.cc
        CodeGenA32/isel_gen.h
        CodeGenA32/codegen.cc
        CodeGenA32/codegen.h
        CodeGenA32/legalize.cc
        CodeGenA32/legalize.h
        CodeGenA32/regs.cc
        CodeGenA32/regs.h
)

add_executable(a32_isel_tester.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:CPUA32_LIB>
      #  $<TARGET_OBJECTS:ELF_LIB>
        $<TARGET_OBJECTS:CODEGENA32_LIB>
        $<TARGET_OBJECTS:BASE_LIB>
        CodeGenA32/isel_tester.cc
)
target_link_libraries(a32_isel_tester.exe PUBLIC UTIL_LIB ELF_LIB)

add_executable(a32_codegen_tool.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:CPUA32_LIB>
       # $<TARGET_OBJECTS:ELF_LIB>
        $<TARGET_OBJECTS:CODEGENA32_LIB>
        $<TARGET_OBJECTS:BASE_LIB>
        CodeGenA32/codegen_tool.cc
)
target_link_libraries(a32_codegen_tool.exe PUBLIC UTIL_LIB ELF_LIB)

add_library(CODEGENA64_LIB OBJECT
        CodeGenA64/isel_gen.cc
        CodeGenA64/isel_gen.h
        CodeGenA64/codegen.cc
        CodeGenA64/codegen.cc
        CodeGenA64/codegen.h
        CodeGenA64/legalize.cc
        CodeGenA64/legalize.h
        CodeGenA64/regs.cc
        CodeGenA64/regs.h
)

add_executable(a64_isel_tester.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:CPUA64_LIB>
       #  $<TARGET_OBJECTS:ELF_LIB>
        $<TARGET_OBJECTS:CODEGENA64_LIB>
        $<TARGET_OBJECTS:BASE_LIB>
        CodeGenA64/isel_tester.cc
)
target_link_libraries(a64_isel_tester.exe PUBLIC UTIL_LIB ELF_LIB)

add_executable(a64_codegen_tool.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:CPUA64_LIB>
       # $<TARGET_OBJECTS:ELF_LIB>
        $<TARGET_OBJECTS:CODEGENA64_LIB>
        $<TARGET_OBJECTS:BASE_LIB>
        CodeGenA64/codegen_tool.cc
)
target_link_libraries(a64_codegen_tool.exe PUBLIC UTIL_LIB ELF_LIB)

add_library(CODEGENX64_LIB OBJECT
        CodeGenX64/isel_gen.cc
        CodeGenX64/isel_gen.h
        CodeGenX64/isel_gen_patterns.h
        CodeGenX64/codegen.cc
        CodeGenX64/codegen.h
        CodeGenX64/legalize.cc
        CodeGenX64/legalize.h
        CodeGenX64/regs.cc
        CodeGenX64/regs.h
)

add_executable(x64_isel_tester.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:CPUX64_LIB>
       # $<TARGET_OBJECTS:ELF_LIB>
        $<TARGET_OBJECTS:CODEGENX64_LIB>
        $<TARGET_OBJECTS:BASE_LIB>
        CodeGenX64/isel_tester.cc
)
target_link_libraries(x64_isel_tester.exe PUBLIC UTIL_LIB ELF_LIB)

add_executable(x64_codegen_tool.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:CPUX64_LIB>
       # $<TARGET_OBJECTS:ELF_LIB>
        $<TARGET_OBJECTS:CODEGENX64_LIB>
        $<TARGET_OBJECTS:BASE_LIB>
        CodeGenX64/codegen_tool.cc
)
target_link_libraries(x64_codegen_tool.exe PUBLIC UTIL_LIB ELF_LIB)

add_executable(builder_example.exe
       # $<TARGET_OBJECTS:UTIL_LIB>
        $<TARGET_OBJECTS:BASE_LIB>
        Examples/fib_ir.h
        Examples/builder_example.cc
)
target_link_libraries(builder_example.exe PUBLIC UTIL_LIB)

# this only works when (cross) compiled for a32
add_executable(jit_cpu_a32.exe EXCLUDE_FROM_ALL
       # $<TARGET_OBJECTS:UTIL_LIB>
       # $<TARGET_OBJECTS:ELF_LIB>
        $<TARGET_OBJECTS:CPUA32_LIB>
        Examples/jit_cpu_a32.cc
)
target_link_libraries(jit_cpu_a32.exe PUBLIC UTIL_LIB ELF_LIB)

# this only works when (cross) compiled for a32
add_executable(jit_ir_a32.exe EXCLUDE_FROM_ALL
       # $<TARGET_OBJECTS:UTIL_LIB>
       # $<TARGET_OBJECTS:ELF_LIB>
        $<TARGET_OBJECTS:CPUA32_LIB>
        $<TARGET_OBJECTS:CODEGENA32_LIB>
        $<TARGET_OBJECTS:BASE_LIB>
        Examples/fib_ir.h
        Examples/jit_ir_a32.cc

)
target_link_libraries(jit_ir_a32.exe PUBLIC UTIL_LIB ELF_LIB)

# this only works when (cross) compiled for a64
add_executable(jit_cpu_a64.exe EXCLUDE_FROM_ALL
      #  $<TARGET_OBJECTS:UTIL_LIB>
      #  $<TARGET_OBJECTS:ELF_LIB>
        $<TARGET_OBJECTS:CPUA64_LIB>
        Examples/jit_cpu_a64.cc
)
target_link_libraries(jit_cpu_a64.exe PUBLIC UTIL_LIB ELF_LIB)

# this only works when (cross) compiled for a64
add_executable(jit_ir_a64.exe EXCLUDE_FROM_ALL
       # $<TARGET_OBJECTS:UTIL_LIB>
       # $<TARGET_OBJECTS:ELF_LIB>
        $<TARGET_OBJECTS:CPUA64_LIB>
        $<TARGET_OBJECTS:CODEGENA64_LIB>
        $<TARGET_OBJECTS:BASE_LIB>
        Examples/fib_ir.h
        Examples/jit_ir_a64.cc
)
target_link_libraries(jit_ir_a64.exe PUBLIC UTIL_LIB ELF_LIB)

# this only works when (cross) compiled for x64
add_executable(jit_cpu_x64.exe EXCLUDE_FROM_ALL
       # $<TARGET_OBJECTS:UTIL_LIB>
       # $<TARGET_OBJECTS:ELF_LIB>
        $<TARGET_OBJECTS:CPUX64_LIB>
        Examples/jit_cpu_x64.cc
)
target_link_libraries(jit_cpu_x64.exe PUBLIC UTIL_LIB ELF_LIB)


# this only works when (cross) compiled for x64
add_executable(jit_ir_x64.exe EXCLUDE_FROM_ALL
       # $<TARGET_OBJECTS:UTIL_LIB>
       # $<TARGET_OBJECTS:ELF_LIB>
        $<TARGET_OBJECTS:CPUX64_LIB>
        $<TARGET_OBJECTS:CODEGENX64_LIB>
        $<TARGET_OBJECTS:BASE_LIB>
        Examples/fib_ir.h
        Examples/jit_ir_x64.cc
)
target_link_libraries(jit_ir_x64.exe PUBLIC UTIL_LIB ELF_LIB)


add_executable(example_c_fib.exe
        #$<TARGET_OBJECTS:UTIL_LIB>
        #$<TARGET_OBJECTS:ELF_LIB>
        $<TARGET_OBJECTS:CPUX64_LIB>
        $<TARGET_OBJECTS:CODEGENX64_LIB>
        $<TARGET_OBJECTS:BASE_LIB>
        C/cwerg.h
        C/cwerg.cc
        C/example_fib.c
)
target_link_libraries(example_c_fib.exe PUBLIC UTIL_LIB ELF_LIB)

if(IS_WINDOWS)
# default support openmp
IF(NOT (FORCE_SKIP_OPENMP))
    INCLUDE (${PROJECT_SOURCE_DIR}/cmake/openmp.cmake)
    TENGINE_CHECK_LIB_OPENMP (_has_openmp)
    IF (_has_openmp)
        #TENGINE_USE_LIB_OPENMP (TH-static ON)
        TENGINE_USE_LIB_OPENMP (UTIL_LIB    ON)
        IF(MSVC)
            SET_SOURCE_FILES_PROPERTIES(${TENGINE_SOURCE_FILES} PROPERTIES LANGUAGE CXX)
            #TARGET_COMPILE_OPTIONS(TH-static PRIVATE /std:c++latest)
            TARGET_COMPILE_OPTIONS(UTIL_LIB   PRIVATE /std:c++latest)
        ENDIF()

        SET (TENGINE_ENABLED_OPENMP ${OpenMP_C_VERSION} CACHE INTERNAL "" FORCE)
    ENDIF()
ENDIF()
UNSET (_has_openmp)
endif()

#summary all compile info
include(cmake/summary.cmake)
