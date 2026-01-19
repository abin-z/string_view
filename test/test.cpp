#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "abin/string_view.h"

using abin::string_view;

TEST_CASE("string_view basic")
{
  abin::string_view sv = "hello";
  REQUIRE(sv.size() == 5);
}

TEST_CASE("string_view construction")
{
  const char *cstr = "hello world";
  std::string str = "hello world";

  abin::string_view sv1(cstr);
  abin::string_view sv2(str);
  abin::string_view sv3("example");

  REQUIRE(sv1.size() == 11);
  REQUIRE(sv2.size() == 11);
  REQUIRE(sv3.size() == 7);

  REQUIRE(sv1.data() == cstr);
  REQUIRE(sv2.data() == str.data());
}

TEST_CASE("string_view element access")
{
  abin::string_view sv = "abc";
  REQUIRE(sv[0] == 'a');
  REQUIRE(sv[1] == 'b');
  REQUIRE(sv[2] == 'c');
  REQUIRE_THROWS_AS(sv.at(3), std::out_of_range);

  REQUIRE(sv.front() == 'a');
  REQUIRE(sv.back() == 'c');
}

TEST_CASE("string_view comparison")
{
  abin::string_view sv1 = "abc";
  abin::string_view sv2 = "abc";
  abin::string_view sv3 = "abd";

  REQUIRE(sv1 == sv2);
  REQUIRE(sv1 != sv3);
  REQUIRE(sv1 < sv3);
  REQUIRE(sv3 > sv1);
  REQUIRE(sv1 <= sv2);
  REQUIRE(sv1 >= sv2);
}

TEST_CASE("string_view find")
{
  using abin::string_view;
  string_view sv = "hello world";

  REQUIRE(sv.find('h') == 0);
  REQUIRE(sv.find('o') == 4);
  REQUIRE(sv.find('z') == string_view::npos);

  REQUIRE(sv.find("hello") == 0);
  REQUIRE(sv.find("world") == 6);
  REQUIRE(sv.find("abc") == string_view::npos);

  REQUIRE(sv.rfind('l') == 9);
  REQUIRE(sv.rfind("l") == 9);
  REQUIRE(sv.rfind("hello") == 0);
  REQUIRE(sv.rfind("abc") == string_view::npos);
}

TEST_CASE("string_view contains2")
{
  using abin::string_view;
  string_view sv = "hello world";

  REQUIRE(sv.contains('h'));
  REQUIRE(!sv.contains('z'));
  REQUIRE(sv.contains("hello"));
  REQUIRE(!sv.contains("abc"));
  REQUIRE(sv.contains("world"));
}

TEST_CASE("string_view starts_with / ends_with")
{
  using abin::string_view;
  string_view sv = "hello world";

  REQUIRE(sv.starts_with('h'));
  REQUIRE(!sv.starts_with('x'));
  REQUIRE(sv.starts_with("hello"));
  REQUIRE(!sv.starts_with("world"));

  REQUIRE(sv.ends_with('d'));
  REQUIRE(!sv.ends_with('x'));
  REQUIRE(sv.ends_with("world"));
  REQUIRE(!sv.ends_with("hello"));
}

TEST_CASE("string_view substr and remove_prefix/suffix")
{
  using abin::string_view;
  string_view sv = "hello world";

  auto sub = sv.substr(0, 5);
  REQUIRE(sub.size() == 5);
  REQUIRE(sub == "hello");

  string_view temp = sv;
  temp.remove_prefix(6);
  REQUIRE(temp == "world");

  temp.remove_suffix(3);
  REQUIRE(temp == "wo");
}

TEST_CASE("string_view find_first_of / find_first_not_of")
{
  using abin::string_view;
  string_view sv = "abcde";

  REQUIRE(sv.find_first_of('c') == 2);
  REQUIRE(sv.find_first_of("xyzabc") == 0);
  REQUIRE(sv.find_first_of("xyz") == string_view::npos);

  REQUIRE(sv.find_first_not_of('a') == 1);
  REQUIRE(sv.find_first_not_of("a") == 1);
  REQUIRE(sv.find_first_not_of("abcde") == string_view::npos);
}

TEST_CASE("string_view to_string")
{
  using abin::string_view;
  string_view sv = "hello";
  std::string s = sv.to_string();
  REQUIRE(s == "hello");
}

TEST_CASE("string_view constructors")
{
  const char *cstr = "hello";
  std::string str = "world";

  string_view sv1;           // 默认构造
  string_view sv2(cstr);     // const char*
  string_view sv3(cstr, 5);  // const char*, length
  string_view sv4(str);      // std::string
  string_view sv5(sv4);      // copy constructor

  REQUIRE(sv1.empty());
  REQUIRE(sv2.size() == 5);
  REQUIRE(sv3.size() == 5);
  REQUIRE(sv4.size() == 5);
  REQUIRE(sv5 == sv4);
}

