cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(Lab4)

pico_sdk_init()

add_executable(Lab4
       TFTMaster.c TouchScreen.c TFT_PIO.c glcdfont.c ts_test.c ts_lcd.c
        )

# pull in common dependencies
target_link_libraries(Lab4 pico_stdlib pico_bootsel_via_double_reset)

# create map/bin/hex file etc.
pico_add_extra_outputs(Lab4)

