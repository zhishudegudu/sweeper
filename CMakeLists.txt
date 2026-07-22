cmake_minimum_required(VERSION 3.16)
project(sweeper C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(sweeper
    src/alert.c
    src/config.c
    src/controller.c
    src/executor.c
)
target_include_directories(sweeper PUBLIC include)

if(MSVC)
    target_compile_options(sweeper PRIVATE /W4)
else()
    target_compile_options(sweeper PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(sweeper_demo src/main.c)
target_link_libraries(sweeper_demo PRIVATE sweeper)

enable_testing()
add_subdirectory(thirdparty/googletest EXCLUDE_FROM_ALL)

foreach(iteration RANGE 1 3)
    add_executable(test_iteration${iteration}
        test/test_iteration${iteration}.cpp
    )
    target_link_libraries(test_iteration${iteration} PRIVATE sweeper gtest_main)
    add_test(NAME iteration${iteration} COMMAND test_iteration${iteration})
endforeach()
