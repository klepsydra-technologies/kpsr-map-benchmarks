#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_string.h>
#include <map_parameters/map_parameters.h>
#include <mutex>


BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapRemoveBenchmark, std::string, int)(benchmark::State &state)
{
    int tid = threadCount - 1;
    for (auto _ : state) {
        LockfreeHashTableString<int> ht(2 * mapSize, threadCount);
        auto insert = [&ht, tid] (std::string key, int value) { ht.insert(key, value, tid);};
        initializeCollection(insert, keyStringCreator);
        auto remove = [&ht, tid] (std::string key) {ht.remove(key, tid);};
        state.SetIterationTime(measureOperationMultithreaded(remove));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapEraseBenchmark, std::string, int) (benchmark::State &state)
{
    std::mutex mutex;
    for (auto _ : state) {
        std::map<std::string, int> orderedMap;
        auto insert = [&orderedMap] (std::string key, int value) {orderedMap[key] = value;};
        initializeCollection(insert, keyStringCreator);
        auto remove = [&orderedMap, &mutex] (std::string key) {
            std::lock_guard<std::mutex> guard(mutex);
            orderedMap.erase(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(remove));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapEraseBenchmark, std::string, int) (benchmark::State &state)
{
    std::mutex mutex;
    for (auto _ : state) {
        std::unordered_map<std::string, int> unorderedMap;
        auto insert = [&unorderedMap] (std::string key, int value) {unorderedMap[key] = value;};
        initializeCollection(insert, keyStringCreator);
        auto remove = [&unorderedMap, &mutex] (std::string key) {
            std::lock_guard<std::mutex> guard(mutex);
            unorderedMap.erase(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(remove));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapRemoveBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapEraseBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapEraseBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
