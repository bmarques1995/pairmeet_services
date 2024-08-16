if [ "$1" == "Debug" ] || [ "$1" == "Release" ] && [ ! -z "$2" ] && [ ! -z "$3" ]; then
    git clone --recursive https://github.com/boostorg/boost.git "$3/modules/boost"
    cd "$3/modules/boost"
    git reset --hard 65c1319bb92fe7a9a4abd588eff5818d9c2bccf9
    cd "$3"
    cmake -S "$3/modules/boost" -B "$3/dependencies/linux/boost" -DCMAKE_INSTALL_PREFIX="$2" -DCMAKE_BUILD_TYPE="$1" -DBUILD_SHARED_LIBS=ON
    cmake --build "$3/dependencies/linux/boost" --target install
else
    echo "Invalid build type or install path. Please provide either 'Debug' or 'Release' and a valid prefix path"
fi
