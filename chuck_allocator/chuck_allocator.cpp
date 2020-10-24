#include <iostream>
#include <vector>

size_t CHUNK_SIZE = 100;

template <typename T>
class Allocator {
 private:
  template <typename U>
  struct Chunk {
    size_t size;
    U* ptr = nullptr;
    Chunk* next;
  };

  Chunk<T>* firstChunk;
  static unsigned memberCount;

  Chunk<T>* allocateChunk() {
    Chunk<T>* chunk = new Chunk<T>;
    chunk->ptr = reinterpret_cast<T*>(new char[CHUNK_SIZE * sizeof(T)]);
    chunk->size = CHUNK_SIZE;
    chunk->next = nullptr;
    return chunk;
  };

 public:
  typedef T value_type;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef std::true_type propagate_on_container_move_assignment;
  template <class U>
  struct rebind {
    typedef Allocator<U> other;
  };
  typedef std::true_type is_always_equal;

  Allocator() {
    this->firstChunk = nullptr;
    memberCount = 1;
  };

  Allocator<T>& operator=(const Allocator& other) {
    if (&other == this) {
      return *this;
    }
    this->firstChunk = other.firstChunk;
    ++memberCount;
    return *this;
  };

  Allocator(const Allocator& copy) { *this = copy; };

  ~Allocator() {
    memberCount--;
    if (memberCount == 0) {
      Chunk<T>* chunk = this->firstChunk;
      bool destroyed = false;
      while (destroyed != true) {
        if (chunk == nullptr) {
          destroyed = true;
        } else {
          Chunk<T>* to_del = chunk;
          chunk = chunk->next;
          delete[] reinterpret_cast<char*>(to_del);
        }
      }
    }
  };

  T* allocate(const size_t n) {
    if (this->firstChunk == nullptr) {
      this->firstChunk = allocateChunk();
    }
    Chunk<T>* chunk = this->firstChunk;
    T* mem_ptr;
    bool allocated = false;
    while (!allocated) {
      if (chunk->size >= n) {
        mem_ptr = chunk->ptr;
        chunk->ptr += n * sizeof(T);
        chunk->size -= n;
        allocated = true;
      } else if (chunk->next != nullptr) {
        chunk = chunk->next;
      } else {
        chunk->next = allocateChunk();
      }
    }
    return mem_ptr;
  };

  void deallocate(T* p, const size_t n){};

  template <typename... Args>
  void construct(T* p, const Args&&... args) {
    new (p) T(args...);
  };

  void destroy(T* p) { p->~T(); };
};