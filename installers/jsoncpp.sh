if [ "$1" == "Debug" ] || [ "$1" == "Release" ] && [ ! -z "$2" ] && [ ! -z "$3" ]; then
    git clone --recursive https://github.com/open-source-parsers/jsoncpp.git "$3/modules/jsoncpp"
    cd "$3/modules/jsoncpp"
    git reset --hard 5defb4ed1a4293b8e2bf641e16b156fb9de498cc
    cd "$3"
    cmake -S "$3/modules/jsoncpp" -B "$3/dependencies/linux/jsoncpp" -DCMAKE_INSTALL_PREFIX="$2" -DCMAKE_BUILD_TYPE="$1"
    cmake --build "$3/dependencies/linux/jsoncpp" --target install
else
    echo "Invalid build type or install path. Please provide either 'Debug' or 'Release' and a valid prefix path"
fi
