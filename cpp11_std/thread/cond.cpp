#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>

/*
    template<typename Predicate>
    void wait(std::unique_lock<std::mutex>& lock, Predicate pred);

    pred返回fase, wait()解锁互斥元,并将该线程转走于阻塞或等待状态.
    当来自数据准备线程中notify_one()的调用通知条件变量时,线种从睡眠状态中苏醒，重新获得互元上的锁, 并再次检查pred.
    如果pred返回true,就从wait()返回, mutex仍被锁定
*/

bool MoreData() {
    return false;
}

struct DataChunk
{};

DataChunk PrepareData() {
    return DataChunk();
}

void Process(DataChunk&) {}

bool IsLastChunk(DataChunk&) {
    return true;
}

std::mutex mut;
std::queue<DataChunk> data_queue;
std::condition_variable data_cond;

void DataProcedureThread() {
    while (MoreData()) {
        DataChunk const data = PrepareData();
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }
}

void DataConsumerThread() {
    while(true) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [] {
            return !data_queue.empty();
        });
        DataChunk data = data_queue.front();
        data_queue.pop();
        lk.unlock();
        Process(data);
        if (IsLastChunk(data)) {
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    std::thread t1(DataProcedureThread);
    std::thread t2(DataConsumerThread);
    
    t1.join();
    t2.join();
}
