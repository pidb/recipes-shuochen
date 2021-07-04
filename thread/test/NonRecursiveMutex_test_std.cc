#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class Foo {
 public:
  void doit() const;
};

std::mutex mutex_;
std::vector<Foo> foos;

void post(const Foo& f) {
  std::lock_guard<std::mutex> lock(mutex_);
  postWithLockHold(f);
}

void postWithLockHold(const Foo& f) { foos.push_back(f); }


void traverse() {
  for (std::vector<Foo>::const_iterator it = foos.cbegin(); it != foos.cend();
       it++) {
    it->doit();
  }
}

void Foo::doit() const {
  Foo f;
  // deadlock here!
  post(f);
}

int main() {
  Foo f;
  post(f);
  traverse();
}