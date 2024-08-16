cmake_minimum_required(VERSION 3.25.1)

project(TMonkey)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

add_subdirectory(external/googletest)

add_executable(tmonkey
    src/main.cpp
    src/parser.cpp
    src/pretty.cpp
    src/codegen.cpp
    src/lexer.cpp
    src/token.cpp
)

enable_testing()

add_executable(lexer_test src/lexer_test.cpp src/lexer.cpp src/token.cpp)
add_executable(parser_test src/parser_test.cpp src/lexer.cpp src/token.cpp src/parser.cpp src/pretty.cpp)
add_executable(strintern_test src/strintern_test.cpp)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  set(
    CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -fsanitize=undefined -fsanitize=address -g"
  )
  target_link_options(tmonkey
    BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address
  )
  target_link_options(parser_test
    BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address
  )
endif()

target_link_libraries(lexer_test
  GTest::gmock_main
  GTest::gtest_main
)

target_link_libraries(parser_test
  GTest::gmock_main
  GTest::gtest_main
)

target_link_libraries(strintern_test
  GTest::gmock_main
  GTest::gtest_main
)

add_test(NAME lexer_test COMMAND lexer_test)
add_test(NAME parser_test COMMAND parser_test)
add_test(NAME strintern_test COMMAND strintern_test)
