cmake_minimum_required(VERSION 3.24)
project(owl)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/main.cpp src/lexer.cpp src/lexer.h src/parser.cpp src/parser.h src/ast.cpp src/ast.h)

include(FetchContent)

# LLVM
find_package(llvm QUIET)
if (NOT llvm_FOUND)
    FetchContent_Declare(llvm URL https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-15.0.6.tar.gz)
    FetchContent_GetProperties(llvm)
    if (NOT llvm_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(llvm)
    endif()
endif()
add_subdirectory(${llvm_SOURCE_DIR}/llvm ${llvm_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${llvm_SOURCE_DIR}/llvm/include ${llvm_BINARY_DIR}/include)
llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries(${PROJECT_NAME} PRIVATE ${llvm_libs})

