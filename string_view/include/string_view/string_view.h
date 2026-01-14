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
#include <utility>

namespace abin
{

// ---------- string_view 类 ----------
class string_view
{
 public:
  using traits_type = std::char_traits<char>;  // 目前仅支持 char 类型
  using value_type = char;                     // 目前仅支持 char 类型
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using reference = value_type &;
  using const_reference = const value_type &;
  using const_iterator = const value_type *;
  using iterator = const_iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = const_reverse_iterator;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  static constexpr size_type npos = static_cast<size_type>(-1);

 private:
  const value_type *data_;
  size_type size_;

 public:
  // ---------- 构造 ----------
  constexpr string_view() noexcept : data_(nullptr), size_(0) {}

  constexpr string_view(const char *str, size_type len) noexcept : data_(str), size_(len) {}

  explicit string_view(const std::string &str) noexcept : data_(str.data()), size_(str.size()) {}
  // Precondition: str != nullptr
  explicit string_view(const char *str) : data_(str), size_(traits_type::length(str)) {}

  constexpr string_view(const string_view &) noexcept = default;
  string_view(nullptr_t) = delete;

  constexpr string_view &operator=(const string_view &) noexcept = default;
  ~string_view() = default;

  // ---------- 访问 ----------
  constexpr const_pointer data() const noexcept
  {
    return data_;
  }
  constexpr size_type size() const noexcept
  {
    return size_;
  }
  constexpr size_type length() const noexcept
  {
    return size_;
  }
  constexpr size_type max_size() const noexcept
  {
    return npos;
  }
  constexpr bool empty() const noexcept
  {
    return size_ == 0;
  }

  // If pos < size() is false, the behavior is undefined.
  constexpr const_reference operator[](size_type pos) const
  {
    return data_[pos];
  }

  constexpr const_reference at(size_type pos) const
  {
    if (pos >= size_) throw std::out_of_range("abin::string_view::at");
    return data_[pos];
  }
  // If empty() is true, the behavior is undefined.
  constexpr const_reference front() const
  {
    return data_[0];
  }
  // If empty() is true, the behavior is undefined.
  constexpr const_reference back() const
  {
    return data_[size_ - 1];
  }

  // ---------- 子串 ----------
  constexpr string_view substr(size_type pos, size_type count = string_view::npos) const
  {
    if (pos > size_) throw std::out_of_range("abin::string_view::substr");
    count = std::min(count, size_ - pos);
    return {data_ + pos, count};
  }

  // ---------- 修改视图 ----------
  constexpr void remove_prefix(size_type n)
  {
    n = std::min(n, size_);
    data_ += n;
    size_ -= n;
  }

  constexpr void remove_suffix(size_type n)
  {
    n = std::min(n, size_);
    size_ -= n;
  }

  constexpr void swap(string_view &other) noexcept
  {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
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
  const_iterator cbegin() const noexcept
  {
    return data_;
  }
  const_iterator cend() const noexcept
  {
    return data_ + size_;
  }
  const_reverse_iterator rbegin() const noexcept
  {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator rend() const noexcept
  {
    return const_reverse_iterator(begin());
  }
  const_reverse_iterator crbegin() const noexcept
  {
    return const_reverse_iterator(end());
  }
  const_reverse_iterator crend() const noexcept
  {
    return const_reverse_iterator(begin());
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
  if (sv.size() != 0) os.write(sv.data(), sv.size());
  return os;
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
