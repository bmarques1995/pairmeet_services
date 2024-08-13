if [ "$1" == "Debug" ] || [ "$1" == "Release" ] && [ ! -z "$2" ] && [ ! -z "$3" ]; then
    git clone --recursive https://github.com/gabime/spdlog.git "$3/modules/spdlog"
    cd "$3/modules/spdlog"
    git reset --hard 27cb4c76708608465c413f6d0e6b8d99a4d84302
    cd "$3"
    cmake -S "$3/modules/spdlog" -B "$3/dependencies/linux/spdlog" -DCMAKE_INSTALL_PREFIX="$2" -DCMAKE_BUILD_TYPE="$1" -DBUILD_SHARED_LIBS=ON
    cmake --build "$3/dependencies/linux/spdlog" --target install
else
    echo "Invalid build type or install path. Please provide either 'Debug' or 'Release' and a valid prefix path"
fi
