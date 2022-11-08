#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_string.h>
#include <map_parameters/map_parameters.h>
#include <mutex>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapStringSearchBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    int tid = threadCount - 1;
    LockfreeHashTableString<std::vector<int>> ht(2 * mapSize, threadCount);
    auto insert = [&ht, tid] (std::string key, std::vector<int> value) { ht.insert(key, value, tid);};
    initializeCollection(insert, keyStringCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    bool found = false;
    auto search = [&ht, tid, &found] (std::string key) {ht.search(key, tid, found);};
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(search));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapStringBracketsBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    std::map<std::string, std::vector<int>> orderedMap;
    auto insert = [&orderedMap] (std::string key, std::vector<int> value) { orderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto search = [&orderedMap, &mutex] (std::string key) {
        std::lock_guard<std::mutex> guard(mutex);
        auto value = orderedMap[key];
    };
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(search));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapStringAtBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    std::map<std::string, std::vector<int>> orderedMap;
    auto insert = [&orderedMap] (std::string key, std::vector<int> value) { orderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto search = [&orderedMap, &mutex] (std::string key) {
        std::lock_guard<std::mutex> guard(mutex);
        auto value = orderedMap.at(key);
    };
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(search));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapStringBracketsBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    std::unordered_map<std::string, std::vector<int>> unorderedMap;
    auto insert = [&unorderedMap] (std::string key, std::vector<int> value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto search = [&unorderedMap, &mutex] (std::string key) {
        std::lock_guard<std::mutex> guard(mutex);
        auto value = unorderedMap[key];
    };
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(search));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapStringAtBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    std::unordered_map<std::string, std::vector<int>> unorderedMap;
    auto insert = [&unorderedMap] (std::string key, std::vector<int> value) { unorderedMap[key] = value;};
    initializeCollection(insert, keyStringCreator, valueVectorIntCreator);
    std::vector<std::vector<int>> vectorValues(mapSize);
    auto search = [&unorderedMap, &mutex] (std::string key) {
        std::lock_guard<std::mutex> guard(mutex);
        auto value = unorderedMap.at(key);
    };
    for (auto _ : state) {
        state.SetIterationTime(measureOperationMultithreaded(search));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapStringSearchBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapStringBracketsBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapStringAtBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapStringBracketsBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapStringAtBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
