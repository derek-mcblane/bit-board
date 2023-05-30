add_library(BitBoard bit_board.cpp)
target_compile_features(BitBoard PUBLIC cxx_std_20)
target_include_directories(BitBoard PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(BitBoard PUBLIC Vector2D)
