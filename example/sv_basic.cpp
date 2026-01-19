#include <iostream>

#include "abin/string_view.h"

int main()
{
  using abin::string_view;

  // ---------- 构造 ----------
  const char *cstr = "Hello, String View!";
  std::string str = "Hello, String View!";

  string_view sv1(cstr);               // 从 C 字符串
  string_view sv2(str);                // 从 std::string
  string_view sv3("Example");          // 直接字面量
  string_view sv4 = sv1.substr(7, 6);  // 子串: "String"

  // ---------- 输出 ----------
  std::cout << "sv1: " << sv1 << '\n';
  std::cout << "sv2: " << sv2 << '\n';
  std::cout << "sv3: " << sv3 << '\n';
  std::cout << "sv4 (substr): " << sv4 << '\n';

  // ---------- 比较 ----------
  std::cout << "sv1 == sv2? " << (sv1 == sv2 ? "true" : "false") << '\n';
  std::cout << "sv1 != sv3? " << (sv1 != sv3 ? "true" : "false") << '\n';

  // ---------- 查找 ----------
  std::cout << "Position of 'String' in sv1: " << sv1.find("String") << '\n';
  std::cout << "Position of 'z' in sv1: " << (sv1.find('z') == string_view::npos ? "npos" : "found")
            << '\n';  // 找不到 -> npos

  // ---------- contains ----------
  std::cout << "sv1 contains 'Hello'? " << (sv1.contains("Hello") ? "true" : "false") << '\n';
  std::cout << "sv1 contains 'World'? " << (sv1.contains("World") ? "true" : "false") << '\n';

  // ---------- starts_with / ends_with ----------
  std::cout << "sv1 starts with 'Hello'? " << (sv1.starts_with("Hello") ? "true" : "false") << '\n';
  std::cout << "sv1 ends with 'View!'? " << (sv1.ends_with("View!") ? "true" : "false") << '\n';
  std::cout << "sv1 ends with 'Example'? " << (sv1.ends_with("Example") ? "true" : "false") << '\n';

  // ---------- to_string ----------
  std::string s = sv4.to_string();
  std::cout << "sv4.to_string(): " << s << '\n';

  // ---------- remove_prefix / remove_suffix ----------
  string_view temp = sv1;
  temp.remove_prefix(7);  // 去掉 "Hello, "
  std::cout << "After remove_prefix(7): " << temp << '\n';
  temp.remove_suffix(6);  // 去掉末尾 "View!"
  std::cout << "After remove_suffix(6): " << temp << '\n';

  return 0;
}