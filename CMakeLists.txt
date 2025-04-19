# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 13_3_Rel1) #ARM
#set(toolchainVersion RISCV_RPI_2_0_0_5) #RISC-V
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ==============================================================================ni======
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.18)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(mario C CXX ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# turn on all compiler warnings 
add_compile_options(-Wall -Wextra)

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
  code/mario-demo.cpp
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

add_library(pico_mario INTERFACE)

target_sources(pico_mario INTERFACE
${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/st7735.cpp
${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/displaylib_16_graphics.cpp
${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/displaylib_16_Print.cpp
${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/displaylib_16_Font.cpp
${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/displaylib_16_common.cpp
${CMAKE_CURRENT_LIST_DIR}/src/displaylib_16/Bitmap_TEST_Data.cpp
)

target_include_directories(pico_mario INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

# Pull in pico libraries that we need
target_link_libraries(${PROJECT_NAME} pico_stdlib pico_multicore hardware_spi hardware_pwm  pico_mario)


# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

