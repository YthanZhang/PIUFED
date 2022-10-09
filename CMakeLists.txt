# Platform Independent Utilities For Embedded Development
#*******************************************************************************
#* Copyright 2021 Ythan(Ethan) Zhang
#*
#* Redistribution and use in source and binary forms, with or without
#* modification, are permitted provided that the following conditions are met:
#*
#* 1. Redistributions of source code must retain the above copyright notice,
#* this list of conditions and the following disclaimer.
#* 2. Redistributions in binary form must reproduce the above copyright notice,
#* this list of conditions and the following disclaimer in the documentation
#* and/or other materials provided with the distribution.
#*
#* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#* POSSIBILITY OF SUCH DAMAGE.
#*******************************************************************************
#* This software component is licensed by Ythan Zhang under the
#*                           BSD-2-Clause license.
#* You may not use this file except in compliance with the license.
#* You may obtain a copy of the License at: opensource.org/licenses/BSD-2-Clause
#*******************************************************************************

cmake_minimum_required(VERSION 3.23)
project(PIUFED C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

include_directories(.)
add_library(PIUFED
        piu_button.c
        piu_vtimer.c
        piu_margined_linear.c
        piu_sim_uart.c
        piu_modbus_crc16.c
        piu_modbus_crc16.h
        )

target_compile_options(PIUFED PRIVATE -std=c99)

# Define DoUnitTest and Catch2_DIR if wish to use unit test
if (PIUFED_DoUnitTest)
    # Add Catch2
    if (NOT DEFINED Catch2_DIR)
        set(Catch2_DIR Catch2)
        message(NOTICE "Catch2_DIR is not defined, using default path")
        message(NOTICE " ${PROJECT_SOURCE_DIR}/${Catch2_DIR}")
        message(NOTICE "Ignore this notice if no further error occurs.")
    endif ()
    include_directories(${Catch2_DIR} ${PROJECT_SOURCE_DIR}/UnitTest)
    add_subdirectory(${Catch2_DIR} Catch2)

    add_executable(piufed-unittest
            UnitTest/margined_linear_test.cpp
            UnitTest/sim_uart_test.cpp
            UnitTest/vtimer_test.cpp
            UnitTest/testMain.cpp)
    target_link_libraries(piufed-unittest PRIVATE
            PIUFED
            Catch2::Catch2WithMain
            )

    # Optimization level for different compile options
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        message(STATUS "O2 optimization for release build")
        add_compile_options(-O2 -flto)
    elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
        message(STATUS "O2 optimization for release build, debug info included")
        add_compile_options(-O2 -ggdb -flto)
    elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
        message(STATUS "Maximum optimization for size")
        add_compile_options(-Os -flto)
    else ()
        message(STATUS "Minimal optimization, debug info included")
        add_compile_options(-Og -ggdb)
    endif ()
    # End of optimization level for different compile options
endif ()

