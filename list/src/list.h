#pragma once
#include <iostream>
#include <iterator>

#define assert(expr, msg) \
  if (!expr) std::cerr << msg << std::endl;

namespace task {

size_t MAX_LIST_SIZE = 1'000'000'000;

template <class T, class Allocator = std::allocator<T>>
class list {
 private:
  template <typename U>
  struct Node {
    U val;
    Node* next;
    Node* prev;

    Node() = default;

    Node(const U& value) {
      // std::cout << "Node " << *val << std::endl;
      val = value;
    };
    Node(Node<U>&&) = default;
    // ~Node() = default;
  };

  typedef std::allocator_traits<Allocator> _traits;
  using _rebound_alloc = typename _traits::template rebind_alloc<Node<T>>;
  using _rebound_traits = typename _traits::template rebind_traits<Node<T>>;
  // _traits.rebind_traits<Node<T>> _rebound_traits;
  // _traits.rebind_alloc<Node<T>> _rebound_alloc;

 public:
  // typedef Node<T> value_type;
  typedef T value_type;
  typedef _rebound_alloc allocator_type;
  typedef typename _traits::size_type size_type;
  typedef typename _traits::difference_type difference_type;
  typedef typename _traits::pointer pointer_val;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef typename _traits::const_pointer const_pointer_val;

  typedef typename _rebound_traits::pointer pointer_node;

 public:
  class iterator {
   public:
    friend class list;
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::bidirectional_iterator_tag;

   private:
    pointer_node ptr;
    explicit iterator(pointer_node ptr) : ptr(ptr) {}
    pointer_node get_node_pointer() { return ptr; }
    // N& get_node_reference() { return *ptr; }
   public:
    iterator() = delete;
    iterator(const iterator& other) = default;
    ~iterator() = default;
    iterator& operator=(const iterator&) = default;

    iterator& operator++() {
      ptr = ptr->next;
      return *this;
    }
    iterator operator++(int) {
      iterator t(*this);
      ptr->next;
      return t;
    }
    reference operator*() const { return *(ptr->val); }
    pointer operator->() const { return ptr->val; }
    iterator& operator--() {
      ptr = ptr->prev;
      return *this;
    }
    iterator operator--(int) {
      iterator t(*this);
      ptr->prev;
      return t;
    }

    bool operator==(iterator other) const {
      return ptr == other.ptr;
    }  // may be change to:    bool operator== (const iterator &other) { return
       // ptr == other.ptr; }
    bool operator!=(iterator other) const { return ptr != other.ptr; }
  };

  class const_iterator {
   public:
    friend class list;
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = const T&;
    using iterator_category = std::bidirectional_iterator_tag;

   private:
    pointer_node ptr;
    explicit const_iterator(pointer_node ptr) : ptr(ptr) {}
    pointer_node get_node_pointer() { return ptr; }
    // N& get_node_reference() { return *ptr; }
   public:
    const_iterator() = delete;
    const_iterator(const const_iterator& other) = default;
    ~const_iterator() = default;
    const_iterator& operator=(const const_iterator&) = default;

    const_iterator& operator++() {
      ptr = ptr->next;
      return *this;
    }
    const_iterator operator++(int) {
      const_iterator t(*this);
      ptr->next;
      return t;
    }
    reference operator*() const { return *(ptr->val); }
    pointer operator->() const {
      pointer p;
      p = ptr->val;
      return p;
    }
    const_iterator& operator--() {
      ptr = ptr->prev;
      return *this;
    }
    const_iterator operator--(int) {
      const_iterator t(*this);
      ptr->prev;
      return t;
    }

    bool operator==(const_iterator other) const {
      return ptr == other.ptr;
    }  // may be change to:    bool operator== (const iterator &other) { return
       // ptr == other.ptr; }
    bool operator!=(const_iterator other) const { return ptr != other.ptr; }
  };

  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

 private:
  allocator_type _allocator;
  pointer_node _front;
  pointer_node _back;
  size_type _size;

  static void _allocate_node(allocator_type& allocator, pointer_node& ptr,
                             size_type n = 1) {
    ptr = _rebound_traits::allocate(allocator, n);
  }

  static void _clear_node(allocator_type& allocator, pointer_node& node

  ) {
    pointer_node tmp = node;
    node = node->prev;
    node->next = node->next->next;
    node->next->prev = node;
    _rebound_traits::destroy(allocator, tmp);
  }

