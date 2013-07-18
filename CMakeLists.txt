project(pp35DataAnalysis)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-g --std=c++0x ${CMAKE_CXX_FLAGS}")

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Modules" ${CMAKE_MODULE_PATH})
find_package(ROOT)
#find_package(HYDRA)

set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "1")
set(PROJECT_VERSION_PATCH "0")
set(PROJECT_VERSION_COUNT 1)
set(PROJECT_VERSION
	"${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

set(PROJECT_VENDOR "RLalik")
set(PROJECT_COPYRIGHT_YEAR "2013")
# set(PROJECT_DOMAIN_FIRST "gasik")
# set(PROJECT_DOMAIN_SECOND "com")
# set(PROJECT_DOMAIN "${PROJECT_DOMAIN_FIRST}.${PROJECT_DOMAIN_SECOND}")

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")

set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
	set(var INSTALL_${p}_DIR)
	if(NOT IS_ABSOLUTE "${${var}}")
		set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
	endif()
endforeach()

if ( CMAKE_COMPILER_IS_GNUCXX OR APPLE )
	add_definitions(-Wall -Wextra)
	if ( CMAKE_BUILD_RELEASE )
		add_definitions(-Werror)
	endif ()
endif ()

add_definitions(-DPROJECT_VERSION=\"${PROJECT_VERSION}\")

##### find programs

find_path(roottools_LIBS libRootTools.so PATHS ${RT_LIB_DIR} PATH_SUFFIXES lib)
if (NOT roottools_LIBS)
        message(FATAL_ERROR "Unable to find libRootTools.so")
else ()
        message(STATUS "Found libRootTools.so in ${roottools_LIBS}")
endif (NOT roottools_LIBS)


set(imgextractor_SRCS imgextract.cpp)

set (INC_DIRS_G
	${ROOT_INCLUDES}
	${roottools_LIBS}/../include
	${CMAKE_CURRENT_SOURCE_DIR}
)

set (LIB_DIRS_G
	${ROOT_LIBRARY_DIR}
	${roottools_LIBS}
)

include_directories(${INC_DIRS_G})
link_directories(${LIB_DIRS_G})

add_executable(imgextractor ${imgextractor_SRCS})

target_link_libraries(imgextractor RootTools ASImage ${ROOT_LIBRARIES})

# ${${BIN_PREFIX}_corrdet}
install(TARGETS
		imgextractor
	RUNTIME DESTINATION ${INSTALL_BIN_DIR}
	LIBRARY DESTINATION ${INSTALL_LIB_DIR}
	ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
	PUBLIC_HEADER DESTINATION ${INSTALL_INCLUDE_DIR})

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
