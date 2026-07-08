include(FetchContent)

# TOML++
fetchcontent_declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v3.4.0
)
fetchcontent_makeavailable(tomlplusplus)

# Boost
find_package(Boost REQUIRED)
find_package(Boost REQUIRED COMPONENTS process)

# CLI11
include(FetchContent)
fetchcontent_declare(
    cli11_proj
    QUIET
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.6.2
)

fetchcontent_makeavailable(cli11_proj)
