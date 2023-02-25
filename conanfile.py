from conan import ConanFile, tools
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain
from conan.tools.files import copy
import os

class GloomGameConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    options = {"vulkan": [True, False]}
    default_options = {"vulkan": False}
    requires = "small3d/master"

    def config_options(self):
        self.options["small3d"].vulkan = self.options.vulkan

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        if not self.options.vulkan:
            tc.variables["SMALL3D_OPENGL"] = True
        tc.generate()
        if self.options.vulkan:
            copy(self, "*spv", self.dependencies["small3d"].cpp_info.bindirs[0], os.path.join(self.build_folder, "bin"))
        else:
            copy(self, "*vert", self.dependencies["small3d"].cpp_info.bindirs[0], os.path.join(self.build_folder, "bin"))
            copy(self, "*frag", self.dependencies["small3d"].cpp_info.bindirs[0], os.path.join(self.build_folder, "bin"))
        
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
