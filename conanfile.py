from conan import ConanFile, tools
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain
from conan.tools.files import copy
import os

class GloomGameConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "small3d/master"

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["SMALL3D_OPENGL"] = True
        tc.generate()
        copy(self, "*vert", self.dependencies["small3d"].cpp_info.bindirs[0], os.path.join(self.build_folder, "bin"))
        copy(self, "*frag", self.dependencies["small3d"].cpp_info.bindirs[0], os.path.join(self.build_folder, "bin"))
        
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
