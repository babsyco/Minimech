cmake_minimum_required(VERSION 3.2)

set(CMAKE_C_OUTPUT_EXTENSION ".rel")
set(CMAKE_C_COMPILER sdcc)
set(CMAKE_SYSTEM_NAME Generic) # No linux target etc

# Prevent default configuration
set(CMAKE_C_FLAGS_INIT "")
set(CMAKE_EXE_LINKER_FLAGS_INIT "")

# Poject source files
set(MM_SRC_FILES
    "/home/workspace/Milestone_3/project_code/src/MM_main.c"
    "/home/workspace/Milestone_3/project_code/src/MM_bt_hc06.c"
    "/home/workspace/Milestone_3/project_code/src/MM_t2s_xfs5152.c"
    "/home/workspace/Milestone_3/project_code/src/MM_lib.c"
    "/home/workspace/Milestone_3/project_code/src/MM_stm8s.c"
    ""
)

# STM8S Standard Peripheral Library source files required
set(SPL_SRC_FILES
    "/home/workspace/Milestone_3/STM8S-SDCC-SPL/src/stm8s_tim1.c"
    "/home/workspace/Milestone_3/STM8S-SDCC-SPL/src/stm8s_clk.c"
    "/home/workspace/Milestone_3/STM8S-SDCC-SPL/src/stm8s_uart1.c"
    "/home/workspace/Milestone_3/STM8S-SDCC-SPL/src/stm8s_uart3.c"
    "/home/workspace/Milestone_3/STM8S-SDCC-SPL/src/stm8s_gpio.c"
)

project(STM8Blink C)
SET(CMAKE_C_FLAGS "-L/usr/share/sdcc/lib/small -mstm8 --std-c99")
set(SPL_PATH STM8S-SDCC-SPL)
include_directories(STM8S-SDCC-SPL/inc)
include_directories(STM8S-SDCC-SPL/conf)
# Added by Daniel
include_directories(project_code/inc)


# Configure which STM8S we are using
add_definitions(-DSTM8S208)

add_executable(MiniMech.ihx
    ${SPL_SRC_FILES}
    ## added by Daniel
    ${MM_SRC_FILES}
)

# Flash targets
add_custom_target(flash COMMAND stm8flash -c stlink -p stm8s208rb -w MiniMech.ihx)
