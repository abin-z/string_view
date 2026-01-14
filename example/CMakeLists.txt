add_executable(sv_basic sv_basic.cpp)
target_link_libraries(sv_basic PRIVATE abin::string_view)