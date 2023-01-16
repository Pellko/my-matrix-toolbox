#ifndef _MY_MATRIX_TOOLBOX_TYPES_SAFE_QUEUE_H_
#define _MY_MATRIX_TOOLBOX_TYPES_SAFE_QUEUE_H_

#include <queue>
#include <mutex>
#include <optional>

namespace mymatrixtoolbox {

template<typename T>
class SafeQueue {
 public:
  SafeQueue() = default;
  SafeQueue(const SafeQueue<T>&) = delete;
  SafeQueue& operator=(const SafeQueue<T>&) = delete;

  SafeQueue(SafeQueue<T>&& other) {
    std::lock_guard<std::mutex> lock(mutex);
    queue = std::move(other.queue);
  }

  virtual ~SafeQueue() {}

  unsigned long size() {
    std::lock_guard<std::mutex> lock(mutex);
    return queue.size();
  }

  std::optional<T> pop() {
    std::lock_guard<std::mutex> lock(mutex);
    if(queue.empty()) {
      return {};
    }
    T tmp = queue.front();
    queue.pop();
    return tmp;
  }

  void push(const T& item) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(item);
  }

 private:
  std::queue<T> queue;
  mutable std::mutex mutex;

  bool empty() const {
    return queue.empty();
  }
};

}

#endif