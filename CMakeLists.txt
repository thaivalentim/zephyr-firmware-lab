# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(button_led)

# cada linha dentro do target_sources adiciona um arquivo
# fonte (argumento) ao projeto

target_sources(app PRIVATE 
                        src/main.c 
                        src/button.c
                        src/led.c
                        src/app_events.c
)

target_include_directories(app PRIVATE
    include
)