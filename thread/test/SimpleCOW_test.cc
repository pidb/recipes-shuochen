#include <mutex>
#include <vector>
#include <memory>
#include <cassert>
#include <thread>
#include <iostream>
#include <chrono>

using std::mutex;
using std::vector;
using std::thread;
using std::shared_ptr;
using std::make_shared;

class Foo
{
public:
    Foo() = default;
    explicit Foo(int32_t id) :id_(id) {};
    ~Foo() = default;
    void doit() const;
private:
    int32_t id_;
};

typedef vector<Foo> FooList;
shared_ptr<FooList> g_foos = make_shared<FooList>(FooList());
mutex g_mutex_;

void post(Foo f)
{
    std::lock_guard<mutex> lock(g_mutex_);
    // copy on write
    if (!g_foos.unique()) {
        // g_foos points to new FooList.
        g_foos.reset(new FooList(*g_foos));
    }
    assert(g_foos.unique());
    g_foos->push_back(f);
}

void traverse()
{
    // copy shared_ptr 
    shared_ptr<FooList> newFoo;
    {
        std::lock_guard<mutex> lock(g_mutex_);
        newFoo = g_foos;
    }

    for(FooList::const_iterator it = newFoo->cbegin(); it != newFoo->cend(); it++)
    {
        it->doit();
    }
}

void Foo::doit() const {
    std::lock_guard<mutex> lock_(g_mutex_);
    std::cout << "[" << id_ <<  "]" << "doit " << std::endl;
}

typedef vector<thread> ThreadList;

ThreadList g_thread_list_;

int main() {

    size_t threads_ = 100;
    for(size_t i = 0; i < threads_; i++) {
        g_thread_list_.push_back(thread(post, Foo(i)));
    }
    
    for(thread& th_ : g_thread_list_) {
        th_.detach();
    }

    traverse();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}