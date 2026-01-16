set(tgt_name sv_utest)

add_executable(${tgt_name} test.cpp)

target_link_libraries(${tgt_name} PRIVATE abin::string_view)
target_link_libraries(${tgt_name} PRIVATE ${THREAD_LIB})
target_link_libraries(${tgt_name} PUBLIC Catch2::Catch2)