cmake_minimum_required(VERSION 3.19)
project(platform_info)

set(CMAKE_CXX_STANDARD 17)

# https://www.siliceum.com/en/blog/post/cmake_02_customization-points
if (CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo;Release" CACHE STRING "" FORCE)
endif (CMAKE_CONFIGURATION_TYPES)

# Introduce the vendorize(platform library semantic_version) syntax for
# providing libraries from known local sources (vendoring).
include(vendor/common/vendorize-cmake-0.1/vendorize.cmake)

vendorize(common catch2 2.13.0)
include_directories(SYSTEM ${VENDOR_CATCH2_ROOT})

include(cmake/CompilerOptions.cmake)

add_subdirectory(src)
add_subdirectory(test)
