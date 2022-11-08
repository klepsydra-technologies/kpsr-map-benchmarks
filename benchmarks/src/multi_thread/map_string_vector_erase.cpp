#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_string.h>
#include <map_parameters/map_parameters.h>
#include <mutex>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapStringRemoveBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    int tid = threadCount - 1;
    for (auto _ : state) {
        LockfreeHashTableString<std::vector<int>> ht(2 * mapSize, threadCount);
        auto insert = [&ht, tid] (std::string key, std::vector<int> value) { ht.insert(key, value, tid);};
        initializeCollection(insert, keyStringCreator, valueVectorIntCreator);
        auto remove = [&ht, tid] (std::string key) {ht.remove(key, tid);};
        state.SetIterationTime(measureOperationMultithreaded(remove));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapStringEraseBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    for (auto _ : state) {
        std::map<std::string, std::vector<int>> orderedMap;
        auto insert = [&orderedMap] (std::string key, std::vector<int> value) { orderedMap[key] = value;};
        initializeCollection(insert, keyStringCreator, valueVectorIntCreator);
        auto remove = [&orderedMap, &mutex] (std::string key) {
            std::lock_guard<std::mutex> guard(mutex);
            orderedMap.erase(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(remove));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapStringEraseBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    for (auto _ : state) {
        std::unordered_map<std::string, std::vector<int>> unorderedMap;
        auto insert = [&unorderedMap] (std::string key, std::vector<int> value) { unorderedMap[key] = value;};
        initializeCollection(insert, keyStringCreator, valueVectorIntCreator);
        auto remove = [&unorderedMap, &mutex] (std::string key) {
            std::lock_guard<std::mutex> guard(mutex);
            unorderedMap.erase(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(remove));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapStringRemoveBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapStringEraseBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapStringEraseBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
