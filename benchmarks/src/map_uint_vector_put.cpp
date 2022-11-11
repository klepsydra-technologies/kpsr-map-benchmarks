#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_uint.h>

void MapParameters(benchmark::internal::Benchmark* benchmark) {
    benchmark->ArgNames({"Size"});

    for (int e = 2; e <= 64; e*=2) {
        benchmark->Args({e});
    }
}

static void LockFreeMapUintInsertBenchmark(benchmark::State &state)
{
    unsigned int mapSize = state.range(0);
    int threadCount = 1;
    int tid = threadCount - 1;
    std::vector<unsigned int> keys(mapSize);
    for (unsigned int i = 0; i < mapSize; i++) {
        keys[i] = i;
    }

    for (auto _ : state) {
        LockfreeHashTableUint<std::vector<int>> ht(2 * mapSize, threadCount);

        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            ht.insert(keys[i], std::vector<int>(mapSize, i), tid);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void OrderedMapBracketsBenchmark(benchmark::State &state)
{
    unsigned int mapSize = state.range(0);
    std::vector<unsigned int> keys(mapSize);
    for (unsigned int i = 0; i < mapSize; i++) {
        keys[i] = i;
    }

    for (auto _ : state) {
        std::map<unsigned int, std::vector<int>> orderedMap;

        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            orderedMap[keys[i]] = std::vector<int>(mapSize, i); 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void OrderedMapInsertBenchmark(benchmark::State &state)
{
    unsigned int mapSize = state.range(0);
    std::vector<unsigned int> keys(mapSize);
    for (unsigned int i = 0; i < mapSize; i++) {
        keys[i] = i;
    }

    for (auto _ : state) {
        std::map<unsigned int, std::vector<int>> orderedMap;
        
        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            orderedMap.insert({keys[i], std::vector<int>(mapSize, i)}); 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void UnorderedMapBracketsBenchmark(benchmark::State &state)
{
    unsigned int mapSize = state.range(0);
    std::vector<unsigned int> keys(mapSize);
    for (unsigned int i = 0; i < mapSize; i++) {
        keys[i] = i;
    }

    for (auto _ : state) {
        std::unordered_map<unsigned int, std::vector<int>> unorderedMap;

        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            unorderedMap[keys[i]] = std::vector<int>(mapSize, i); 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void UnorderedMapInsertBenchmark(benchmark::State &state)
{
    unsigned int mapSize = state.range(0);
    std::vector<unsigned int> keys(mapSize);
    for (unsigned int i = 0; i < mapSize; i++) {
        keys[i] = i;
    }

    for (auto _ : state) {
        std::unordered_map<unsigned int, std::vector<int>> unorderedMap;

        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            unorderedMap.insert({keys[i], std::vector<int>(mapSize, i)}); 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

BENCHMARK(LockFreeMapUintInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapInsertBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapInsertBenchmark)->Apply(MapParameters)->UseRealTime();