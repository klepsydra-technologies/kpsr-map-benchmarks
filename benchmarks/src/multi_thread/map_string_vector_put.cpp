#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_string.h>
#include <map_parameters/map_parameters.h>
#include <mutex>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapStringInsertBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    int tid = threadCount - 1;
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        LockfreeHashTableString<std::vector<int>> ht(2 * mapSize, threadCount);
        auto insert = [&ht, tid] (std::string key, std::vector<int> value) { ht.insert(key, value, tid);};
        bool found;
        auto search = [&ht, tid, &found] (std::string key) {ht.search(key, tid, found);};
        state.SetIterationTime(measureOperationMultithreaded(insert, search, valueVectorIntCreator));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapStringInsertBracketsBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::map<std::string, std::vector<int>> orderedMap;
        auto insert = [&orderedMap, &mutex] (std::string key, std::vector<int> value) {
            std::lock_guard<std::mutex> guard(mutex);
            orderedMap[key] = value;
        };
        auto find = [&orderedMap, &mutex] (std::string key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = orderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueVectorIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapStringInsertBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::map<std::string, std::vector<int>> orderedMap;
        auto insert = [&orderedMap, &mutex] (std::string key, std::vector<int> value) { 
            std::lock_guard<std::mutex> guard(mutex);
            orderedMap.insert({key, value});
        };
        auto find = [&orderedMap, &mutex] (std::string key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = orderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueVectorIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapStringInsertBracketsBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::unordered_map<std::string, std::vector<int>> unorderedMap;
        auto insert = [&unorderedMap, &mutex] (std::string key, std::vector<int> value) {
            std::lock_guard<std::mutex> guard(mutex); 
            unorderedMap[key] = value;
        };
        auto find = [&unorderedMap, &mutex] (std::string key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = unorderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueVectorIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapStringInsertBenchmark, std::string, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyStringCreator);
    for (auto _ : state) {
        std::unordered_map<std::string, std::vector<int>> unorderedMap;
        auto insert = [&unorderedMap, &mutex] (std::string key, std::vector<int> value) {
            std::lock_guard<std::mutex> guard(mutex); 
            unorderedMap.insert({key, value});
        };
        auto find = [&unorderedMap, &mutex] (std::string key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = unorderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueVectorIntCreator));
    }
}


BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapStringInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapStringInsertBracketsBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapStringInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapStringInsertBracketsBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapStringInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
