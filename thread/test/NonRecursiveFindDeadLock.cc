#include <vector>
#include <mutex>
#include <thread>
#include <iostream>

class Foo
{
public:
    void doit();
};

typedef std::vector<Foo> FooList;

FooList g_foos_;
std::mutex g_mutex_;

void traverse()
{
    std::lock_guard<std::mutex> lock_(g_mutex_);
    std::cout << "traverse()" << std::endl;
    for (auto it = g_foos_.begin(); it != g_foos_.end(); ++it) {
        it->doit();
    }
}

void post(Foo& f) {
    std::lock_guard<std::mutex> lock_(g_mutex_);
    std::cout << "post()" << std::endl;
    g_foos_.push_back(f);
}

void Foo::doit()
{

    Foo f;
    post(f); // deadlock
}

int main()
{
    Foo f;
    std::thread t1(post, std::ref(f));
    std::thread t2(traverse);
    t1.join();
    t2.join();
}