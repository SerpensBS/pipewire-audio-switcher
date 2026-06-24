include(FetchContent)

# GoogleTest.
fetchcontent_declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
fetchcontent_makeavailable(googletest)

# Boost
find_package(Boost REQUIRED)
find_package(Boost REQUIRED COMPONENTS process)
