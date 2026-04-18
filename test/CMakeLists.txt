set(tgt_name sv_utest)

add_executable(${tgt_name} test.cpp)

target_link_libraries(${tgt_name} PRIVATE abin::string_view)
target_link_libraries(${tgt_name} PRIVATE Catch2::Catch2)

# 注册 sv_utest 作为 CTest 可识别的测试用例
# 当执行 `ctest` 时，会运行 sv_utest 并检查其返回值
add_test(NAME sv_CTest COMMAND sv_utest)