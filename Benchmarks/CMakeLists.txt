include(FetchContent)
set(BENCHMARK_ENABLE_TESTING NO)
# Externally provided libraries
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0)

FetchContent_Declare(googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main) # need main for benchmark::benchmark

FetchContent_MakeAvailable(
        googletest
        googlebenchmark)

add_executable(GameBenchmarks main.cpp)
target_link_libraries(GameBenchmarks benchmark::benchmark)