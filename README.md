# conan2-intro

Sample Project for Presenting

- Step 1: [Creating a basic "Hello World" app](https://github.com/prince-chrismc/conan2-intro/tree/spdlog-hello-word)
- Step 2: [Adding application logic and dependencies](https://github.com/prince-chrismc/conan2-intro/tree/spdlog-hello-word)
- Step 3: [Debugging and Testing our app](https://github.com/prince-chrismc/conan2-intro/tree/testing-and-lockfiles)

## Instructions

1. `conan install`
2. `cmake --preset conan-release`
3. `cmake --build --preset conan-release`

### Debugging our Code

1. `conan install . -s "&:build_type=Debug"`
2. `cmake --preset conan-debug`
3. `cmake --build --preset conan-debug`
4. `./build/Debug/app`

### Running the Tests

1. `conan install`
2. `cmake --preset conan-release`
3. `ctest --preset conan-release --output-junit results.xml`

### Locking down our dependency graph

> **Note**: This a one time operation that a developer only needs to run once but could be useful to update packages.

1. `conan lock create`
2. `conan install`
