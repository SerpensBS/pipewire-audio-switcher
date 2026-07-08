include(FetchContent)

# CLI11
include(FetchContent)
fetchcontent_declare(
    cli11_proj
    QUIET
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.6.2
)

fetchcontent_makeavailable(cli11_proj)
