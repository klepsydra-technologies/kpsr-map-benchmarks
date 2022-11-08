#ifndef __KPSR_MAP_BENCHMARKS_MAP_PARAMETERS_H__
#define __KPSR_MAP_BENCHMARKS_MAP_PARAMETERS_H__
#include <benchmark/benchmark.h>
#include <chrono>
#include <functional>
#include <thread>
#include <atomic>
#include <chrono>
#define MAP_SIZE 8192
void MapParameters(benchmark::internal::Benchmark* benchmark);
void MapParametersMultithreaded(benchmark::internal::Benchmark* benchmark);
template<typename K, typename V>
class MapPerformanceTest : public benchmark::Fixture
{
public:
    MapPerformanceTest()
        : benchmark::Fixture()
    {
    }
    ~MapPerformanceTest() {}

    void SetUp(const ::benchmark::State &state)
    {
        mapSize = MAP_SIZE;
        keys = std::vector<K>(mapSize);
        keyStringCreator = [] (int i) {return std::to_string(i);};
        keyUintCreator = [] (int i) {return i;};
        valueIntCreator = [] (int i) {return i;};
        valueVectorIntCreator = [this] (int i) {return std::vector<int>(mapSize/1000, i);};
        
        threadCount = state.range(0);

        if (mapSize < threadCount) {
            throw std::runtime_error("Map size should ensure be greater than number of threads.");
        }
    }

    void TearDown(const ::benchmark::State &state)
    {
        
    }
    template<typename Functor>
    void initializeCollection(Functor insertor, 
        std::function<K(int)> keyCreator, std::function<V(int)> valueCreator) {
        for (int i = 0; i < mapSize; i++) {
            keys[i] = keyCreator(i);
            insertor(keys[i], valueCreator(i));
        }
    }
    template<typename Functor>
    void initializeCollection(Functor insertor, std::function<K(int)> keyCreator) {
        initializeCollection(insertor, keyCreator, valueIntCreator);
    }
    void initializeCollection(std::function<K(int)> keyCreator) {
        for (int i = 0; i < mapSize; i++) {
            keys[i] = keyCreator(i);
        }
    }
    template<typename Functor>
    double measureOperationMultithreaded(const Functor& operation) {
        std::vector<std::thread> vectorThreads;
        std::vector<double> times(threadCount);
        std::atomic<int> barrier(0);
        for (int i = 0; i < threadCount; i++) {
            vectorThreads.emplace_back([&](int id) {
                barrier++;
                while (barrier < threadCount + 1) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                times[id] = measureOperation(operation, keys[i]);

            }, i);
        }
        while (barrier < threadCount) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        barrier++;
        for (int i = 0; i < threadCount; i++) {
            vectorThreads[i].join();
        }
        auto it = std::max_element(std::begin(times), std::end(times));
        return *it;
    }

    template<typename WriteFunctor, typename ReadFunctor>
    double measureOperationMultithreaded(const WriteFunctor& operationWrite, 
    const ReadFunctor& operationRead, 
    std::function<V(int)> valueCreator) {
        std::vector<std::thread> vectorThreads;
        std::vector<double> times(threadCount);
        std::atomic<int> barrier(0);
        vectorThreads.emplace_back([&](int id) {
                barrier++;
                while (barrier < threadCount + 1) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                times[id] = measureOperation(operationWrite, keys[0], valueCreator);

        }, 0);
        for (int i = 1; i < threadCount; i++) {
            vectorThreads.emplace_back([&, i](int id) {
                barrier++;
                while (barrier < threadCount + 1) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                times[id] = measureOperation(operationRead, keys[i]);

            }, i);
        }
        while (barrier < threadCount) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        barrier++;
        for (int i = 0; i < threadCount; i++) {
            vectorThreads[i].join();
        }
        auto it = std::max_element(std::begin(times), std::end(times));
        return *it;
    }
    template<typename Functor>
    double measureOperation(Functor operation) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
            operation(keys[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        return elapsedSeconds.count();
    }
    template<typename Functor>
    double measureOperation(Functor operation, std::function<V(int)> valueCreator) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
            operation(keys[i], valueCreator(i));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        return elapsedSeconds.count();
    }
    int threadCount;
    int mapSize;
    std::function<std::string(int)> keyStringCreator;
    std::function<unsigned(int)> keyUintCreator;
    std::function<int(int)> valueIntCreator;
    std::function<std::vector<int>(int)> valueVectorIntCreator;
private:
    template<typename Functor>
    double measureOperation(const Functor& operation, K key) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize/threadCount; i++) {
            operation(key);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        return elapsedSeconds.count();
    }
    template<typename Functor>
    double measureOperation(const Functor& operation, K key, std::function<V(int)> valueCreator) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize/threadCount; i++) {
            operation(key, valueCreator(i));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        return elapsedSeconds.count();
    }
private:
    std::vector<K> keys;
};

#endif