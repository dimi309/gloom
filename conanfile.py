from conans import ConanFile, CMake, tools

class GloomGameConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake", "CMakeToolchain", "VirtualBuildEnv", "VirtualRunEnv"
    requires = "small3d/master"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.spv", dst="", src="")
        
