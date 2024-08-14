if [ "$1" == "Debug" ] || [ "$1" == "Release" ] && [ ! -z "$2" ] && [ ! -z "$3" ]; then
    git clone --recursive https://github.com/nlohmann/json.git "$3/modules/nlohmann_json"
    cd "$3/modules/nlohmann_json"
    git reset --hard 9cca280a4d0ccf0c08f47a99aa71d1b0e52f8d03
    cd "$3"
    cmake -S "$3/modules/nlohmann_json" -B "$3/dependencies/linux/nlohmann_json" -DCMAKE_INSTALL_PREFIX="$2" -DCMAKE_BUILD_TYPE="$1"
    cmake --build "$3/dependencies/linux/nlohmann_json" --target install
else
    echo "Invalid build type or install path. Please provide either 'Debug' or 'Release' and a valid prefix path"
fi
