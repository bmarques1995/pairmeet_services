if [ "$1" == "Debug" ] || [ "$1" == "Release" ] && [ ! -z "$2" ] && [ ! -z "$3" ]; then
    git clone --recursive https://github.com/drogonframework/drogon.git "$3/modules/drogon"
    cd "$3/modules/drogon"
    git reset --hard e78690747858f49d7ea08002083420ff046d8785
    cd "$3"
    cmake -S "$3/modules/drogon" -B "$3/dependencies/linux/drogon" -DCMAKE_INSTALL_PREFIX="$2" -DCMAKE_PREFIX_PATH="$2" -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE="$1"
    cmake --build "$3/dependencies/linux/drogon" --target install
else
    echo "Invalid build type or install path. Please provide either 'Debug' or 'Release' and a valid prefix path"
fi