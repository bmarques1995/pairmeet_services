if [ "$1" == "Debug" ] || [ "$1" == "Release" ] && [ ! -z "$2" ] && [ ! -z "$3" ]; then
    git clone --recursive -b master https://github.com/madler/zlib.git "$3/modules/zlib"
    cmake -S "$3/modules/zlib" -B "$3/dependencies/linux/zlib" -DCMAKE_INSTALL_PREFIX="$2" -DCMAKE_BUILD_TYPE="$1"
    cmake --build "$3/dependencies/linux/zlib" --target install
    cd "$2/lib"
    mkdir "manifest"
    echo "zlib own manifest" > "./manifest/zlib.txt"
else
    echo "Invalid build type or install path. Please provide either 'Debug' or 'Release' and a valid prefix path"
fi