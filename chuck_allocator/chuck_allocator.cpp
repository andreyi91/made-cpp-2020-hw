#include <iostream>
#include <vector>

size_t CHUNK_SIZE = 20;

template <typename T>
struct Chunk {
    size_t size;
    T * ptr = nullptr;
    Chunk * next;
};

template <typename T>
class Allocator {
private:
    Chunk<T> *firstChunk;

    Chunk<T>* allocateChunk() {
        std::cout << "allocate chunk" << std::endl;
        Chunk<T> *chunk;
        T * p = reinterpret_cast<T*>(new char[CHUNK_SIZE * sizeof(T)]);
        std::cout << "new ptr to chunk created " << p << std::endl;
        std::cout << "             chunk->ptr  " << chunk->ptr << std::endl;
        &p;
        chunk->ptr = p;
        std::cout << "new ptr to chunk created " << p << std::endl;
        std::cout << "             chunk->ptr  " << chunk->ptr << std::endl;
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
    template< class U > struct rebind { typedef Allocator<U> other; };
    typedef std::true_type is_always_equal;

    Allocator() {
        this->firstChunk = nullptr;
    };

    Allocator<T>&  operator=(const Allocator& other) {
        if (&other == this) {
            return *this;
        }
        this->firstChunk = other.firstChunk;
        return *this;

    };

    Allocator(const Allocator& copy) {
        std::cout << "alloc copy before  this  " << this->firstChunk << std::endl;
        std::cout << "alloc copy before  other  " << copy.firstChunk << std::endl;
        *this = copy;
        std::cout << "alloc copy after   " << this->firstChunk << std::endl;
    };

    ~Allocator() = default;

    T* allocate(const size_t n) {
        if (this->firstChunk == nullptr) {
            //reinterpret_cast<T*>(new char[CHUNK_SIZE * sizeof(T)]);
            std::cout << "new alloc before " << this->firstChunk << std::endl;
            this->firstChunk = allocateChunk();
            std::cout << "new alloc after " << this->firstChunk << std::endl;
        }
        Chunk<T> *chunk = this->firstChunk;
        T* mem_ptr;
        bool allocated = false;
        std::cout << "start allocation " << chunk << std::endl;
        while (!allocated) {
            if (chunk->size >= n) {
                mem_ptr = chunk->ptr;
                chunk->ptr += n * sizeof(T);
                chunk->size -= n;
                allocated = true;
            } else if (chunk->next != nullptr) {
                chunk = chunk->next;
            } else {
                chunk = allocateChunk();
            }
        }
        std::cout << "end allocation mem_ptr " << mem_ptr << std::endl;
        std::cout << "end allocation mem_ptr " << mem_ptr << std::endl;
        return mem_ptr;
    };

    void deallocate (T* p, const size_t n) {
        //delete[] reinterpret_cast<char*>(p);
    };

    template <typename ... Args>
    void construct (T* p, const Args&&... args) {
        std::cout << "construct" << std::endl;
        new (p) T(args...);
    };

    void destroy (T* p) {
        std::cout << "destroy" << std::endl;
        Chunk<T>* chunk = this->firstChunk;
        this->firstChunk = nullptr;
        bool destroyed = false;
        while (destroyed != true) {
            if (chunk == nullptr) {
                destroyed = true;
            } else {
                delete[] reinterpret_cast<char*>(chunk);
                chunk = chunk->next;
            }
        }
        p->~T();
    };
};

class A {
public:
    int x, y;
public:
    A (int x, int y) {
        //this->x = x;
        //this->y = y;
        std::cout << x << " " << y << std::endl; 
    };
    ~A () { std::cout << "~" << std::endl; }
};

int main () {
    //std::vector<A, Allocator<A>> vec;
    //vec.push_back(A(1, 3));


    Allocator<A> palloc;
    //Allocator<A> talloc;
    // Allocator<A> talloc = palloc;
    std::cout << "main before allocate " << std::endl;
    A * p1 = palloc.allocate(8);
    // auto t1 = talloc.allocate(7);
    std::cout << "main before construct " << std::endl;
    palloc.construct(p1, 1, 2);
    palloc.construct(p1, 2, 3);
    //std::cout << "output  " << palloc.firstChunk << std::endl;
    palloc.destroy(p1);
    palloc.deallocate(p1, 8);
    // std::cout << "output  " << talloc.firstChunk << std::endl;
    // talloc.construct(t1, 1, 2);
    // talloc.destroy(t1);





    // std::vector<A, Allocator<A>> vec;
    // vec.push_back(A(1, 2));

    //alloc.deallocate(p, 1);
    //int *arr = ch_alloc.allocate(10);
    //ch_alloc.deallocate(arr, 10);

    // auto p = ch_alloc.allocate(3);
    // ch_alloc.construct(p, 3, 5);
    // ch_alloc.destroy(p);
    // ch_alloc.deallocate(p, 3);

}