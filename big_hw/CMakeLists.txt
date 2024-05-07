cmake_minimum_required(VERSION 3.27)
project(PA2_big_task)

set(CMAKE_CXX_STANDARD 20)
set(WARNINGS "-Wall -pedantic")
set(MEMDEBUGGER "-fsanitize=address")

set(CMAKE_CXX_FLAGS ${WARNINGS})

file(GLOB SOURCES
        "src/*.cpp"
        "src/ExpressionBuilders/*.cpp"
        "src/ExpressionBuilders/ASTNodes/*.cpp"
        "src/ExpressionBuilders/CycleDetectionVisitor/*.cpp"
        "src/InputOutputUtilities/*.cpp"
        "src/SpreadsheetStructure/*.cpp")

add_executable(big_task main.cpp ${SOURCES}
        tests/Tester.h)
add_executable(memdebug main.cpp ${SOURCES}
        tests/Tester.h)

target_link_libraries(big_task ${CMAKE_SOURCE_DIR}/x86_64-linux-gnu/libexpression_parser.a)
target_link_libraries(memdebug ${CMAKE_SOURCE_DIR}/x86_64-linux-gnu/libexpression_parser.a)

# Setting additional memory debugger flag for memdebug target
target_compile_options(memdebug PRIVATE ${MEMDEBUGGER})
# Need to add memory debugger to linker too
target_link_options(memdebug PRIVATE ${MEMDEBUGGER})
