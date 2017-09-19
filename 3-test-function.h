// DO NOT EDIT THIS FILE (unless you feel like it)

#include <cassert>
#include <typeinfo>

struct Large {
    int m_big[1000];
    int m_x;
    Large(int x) : m_x(x) {}
    int operator()(int a, int b) { return a + b + m_x; }
};

void run_tests()
{
    // Test simple cases.

    function<void()> f = [](){ puts("Hello world!"); };
    assert(f);
    f();

    int i = 0;
    f = [&]() { printf("Counting %d\n", ++i); };
    f(); f(); f();

    // Test that copy-assignment does the right thing.

    function<void()> f2 = f;
    assert(f2.target_type() == f.target_type());
    f2 = f;
    assert(f2.target_type() == f.target_type());
    f(); f2();  // Counting 4. Counting 5.

    // Test that arguments and return values work.

    function<int(int, int)> g = [](int x, int y) { return x + y; };
    assert(g(1, 1) == 2);
    
    // Test that very large functor objects work.
    
    function<int(int, int)> g2 = Large(1);
    assert(g2(1, 1) == 3);
    g = g2;
    assert(g(1, 1) == 3);
    g = Large(2);
    assert(g(1, 1) == 4);
    
    // Test type-unerasure.

    assert(g.target<Large>() != nullptr && g.target<Large>()->m_x == 2);
    assert(std::as_const(g).target<Large>() != nullptr && g.target<Large>()->m_x == 2);
    
    g = function<int(int, int)>{};
    assert(!g);
    assert(g.target_type() == typeid(void) && g.target<Large>() == nullptr);

    // Test conversion from an empty function (or null function pointer).

    g = nullptr;
    assert(!g);
    g = function<long(int, int)>{};
    assert(!g);
    g = static_cast<int (*)(int, int)>(nullptr);
    assert(!g);
}