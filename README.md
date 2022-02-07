Gloom
=====

[![Build status](https://ci.appveyor.com/api/projects/status/5wkolhu8qg87j992?svg=true)](https://ci.appveyor.com/project/dimi309/gloom-game-conan)

You are in a gloomy place and some anthropoids are trying to kill you.
Kill them first.

Move with the arrows. Shoot with the spacebar. Escape to quit the game.

![screenshot](screenshot.png)

# Building

## Prerequisites

- A C++ compiler
- Git
- The [conan](https://conan.io) package manager
	
## Procedure

Add the [Barbarian package index](https://barbarian.bfgroup.xyz):

	conan remote add barbarian-github https://barbarian.bfgroup.xyz/github
	 
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
