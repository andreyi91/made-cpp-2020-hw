#pragma once

namespace task {

template <class T>
class UniquePtr {
 private:
  T* ptr;

 public:
  explicit UniquePtr(T* ptr) noexcept : ptr(ptr){};

  UniquePtr(UniquePtr<T>&& other) noexcept : ptr(other.ptr) {
    other.ptr = nullptr;
  }

  UniquePtr<T>& operator=(UniquePtr<T>&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    delete ptr;
    ptr = other.ptr;
    other.ptr = nullptr;
    return *this;
  }

  ~UniquePtr() { delete ptr; };

  T& operator*() { return *ptr; };

  T* operator->() { return ptr; };

  T* get() const noexcept { return ptr; };

  T* release() noexcept {
    T* p = ptr;
    ptr = nullptr;
    return p;
  };

  void reset(T* p = nullptr) noexcept {
    delete ptr;
    ptr = p;
  };

  void swap(UniquePtr<T>& other) noexcept {
    T c{std::move(this->ptr)};
    this->ptr = std::move(other.ptr);
    other.ptr = std::move(c);
  };
};

template <class T>
struct ControlBlock {
  T* ptr;
  size_t shrd_count;
  size_t weak_count;
};

template <class T>
class WeakPtr;

template <class T>
class SharedPtr {
 public:
  ControlBlock<T>* cb;

  explicit SharedPtr(T* ptr = nullptr) {
    cb = new ControlBlock<T>;
    cb->weak_count = 0;
    cb->ptr = ptr;
    cb->shrd_count = 1;
  };

  ~SharedPtr() {
    if (cb->shrd_count == 1) {
      if (cb->weak_count == 0) {
        delete cb;
      } else {
        T* tmp = cb->ptr;
        cb->ptr = nullptr;
        delete tmp;
      }
    }
    --cb->shrd_count;
  };

  SharedPtr(const SharedPtr<T>& other) : cb(other.cb) { ++cb->shrd_count; };

  SharedPtr(const WeakPtr<T>& other) {
    cb = other.cb;
    ++cb->shrd_count;
  };

  SharedPtr(SharedPtr<T>&& other) noexcept : cb(std::move(other.cb)) {
    ++cb->shrd_count;
  };

  SharedPtr<T>& operator=(const SharedPtr<T>& other) noexcept {
    this->~SharedPtr<T>();
    cb = other.cb;
    ++other.cb->shrd_count;
    return *this;
  };

  SharedPtr<T>& operator=(const SharedPtr<T>&& other) {
    if (&other == this) {
      return *this;
    }
    SharedPtr<T>(std::move(other)).swap(*this);
    return *this;
  };

  T& operator*() const { return *cb->ptr; }
  T* operator->() const { return cb->ptr; }

  T* get() const noexcept { return cb->ptr; };

  size_t use_count() const noexcept { return cb->shrd_count; };

  void reset(T* p = nullptr) noexcept {
    this->~SharedPtr<T>();
    *this = SharedPtr<T>(p);
  };

  void swap(SharedPtr<T>& other) noexcept {
    ControlBlock<T>* c = std::move(this->cb);
    this->cb = std::move(other.cb);
    other.cb = std::move(c);
  };
};

template <class T>
class WeakPtr {
 public:
  ControlBlock<T>* cb = nullptr;
  WeakPtr(){};

  WeakPtr(const SharedPtr<T>& other) noexcept : cb(other.cb) {
    ++cb->weak_count;
  };

  explicit WeakPtr(const WeakPtr<T>& other) noexcept : cb(other.cb) {
    ++cb->weak_count;
  };

  WeakPtr(WeakPtr<T>&& other) : cb(std::move(other.cb)) {
    other.cb = new ControlBlock<T>;
    other.cb->ptr = nullptr;
    other.cb->shrd_count = 0;
    other.cb->weak_count = 0;
  };

  WeakPtr<T>& operator=(const WeakPtr<T>& other) noexcept {
    cb->ptr = other.cb->ptr;
    cb->shrd_count = other.cb->shrd_count;
    cb->weak_count = other.cb->weak_count;
    ++cb->weak_count;
    return *this;
  };

  WeakPtr<T>& operator=(const SharedPtr<T>& other) noexcept {
    cb = other.cb;
    ++cb->weak_count;
    return *this;
  };

  WeakPtr<T>& operator=(const WeakPtr<T>&& other) noexcept {
    if (&other == this) {
      return *this;
    }
    cb = other.cb;
    other.cb = nullptr;
    return *this;
  };

  ~WeakPtr() {
    if (cb != nullptr) {
      if (cb->weak_count > 0) {
        --cb->weak_count;
      }
    }
  };

  bool expired() const noexcept {
    if (cb->shrd_count == 0) {
      return true;
    } else {
      return false;
    }
  };

  size_t use_count() const noexcept { return cb->shrd_count; };

  SharedPtr<T> lock() const noexcept {
    if (expired()) {
      return SharedPtr<T>();
    } else {
      return SharedPtr<T>(*this);
    }
  };

  void reset() noexcept {
    --cb->weak_count;
    cb = nullptr;
  };

  void swap(WeakPtr& other) noexcept {
    ControlBlock<T>* c{std::move(this->cb)};
    this->cb = std::move(other.cb);
    other.cb = std::move(c);
  };
};

// Your template function definitions may go here...

}  // namespace task

#include "smart_pointers.tpp"
