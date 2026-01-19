# abin::string_view

`abin::string_view` 是一个轻量级、非拥有型的字符串视图类，模仿 C++17 的 `std::string_view`，用于高效访问已有的字符串数据而不做拷贝。

完全在C++11标准下实现, 支持 `const char*`、`std::string` 和 `string_view` 的无拷贝访问。

## 设计理念

- **非拥有型**：不管理字符串内存，只持有指针和长度，轻量高效。  
- **零拷贝访问**：直接操作已有字符串数据，无需额外拷贝。  
- **高效轻量**：内部仅使用 `const char*` 和 `size_t` 存储，性能开销极低。  
- **安全便捷**：提供越界检查访问（`at()`）及丰富的查找、比较、子串操作。  
- **标准兼容**：可直接与 `std::ostream` 一起使用，支持输出。  
- **哈希支持**：提供 `std::hash` 特化，可用于 `unordered_map` 或 `unordered_set`。

## 使用

将 `string_view.h` 头文件包含到你的项目中即可，无需额外依赖。
支持 C++11 及以上版本。

## 使用示例

```cpp
#include "string_view.h"
#include <iostream>
#include <string>

int main() {
    abin::string_view sv1 = "Hello, world!";
    std::string str = "world";

    // 输出
    std::cout << sv1 << std::endl;

    // 查找
    size_t pos = sv1.find("world");
    if (pos != abin::string_view::npos) {
        std::cout << "Found at position: " << pos << std::endl;
    }

    // 判断包含
    if (sv1.contains('H') && sv1.contains(str)) {
        std::cout << "sv1 contains H and " << str << std::endl;
    }

    // 子串
    abin::string_view sub = sv1.substr(7, 5); // "world"
    std::cout << "Substring: " << sub << std::endl;

    // 转 std::string
    std::string s = sub.to_string();
    std::cout << "std::string: " << s << std::endl;

    return 0;
}
```

## API 接口概览

### 构造函数

```cpp
string_view();
string_view(const char* s);
string_view(const char* s, size_t n);
string_view(const std::string &s);
string_view(const string_view &other);
```

### 容量和元素访问

```cpp
size_t size() const noexcept;
bool empty() const noexcept;
const char& operator[](size_t pos) const noexcept;
const char& at(size_t pos) const;
const char& front() const noexcept;
const char& back() const noexcept;
const char* data() const noexcept;
```

### 子串和修改

```cpp
string_view substr(size_t pos, size_t count = npos) const;
void remove_prefix(size_t n) noexcept;
void remove_suffix(size_t n) noexcept;
void swap(string_view &other) noexcept;
std::string to_string() const;
```

### 查找

```cpp
size_t find(char c, size_t pos = 0) const noexcept;
size_t find(string_view sv, size_t pos = 0) const noexcept;
size_t rfind(char c, size_t pos = npos) const noexcept;
size_t rfind(string_view sv, size_t pos = npos) const noexcept;
size_t find_first_of(char c, size_t pos = 0) const noexcept;
size_t find_first_of(string_view sv, size_t pos = 0) const noexcept;
size_t find_first_not_of(char c, size_t pos = 0) const noexcept;
size_t find_first_not_of(string_view sv, size_t pos = 0) const noexcept;
```

### 检查

```cpp
bool contains(char c) const noexcept;
bool contains(string_view sv) const noexcept;
bool starts_with(char c) const noexcept;
bool starts_with(string_view sv) const noexcept;
bool ends_with(char c) const noexcept;
bool ends_with(string_view sv) const noexcept;
```

### 比较

```cpp
int compare(string_view sv) const noexcept;
bool operator==(string_view lhs, string_view rhs) noexcept;
bool operator!=(string_view lhs, string_view rhs) noexcept;
bool operator<(string_view lhs, string_view rhs) noexcept;
bool operator<=(string_view lhs, string_view rhs) noexcept;
bool operator>(string_view lhs, string_view rhs) noexcept;
bool operator>=(string_view lhs, string_view rhs) noexcept;
```

### 流输出

```cpp
std::ostream &operator<<(std::ostream &os, string_view sv);
```

## 测试

使用 Catch2 编写了完整的单元测试，覆盖构造、比较、查找、检查、修改等所有功能。