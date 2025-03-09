# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
#set(toolchainVersion 13_3_Rel1) #ARM
set(toolchainVersion RISCV_RPI_2_0_0_5) #RISC-V
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico2 CACHE STRING "Board type")

# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.18)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(mario-demo C CXX ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# turn on all compiler warnings 
add_compile_options(-Wall -Wextra)

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    mario-demo.cpp
  #examples/ST7735_TFT_HELLO/main.cpp
  #examples/ST7735_TFT_GRAPHICS/main.cpp
  #examples/ST7735_TFT_FUNCTIONS_FPS/main.cpp
  #examples/ST7735_TFT_TESTS/main.cpp
  #examples/ST7735_TFT_BMP_DATA/main.cpp
  #examples/ST7735_TFT_BMP_TAN/main.cpp
  #examples/ST7735_TFT_BMP_SONIC/main.cpp 
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

add_library(pico_st7735 INTERFACE)

target_sources(pico_st7735 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/st7735/ST7735_TFT.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/st7735/ST7735_TFT_graphics.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/st7735/ST7735_TFT_Print.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/st7735/ST7735_TFT_Font.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/st7735/ST7735_TFT_Bitmap_Data.cpp
)

target_include_directories(pico_st7735 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in pico libraries that we need
target_link_libraries(${PROJECT_NAME} pico_stdlib pico_multicore hardware_spi hardware_pwm pico_st7735 )


# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

