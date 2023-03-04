Gloom
=====

[![Build status](https://ci.appveyor.com/api/projects/status/79ttme5r1i3m217q?svg=true)](https://ci.appveyor.com/project/dimi309/gloom)

You are in a gloomy place and some anthropoids are trying to kill you.
Kill them first.

Move with the arrows. Shoot with the spacebar. Escape to quit the game.

![screenshot](screenshot.png)

# Building

## Prerequisites

- A C++ compiler
- Git
- The [conan](https://conan.io) package manager - recently migrated to conan 2.0.
	
## Procedure


Deploy my variation of the portaudio package for conan as a prerequisite:

	git clone https://github.com/dimi309/portaudio-conan
	cd portaudio-conan
	conan export . --version=19.7.0
	
If you would like to use Vulkan, you will also need my variation of the
vulkan-loader package (this is temporary until the conan center index
verion gets fixed - it has a small bug):

	git clone https://github.com/dimi309/vulkan-loader-conan
	cd vulkan-loader-conan
	conan export . --version=1.3.239.0

And of course, small3d too:

	git clone https://github.com/dimi309/small3d-conan
	cd small3d-conan
	conan export .

Then clone this repository and build and run the game:

     git clone https://github.com/dimi309/gloom
     cd gloom
     conan build . --build=missing
     cd bin
     ./gloom
     # (Or just `gloom` on Windows)
	 
This will build the game for OpenGL. In order to build it
for vulkan, replace the build command by:

	conan build . -o small3d/*:vulkan=True --build=missing
	
Make sure you clean the repository between different build:

	git clean -fdx
