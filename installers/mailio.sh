if [ "$1" == "Debug" ] || [ "$1" == "Release" ] && [ ! -z "$2" ] && [ ! -z "$3" ]; then
    git clone --recursive https://github.com/karastojko/mailio.git "$3/modules/mailio"
    cd "$3/modules/mailio"
    git reset --hard e8cd4045f833217caf6073a91c2691a1c2fb2912
    cd "$3"
    cmake -S "$3/modules/mailio" -B "$3/dependencies/linux/mailio" -DCMAKE_INSTALL_PREFIX="$2" -DCMAKE_BUILD_TYPE="$1" -DBUILD_SHARED_LIBS=ON
    cmake --build "$3/dependencies/linux/mailio" --target install
else
    echo "Invalid build type or install path. Please provide either 'Debug' or 'Release' and a valid prefix path"
fi
