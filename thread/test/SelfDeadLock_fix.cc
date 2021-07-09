#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

using std::lock_guard;
using std::mutex;

class Request {
 public:
  void process()  // __attribute__(noinline)
  {
    lock_guard<mutex> lock_(mutex_);
    std::cout << std::this_thread::get_id() << ": call process()" << std::endl;
    printWithLockHold();
  }

  void print()  // __attribute__(noinline)
  {
    lock_guard<mutex> lock_(mutex_);
    std::cout << std::this_thread::get_id() << ": call print()" << std::endl;
    printWithLockHold();
  }

  void printWithLockHold()  // __attribute__(noinline)
  {
    std::cout << std::this_thread::get_id() << ": call printWithLockHold()"
              << std::endl;
  }

  ~Request() { std::cout << "~Request()" << std::endl; }

 private:
  mutex mutex_;
};

int main() {
  std::shared_ptr<Request> sp{new Request()};
  std::thread t1(&Request::process, sp);
  std::thread t2(&Request::print, sp);
  t1.join();
  t2.join();
}
