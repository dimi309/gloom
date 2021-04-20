if [ "$1" != "Debug" ] && [ "$1" != "Release" ]; then
    echo "Please indicate build type: Debug or Release, followed by opengl if you would like to also prepare OpenGL-related libraries."
    exit 1
fi

if [ ! -z "$2" ] && [ "$2" != "opengl" ]; then
    echo "The second parameter, if entered, can only be opengl if you would like to build for OpenGL."
    exit 1
fi

exit_if_error() {
    rc=$?
    if [[ $rc != 0 ]]; then
	echo "Exiting on error."
	exit $rc
    fi
}

echo "Entering small3d directory (must have been copied here)..."
cd small3d/scripts
exit_if_error
./build.sh $1 $2
exit_if_error
cd ..

if [ "$2" == "opengl" ]; then
    export CMAKE_DEFINITIONS="-DCMAKE_BUILD_TYPE=$1 -DSMALL3D_OPENGL=ON"
else
    export CMAKE_DEFINITIONS=-DCMAKE_BUILD_TYPE=$1
fi

for sampledir in avoidthebug ball chasethegoat frogremixed gloom serial-model-renderer
do

    if [ "$2" == "opengl" ] && [ "$sampledir" == "ball" ]; then
	echo "Not building ball sample for OpenGL"
	continue
    fi
    cd $sampledir
    if [ -d "deps" ]; then rm -rf deps; fi
    mkdir deps
    cd deps
    for depdir in include lib shaders
    do
	cp -rf ../../small3d/build/$depdir .
	exit_if_error
    done
    cd ..
    if [ -d "build" ]; then rm -rf build; fi
    mkdir build
    cd build
    cmake .. $CMAKE_DEFINITIONS
    exit_if_error
    cmake --build .
    exit_if_error
    cd ../..
done

echo "all games built successfully"


