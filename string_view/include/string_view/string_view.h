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
  static constexpr auto npos{static_cast<size_type>(-1)};

 private:
  const value_type *data_;
  size_type size_;

 public:
  // ---------- 构造 ----------
  string_view() noexcept : data_(nullptr), size_(0) {}

  string_view(const char *str, size_type len) noexcept : data_(str), size_(len) {}
  // Precondition: str != nullptr
  string_view(const char *str) : data_(str), size_(traits_type::length(str)) {}

  explicit string_view(const std::string &str) noexcept : data_(str.data()), size_(str.size()) {}

  string_view(const string_view &) noexcept = default;
  string_view(nullptr_t) = delete;

  string_view &operator=(const string_view &) noexcept = default;
  ~string_view() = default;

  // ---------- Capacity ----------
  size_type size() const noexcept
  {
    return size_;
  }
  size_type length() const noexcept
  {
    return size_;
  }
  size_type max_size() const noexcept
  {
    return npos;
  }
  bool empty() const noexcept
  {
    return size_ == 0;
  }

  // ---------- Element access ----------
  const_pointer data() const noexcept
  {
    return data_;
  }

  // If pos < size() is false, the behavior is undefined.
  const_reference operator[](size_type pos) const
  {
    return data_[pos];
  }

  const_reference at(size_type pos) const
  {
    if (pos >= size_) throw std::out_of_range("abin::string_view::at");
    return data_[pos];
  }
  // If empty() is true, the behavior is undefined.
  const_reference front() const
  {
    return data_[0];
  }
  // If empty() is true, the behavior is undefined.
  const_reference back() const
  {
    return data_[size_ - 1];
  }

  // ---------- Operations ----------
  size_type copy(char *dest, size_type count, size_type pos = 0) const
  {
    if (pos > size_) throw std::out_of_range("abin::string_view::copy");
    size_type rcount = std::min(count, size_ - pos);
    if (rcount > 0)
    {
      traits_type::copy(dest, data_ + pos, rcount);  // 等价于 memcpy
    }
    return rcount;
  }

  string_view substr(size_type pos, size_type count = string_view::npos) const
  {
    if (pos > size_) throw std::out_of_range("abin::string_view::substr");
    count = std::min(count, size_ - pos);
    return {data_ + pos, count};
  }

  // 与另一个 string_view 比较
  int compare(string_view other) const noexcept
  {
    size_type min_len = std::min(size_, other.size_);
    int r = traits_type::compare(data_, other.data_, min_len);
    if (r != 0) return r;
    if (size_ < other.size_) return -1;
    if (size_ > other.size_) return 1;
    return 0;
  }

  // 与 C 字符串比较
  int compare(const char *s) const
  {
    string_view sv(s);  // 利用已有的 string_view(const char*)
    return compare(sv);
  }

  // 子串比较
  int compare(size_type pos1, size_type count1, string_view sv) const
  {
    if (pos1 > size_) throw std::out_of_range("abin::string_view::compare");
    string_view sub1 = substr(pos1, count1);
    return sub1.compare(sv);
  }

  int compare(size_type pos1, size_type count1, string_view sv, size_type pos2, size_type count2) const
  {
    if (pos1 > size_) throw std::out_of_range("abin::string_view::compare");
    string_view sub1 = substr(pos1, count1);
    string_view sub2 = sv.substr(pos2, count2);
    return sub1.compare(sub2);
  }

  int compare(size_type pos1, size_type count1, const char *s) const
  {
    return compare(pos1, count1, string_view(s));
  }

  int compare(size_type pos1, size_type count1, const char *s, size_type n) const
  {
    return compare(pos1, count1, string_view(s, n));
  }

  // ---------- Modifiers ----------
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

  void swap(string_view &other) noexcept
  {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
  }

  // ---------- Iterators ----------
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
};

inline bool operator==(string_view lhs, string_view rhs) noexcept
{
  return lhs.compare(rhs) == 0;
}
inline bool operator!=(string_view lhs, string_view rhs) noexcept
{
  return !(lhs == rhs);
}
inline bool operator<(string_view lhs, string_view rhs) noexcept
{
  return lhs.compare(rhs) < 0;
}
inline bool operator<=(string_view lhs, string_view rhs) noexcept
{
  return lhs.compare(rhs) <= 0;
}
inline bool operator>(string_view lhs, string_view rhs) noexcept
{
  return lhs.compare(rhs) > 0;
}
inline bool operator>=(string_view lhs, string_view rhs) noexcept
{
  return lhs.compare(rhs) >= 0;
}

// ---------- 流输出 ----------
inline std::ostream &operator<<(std::ostream &os, string_view sv)
{
  if (sv.size() != 0) os.write(sv.data(), static_cast<std::streamsize>(sv.size()));
  return os;
}

// ---------- 转 std::string ----------
inline std::string to_string(string_view sv)
{
  return {sv.data(), sv.size()};
}

}  // namespace abin
