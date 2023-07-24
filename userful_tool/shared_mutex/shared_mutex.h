
#ifndef _SHARED_MUTEX_H_
#define _SHARED_MUTEX_H_

#include <atomic>
#include <array>
#include <thread>

#ifndef LEVEL1_DCACHE_LINESIZE
#define LEVEL1_DCACHE_LINESIZE	64
#endif

namespace ema {

template<size_t N>
class shared_mutex {
public:
    shared_mutex() {
    }

    ~shared_mutex() {
    }

    void lock_shared(void) {
        while (true) {
            size_t cur_rw_lock = el_[get_thread_idx()].wr_lock.load();
            if (entry_lock::W_MASK & cur_rw_lock) {
                // if someone has got Write access yield and retry...
                std::this_thread::yield();
                continue;
            }

            if (el_[get_thread_idx()].wr_lock.compare_exchange_weak(cur_rw_lock, cur_rw_lock + 1)) {
                break;
            }
        }
    }

    void unlock_shared(void) {
        while (true) {
            size_t cur_rw_lock = el_[get_thread_idx()].wr_lock.load();
            if (el_[get_thread_idx()].wr_lock.compare_exchange_weak(cur_rw_lock, cur_rw_lock - 1)) {
                break;
            }
        }
    }

    void lock(void) {
        for (size_t i = 0; i < N; ++i) {
            // acquire all locks from all buckets
            while (true) {
                size_t cur_rw_lock = el_[i].wr_lock.load();
                if(cur_rw_lock != 0) {
                    std::this_thread::yield();
                    continue;
                }

                // if cur_rw_lock is 0 then proceed
                if(el_[i].wr_lock.compare_exchange_weak(cur_rw_lock, entry_lock::W_MASK)) {
                    break;
                }
            }
        }
    }

    void unlock(void) {
        for(size_t i = 0; i < N; ++i) {
            // release all locks
            while (true) {
                size_t	cur_rw_lock = el_[i].wr_lock.load();
                // then proceed resetting to 0
                if(el_[i].wr_lock.compare_exchange_weak(cur_rw_lock, 0)) {
                    break;
                }
            }
        }
    }

    /*
        Purpose of this structure is to hold status fo each individual bucket-mutex object.
        Ideally each thread should be mapped to one entry only of 'el_' during its lifetime
    */
    struct entry_lock {
        const static uint64_t W_MASK = 0x8000000000000000, R_MASK = ~W_MASK;
        /*
            first bit to tell if we're locking in exclusive mode.
            otherwise use the remaining 63 bits to count how many share locks
        */
        std::atomic<uint64_t> wr_lock;

        entry_lock() : wr_lock(0) {
        }
    } __attribute__((aligned(LEVEL1_DCACHE_LINESIZE)));

    std::array<entry_lock, N> el_;
    static std::atomic<size_t>	idx_hint_;
    static uint64_t get_hint_idx() {
        while (true) {
            size_t cur_hint = idx_hint_.load();
            if (idx_hint_.compare_exchange_weak(cur_hint, cur_hint + 1)) {
                return cur_hint;
            }
        }
    }

    inline static size_t get_thread_idx() {
        const thread_local size_t rv = get_hint_idx() % N;
        return rv;
    }

}; // class shard_mutex

template<size_t N>
std::atomic<size_t> shared_mutex<N>::idx_hint_{0};

template<size_t N>
class x_lock {
    shared_mutex<N>&	sm_;
public:
    x_lock(shared_mutex<N>& sm) : sm_(sm) {
        sm_.lock();
    }

    ~x_lock() {
        sm_.unlock();
    }
}; // class x_lock

// utility class for share RAII lock
template<size_t N>
class s_lock {
    shared_mutex<N>&	sm_;
public:
    s_lock(shared_mutex<N>& sm) : sm_(sm) {
        sm_.lock_shared();
    }

    ~s_lock() {
        sm_.unlock_shared();
    }
}; // class s_lock

} // namespace ema

#endif //_SHARED_MUTEX_H_

