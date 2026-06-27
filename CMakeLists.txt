cmake_minimum_required(VERSION 3.18)

project(fe310_redboard_baremetal C ASM)

set(BOARD "redv_redboard" CACHE STRING "Board name")
set(CPU_HZ "16000000" CACHE STRING "CPU/peripheral clock used for driver timing")
set(UART_BAUD "115200" CACHE STRING "CLI UART baud rate")

add_executable(fe310_cli
    startup/crt0.S
    startup/trap.S
)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    startup/*.c
    src/*.c
    tests/*.c
    third_party/embedded-cli/lib/src/*.c
)

target_sources(fe310_cli PRIVATE ${APP_SOURCES})

target_include_directories(fe310_cli PRIVATE
    include
    third_party/embedded-cli/lib/include
)

target_compile_definitions(fe310_cli PRIVATE
    BOARD_REDV_REDBOARD=1
    BOARD_CPU_HZ=${CPU_HZ}UL
    BOARD_UART_BAUD=${UART_BAUD}UL
)

target_compile_options(fe310_cli PRIVATE
    -march=rv32imac_zicsr
    -mabi=ilp32
    -mcmodel=medany
    -Os
    -g3
    -ffunction-sections
    -fdata-sections
    -fno-common
    -Wall
    -Wextra
    -Wshadow
    -Wconversion
)

target_link_options(fe310_cli PRIVATE
    -march=rv32imac_zicsr
    -mabi=ilp32
    -mcmodel=medany
    -nostartfiles
    -T${CMAKE_SOURCE_DIR}/linker/fe310_flash.ld
    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_BINARY_DIR}/fe310_cli.map
)

target_link_libraries(fe310_cli PRIVATE c gcc nosys)

add_custom_command(TARGET fe310_cli POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:fe310_cli> fe310_cli.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:fe310_cli>
)
