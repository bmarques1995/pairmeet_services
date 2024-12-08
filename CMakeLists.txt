cmake_minimum_required(VERSION 3.22.0)

project(SampleServer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_helpers)

include(dep_tracer)
include(target_destination)

trace_installable_file(NAME "zlib" EXTENSION "txt" INSTALL_SCRIPT ${PROJECT_SOURCE_DIR}/installers/zlib.sh LOCATION ${CMAKE_PREFIX_PATH}/lib/manifest)
trace_dependency(NAME "zstd" INSTALL_SCRIPT ${PROJECT_SOURCE_DIR}/installers/zstd.sh)
trace_library(NAME brotlidec INSTALL_SCRIPT ${PROJECT_SOURCE_DIR}/installers/brotli.sh)
trace_library(NAME brotlienc INSTALL_SCRIPT ${PROJECT_SOURCE_DIR}/installers/brotli.sh)
trace_library(NAME brotlicommon INSTALL_SCRIPT ${PROJECT_SOURCE_DIR}/installers/brotli.sh)
trace_installable_file(NAME "openssl" EXTENSION "txt" INSTALL_SCRIPT ${PROJECT_SOURCE_DIR}/installers/openssl.sh LOCATION ${CMAKE_PREFIX_PATH}/lib/manifest)
find_package(OpenSSL REQUIRED)
trace_dependency(NAME "spdlog" INSTALL_SCRIPT ${PROJECT_SOURCE_DIR}/installers/spdlog.sh)
trace_dependency(NAME "mysql-concpp" INSTALL_SCRIPT ${PROJECT_SOURCE_DIR}/installers/mysql_conn.sh)
trace_installable_file(NAME "libjsoncpp" INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/jsoncpp.sh" LOCATION "${CMAKE_PREFIX_PATH}/lib" EXTENSION "so")
find_package(jsoncpp)
trace_dependency(NAME "nlohmann_json" INSTALL_SCRIPT ${PROJECT_SOURCE_DIR}/installers/nlohmann_json.sh)
trace_dependency(NAME "jwt-cpp" INSTALL_SCRIPT ${PROJECT_SOURCE_DIR}/installers/jwt.sh)
trace_dependency(NAME "Boost" COMPONENTS System date_time regex unit_test_framework COMPONENT_INFIX "_" INSTALL_SCRIPT ${PROJECT_SOURCE_DIR}/installers/boost.sh)
trace_dependency(NAME "mailio" INSTALL_SCRIPT ${PROJECT_SOURCE_DIR}/installers/mailio.sh)
trace_installable_file(NAME "libdrogon" INSTALL_SCRIPT "${PROJECT_SOURCE_DIR}/installers/drogon.sh" LOCATION "${CMAKE_PREFIX_PATH}/lib" EXTENSION "so")
find_package(Drogon)

file(GLOB_RECURSE SAMPLE_HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hpp")
file(GLOB_RECURSE SAMPLE_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_executable(SampleServer ${SAMPLE_SRCS})
target_link_libraries(SampleServer PUBLIC Drogon::Drogon OpenSSL::Crypto jsoncpp_lib mysql::concpp-xdevapi jwt-cpp::jwt-cpp mailio)
set_cxx_project_standards(SampleServer "20" OFF)
