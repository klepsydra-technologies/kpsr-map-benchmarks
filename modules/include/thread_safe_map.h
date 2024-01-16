#ifndef KPE_GPU_STREAMING_THREAD_SAFE_MAP_H
#define KPE_GPU_STREAMING_THREAD_SAFE_MAP_H

#include <mutex>
#include <map>

template<class K, class V>
class ThreadSafeMap {
public:
    ThreadSafeMap()
            : _map(), _mutex() {}

    V &getValue(const K &key) {
        std::lock_guard<std::mutex> guard(_mutex);
        return _map.at(key);
    }

    void insert(const K &key, V &value) {
        std::lock_guard<std::mutex> guard(_mutex);
        _map.insert(std::make_pair(key, value));
    }

    void erase(const K &key) {
        std::lock_guard<std::mutex> guard(_mutex);
        _map.erase(key);
    }

    size_t getSize() {
        std::lock_guard<std::mutex> guard(_mutex);
        return _map.size();
    }

private:
    std::map<K, V> _map;
    std::mutex _mutex;
};

#endif //KPE_GPU_STREAMING_THREAD_SAFE_MAP_H
