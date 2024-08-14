if [ "$1" == "Debug" ] || [ "$1" == "Release" ] && [ ! -z "$2" ] && [ ! -z "$3" ]; then
    git clone --recursive https://github.com/Thalhammer/jwt-cpp.git "$3/modules/jwt_cpp"
    cd "$3/modules/jwt_cpp"
    git reset --hard 08bcf77a687fb06e34138e9e9fa12a4ecbe12332
    cd "$3"
    cmake -S "$3/modules/jwt_cpp" -B "$3/dependencies/linux/jwt_cpp" -DCMAKE_INSTALL_PREFIX="$2" -DCMAKE_BUILD_TYPE="$1"
    cmake --build "$3/dependencies/linux/jwt_cpp" --target install
else
    echo "Invalid build type or install path. Please provide either 'Debug' or 'Release' and a valid prefix path"
fi
