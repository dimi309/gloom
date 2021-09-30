Gloom
=====

You are in a gloomy place and some anthropoids are trying to kill you.
Kill them first.

Move with the arrows. Shoot with the spacebar. Escape to quit the game.

This is a reconfiguration of the same [small3d sample game](https://github.com/dimi309/small3d-samples/tree/master/gloom), this time using [conan](https://conan.io) for dependency management and building, which greatly decreses the build command complexity and the amount of configuration needed in the CMakeLists.txt files.

![screenshot](screenshot.png)


Building
--------

Set up [small3d](https://github.com/dimi309/small3d-conan) with [conan](https://conan.io) on your system. Then just clone this repository and run:

	conan install .
	conan build .
	
... and run the game from the `bin` directory.
	
