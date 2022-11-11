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

static void LockFreeMapStringRemoveBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    int threadCount = 1;
    int tid = threadCount - 1;
    std::vector<std::string> keys(mapSize);

    for (auto _ : state) {
        LockfreeHashTableString<std::vector<int>> ht(2 * mapSize, threadCount);
        for (int i = 0; i < mapSize; i++) {
            keys[i] = std::to_string(i);
            ht.insert(keys[i], std::vector<int>(mapSize, i), tid);
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
            ht.remove(keys[i], tid);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void OrderedMapEraseBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    std::vector<std::string> keys(mapSize);

    for (auto _ : state) {
        std::map<std::string, std::vector<int>> orderedMap;
        for (int i = 0; i < mapSize; i++) {
            keys[i] = std::to_string(i);
            orderedMap[keys[i]] = std::vector<int>(mapSize, i); 
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
            orderedMap.erase(keys[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void UnorderedMapEraseBenchmark(benchmark::State &state)
{
    int mapSize = state.range(0);
    std::vector<std::string> keys(mapSize);

    for (auto _ : state) {
        std::unordered_map<std::string, std::vector<int>> unorderedMap;
        for (int i = 0; i < mapSize; i++) {
            keys[i] = std::to_string(i);
            unorderedMap[keys[i]] = std::vector<int>(mapSize, i); 
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < mapSize; i++) {
            unorderedMap.erase(keys[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end -
                                                                                         start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

BENCHMARK(LockFreeMapStringRemoveBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(OrderedMapEraseBenchmark)->Apply(MapParameters)->UseRealTime();
BENCHMARK(UnorderedMapEraseBenchmark)->Apply(MapParameters)->UseRealTime();