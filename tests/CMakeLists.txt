include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # For Windows: Prevent overriding the parent project's compiler/linker settings
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(BitBoardTest "")
target_sources(BitBoardTest PRIVATE bit_board_test.cpp)
target_include_directories(BitBoardTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(BitBoardTest PRIVATE gtest_main)
target_link_libraries(BitBoardTest PRIVATE BitBoard)
gtest_discover_tests(BitBoardTest)

