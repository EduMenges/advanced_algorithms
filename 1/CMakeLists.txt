add_executable(dijkstra main.cpp)
add_executable(scaling_dijkstra scaling_dijkstra.cpp)
target_link_libraries(scaling_dijkstra PRIVATE mimalloc)

add_executable(test_dijkstra test_dijkstra.cpp)

target_link_libraries(test_dijkstra PRIVATE mimalloc OpenMP::OpenMP_CXX)
