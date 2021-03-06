cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# https://cmake.org/cmake/help/v3.0/policy/CMP0025.html
if(POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif()

# NB: this version number is duplicated in
project(raytracer VERSION 0.0.1 LANGUAGES CXX)

include(GNUInstallDirs)

# <Boost>
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(
        Boost
        1.58 # only 1.58 because that's the version apt-get installs on ubuntu 16.04
        REQUIRED
        COMPONENTS
        log_setup
        log
)
add_library(boost INTERFACE IMPORTED)
set_property(TARGET boost PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
add_definitions(-DBOOST_ALL_DYN_LINK)
# </Boost>

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_EXTENSIONS                OFF)
set(CMAKE_SKIP_BUILD_RPATH              false)
set(CMAKE_BUILD_WITH_INSTALL_RPATH      false)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH   true)
set(CMAKE_MACOSX_RPATH                  1) # Ensure that RPATH is used on OSX

# Help spot unknown compilers and versions.
# Other compilers are probably supported just fine but aren't officially checked
if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
        message(FATAL_ERROR "Insufficient Apple clang version - XCode 9+ required")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.0")
        message(FATAL_ERROR "Insufficient clang version - clang 5.0+ required")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.3.0")
        message(FATAL_ERROR "Insufficient GCC/GNU version - gnu/gcc 7.3.0+ required")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.14.26430")
        message(FATAL_ERROR "Insufficient Microsoft Visual C++ version - VS 2017 15.7+ (compiler version 19.14.26430). Found compiler version ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
else()
    message(FATAL_ERROR "Unknown compiler... ${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION}")
endif()

# If the user did not customize the install prefix to ${genny_root}/install
# set it to live under build so we don't inadverently pollute /usr/local
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "default install path" FORCE)
endif()

enable_testing()
add_subdirectory(src)
