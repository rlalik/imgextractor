cmake_minimum_required(VERSION 2.6)
project(ImgExtractor C CXX)
set(CAPS_PROJECT_NAME IMGEXTRACTOR)

##### the version number
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_PATCH 1)
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ARCH_LIB lib64)
else()
	set(ARCH_LIB lib32)
	set(FIND_LIBRARY_USE_LIB64_PATHS OFF)
endif()

set(LIB_INSTALL_DIR ${ARCH_LIB} CACHE STRING "library location")

##### set paths
if(WIN32 AND NOT CYGWIN)
	set(DEF_CMAKE_INSTALL_CMAKEDIR cmake)
else()
	set(DEF_CMAKE_INSTALL_CMAKEDIR lib/cmake/${CMAKE_PROJECT_NAME})
endif()
set(CMAKE_INSTALL_CMAKEDIR ${DEF_CMAKE_INSTALL_CMAKEDIR} CACHE PATH "Installation directory for CMake files")
set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "object code libraries (lib)")
set(CMAKE_INSTALL_LIBDIR "${LIB_INSTALL_DIR}" CACHE PATH "object code libraries (lib)")
set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "object code libraries (lib)")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

##### find ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT QUIET
	REQUIRED COMPONENTS Core  RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread
	OPTIONAL_COMPONENTS Cling Cint)

include(${ROOT_USE_FILE})

find_package(RootTools)

##### directories
include_directories(${ROOT_INCLUDE_DIR} ${ROOTTOOLS_INCLUDE_DIR})
link_directories(${ROOT_LIBRARIES_DIR} ${ROOTTOOLS_LIBRARY_DIR})

set(imgextractor_SRCS imgextract.cpp)

add_executable(imgextractor ${imgextractor_SRCS})

target_link_libraries(imgextractor RootTools ASImage ${ROOT_LIBRARIES} jsoncpp_lib)

# ${${BIN_PREFIX}_corrdet}
install(TARGETS
		imgextractor
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

####################################################
# SUMMARY
####################################################
MESSAGE(STATUS "<<< Configuration >>>
Project:                ${CMAKE_PROJECT_NAME}
Generator:              ${CMAKE_GENERATOR}

System name             ${CMAKE_SYSTEM_NAME}

Source dir              ${CMAKE_CURRENT_SOURCE_DIR}
Build dir               ${CMAKE_CURRENT_BINARY_DIR}

Build type      		${CMAKE_BUILD_TYPE}
Install path            ${CMAKE_INSTALL_PREFIX}
Compiler:
C                       ${CMAKE_C_COMPILER}
C++                     ${CMAKE_CXX_COMPILER}
Linker:
Ld                      ${CMAKE_LINKER}
Compiler flags:
C                       ${CMAKE_C_FLAGS}
C+                      ${CMAKE_CXX_FLAGS}
Linker flags:
Executable              ${CMAKE_EXE_LINKER_FLAGS}
Module                  ${CMAKE_MODULE_LINKER_FLAGS}
Shared                  ${CMAKE_SHARED_LINKER_FLAGS}\n")
