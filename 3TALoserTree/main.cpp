#include<iostream>
#include"./LoserTree.h"
using namespace std;

class Foo {
public:
    Foo () {}
    explicit Foo (int v) : _v (v) {}
    Foo (const Foo& foo) { _v = foo._v; }
    int value () const { return _v; }
    Foo& operator=(const Foo& foo) { _v = foo._v; return *this; }
    bool operator==(const Foo& foo) { return _v == foo._v; }

private:
    int _v;
};
std::ostream& operator<<(std::ostream& os, Foo& foo) {
    return os << foo.value ();
}

extern const Foo FOO_MAX (INT_MAX);

namespace  std {
    template<>
    class less<Foo> : std::binary_function<Foo, Foo, bool> {
    public:
        bool operator() (const Foo& x, const Foo& y) const {
            return x.value () < y.value ();
        }
    };
} // namespace std

template<class ValueType>
class FooContainer {
public:
    ValueType operator[](size_t idx) const { return _container[idx]; }
    size_t size () const { return _container.size (); }
    void push_back (const ValueType& value) {
        _container.push_back (value);
    }

private:
    std::vector<ValueType> _container;
};

void test_foo () {
    std::vector<FooContainer<Foo> > data = generate_data<Foo, FooContainer<Foo> > ();
    /*
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[i].size(); ++j) {
            //std::cout << data[i][j] << ", ";
            Foo foo = data[i][j];
            std::cout << foo << ", ";
        }
        std::cout << std::endl;
    }
    */

    LoserTree<Foo, FooContainer<Foo>, FOO_MAX> lt (data.data (), data.size ());
    Foo v;
    Foo correct_res (0);
    while (lt.extract_one (v)) {
        //assert(v == correct_res);
        //correct_res = Foo(correct_res.value()+1);
        std::cout << v.value () << ", ";
    }
    std::cout << std::endl;
}

extern const int int_max = INT_MAX;
void test () {
    std::vector<std::vector<int> > data = generate_data<int, std::vector<int> > ();
    LoserTree<int, std::vector<int>, int_max> lt (data.data (), data.size ());
    int v;
    int correct_res (0);
    while (lt.extract_one (v)) {
        assert (v == correct_res++);
        std::cout << v << ", ";
    }
    std::cout << std::endl;
}

int main () {
    try {
        //LoserTree<int, std::vector<int>, int_max> lt(NULL, 3);
        test_foo ();
        test ();
    } catch (const std::exception& exc) {
        std::cerr << exc.what () << std::endl;
    }

    return 0;
}