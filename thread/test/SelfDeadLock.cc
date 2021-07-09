#include <mutex>
#include <iostream>
#include <thread>

using std::mutex;
using std::lock_guard;

class Request {
public:
  void process() // __attribute__(oninline) 
  {
    lock_guard<mutex> lock_(mutex_);
    std::cout << "call process()" << std::endl;
    print();
  }

  void print() // __attribute__(oninline)
  {
    lock_guard<mutex> lock_(mutex_);
    std::cout << "call print()" << std::endl;
  }
private:
  mutex mutex_;
};

int main()
{
  Request req;
  req.process();
}


/*
Macos m1
(lldb) thread backtrace 
* thread #1, queue = 'com.apple.main-thread', stop reason = signal SIGSTOP
  * frame #0: 0x000000019aaafa48 libsystem_kernel.dylib`__psynch_mutexwait + 8
    frame #1: 0x000000019aae4918 libsystem_pthread.dylib`_pthread_mutex_firstfit_lock_wait + 88
    frame #2: 0x000000019aae2244 libsystem_pthread.dylib`_pthread_mutex_firstfit_lock_slow + 232
    frame #3: 0x000000019aa74af4 libc++.1.dylib`std::__1::mutex::lock() + 16
    frame #4: 0x0000000100003050 a.out`std::__1::lock_guard<std::__1::mutex>::lock_guard(this=0x000000016fdff2f0, __m=0x000000016fdff368) at __mutex_base:91:27
    frame #5: 0x0000000100002e70 a.out`std::__1::lock_guard<std::__1::mutex>::lock_guard(this=0x000000016fdff2f0, __m=0x000000016fdff368) at __mutex_base:91:21
    frame #6: 0x0000000100002f90 a.out`Request::print(this=0x000000016fdff368) at SelfDeadLock.cc:19:23
    frame #7: 0x0000000100002d28 a.out`Request::process(this=0x000000016fdff368) at SelfDeadLock.cc:14:5
    frame #8: 0x0000000100002c40 a.out`main at SelfDeadLock.cc:29:7
    frame #9: 0x000000019ab05450 libdyld.dylib`start + 4
*/