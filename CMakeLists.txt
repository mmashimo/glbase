cmake_minimum_required(VERSION 3.0)

project(glbase)
set(PROJECT_VERSION 0.1)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

set (CMAKE_CXX_FLAGS "-g ${CMAKE_CXX_FLAGS}")

#if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#  set (CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")
#elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#  set (CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")
#endif ()
#if (APPLE)
#  set (CMAKE_CXX_FLAGS "-Wno-deprecated-declarations ${CMAKE_CXX_FLAGS}")
#endif ()

SET(GLBASE_SRCS
	src/glbase.cpp
	src/glbasewinprop.cpp
	src/glbase2d.cpp
	src/glbase3d.cpp
	src/glbaseobj.cpp
	src/glbasectrl.cpp
	src/glbasewin.cpp
)

add_library(glbase_obj OBJECT
	${GLBASE_SRCS}
)

target_include_directories(glbase_obj
 PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
	${OPENGL_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
)

# we need to enable -fPIC this so that the same object code can be used to
# create static *and* shared libraries without double compilation
set_target_properties( glbase_obj PROPERTIES POSITION_INDEPENDENT_CODE 1)

add_library(glbase
	SHARED
	$<TARGET_OBJECTS:glbase_obj>
)

target_include_directories(glbase
 PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:include>"
	${OPENGL_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
)

target_link_libraries(glbase
 PUBLIC
	${GLUT_LIBRARIES}
	${OPENGL_LIBRARIES}
)

set_target_properties(glbase
 PROPERTIES
	DEBUG_POSTFIX "d"
	VERSION ${PROJECT_VERSION}
	SOVERSION ${PROJECT_VERSION}
)

add_library(glbase_static
 STATIC
	$<TARGET_OBJECTS:glbase_obj>
)

target_include_directories(glbase_static
 PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:include>"
	${OPENGL_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
)

target_link_libraries(glbase_static
 PUBLIC
	${GLUT_LIBRARIES}
	${OPENGL_LIBRARIES}
)

set_target_properties(glbase_static PROPERTIES DEBUG_POSTFIX "d")

# Test inplementation of library before install
add_executable(demo1 demo/demo1.cpp)
#target_include_directories(demo1
#      PUBLIC
#      "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
#      ${OPENGL_INCLUDE_DIR}
#      ${GLUT_INCLUDE_DIR})
target_link_libraries(demo1 glbase_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(demo2 demo/demo2.cpp)
#target_include_directories(demo2
#      PUBLIC
#      "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
#      ${OPENGL_INCLUDE_DIR}
#     ${GLUT_INCLUDE_DIR})
target_link_libraries(demo2 glbase_static ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

####
# Installation

set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
write_basic_package_version_file(
    "${version_config}" VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
configure_package_config_file(
    "cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

# Targets:
install(
    TARGETS glbase_static glbase
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "${include_install_dir}"
)

# Headers:
install(
    DIRECTORY "${CMAKE_SOURCE_DIR}/include/"
    DESTINATION "${include_install_dir}"
    FILES_MATCHING PATTERN "*.h"
)


# Config
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

# Config
install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)
