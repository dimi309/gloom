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

And of course, [the small3d library](https://github.com/dimi309/small3d) too:

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
	 
Make sure you clean the repository between different builds:

	git clean -fdx
	
## Building the game specifically on FreeBSD

I have recently been told that it might be a good idea to contribute a port of
this game to FreeBSD. While technically I could do it, it is not clear to me 
whether or not it would be accepted as a contribution or how to go about 
"applying" for something like that. I might figure it out someday but, in the
meantime, I think that anyone can perfectly download the source code and build
the game for themselves on that platform. The whole point of using conan.io is
to make such a process a bit simpler :) 

Since some more steps are required than on Linux, I am listing a step-by-step
walkthrough here below, which also avoids the use of git.

If you try this I would love to hear from you, whether it be about questions and 
problems, or even to just tell me that it has worked, so please do not hesitate 
to open an [issue](https://github.com/dimi309/gloom/issues).

### Steps

Execute the following on the command line:

	pkg update
	pkg install gcc cmake conan
	conan profile detect

	curl -LO https://github.com/dimi309/portaudio-conan/archive/refs/heads/master.zip
	unzip master.zip
	rm master.zip
	cd portaudio-conan-master
	conan export . --version=19.7.0
	cd ..

	curl -LO https://github.com/dimi309/small3d-conan/archive/refs/heads/master.zip
	unzip master.zip
	rm master.zip
	cd small3d-conan-master
	conan export .
	cd ..

	curl -LO https://github.com/dimi309/gloom/archive/refs/heads/master.zip
	unzip master.zip
	rm master.zip
	cd gloom-master
	conan build . --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True

	cd bin
	./gloom

You will have to be in X windows for the game to run. 

If there is no sound in the game, you probably need to set up jack audio. I find
this guide quite useful to that end: https://github.com/0EVSG/freebsd_jack_notes#jack-configuration
