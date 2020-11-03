#include <iostream>
#include <memory>
#include <vector>

size_t CHUNK_SIZE = 10;

template <typename T>
struct Chunk {
  size_t size;
  T* ptr;

  Chunk() : size(0), ptr(nullptr){};
};

template <typename T>
struct Node {
  Node* next;
  Chunk<T>* chunk;

  Node() : next(nullptr), chunk(nullptr){};
  Node(const Chunk<T>& chunk) : next(nullptr), chunk(chunk){};
};

template <typename T>
struct List {
  Node<T>* begin;
  Node<T>* tail;
  size_t size;

  List() : begin(nullptr), tail(nullptr), size(0){};

  void insert() {
    Chunk<T>* chunk = new Chunk<T>;
    chunk->ptr = reinterpret_cast<T*>(new char[CHUNK_SIZE * sizeof(T)]);
    chunk->size = CHUNK_SIZE;
    Node<T>* node = new Node<T>;
    node->chunk = chunk;
    if (this->size == 0) {
      this->begin = node;
      this->tail = node;
    } else {
      this->tail->next = node;
      this->tail = node;
    }
    ++this->size;
  };

  void erase() {
    delete[] reinterpret_cast<char*>(this->begin->chunk);
    if (this->size == 1) {
      delete[] this->begin;
      this->begin = this->tail = nullptr;
    } else if (this->size > 1) {
      Node<T>* node = this->begin->next;
      delete[] this->begin;
      this->begin = node;
    }
    --this->size;
  };
};

template <typename T>
class Allocator {
 private:
  std::shared_ptr<List<T>> list_ptr;

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
    this->list_ptr = *(new std::shared_ptr<List<T>>(new List<T>()));
  };

  Allocator<T>& operator=(const Allocator& other) {
    if (&other == this) {
      return *this;
    }
    this->list_ptr = other.list_ptr;
    return *this;
  };

  Allocator(const Allocator& copy) { *this = copy; };

  ~Allocator() {
    while (this->list_ptr->begin != nullptr) {
      this->list_ptr->erase();
    }
  };

  T* allocate(size_t n) {
    if (list_ptr->begin == nullptr) {
      list_ptr->insert();
    }
    Node<T>* node = list_ptr->begin;
    pointer mem_ptr;
    bool allocated = false;
    while (!allocated) {
      if (node->chunk->size >= n) {
        mem_ptr = node->chunk->ptr;
        node->chunk->ptr += n * sizeof(T);
        node->chunk->size -= n;
        allocated = true;
      } else if (node->next != nullptr) {
        node = node->next;
      } else {
        list_ptr->insert();
        node = node->next;
      }
    }
    return mem_ptr;
  };

  void deallocate(T* p, const size_t n){};

  template <typename... Args>
  void construct(T* p, Args&&... args) {
    ::new ((void*)p) T(std::forward<Args>(args)...);
  };

  void destroy(T* p) { p->~T(); };
};