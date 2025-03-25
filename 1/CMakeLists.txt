add_executable(dijkstra main.cpp)
add_executable(scaling_dijkstra scaling_dijkstra.cpp)

find_package(OpenMP REQUIRED)

add_executable(test_dijkstra test_dijkstra.cpp)

target_link_libraries(test_dijkstra PRIVATE OpenMP::OpenMP_CXX)
