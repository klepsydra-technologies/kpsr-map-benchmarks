#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_uint.h>
#include <map_parameters/map_parameters.h>
#include <mutex>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapUintRemoveBenchmark, unsigned int, int)(benchmark::State &state)
{
    int tid = threadCount - 1;
    for (auto _ : state) {
        LockfreeHashTableUint<int> ht(2 * mapSize, threadCount);
        auto insert = [&ht, tid] (unsigned int key, int value) { ht.insert(key, value, tid);};
        initializeCollection(insert, keyUintCreator);
        auto remove = [&ht, tid] (unsigned int key) {ht.remove(key, tid);};
        state.SetIterationTime(measureOperationMultithreaded(remove));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintEraseBenchmark, unsigned int, int)(benchmark::State &state)
{
    std::mutex mutex;
    for (auto _ : state) {
        std::map<unsigned int, int> orderedMap;
        auto insert = [&orderedMap] (unsigned int key, int value) { orderedMap[key] = value;};
        initializeCollection(insert, keyUintCreator);
        auto remove = [&orderedMap, &mutex] (unsigned int key) {
            std::lock_guard<std::mutex> guard(mutex);
            orderedMap.erase(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(remove));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintEraseBenchmark, unsigned int, int)(benchmark::State &state)
{
    std::mutex mutex;
    for (auto _ : state) {
        std::unordered_map<unsigned int, int> unorderedMap;
        auto insert = [&unorderedMap] (unsigned int key, int value) { unorderedMap[key] = value;};
        initializeCollection(insert, keyUintCreator);
        auto remove = [&unorderedMap, &mutex] (unsigned int key) {
            std::lock_guard<std::mutex> guard(mutex);
            unorderedMap.erase(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(remove));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapUintRemoveBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintEraseBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintEraseBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
