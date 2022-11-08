#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_uint.h>
#include <map_parameters/map_parameters.h>
#include <mutex>

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, LockFreeMapUintVectorInsertBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    int tid = threadCount - 1;
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        LockfreeHashTableUint<std::vector<int>> ht(2 * mapSize, threadCount);
        auto insert = [&ht, tid] (unsigned int key, std::vector<int> value) { ht.insert(key, value, tid);};
        bool found;
        auto search = [&ht, tid, &found] (unsigned int key) {ht.search(key, tid, found);};
        state.SetIterationTime(measureOperationMultithreaded(insert, search, valueVectorIntCreator));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintVectorBracketsInsertBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::map<unsigned int, std::vector<int>> orderedMap;
        auto insert = [&orderedMap, &mutex] (unsigned int key, std::vector<int> value) { 
            std::lock_guard<std::mutex> guard(mutex);
            orderedMap[key] = value;
        };
        auto find = [&orderedMap, &mutex] (unsigned int key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = orderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueVectorIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, OrderedMapUintVectorInsertBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::map<unsigned int, std::vector<int>> orderedMap;
        auto insert = [&orderedMap, &mutex] (unsigned int key, std::vector<int> value) { 
            std::lock_guard<std::mutex> guard(mutex);
            orderedMap.insert({key, value});
        };
        auto find = [&orderedMap, &mutex] (unsigned int key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = orderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueVectorIntCreator));
    }
}
BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintVectorBracketsInsertBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::map<unsigned int, std::vector<int>> unorderedMap;
        auto insert = [&unorderedMap, &mutex] (unsigned int key, std::vector<int> value) {
            std::lock_guard<std::mutex> guard(mutex); 
            unorderedMap[key] = value;
        };
        auto find = [&unorderedMap, &mutex] (unsigned int key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = unorderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueVectorIntCreator));
    }
}

BENCHMARK_TEMPLATE_DEFINE_F(MapPerformanceTest, UnorderedMapUintVectorInsertBenchmark, unsigned int, std::vector<int>)(benchmark::State &state)
{
    std::mutex mutex;
    initializeCollection(keyUintCreator);
    for (auto _ : state) {
        std::map<unsigned int, std::vector<int>> unorderedMap;
        auto insert = [&unorderedMap, &mutex] (unsigned int key, std::vector<int> value) { 
            std::lock_guard<std::mutex> guard(mutex); 
            unorderedMap.insert({key, value});
        };
        auto find = [&unorderedMap, &mutex] (unsigned int key) {
            std::lock_guard<std::mutex> guard(mutex);
            auto value = unorderedMap.find(key);
        };
        state.SetIterationTime(measureOperationMultithreaded(insert, find, valueVectorIntCreator));
    }
}

BENCHMARK_REGISTER_F(MapPerformanceTest,LockFreeMapUintVectorInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintVectorBracketsInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,OrderedMapUintVectorInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintVectorBracketsInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();
BENCHMARK_REGISTER_F(MapPerformanceTest,UnorderedMapUintVectorInsertBenchmark)->Apply(MapParametersMultithreaded)->UseRealTime();