TEST_CASE("string_view capacity")
{
  string_view sv = "abc";
  REQUIRE(sv.size() == 3);
  REQUIRE(sv.length() == 3);
  REQUIRE(sv.max_size() == string_view::npos);
  REQUIRE(!sv.empty());

  string_view empty_sv;
  REQUIRE(empty_sv.empty());
}

TEST_CASE("string_view comparison2")
{
  string_view sv1 = "abc";
  string_view sv2 = "abc";
  string_view sv3 = "abd";

  REQUIRE(sv1 == sv2);
  REQUIRE(sv1 != sv3);
  REQUIRE(sv1 < sv3);
  REQUIRE(sv3 > sv1);
  REQUIRE(sv1 <= sv2);
  REQUIRE(sv1 >= sv2);

  REQUIRE(sv1.compare(sv2) == 0);
  REQUIRE(sv1.compare(sv3) < 0);
  REQUIRE(sv3.compare(sv1) > 0);

  REQUIRE(sv1.compare(0, 2, sv3) < 0);  // compare "ab" vs "ab"
  REQUIRE(sv1.compare(0, 3, "abc") == 0);
}

TEST_CASE("string_view find / rfind")
{
  string_view sv = "hello world";

  // 单字符查找
  REQUIRE(sv.find('h') == 0);
  REQUIRE(sv.find('o') == 4);
  REQUIRE(sv.find('z') == string_view::npos);

  // 子串查找
  REQUIRE(sv.find("hello") == 0);
  REQUIRE(sv.find("world") == 6);
  REQUIRE(sv.find("abc") == string_view::npos);

  // rfind
  REQUIRE(sv.rfind('l') == 9);
  REQUIRE(sv.rfind("l") == 9);
  REQUIRE(sv.rfind("hello") == 0);
  REQUIRE(sv.rfind("abc") == string_view::npos);

  // C 字符串查找
  REQUIRE(sv.find("world", 0) == 6);
  REQUIRE(sv.rfind("world", sv.size() - 1) == 6);
}

TEST_CASE("string_view find_first_of / find_first_not_of2")
{
  string_view sv = "abcde";

  REQUIRE(sv.find_first_of('c') == 2);
  REQUIRE(sv.find_first_of("xyzabc") == 0);
  REQUIRE(sv.find_first_of("xyz") == string_view::npos);

  REQUIRE(sv.find_first_not_of('a') == 1);
  REQUIRE(sv.find_first_not_of("a") == 1);
  REQUIRE(sv.find_first_not_of("abcde") == string_view::npos);

  // C 字符串重载
  REQUIRE(sv.find_first_of("cde", 0, 3) == 2);
  REQUIRE(sv.find_first_not_of("ab", 0, 2) == 2);
}

TEST_CASE("string_view contains")
{
  string_view sv = "hello world";
  std::string s1 = "hello";
  const char *cstr = "world";

  REQUIRE(sv.contains('h'));
  REQUIRE(!sv.contains('x'));
  REQUIRE(sv.contains("hello"));
  REQUIRE(!sv.contains("abc"));
  REQUIRE(sv.contains(s1));
  REQUIRE(sv.contains(cstr));
}

TEST_CASE("string_view starts_with / ends_with2")
{
  string_view sv = "hello world";

  REQUIRE(sv.starts_with('h'));
  REQUIRE(!sv.starts_with('x'));
  REQUIRE(sv.starts_with("hello"));
  REQUIRE(!sv.starts_with("world"));

  REQUIRE(sv.ends_with('d'));
  REQUIRE(!sv.ends_with('x'));
  REQUIRE(sv.ends_with("world"));
  REQUIRE(!sv.ends_with("hello"));

  // C 字符串
  REQUIRE(sv.starts_with("hello"));
  REQUIRE(sv.ends_with("world"));
}

TEST_CASE("string_view substr / remove_prefix / remove_suffix2")
{
  string_view sv = "hello world";

  auto sub = sv.substr(0, 5);
  REQUIRE(sub == "hello");

  string_view temp = sv;
  temp.remove_prefix(6);
  REQUIRE(temp == "world");

  temp.remove_suffix(3);
  REQUIRE(temp == "wo");

  // remove 超过长度
  temp.remove_prefix(10);
  REQUIRE(temp.empty());
}

TEST_CASE("string_view swap2")
{
  string_view sv1 = "abc";
  string_view sv2 = "def";

  sv1.swap(sv2);
  REQUIRE(sv1 == "def");
  REQUIRE(sv2 == "abc");
}

TEST_CASE("string_view to_string2")
{
  string_view sv = "hello";
  std::string s = sv.to_string();
  REQUIRE(s == "hello");
}

TEST_CASE("string_view ostream output2")
{
  string_view sv = "abc";
  std::ostringstream oss;
  oss << sv;
  REQUIRE(oss.str() == "abc");
}