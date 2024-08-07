if [ "$1" == "Debug" ] || [ "$1" == "Release" ] && [ ! -z "$2" ] && [ ! -z "$3" ]; then
    git clone --recursive -b openssl-3.3 https://github.com/openssl/openssl.git "$3/modules/openssl"
    cd "$3/modules/openssl"
    git reset --hard db2ac4f6ebd8f3d7b2a60882992fbea1269114e2
    echo $(pwd)
    ./Configure "--${1,,}" --prefix="$2" --openssldir="$2/openssl" -shared zlib-dynamic --with-zlib-lib="$2/lib" --with-zlib-include="$2/include" --with-brotli-include="$2/include" --with-brotli-lib="$2/lib" enable-zstd-dynamic --with-zstd-lib="$2/lib" --with-zstd-include="$2/include"
    make -j6
    make install
    cd ../..
else
    echo "Invalid build type or install path. Please provide either 'Debug' or 'Release' and a valid prefix path"
fi