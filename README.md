Gloom
=====

[![Build status](https://ci.appveyor.com/api/projects/status/5wkolhu8qg87j992?svg=true)](https://ci.appveyor.com/project/dimi309/gloom-game-conan)

You are in a gloomy place and some anthropoids are trying to kill you.
Kill them first.

Move with the arrows. Shoot with the spacebar. Escape to quit the game.

This is a reconfiguration of the same [small3d sample game](https://github.com/dimi309/small3d-samples/tree/master/gloom), this time using [conan](https://conan.io) for dependency management and building, which greatly decreses the build command complexity and the amount of configuration needed in the CMakeLists.txt files.

![screenshot](screenshot.png)


Building
--------

Add the [Bincrafters repository](https://bincrafters.readthedocs.io/en/latest/using_packages.html#adding-the-bincrafters-repository-as-a-conan-remote):

    conan remote add bincrafters https://bincrafters.jfrog.io/artifactory/api/conan/public-conan
    conan config set general.revisions_enabled=1
 
Set up the small3d package locally:

    git clone https://github.com/dimi309/small3d
	cd small3d
	cd conan_io
    conan export . small3d/master@
	 
Then clone this repository and build and run the game:

     git clone https://github.com/dimi309/gloom-game-conan
     cd gloom-game-conan
     conan install . --build=missing
     conan build .
     cd bin
     ./gloom
     # (Or just `gloom` on Windows)

Use `conan install . -s cppstd=14 --build=missing` for compilers that do not support C++14 or at least C++11 by default. Otherwise the build will fail.)
	
In order to build the game to run on Vulkan, execute `conan install . -o vulkan=True`. Otherwise, OpenGL is used by default.
