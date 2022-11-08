#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_string.h>
#include <map_parameters/map_parameters.h>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapStringSearchBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    int threadCount = 1;
    int tid = threadCount - 1;
    LockfreeHashTableString<std::vector<int>> ht(2 * mapSize, threadCount);
    auto insert = [&ht, tid] (std::string key, std::vector<int> value) { ht.insert(key, value, tid);};
    initializeCollection(insert, keyStringCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto it = vectorValues.begin();
    bool found = false;
    auto search = [&ht, tid, &found, &it] (std::string key) {*(it++) = ht.search(key, tid, found);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(search));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapStringBracketsBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::map<std::string, std::vector<int>> orderedMap;
    auto insert = [&orderedMap] (std::string key, std::vector<int> value) { orderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto it = vectorValues.begin();
    auto search = [&orderedMap, &it] (std::string key) {*(it++) = orderedMap[key];};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(search));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapStringAtBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::map<std::string, std::vector<int>> orderedMap;
    auto insert = [&orderedMap] (std::string key, std::vector<int> value) { orderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto it = vectorValues.begin();
    auto search = [&orderedMap, &it] (std::string key) {*(it++) = orderedMap.at(key);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(search));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapStringBracketsBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::unordered_map<std::string, std::vector<int>> unorderedMap;
    auto insert = [&unorderedMap] (std::string key, std::vector<int> value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto it = vectorValues.begin();
    auto search = [&unorderedMap, &it] (std::string key) {*(it++) = unorderedMap[key];};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(search));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapStringAtBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::unordered_map<std::string, std::vector<int>> unorderedMap;
    auto insert = [&unorderedMap] (std::string key, std::vector<int> value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto it = vectorValues.begin();
    auto search = [&unorderedMap, &it] (std::string key) {*(it++) = unorderedMap.at(key);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(search));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapStringSearchBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapStringBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapStringAtBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapStringBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapStringAtBenchmark)->Apply(MapParameters)->UseRealTime();
