cmake_minimum_required(VERSION 3.5)
project(clang_test)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_HOME_DIRECTORY}/cmake/Modules/")


find_package(LibClang REQUIRED)
include_directories(
        ${LIBCLANG_INCLUDE_DIR}
)
set(SOURCE_FILES src/gen.cc)
add_executable(clang_test ${SOURCE_FILES})
target_link_libraries(clang_test
        ${LIBCLANG_LIBRARY}
        )