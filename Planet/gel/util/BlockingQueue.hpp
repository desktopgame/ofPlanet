#ifndef GEL_UTIL_BLOCKINGQUEUE_HPP
#define GEL_UTIL_BLOCKINGQUEUE_HPP
#include <mutex>
#include <queue>
#include <thread>
namespace gel {
// https://qiita.com/kumagi/items/4224282d95804d46fc39
template <typename T>
class BlockingQueue {
       public:
        BlockingQueue() {}
        void enqueue(const T& item) {
                {
                        std::scoped_lock<std::mutex> lk(lk_);
                        const bool was_empty = queue_.empty();
                        queue_.push(item);
                        if (was_empty) {
                                empty_wait_.notify_all();  // notify_one() may
                                                           // be suitable?
                        }
                }
        }

        T dequeue() {
                while (true) {
                        std::unique_lock<std::mutex> lk(lk_);
                        if (queue_.empty()) {  // if empty
                                empty_wait_.wait(lk, [&]() {
                                        return !queue_.empty();
                                });  // unlock
                                     //  relock
                        }
                        /*
                        if (queue_.empty()) {
                                continue;
                        }
                        */

                        T result = queue_.front();
                        queue_.pop();
                        return result;
                }
        }

        size_t size() const {
                std::scoped_lock<std::mutex> lk(lk_);
                return queue_.size();
        }

       private:
        mutable std::mutex lk_;
        mutable std::condition_variable empty_wait_;
        std::queue<T> queue_;
};
}  // namespace gel
#endif