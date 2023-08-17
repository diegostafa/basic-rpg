cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
set(buildDest "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(VCPKG_ROOT "/opt/vcpkg/installed/x64-linux")
set(out "basic-rpg")

project(
    basic-rpg
    VERSION 1.0
    DESCRIPTION "basic-rpg"
    HOMEPAGE_URL "https://github.com/diegostafa/basic-rpg"
    LANGUAGES C CXX
)

find_package(raylib)

file(GLOB_RECURSE sources
    "src/*.cpp"
    "deps/ray-widgets/core/*.cpp"
    "deps/ray-widgets/extra/*.cpp"
    "deps/ray-widgets/helpers/*.cpp"
    "deps/ray-widgets/utils/*.cpp"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${VCPKG_ROOT}/include)

set(libs "-ltmxlite")

add_executable(${out} ${sources})

target_link_libraries(${out} PRIVATE raylib)
target_link_libraries(${out} PRIVATE ${libs})
target_link_directories(${out} PRIVATE ${VCPKG_ROOT}/lib)

