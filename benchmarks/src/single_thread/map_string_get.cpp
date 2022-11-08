#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_string.h>
#include <map_parameters/map_parameters.h>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapSearchBenchmark, std::string, int)(benchmark::State &state)
{
    int threadCount = 1;
    int tid = threadCount - 1;
    LockfreeHashTableString<int> ht(2 * mapSize, threadCount);
    auto insert = [&ht, tid] (std::string key, int value) { ht.insert(key, value, tid);};
    initializeCollection(insert, keyStringCreator);
    bool found = false;
    auto search = [&ht, tid, &found] (std::string key) {ht.search(key, tid, found);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(search));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest,OrderedMapBracketsBenchmark, std::string, int)(benchmark::State &state)
{
    std::map<std::string, int> orderedMap;
    auto insert = [&orderedMap] (std::string key, int value) { orderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator);
    auto bracketAccess = [&orderedMap] (std::string key) {int value = orderedMap[key];};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(bracketAccess));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest,OrderedMapAtBenchmark, std::string, int)(benchmark::State &state)
{
    std::map<std::string, int> orderedMap;
    auto insert = [&orderedMap] (std::string key, int value) { orderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator);
    auto atAccess = [&orderedMap] (std::string key) {int value = orderedMap.at(key);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(atAccess));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest,UnorderedMapBracketsBenchmark, std::string,int) (benchmark::State &state)
{
    std::unordered_map<std::string, int> unorderedMap;
    auto insert = [&unorderedMap] (std::string key, int value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator);
    auto bracketAccess = [&unorderedMap] (std::string key) {int value = unorderedMap[key];};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(bracketAccess));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest,UnorderedMapAtBenchmark, std::string, int) (benchmark::State &state)
{
    std::unordered_map<std::string, int> unorderedMap;
    auto insert = [&unorderedMap] (std::string key, int value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator);
    auto atAccess = [&unorderedMap] (std::string key) {int value = unorderedMap.at(key);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(atAccess));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapSearchBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapAtBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapAtBenchmark)->Apply(MapParameters)->UseRealTime();