  static void _clear_list(allocator_type& allocator, pointer_node& list_back,
                          size_type& list_size) noexcept {
    for (size_type i(0); i < list_size; i++) {
      _clear_node(allocator, list_back->prev);
    }
    list_size = 0;
  }

  static void _add_node(pointer_node& new_node, pointer_node& next_node) {
    // std::cout << "_add_node _front " << new_node << std::endl;
    new_node->next = next_node;
    new_node->prev = next_node->prev;
    next_node->prev = new_node;
    new_node->prev->next = new_node;
  }

  static void _copy_list(allocator_type& allocator,
                         pointer_node from_list_front, size_type from_list_size,
                         pointer_node& to_list_front,
                         pointer_node& to_list_back, size_type& to_list_size) {
    pointer_node ptr_from = from_list_front;
    pointer_node ptr_to;  // may be bad idea
    for (size_type i(0); i < from_list_size; ++i) {
      ptr_to = _rebound_traits::allocate(allocator, 1);
      _rebound_traits::construct(allocator, ptr_to, ptr_from->val);
      _add_node(ptr_to, to_list_back);
      ptr_from = ptr_from->next;

      if (to_list_size == 0) to_list_front = ptr_to;
      ++to_list_size;
    }
  }

  static void _move_list(allocator_type& from_allocator,
                         pointer_node& from_list_front,
                         pointer_node& from_list_back,
                         size_type& from_list_size,
                         allocator_type& to_allocator,
                         pointer_node& to_list_back, size_type& to_list_size) {
    pointer_node ptr_from = from_list_front;
    pointer_node ptr_to;
    for (size_type i(0); i < from_list_size; ++i)
      ptr_to = _rebound_traits::allocate(to_allocator, 1);
    _rebound_traits::construct(to_allocator, ptr_to,
                               std::move_if_noexcept(ptr_from->val));
    _add_node(ptr_to, to_list_back);

    to_list_size = from_list_size;
    _clear_list(from_allocator, from_list_back, from_list_size);
  }

 private:
  void _default_constructor_base() {
    _back = _rebound_traits::allocate(_allocator, 1);
    _rebound_traits::construct(_allocator, _back);
    _front = _back;
    _front->next = _back;
    _front->prev = _back;
    _back->next = _front;
    _back->prev = _front;
    _size = 0;
  }

 public:
  list() { _default_constructor_base(); }

  explicit list(const Allocator& alloc) : _allocator(alloc) {
    _default_constructor_base();
  }

 private:
  void _count_values_constructor_base(
      size_t count, const T& value,
      const Allocator& alloc = Allocator())  //: _allocator(alloc)
  {
    _allocator = alloc;
    _default_constructor_base();
    for (size_t i(0); i < count; ++i) {
      pointer_node node = nullptr;
      // _allocate_node(_allocator, node);
      node = _rebound_traits::allocate(_allocator, 1);
      _rebound_traits::construct(_allocator, node, value);
      _add_node(node, _back);
      if (_size == 0) {
        _front = node;
      }
      ++_size;
    }
  }

 public:
  list(size_t count, const T& value, const Allocator& alloc = Allocator()) {
    _count_values_constructor_base(count, value, alloc);
  }

  explicit list(size_t count, const Allocator& alloc = Allocator()) {
    T value = T();
    _count_values_constructor_base(count, value, alloc);
  }

  ~list() { _clear_list(_allocator, _back, _size); }

  list(const list& other) : _allocator(other._allocator) {
    _default_constructor_base();
    // std::cout << "copy constructor   "  << std::endl;
    _copy_list(_allocator, other._front, other._size, _front, _back, _size);
  }

  list(list&& other) noexcept
      : _allocator(std::move(other._allocator)),
        _front(std::move(other._front)),
        _back(std::move(other._back)),
        _size(std::move(other._size)) {
    other._front = nullptr;
    other._back = nullptr;
    other._size = 0;
  }

  list& operator=(const list& other) {
    if (this == &other) {
      return *this;
    }
    // std::cout << "coperator=   "  << std::endl;
    ~list();

    if (_rebound_traits::propagate_on_container_copy_assignment::value) {
      _allocator = other._allocator;
    }

    allocator_type& allocator_to_use =
        (_allocator == other._allocator) ? _allocator : other._allocator;

    _default_constructor_base();

    _copy_list(allocator_to_use, other._front, other._size, _back, _size);

    return *this;
  }

