include(FetchContent)

# Google Benchmark.
fetchcontent_declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark
    GIT_TAG v1.9.5
)
fetchcontent_makeavailable(benchmark)
