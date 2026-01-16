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
class string_view  // NOLINT(cppcoreguidelines-special-member-functions)
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

  // Precondition: str != nullptr
  string_view(const char *str, size_type len) noexcept : data_(str), size_(len) {}
  string_view(const char *str) : data_(str), size_(traits_type::length(str)) {}  // NOLINT(google-explicit-constructor)

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
  const_reference operator[](size_type pos) const noexcept
  {
    return data_[pos];
  }

  const_reference at(size_type pos) const
  {
    if (pos >= size_) throw std::out_of_range("abin::string_view::at");
    return data_[pos];
  }
  // If empty() is true, the behavior is undefined.
  const_reference front() const noexcept
  {
    return data_[0];
  }
  // If empty() is true, the behavior is undefined.
  const_reference back() const noexcept
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

  /**
   * @brief 查找单个字符 c，从位置 pos 开始
   * @param c 要查找的字符
   * @param pos 起始位置（默认 0）
   * @return 返回首次出现的位置，如果找不到则返回 npos
   * @note 如果 pos >= size()，直接返回 npos
   */
  size_type find(value_type c, size_type pos = 0) const noexcept
  {
    if (pos >= size_) return npos;
    for (size_type i = pos; i < size_; ++i)
    {
      if (data_[i] == c) return i;
    }
    return npos;
  }

  /**
   * @brief 查找子串 sv，从位置 pos 开始
   * @param sv 要查找的子串
   * @param pos 起始位置（默认 0）
   * @return 返回首次匹配的起始位置，如果找不到则返回 npos
   * @note 空子串总是匹配当前位置，如果 pos == size()，也返回 size()
   * @note 如果 pos > size() 或剩余长度不足以匹配子串，则返回 npos
   */
  size_type find(string_view sv, size_type pos = 0) const noexcept
  {
    if (pos > size_) return npos;             // 越界检查
    if (sv.size_ == 0) return pos;            // 空子串匹配当前位置
    if (sv.size_ > size_ - pos) return npos;  // 剩余长度不足

    for (size_type i = pos; i <= size_ - sv.size_; ++i)
    {
      if (traits_type::compare(data_ + i, sv.data_, sv.size_) == 0) return i;
    }
    return npos;
  }

  /**
   * @brief 查找 C 字符串 s（长度为 n）从 pos 开始
   * @param s 要查找的字符数组
   * @param pos 起始位置
   * @param n s 的长度
   * @return 返回首次匹配的起始位置，如果找不到则返回 npos
   * @note 直接包装 string_view 查找逻辑
   */
  size_type find(const char *s, size_type pos, size_type n) const
  {
    return find(string_view(s, n), pos);
  }

  /**
   * @brief 查找 null 结尾 C 字符串 s 从 pos 开始
   * @param s 要查找的 C 字符串
   * @param pos 起始位置（默认 0）
   * @return 返回首次匹配的起始位置，如果找不到则返回 npos
   * @note 直接包装 string_view 查找逻辑
   */
  size_type find(const char *s, size_type pos = 0) const
  {
    return find(string_view(s), pos);
  }

  /**
   * @brief 从末尾向前查找单个字符 c，从 pos 位置开始（pos 默认是末尾）
   * @param c 要查找的字符
   * @param pos 起始搜索位置（默认 size() - 1，即从末尾开始）
   * @return 返回最后一次匹配的位置，如果找不到返回 npos
   * @note 如果 pos >= size()，搜索从 size() - 1 开始
   */
  size_type rfind(value_type c, size_type pos = npos) const noexcept
  {
    if (size_ == 0) return npos;
    // 如果 pos 超过末尾，取末尾位置
    if (pos >= size_) pos = size_ - 1;
    for (size_type i = pos + 1; i-- > 0;)  // 注意 i-- > 0 防止溢出
    {
      if (data_[i] == c) return i;
      if (i == 0) break;  // 防止无符号 size_type 变为最大值
    }
    return npos;
  }

  /**
   * @brief 从末尾向前查找子串 sv，从 pos 位置开始
   * @param sv 要查找的子串
   * @param pos 起始搜索位置（默认 size() - 1，即从末尾开始）
   * @return 返回最后一次匹配的起始位置，如果找不到返回 npos
   * @note 空子串总是匹配当前位置，如果 pos >= size()，从 size() 开始
   */
  size_type rfind(string_view sv, size_type pos = npos) const noexcept
  {
    if (sv.size_ == 0) return std::min(pos, size_);  // 空子串匹配当前位置
    if (sv.size_ > size_) return npos;               // 子串比主串长 → 不可能匹配

    // 搜索起始位置
    pos = std::min(pos, size_ - sv.size_);

    for (size_type i = pos + 1; i-- > 0;)  // 从 pos 向前查找
    {
      if (traits_type::compare(data_ + i, sv.data_, sv.size_) == 0) return i;
      if (i == 0) break;  // 防止 size_type 溢出
    }
    return npos;
  }

  /**
   * @brief 从末尾向前查找 C 字符串 s（长度为 n）从 pos 开始
   * @param s 要查找的字符数组
   * @param pos 起始位置
   * @param n s 的长度
   * @return 返回最后一次匹配的起始位置，如果找不到返回 npos
   */
  size_type rfind(const char *s, size_type pos, size_type n) const noexcept
  {
    return rfind(string_view(s, n), pos);
  }

  /**
   * @brief 从末尾向前查找 null 结尾 C 字符串 s 从 pos 开始
   * @param s 要查找的 C 字符串
   * @param pos 起始位置（默认 size() - 1）
   * @return 返回最后一次匹配的起始位置，如果找不到返回 npos
   */
  size_type rfind(const char *s, size_type pos = npos) const noexcept
  {
    return rfind(string_view(s), pos);
  }

  // ---------- Modifiers ----------
  void remove_prefix(size_type n) noexcept
  {
    n = std::min(n, size_);
    data_ += n;
    size_ -= n;
  }

  void remove_suffix(size_type n) noexcept
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

// ---------- std 命名空间特化 ----------
namespace std
{
template <>
struct hash<abin::string_view>
{
  size_t operator()(const abin::string_view &sv) const noexcept
  {
    size_t h = 0;
    // 使用 unsigned char 而不是 char 的原因:
    // 1. 在 C++ 中, char 的 signedness 是实现定义的,
    //    可能是 signed char 或 unsigned char.
    //    如果 char 为 signed, 负值在转换为 size_t 时会生成大整数,
    //    导致同样的字符串在不同平台得到不同的 hash 值.
    // 2. 使用 unsigned char 可以保证每个字符在 0~255 之间,
    //    无论平台如何, hash 值都一致.
    // 3. 这样也可以安全处理任意字节序列, 包括非 ASCII 或二进制数据.
    for (unsigned char c : sv)
    {
      h = h * 131 + static_cast<size_t>(c);
    }
    return h;
  }
};
}  // namespace std
