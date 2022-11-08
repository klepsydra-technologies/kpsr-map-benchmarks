#include <map_parameters/map_parameters.h>

void MapParameters(benchmark::internal::Benchmark* benchmark) {
    benchmark->ArgNames({"NThreads"});
    benchmark->Args({1});
}

void MapParametersMultithreaded(benchmark::internal::Benchmark* benchmark) {
    benchmark->ArgNames({"NThreads"});

    for (int t = 10; t <= 50; t+=10) {
        benchmark->Args({t});
    }
}