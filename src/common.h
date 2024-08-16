#pragma once

#include <cstdlib>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <variant>

namespace tmonkey {

#define NO_COPYABLE(CLASS_NAME)           \
  CLASS_NAME(const CLASS_NAME&) = delete; \
  auto operator=(const CLASS_NAME&)->CLASS_NAME& = delete;

#define NO_MOVABLE(CLASS_NAME)                \
  CLASS_NAME(CLASS_NAME&&) noexcept = delete; \
  auto operator=(CLASS_NAME&&) noexcept -> CLASS_NAME&& = delete;

#define ASSERT_NO_NULLPTR(PTR) static_assert(!std::is_null_pointer_v<decltype(PTR)>);

class Arena {
  static constexpr size_t kDefaultBlockSize = 4096;
  static constexpr size_t kAlignment = alignof(std::max_align_t);

public:
  explicit Arena() {}

  NO_COPYABLE(Arena)
  NO_MOVABLE(Arena)

  ~Arena() {
    for (auto* b : blocks_) {
      delete[] b;
    }
  }

  auto alloc(size_t bytes) -> char* {
    auto alignmentBytes = (bytes + (kAlignment - 1)) & ~(kAlignment - 1);

    if (alignmentBytes > bytesRemaining_) {
      auto size = std::max(alignmentBytes, kDefaultBlockSize);
      blocks_.push_back(new char[size]);
      bytesRemaining_ = size;
      bytesAllocated_ = 0;
    }

    auto* block = blocks_.back() + bytesAllocated_;
    bytesRemaining_ -= alignmentBytes;
    bytesAllocated_ += alignmentBytes;

    return block;
  }

private:
  size_t bytesAllocated_ = 0;
  size_t bytesRemaining_ = 0;
  std::vector<char*> blocks_;
};

class StringInterningMap {
public:
  StringInterningMap() = default;

  NO_COPYABLE(StringInterningMap)

  StringInterningMap(StringInterningMap&& other) noexcept {
    if (&other != this) {
      map_ = std::move(other.map_);
      vec_ = std::move(other.vec_);
    }
  }

  auto intern(std::string_view s) -> size_t {
    if (auto it = map_.find(s); it != map_.end()) {
      return it->second;
    }
    vec_.push_back(std::make_unique<std::string>(s));
    auto idx = vec_.size() - 1;
    map_[s] = idx;
    return idx;
  }

  auto string(size_t idx) -> std::optional<std::string_view> {
    if (idx >= vec_.size()) {
      return {};
    }
    return std::string_view(*vec_[idx]);
  }

private:
  std::unordered_map<std::string_view, size_t> map_;
  std::vector<std::unique_ptr<std::string>> vec_;
};

}  // namespace tmonkey