  list& operator=(list&& other) noexcept(
      noexcept(_rebound_traits::propagate_on_container_move_assignment::value ||
               _rebound_traits::is_always_equal::value)) {
    std::cout << "coperator= move  " << std::endl;
    ~list();
    if (!_rebound_traits::propagate_on_container_move_assignment::value &&
        _allocator != other._allocator) {
      _default_constructor_base();

      _move_list(other._allocator, other._front, other._back, other._size,
                 _allocator, _back, _size);

    } else {
      _front = std::move(other._front);
      _back = std::move(other._back);
      _size = std::move(other._size);
      if (_rebound_traits::propagate_on_container_move_assignment::value)
        _allocator = other._allocator;  /// Important: copy, not move!
    }
    return *this;
  }

  Allocator get_allocator() const {
    Allocator alloc(_allocator);
    return alloc;
  }

  T& front() { return _front->val; }

  const T& front() const { return _front->val; }

  T& back() { return _back->prev->val; }

  const T& back() const { return _back->prev->val; }

  iterator begin() { return iterator(_front); }

  iterator end() { return iterator(_back); }

  const_iterator cbegin() const { return const_iterator(_front); }

  const_iterator cend() const { return const_iterator(_back); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }

  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(cend());
  }

  const_reverse_iterator crend() const {
    return const_reverse_iterator(cbegin());
  }

  bool empty() const { return _size == 0; }

  size_t size() const { return _size; }

  size_t max_size() const { return MAX_LIST_SIZE; }

  void clear() {
    _clear_list(_allocator, _back, _size);
    _front = _back;
    _size = 0;
  }

  iterator insert(iterator pos, const T& value) {
    // std::cout << "insert const val " << std::endl;
    const_iterator const_pos = const_iterator(pos.get_node_pointer());
    return insert(const_pos, 1, value);
  }

  iterator insert(const_iterator pos, const T& value) {
    return insert(pos, 1, value);
  }

  iterator insert(const_iterator pos, T&& value) {
    return insert(pos, 1, std::move(value));
  }

  iterator insert(const_iterator pos, size_t count, const T& value) {
    pointer_node node;
    for (size_t i = 0; i < count; ++i) {
      node = _rebound_traits::allocate(_allocator, 1);
      _rebound_traits::construct(_allocator, node, value);
      pointer_node next = pos.get_node_pointer();
      if (_front == pos.get_node_pointer()) _front = node;
      _add_node(node, next);
    }
    _size += count;
    return iterator(node);
  }

  iterator insert(iterator pos, size_t count, const T& value) {
    pointer_node node;
    for (size_t i = 0; i < count; ++i) {
      node = _rebound_traits::allocate(_allocator, 1);
      _rebound_traits::construct(_allocator, node, value);
      pointer_node next = pos.get_node_pointer();
      if (_front == pos.get_node_pointer()) _front = node;
      _add_node(node, next);
    }
    _size += count;
    return iterator(node);
  }

  iterator erase(const_iterator pos) {
    pointer_node ptr = pos.get_node_pointer();
    // std::cout << "erase iter  " << ptr->val << std::endl;
    pointer_node next = ptr->next;
    if (ptr == _front) _front = next;
    _clear_node(_allocator, ptr);
    --_size;
    return iterator(next);
  }

  iterator erase(iterator pos) {
    pointer_node ptr = pos.get_node_pointer();
    // std::cout << "erase iter  " << ptr->val << std::endl;
    pointer_node next = ptr->next;
    if (ptr == _front) _front = next;
    _clear_node(_allocator, ptr);
    --_size;
    return iterator(next);
  }

  iterator erase(const_iterator first, const_iterator last) {
    const_iterator p = first;
    while (p != last) {
      p = erase(p);
      ++p;
      --_size;
    }
    return p;
  }

  iterator erase(iterator first, iterator last) {
    iterator p = first;
    while (p != last) {
      p = erase(p);
      ++p;
      --_size;
    }
    return p;
  }

  void push_back(const T& value) {
    // std::cout << "push_back const ptr " << value << std::endl;
    emplace_back(std::forward(value));
  }

  void push_back(T&& value) {
    // std::cout << "push_back move " << std::endl;
    emplace_back(std::move(value));
  }

  void pop_back() { erase(cend()); }

  void push_front(const T& value) {
    // std::cout << "push_front const ptr " << value << std::endl;
    emplace_front(std::forward(value));
  }

  void push_front(T&& value) {
    // std::cout << "push_front move " << value << std::endl;
    emplace_front(std::move(value));
  }

