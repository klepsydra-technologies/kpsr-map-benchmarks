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

static void LockFreeMapUintSearchBenchmark(benchmark::State &state)
{
    unsigned int mapSize = state.range(0);
    int threadCount = 1;
    int tid = threadCount - 1;

    LockfreeHashTableUint<std::vector<int>> ht(2 * mapSize, threadCount);
    for (unsigned int i = 0; i < mapSize; i++) {
        ht.insert(i, std::vector<int>(mapSize, i), tid);
    }
    std::vector<std::vector<int>> vectorValues(mapSize);
    bool found = false;

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            vectorValues[i] = ht.search(i, tid, found);
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
    std::map<unsigned int, std::vector<int>> orderedMap;
    for (unsigned int i = 0; i < mapSize; i++) {
        orderedMap[i] = std::vector<int>(mapSize, i); 
    }
    std::vector<std::vector<int>> vectorValues(mapSize);

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            vectorValues[i] = orderedMap[i];
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void OrderedMapAtBenchmark(benchmark::State &state)
{
    unsigned int mapSize = state.range(0);
    std::map<unsigned int, std::vector<int>> orderedMap;
    for (unsigned int i = 0; i < mapSize; i++) {
        orderedMap[i] = std::vector<int>(mapSize, i); 
    }
    std::vector<std::vector<int>> vectorValues(mapSize);

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            vectorValues[i] = orderedMap.at(i);
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
    std::unordered_map<unsigned int, std::vector<int>> unorderedMap;
    for (unsigned int i = 0; i < mapSize; i++) {
        unorderedMap[i] = std::vector<int>(mapSize, i); 
    }
    std::vector<std::vector<int>> vectorValues(mapSize);

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            vectorValues[i] = unorderedMap[i]; 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void UnorderedMapAtBenchmark(benchmark::State &state)
{
    unsigned int mapSize = state.range(0);
    std::unordered_map<unsigned int, std::vector<int>> unorderedMap;
    for (unsigned int i = 0; i < mapSize; i++) {
        unorderedMap[i] = std::vector<int>(mapSize, i); 
    }
    std::vector<std::vector<int>> vectorValues(mapSize);

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (unsigned int i = 0; i < mapSize; i++) {
            vectorValues[i] = unorderedMap.at(i); 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

BENCHMARK(LockFreeMapUintSearchBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapAtBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapAtBenchmark)->Apply(MapParameters)->UseRealTime();
