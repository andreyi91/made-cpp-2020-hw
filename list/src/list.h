#pragma once
#include <iterator>
#include <iostream>

#define assert(expr, msg) \
    if (!expr) std::cerr << msg << std::endl;

namespace task {

size_t MAX_LIST_SIZE = 1'000'000'000;

template<class T, class Allocator = std::allocator<T>> 
class list {
private:
    template<typename U>
    struct Node {
        U val;
        Node* next;
        Node* prev;

        Node() = default;

        Node(const U& value) : val(value) {};
        Node(U&& value) noexcept : val(std::move(value)) {};

        // operator->
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

    typedef typename _rebound_traits::pointer pointer;

private:
    template <typename P, typename R>
    class iterator_base {
    public:
        friend class list;
        typedef T value_type;
        typedef R reference;
        typedef ptrdiff_t difference_type;
        typedef P pointer;
        typedef std::bidirectional_iterator_tag iterator_category;
    
    private:
        Node<value_type>* ptr;
        explicit iterator_base (Node<value_type>* ptr): ptr(ptr) {}
    
    public:
        iterator_base() = delete;
        iterator_base(const iterator_base<P, R>& other) = default;
        ~iterator_base() = default;
        iterator_base& operator=(const iterator_base<P, R>&) = default;

        iterator_base<P, R>& operator++() { ptr = ptr->next; return *this; }
        iterator_base<P, R> operator++(int) { iterator_base<P, R> t(*this); ptr->next; return t; }
        reference operator*() const { return *(ptr->val); }
        pointer operator->() const { return ptr->val; }
        iterator_base<P, R>& operator--() { ptr = ptr->prev; return *this; }
        iterator_base<P, R> operator--(int) { iterator_base<P, R> t(*this); ptr->prev; return t; }

        bool operator==(iterator_base<P, R> other) const { return ptr == other->ptr; } // may be change to:    bool operator== (const iterator &other) { return ptr == other.ptr; }
        bool operator!=(iterator_base<P, R> other) const { return ptr != other->ptr; }

        // Your code goes here?..

    private:
        // Your code goes here...
    };

public:
    // class const_iterator {
    //     // Your code goes here...
    // };

    typedef iterator_base<pointer_val, reference> iterator;
    typedef iterator_base<const_pointer_val, const_reference> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // static_assert(
    //     std::is_same<typename _rebound_alloc::value_type, value_type>::value,
    //     "Allocator::value_type must be same type as value_type"
    // );

private:
    allocator_type _allocator;
    pointer _front;
    pointer _back;
    size_type _size;

    // static size_type _get_buffer_size (
    //         pointer _buffer_front,
    //         pointer _buffer_back
    // ) noexcept {
    //     size_type size = 0;
    //     iterator it = iterator(_buffer_front);
    //     while (*it != _buffer_back) {
    //         it = it->next;
    //         size += 1;
    //     }
    //     return _size;
    // }

    // static size_type _get_buffer_size (
    //         size_type buffer_size
    // ) noexcept {
    //     return buffer_size;
    // }

    static void _allocate_node (
            allocator_type& allocator,
            pointer& ptr,
            size_type n = 1
    ) {
        ptr = _rebound_traits::allocate(allocator, n);
        // connect node here ???
    }

    static void _clear_node (
            allocator_type& allocator,
            pointer& node

    ) {
        // iterator it = iterator(node);
        // -- it;
        pointer tmp = node;
        node = node->prev;
        node->next = node->next->next;
        node->next->prev = node;
        _rebound_traits::destroy(allocator, tmp);
        _rebound_traits::deallocate(allocator, tmp, 1);
    }

    static void _clear_list (
            allocator_type& allocator,
            pointer& list_back,
            size_type& list_size
    ) noexcept {
        // iterator it = list_back;
        // while(buffer_front != buffer_back)
        for (size_type i(0); i < list_size; i ++) {
            _clear_node(allocator, list_back->prev);
        }
        // list_back = it;
        list_size = 0;
    }

    static void _add_node (
            pointer& new_node,
            pointer& next_node
    ) {
        // std::cout << "_add_node _front " << new_node << std::endl;
        new_node->next = next_node;
        new_node->prev = next_node->prev;
        next_node->prev = new_node;
        new_node->prev->next = new_node;
    }

