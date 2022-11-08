#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_uint.h>
#include <map_parameters/map_parameters.h>
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapUintVectorSearchBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    int threadCount = 1;
    int tid = threadCount - 1;
    LockfreeHashTableUint<std::vector<int>> ht(2 * mapSize, threadCount);
    auto insert = [&ht, tid] (unsigned int key, std::vector<int> value) { ht.insert(key, value, tid);};
    initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto it = vectorValues.begin();
    bool found = false;
    auto search = [&ht, tid, &found, &it] (unsigned int key) {*(it++) = ht.search(key, tid, found);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(search));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintVectorBracketsBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::map<unsigned int, std::vector<int>> orderedMap;
    auto insert = [&orderedMap] (unsigned int key, std::vector<int> value) { orderedMap[key] = value;};
    initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto it = vectorValues.begin();
    auto bracketAccess = [&orderedMap, &it] (unsigned int key) {*(it++) = orderedMap[key];};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(bracketAccess));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintVectorAtBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::map<unsigned int, std::vector<int>> orderedMap;
    auto insert = [&orderedMap] (unsigned int key, std::vector<int> value) { orderedMap[key] = value;};
    initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto it = vectorValues.begin();
    auto atAccess = [&orderedMap, &it] (unsigned int key) {*(it++) = orderedMap.at(key);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(atAccess));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintVectorBracketsBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::unordered_map<unsigned int, std::vector<int>> unorderedMap;
    auto insert = [&unorderedMap] (unsigned int key, std::vector<int> value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto it = vectorValues.begin();
    auto bracketAccess = [&unorderedMap, &it] (unsigned int key) {*(it++) = unorderedMap[key];};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(bracketAccess));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintVectorAtBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::unordered_map<unsigned int, std::vector<int>> unorderedMap;
    auto insert = [&unorderedMap] (unsigned int key, std::vector<int> value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyUintCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto it = vectorValues.begin();
    auto atAccess = [&unorderedMap, &it] (unsigned int key) {*(it++) = unorderedMap.at(key);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperation(atAccess));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapUintVectorSearchBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintVectorBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintVectorAtBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintVectorBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintVectorAtBenchmark)->Apply(MapParameters)->UseRealTime();
