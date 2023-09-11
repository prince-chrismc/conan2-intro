from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMake

class App(ConanFile):
    name = "app"
    version = "1.0"
    package_type = "application"
    settings = "os", "arch", "compiler", "build_type"
    exports_sources = "CMakeLists.txt", "src*", "tests*"
    generators = "CMakeToolchain", "CMakeDeps"

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("spdlog/[>=1.10 <2]")
        self.requires("glfw/3.3.8")
        self.requires("glad/0.1.36")
        self.requires("linmath.h/cci.20220619")
        self.requires("tinycthread/cci.20161001")

    def build_requirements(self):
        self.test_requires("gtest/[>=1.14 <2]")

    def build(self):
        cmake = CMake(self)
        cmake.configure(variables={"CMAKE_CTEST_ARGUMENTS": "--output-junit;results.xml;"})
        cmake.build()
        cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()