    static void _copy_list (
            allocator_type& allocator,
            pointer& from_list_front,
            size_type& from_list_size,
            pointer& to_list_back,
            size_type& to_list_size
    ) {
        // iterator it_from = iterator();
        // iterator it_to = iterator();
        pointer ptr_from = from_list_front;
        for (size_type i(0); i < from_list_size; ++ i) {
            pointer ptr_to;                 // may be bad idea
            // _allocate_node(allocator, ptr_to);
            ptr_to = _rebound_traits::allocate(allocator, 1);
            _rebound_traits::construct(allocator, ptr_to, ptr_from->val);
            _add_node(ptr_to, to_list_back);
            ptr_from = ptr_from->next;
        }
        to_list_size = from_list_size;
    }
    
    static void _move_list (
            allocator_type& from_allocator,
            pointer& from_list_front,
            pointer& from_list_back,
            size_type& from_list_size,
            allocator_type& to_allocator,
            pointer& to_list_back,
            size_type& to_list_size
    ) {
        pointer ptr_from = from_list_front;
        pointer ptr_to;
        for (size_type i(0); i < from_list_size; ++ i)
            // pointer ptr_to = nullptr;                 // may be bad idea
            // _allocate_node(to_allocator, ptr_to);
            ptr_to = _rebound_traits::allocate(to_allocator, 1);
            _rebound_traits::construct(
                to_allocator,
                ptr_to,
                std::move_if_noexcept(ptr_from->val));
            _add_node(ptr_to, to_list_back);

        to_list_size = from_list_size;    
        _clear_list(from_allocator, from_list_back, from_list_size);
    }

private:
    void _default_constructor_base () {
        // _allocate_node(_allocator,_back);
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
    list() {
        _default_constructor_base();
    }
    
    explicit list(const Allocator& alloc) : _allocator(alloc)
    {
        
        _default_constructor_base();
    }

private:
    void _count_values_constructor_base (
            size_t count, 
            const T& value, 
            const Allocator& alloc = Allocator()
            ) //: _allocator(alloc)
    {
        _allocator = alloc;
        _default_constructor_base();
        for (size_t i(0); i < count; ++ i) {
            pointer node = nullptr;
            // _allocate_node(_allocator, node);
            node = _rebound_traits::allocate(_allocator, 1);
            _rebound_traits::construct(_allocator, node, value);
            _add_node(node, _back);
            if (_size == 0) {
                _front = node;
            }
            ++ _size;
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

    ~list() {
        _clear_list(_allocator, _back, _size);
    }

    list(const list& other) :
            _allocator(other._allocator)
    {
        _default_constructor_base();
        
        _copy_list (
            _allocator,
            other._front,
            other._size,
            _back,
            _size
        );
    }

    list(list&& other) noexcept :
        _allocator(std::move(other._allocator)),
        _front(std::move(other._front)),
        _back(std::move(other._back)),
        _size(std::move(other._size))
    {
        other._front = nullptr;
        other._back = nullptr;
        other._size = 0;
    }


    list& operator=(const list& other) {
        if (this == &other) {
            return *this;
        }
        ~list();

        if (_rebound_traits::propagate_on_container_copy_assignment::value) {
            _allocator = other._allocator;
        }
            
        allocator_type &allocator_to_use =
            (_allocator == other._allocator) ? _allocator : other._allocator;        

        _default_constructor_base();
        
        _copy_list (
            allocator_to_use,
            other._front,
            other._size,
            _back,
            _size);
        
        return *this;
    }


    list& operator=(list&& other) 
            noexcept(noexcept(
                _rebound_traits::propagate_on_container_move_assignment::value
                || _rebound_traits::is_always_equal::value
            ))
    {
        ~list();
        if (
            !_rebound_traits::propagate_on_container_move_assignment::value &&
            _allocator != other._allocator
        ) {
            _default_constructor_base();

            _move_list (
                other._allocator,
                other._front,
                other._back,
                other._size,
                _allocator,
                _back,
                _size);

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


    T& front() {
        return _front->val;
    }

    const T& front() const {
        return _front->val;
    }

    T& back() {
        return (--end())->val;
    }

    const T& back() const {
        return _back->prev->val;
    }


    iterator begin() {
        return iterator(_front);
    }

    iterator end() {
        return iterator(_back);
    }

    const_iterator cbegin() const {
        return const_iterator(_front);
    }

    const_iterator cend() const {
        return const_iterator(_back);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(begin());
    }


    bool empty() const {
        return _size == 0;
    }

    size_t size() const {
        return _size;
    }

    size_t max_size() const {
        return MAX_LIST_SIZE;
    }

    void clear() {
        _clear_list(_allocator, _back, _size);
        _front = _back;
        _size = 0;
    }

    iterator insert( iterator pos, const T& value ) {
        const_iterator const_pos = const_iterator(pos->next->prev);
        return insert(const_pos, 1, value);
    }

    iterator insert(const_iterator pos, const T& value) {
        return insert(pos, 1, value);
    }

    iterator insert(const_iterator pos, T&& value) {
        // pointer node;
        // _allocate_node(_allocator, node);
        // _rebound_traits::construct(_allocator, node, std::move(value));
        // _add_node(node, *pos);
        // ++ _size;
        // return iterator(node);
        return insert(pos, 1, std::move(value));
    }

    iterator insert(const_iterator pos, size_t count, const T& value) {
        pointer node;
        for (size_t i = 0; i < count; ++ i) {
            // _allocate_node(_allocator, node);
            node = _rebound_traits::allocate(_allocator, 1);
            _rebound_traits::construct(_allocator, node, value);
            if (_front == pos->next->prev)
                _front = node;
            _add_node(node, pos->next->prev);
        }
        _size += count;
        return iterator(node);   
    }


    iterator erase(const_iterator pos) {
        // iterator next = ++ pos;
        pointer ptr = pos->next;
        if (ptr->prev == _front)
            _front = ptr;
        _clear_node(_allocator, ptr->prev);
        -- _size;
        return iterator(ptr);
    }

    iterator erase(const_iterator first, const_iterator last) {
        const_iterator p = first;
        while (p != last) {
            p = erase(p);
            ++ p;
            -- _size;
        }
        return p;
    }


    void push_back(const T& value) {
        // std::cout << "push_back const ptr " << value << std::endl;
        emplace_back(std::forward(value));
    }

    void push_back(T&& value) {
        // std::cout << "push_back move " << value << std::endl;
        emplace_back(std::move(value));
    }

    void pop_back() {
        erase(cend());
    }

    void push_front(const T& value) {
        // std::cout << "push_front const ptr " << value << std::endl;
        emplace_front(std::forward(value));
    }

    void push_front(T&& value) {
        // std::cout << "push_front move " << value << std::endl;
        emplace_front(std::move(value));
    }

    void pop_front() {
        erase(cbegin());
    }

    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        pointer node;
        // _allocate_node(_allocator, node);
        node = _rebound_traits::allocate(_allocator, 1);
        _rebound_traits::construct(_allocator, node, std::forward<Args>(args)...);
        // std::cout << "emplace node " << node->val << std::endl;
        if (_front == pos->next->prev)
            _front = node;
        _add_node(node, pos->next->prev);
        // std::cout << "emplace _front " << _front->val << std::endl;
        // std::cout << "emplace _back " << _back->val << std::endl;
        ++ _size;
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
            for (size_t i = 0; i < count - _size; ++ i)
                pop_back();
        }
        if (count > _size) {
            for (size_t i = 0; i < _size - count; ++ i)
                emplace_back();    // may be need to rewrite emplace() here
        }
    }

    void swap(list& other)
        noexcept(noexcept(
            /// Need this specification in order to tell the user when he will get the UB.
            _rebound_traits::propagate_on_container_swap::value ||
            _rebound_traits::is_always_equal::value
        ))
    {
        assert(
            _rebound_traits::propagate_on_container_swap::value ||
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
        for (const_iterator me = cbegin(); me != cend(); ++ me) {
            while (ot->val <= me->val && ot != other.end()) {
                emplace(*me, std::move(*ot));
                ++ ot;
            }
        }
        while (ot != other.end()) {
            emplace_back(std::move(*ot));
            ++ ot;
        }
    }

    void splice(const_iterator pos, list& other) {
        pointer me_right = *pos;
        pointer me_left = me_right->prev;
        pointer ot_left = *(other.begin());
        pointer ot_right = *(-- other.end());
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
                ++ it;
        }
    }

    void reverse() {
        iterator it = begin();
        while (it != end()) {
            pointer tmp = it->next;
            it->next = it->prev;
            it->prev = it->next;
            ++ it;
        }
        pointer tmp = _back->next;
        _back->next = _back->prev;
        _back->prev = _back->next;
        _front = _back->next;
    }

    void unique() {
        iterator it = begin();
        while (it != end()) {
            if (it->val == it->prev->val)
                it = erase(it);
            else
                ++ it;
        }
    }

    void sort() {
        for (iterator out = begin(); out != -- end(); ++ out) {
            for (iterator in = out; in != end(); ++ in) {
                if (in->val < out->val)
                    std::swap(in->val, out->val);
            }
        }
    }


    // Your code goes here?..

private:
    // Your code goes here...

};

// Your template function definitions may go here...

}  // namespace task
