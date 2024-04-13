cmake_minimum_required(VERSION 3.27)
project(PA2_big_task)

set(CMAKE_CXX_STANDARD 20)
set(WARNINGS "-Wall -pedantic")
set(MEMDEBUGGER "-fsanitize=address")

set(CMAKE_CXX_FLAGS ${WARNINGS})

file(GLOB SOURCES "src/*.cpp")

add_executable(big_task main.cpp ${SOURCES})
add_executable(memdebug main.cpp ${SOURCES})

# Setting additional memory debugger flag for memdebug target
target_compile_options(memdebug PRIVATE ${MEMDEBUGGER})
# Need to add memory debugger to linker too
target_link_options(memdebug PRIVATE ${MEMDEBUGGER})
