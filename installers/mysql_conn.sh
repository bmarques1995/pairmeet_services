if [ "$1" == "Debug" ] || [ "$1" == "Release" ] && [ ! -z "$2" ] && [ ! -z "$3" ]; then
    git clone --recursive -b trunk https://github.com/mysql/mysql-connector-cpp "$3/modules/mysql_conn_cpp"
    cd "$3/modules/mysql_conn_cpp"
    git reset --hard ac218f0d2d5b7957a8a2efee148bc8d7a1c623a5
    cd "$3"
    cmake -S "$3/modules/mysql_conn_cpp" -B "$3/dependencies/linux/mysql_conn_cpp" -DCMAKE_INSTALL_PREFIX="$2" -DCMAKE_BUILD_TYPE="Release"
    cmake --build "$3/dependencies/linux/mysql_conn_cpp" --target install
else
    echo "Invalid build type or install path. Please provide either 'Debug' or 'Release' and a valid prefix path"
fi
