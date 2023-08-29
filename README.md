# conan2-intro

Sample Project for Presenting

- Step 1: [Creating a basic "Hello World" app](https://github.com/prince-chrismc/conan2-intro/tree/spdlog-hello-word)

## Instructions

1. `conan install`
2. `cmake --preset conan-release`
3. `cmake --build --preset conan-release`

### Debugging our Code

1. `conan install . -s "&:build_type=Debug"`
2. `cmake --preset conan-debug`
3. `cmake --build --preset conan-debug`
4. `./build/Debug/app`
