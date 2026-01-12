/**
 * @file string_view.h
 * @brief 简易的 string_view 实现
 * @author abin
 * @date 2026-01-12
 */

#pragma once

#include <algorithm>
#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <string>

namespace abin
{

// ---------- string_view 类 ----------
class string_view
{
 public:
  using value_type = char;
  using size_type = std::size_t;
  using const_iterator = const char *;

 private:
  const char *data_;
  size_type size_;

 public:
  // ---------- 构造 ----------
  constexpr string_view() noexcept : data_(nullptr), size_(0) {}

  constexpr string_view(const char *str, size_type len) noexcept : data_(str), size_(len) {}

  explicit string_view(const std::string &str) noexcept : data_(str.data()), size_(str.size()) {}

  explicit string_view(const char *str) noexcept :
    data_(str), size_((str != nullptr) ? std::char_traits<char>::length(str) : 0)
  {
  }
  string_view(nullptr_t) = delete;

  // ---------- 访问 ----------
  constexpr const char *data() const noexcept
  {
    return data_;
  }
  constexpr size_type size() const noexcept
  {
    return size_;
  }
  constexpr bool empty() const noexcept
  {
    return size_ == 0;
  }

  const char &operator[](size_type pos) const
  {
    return data_[pos];
  }

  const char &at(size_type pos) const
  {
    if (pos >= size_) throw std::out_of_range("string_view::at");
    return data_[pos];
  }

  // ---------- 子串 ----------
  string_view substr(size_type pos, size_type count = std::string::npos) const
  {
    if (pos > size_) throw std::out_of_range("string_view::substr");
    count = std::min(count, size_ - pos);
    return {data_ + pos, count};
  }

  // ---------- 修改视图 ----------
  void remove_prefix(size_type n)
  {
    n = std::min(n, size_);
    data_ += n;
    size_ -= n;
  }

  void remove_suffix(size_type n)
  {
    n = std::min(n, size_);
    size_ -= n;
  }

  // ---------- 迭代器 ----------
  const_iterator begin() const noexcept
  {
    return data_;
  }
  const_iterator end() const noexcept
  {
    return data_ + size_;
  }

  // ---------- 比较 ----------
  bool operator==(const string_view &other) const noexcept
  {
    return size_ == other.size_ && std::equal(data_, data_ + size_, other.data_);
  }

  bool operator!=(const string_view &other) const noexcept
  {
    return !(*this == other);
  }
};

// ---------- 流输出 ----------
inline std::ostream &operator<<(std::ostream &os, const string_view &sv)
{
  return os.write(sv.data(), sv.size());
}

// ---------- 转 std::string ----------
inline std::string to_string(const string_view &sv)
{
  return {sv.data(), sv.size()};
}

// ---------- 字面量操作符 (_sv) ----------
inline namespace literals
{
inline namespace string_view_literals
{
constexpr string_view operator""_sv(const char *str, std::size_t len)
{
  return {str, len};
}
}  // namespace string_view_literals
}  // namespace literals

}  // namespace abin
