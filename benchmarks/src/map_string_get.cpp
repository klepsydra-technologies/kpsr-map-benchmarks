#include <benchmark/benchmark.h>
#include <iostream>
#include <chrono>

#include <vector>
#include <map>
#include <unordered_map>
#include <lockfree_hash_table_string.h>

void MapParameters(benchmark::internal::Benchmark* benchmark) {
    benchmark->ArgNames({"Size"});

    for (int e = 2; e <= 64; e*=2) {
        benchmark->Args({e});
    }
}

static void LockFreeMapSearchBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    int threadCount = 1;
    int tid = threadCount - 1;

    LockfreeHashTableString<int> ht(2 * mapSize, threadCount);
    for (int i = 0; i < mapSize; i++) {
        ht.insert(std::to_string(i), i, tid);
    }

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
            std::pair<int, bool> value = ht.search(std::to_string(i), tid);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void OrderedMapBracketsBenchmark(benchmark::State &state)
{
    size_t mapSize = state.range(0);
    std::map<std::string, int> orderedMap;
    for (size_t i = 0; i < mapSize; i++) {
        orderedMap[std::to_string(i)] = i; 
    }

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < mapSize; i++) {
            int value = orderedMap[std::to_string(i)]; 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void OrderedMapAtBenchmark(benchmark::State &state)
{
    size_t mapSize = state.range(0);
    std::map<std::string, int> orderedMap;
    for (size_t i = 0; i < mapSize; i++) {
        orderedMap[std::to_string(i)] = i; 
    }
    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < mapSize; i++) {
            int value = orderedMap.at(std::to_string(i));
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void UnorderedMapBracketsBenchmark(benchmark::State &state)
{
    size_t mapSize = state.range(0);
    std::unordered_map<std::string, int> unorderedMap;
    for (size_t i = 0; i < mapSize; i++) {
        unorderedMap[std::to_string(i)] = i; 
    }

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < mapSize; i++) {
            int value = unorderedMap[std::to_string(i)]; 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void UnorderedMapAtBenchmark(benchmark::State &state)
{
    size_t mapSize = state.range(0);
    std::unordered_map<std::string, int> unorderedMap;
    for (size_t i = 0; i < mapSize; i++) {
        unorderedMap[std::to_string(i)] = i; 
    }

    for (auto _ : state) {
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < mapSize; i++) {
            int value = unorderedMap.at(std::to_string(i)); 
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

BENCHMARK(LockFreeMapSearchBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapAtBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapBracketsBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapAtBenchmark)->Apply(MapParameters)->UseRealTime();
