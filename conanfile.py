from conans import ConanFile, CMake, tools

class GloomGameConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch", "cppstd"
    options = {"vulkan": [True, False]}
    default_options = {"vulkan": False}
    generators = "cmake", "CMakeToolchain", "VirtualBuildEnv", "VirtualRunEnv"
    requires = "small3d/1.709@dimi309/small3d"

    def config_options(self):
        self.options["small3d"].vulkan = self.options.vulkan
        
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        if self.options.vulkan:
            self.copy("*.spv", dst="", src="")
        else:
            self.copy("*.vert", dst="", src="")
            self.copy("*.frag", dst="", src="")
        
