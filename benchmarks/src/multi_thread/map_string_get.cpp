#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_string.h>
#include <map_parameters/map_parameters.h>
#include <mutex>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapSearchBenchmark, std::string, int)(benchmark::State &state)
{
    int tid = threadCount - 1;
    LockfreeHashTableString<int> ht(2 * mapSize, threadCount);
    auto insert = [&ht, tid] (std::string key, int value) { ht.insert(key, value, tid);};
    initializeCollection(insert, keyStringCreator);
    bool found = false;
    auto search = [&ht, tid, &found] (std::string key) {ht.search(key, tid, found);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(search));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest,OrderedMapBracketsBenchmark, std::string, int)(benchmark::State &state)
{
    std::mutex mutex;
    std::map<std::string, int> orderedMap;
    auto insert = [&orderedMap] (std::string key, int value) { orderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator);
    auto bracketAccess = [&orderedMap, &mutex] (std::string key) {
        std::lock_guard<std::mutex> guard(mutex);
        int value = orderedMap[key];
    };
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(bracketAccess));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest,OrderedMapAtBenchmark, std::string, int)(benchmark::State &state)
{
    std::mutex mutex;
    std::map<std::string, int> orderedMap;
    auto insert = [&orderedMap] (std::string key, int value) { orderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator);
    auto atAccess = [&orderedMap, &mutex] (std::string key) {
        std::lock_guard<std::mutex> guard(mutex);
        int value = orderedMap.at(key);
    };
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(atAccess));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest,UnorderedMapBracketsBenchmark, std::string,int) (benchmark::State &state)
{
    std::mutex mutex;
    std::unordered_map<std::string, int> unorderedMap;
    auto insert = [&unorderedMap] (std::string key, int value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator);
    auto bracketAccess = [&unorderedMap,&mutex] (std::string key) {
        std::lock_guard<std::mutex> guard(mutex);
        int value = unorderedMap[key];};
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(bracketAccess));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest,UnorderedMapAtBenchmark, std::string, int) (benchmark::State &state)
{
    std::mutex mutex;
    std::unordered_map<std::string, int> unorderedMap;
    auto insert = [&unorderedMap] (std::string key, int value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator);
    auto atAccess = [&unorderedMap, &mutex] (std::string key) {
        std::lock_guard<std::mutex> guard(mutex);
        int value = unorderedMap.at(key);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(atAccess));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapSearchBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapBracketsBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapAtBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapBracketsBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapAtBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
