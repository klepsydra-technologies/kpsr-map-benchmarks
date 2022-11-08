#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_uint.h>
#include <map_parameters/map_parameters.h>
#include <mutex>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapUintVectorRemoveBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    int tid = threadCount - 1;
    for (auto _ : state) {
        LockfreeHashTableUint<std::vector<int>> ht(2 * mapSize, threadCount);
        auto insert = [&ht, tid] (unsigned int key, std::vector<int> value) { ht.insert(key, value, tid);};
        initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
        auto remove = [&ht, tid] (unsigned int key) {ht.remove(key, tid);};
        state.SetIterationTime(measureOperationMultithreaded(remove));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintVectorEraseBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    for (auto _ : state) {
        std::map<unsigned int, std::vector<int>> orderedMap;
        auto insert = [&orderedMap] (unsigned int key, std::vector<int> value) { orderedMap[key] = value;};
        initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
        auto remove = [&orderedMap, &mutex] (unsigned int key) { 
            std::lock_guard<std::mutex> guard(mutex);
            orderedMap.erase(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(remove));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintVectorEraseBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    for (auto _ : state) {
        std::unordered_map<unsigned int, std::vector<int>> unorderedMap;
        auto insert = [&unorderedMap] (unsigned int key, std::vector<int> value) { unorderedMap[key] = value;};
        initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
        auto remove = [&unorderedMap, &mutex] (unsigned int key) { 
            std::lock_guard<std::mutex> guard(mutex);
            unorderedMap.erase(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(remove));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapUintVectorRemoveBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintVectorEraseBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintVectorEraseBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