  void pop_front() {
    // std::cout << "pop_front " << cbegin().get_node_pointer()->val <<
    // std::endl;
    erase(cbegin());
  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    pointer_node node;
    node = _rebound_traits::allocate(_allocator, 1);
    _rebound_traits::construct(_allocator, node, std::forward<Args>(args)...);
    // std::cout << "emplace node " << std::endl;
    pointer_node next = pos.get_node_pointer();
    if (_front == next) _front = node;
    _add_node(node, next);
    // std::cout << "emplace _front " << _front->val << std::endl;
    // std::cout << "emplace _back " << _back->val << std::endl;
    ++_size;
    return iterator(node);
  }

  template <class... Args>
  void emplace_back(Args&&... args) {
    // std::cout << "emplace back " << std::endl;
    emplace(cend(), std::forward<Args>(args)...);
  }

  template <class... Args>
  void emplace_front(Args&&... args) {
    emplace(cbegin(), std::forward<Args>(args)...);
  }

  void resize(size_t count) {
    if (count < _size) {
      size_t num_elems = _size - count;
      for (size_t i = 0; i < num_elems; ++i) {
        // std::cout << "resize pop " << _size << std::endl;
        pop_back();
      }
    }
    if (count > _size) {
      size_t num_elems = count - _size;
      for (size_t i = 0; i < num_elems; ++i) {
        // std::cout << "resize emplace " << count << " " << _size << std::endl;
        emplace_back();  // may be need to rewrite emplace() here
      }
    }
  }

  void swap(list& other) noexcept(noexcept(
      /// Need this specification in order to tell the user when he will get the
      /// UB.
      _rebound_traits::propagate_on_container_swap::value ||
      _rebound_traits::is_always_equal::value)) {
    assert(_rebound_traits::propagate_on_container_swap::value ||
               _rebound_traits::is_always_equal::value,
           "list::swap: Either propagate_on_container_swap must be true"
           "or the allocators must compare equal");

    std::swap(_front, other._front);
    std::swap(_back, other._back);
    std::swap(_size, other._size);

    if (_rebound_traits::propagate_on_container_swap::value)
      std::swap(_allocator, other._allocator);
  }

  void merge(list& other) {
    iterator ot = other.begin();
    for (const_iterator me = cbegin(); me != cend(); ++me) {
      while (ot->val <= me->val && ot != other.end()) {
        emplace(*me, std::move(*ot));
        ++ot;
      }
    }
    while (ot != other.end()) {
      emplace_back(std::move(*ot));
      ++ot;
    }
  }

  void splice(const_iterator pos, list& other) {
    pointer_node me_right = pos.get_node_pointer();
    pointer_node me_left = me_right->prev;
    pointer_node ot_left = other.begin().get_node_pointer();
    pointer_node ot_right = (--other.end()).get_node_pointer();
    me_left->next = ot_left;
    ot_left->prev = me_left;
    me_right->prev = ot_right;
    ot_right->next = me_right;
  }

  void splice(iterator pos, list& other) {
    pointer_node me_right = pos.get_node_pointer();
    pointer_node me_left = me_right->prev;
    pointer_node ot_left = other.begin().get_node_pointer();
    pointer_node ot_right = (--other.end()).get_node_pointer();
    me_left->next = ot_left;
    ot_left->prev = me_left;
    me_right->prev = ot_right;
    ot_right->next = me_right;
  }

  void remove(const T& value) {
    iterator it = begin();
    while (it != end()) {
      if (it->val == value)
        it = erase(it);
      else
        ++it;
    }
  }

  void reverse() {
    iterator it = begin();
    while (it != end()) {
      pointer_node tmp = it->next;
      it->next = it->prev;
      it->prev = it->next;
      ++it;
    }
    pointer_node tmp = _back->next;
    _back->next = _back->prev;
    _back->prev = _back->next;
    _front = _back->next;
  }

  void unique() {
    iterator prev = begin();
    for (iterator it = iterator(_front->next); it != end(); ++it) {
      if (*it == *prev) erase(it);
      prev = it;
    }
  }

  void sort() {
    for (iterator out = begin(); out != --end(); ++out) {
      for (iterator in = out; in != end(); ++in) {
        if (in.get_node_pointer()->val < out.get_node_pointer()->val)
          std::swap(in.get_node_pointer()->val, out.get_node_pointer()->val);
      }
    }
  }

  // Your code goes here?..

 private:
  // Your code goes here...
};

// Your template function definitions may go here...

}  // namespace task
