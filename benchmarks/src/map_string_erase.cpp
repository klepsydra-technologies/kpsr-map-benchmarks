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

static void LockFreeMapRemoveBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    int threadCount = 1;
    int tid = threadCount - 1;

    for (auto _ : state) {
        LockfreeHashTableString<int> ht(2 * mapSize, threadCount);
        for (int i = 0; i < mapSize; i++) {
            ht.insert(std::to_string(i), i, tid);
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
            ht.remove(std::to_string(i), tid);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void OrderedMapEraseBenchmark(benchmark::State &state)
{
    size_t mapSize = state.range(0);

    for (auto _ : state) {
        std::map<std::string, int> orderedMap;
        for (size_t i = 0; i < mapSize; i++) {
            orderedMap[std::to_string(i)] = i; 
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < mapSize; i++) {
            orderedMap.erase(std::to_string(i));
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void UnorderedMapEraseBenchmark(benchmark::State &state)
{
    size_t mapSize = state.range(0);

    for (auto _ : state) {
        std::unordered_map<std::string, int> unorderedMap;
        for (size_t i = 0; i < mapSize; i++) {
            unorderedMap[std::to_string(i)] = i; 
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < mapSize; i++) {
            unorderedMap.erase(std::to_string(i));
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

BENCHMARK(LockFreeMapRemoveBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapEraseBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapEraseBenchmark)->Apply(MapParameters)->UseRealTime();