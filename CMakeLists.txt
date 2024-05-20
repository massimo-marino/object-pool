CMAKE_MINIMUM_REQUIRED(VERSION 3.26)

SET (CMAKE_CXX_COMPILER clang++)

PROJECT(object-pool-lib)

# C++ version
SET (CMAKE_CXX_STANDARD 20)
SET (CMAKE_CXX_STANDARD_REQUIRED ON)
SET (CMAKE_CXX_EXTENSIONS OFF)

SET (CMAKE_EXPORT_COMPILE_COMMANDS on)
SET (CMAKE_VERBOSE_MAKEFILE on )

SET (CMAKE_BUILD_TYPE Release) # Debug, Release, RelWithDebInfo, MinSizeRel

# Set the build type (Debug, Release, RelWithDebInfo, MinSizeRel)
if(NOT CMAKE_BUILD_TYPE)
  SET (CMAKE_BUILD_TYPE Release)
endif()

# Set more compiler flags for debug mode; -g is automatically set
if(CMAKE_BUILD_TYPE MATCHES Debug)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

# Set more compiler flags for release mode; -O3 is automatically set
if(CMAKE_BUILD_TYPE MATCHES Release)
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (src/example)
ADD_SUBDIRECTORY (src/unitTests)

