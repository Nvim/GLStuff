cmake_minimum_required(VERSION 3.12)

option(PRODUCTION_BUILD "Make this a production build" OFF)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization
# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(GLStuff)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(DGLM_BUILD_TESTS OFF CACHE BOOL "" FORCE) 
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(DEPENDENCIES_DIR deps)
add_subdirectory(${DEPENDENCIES_DIR}/assimp)
add_subdirectory(${DEPENDENCIES_DIR}/glfw)
add_subdirectory(${DEPENDENCIES_DIR}/glad)
add_subdirectory(${DEPENDENCIES_DIR}/glm)
add_subdirectory(${DEPENDENCIES_DIR}/stb_image)
add_subdirectory(${DEPENDENCIES_DIR}/imgui-docking)


add_executable("${CMAKE_PROJECT_NAME}")
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)
file (GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${SOURCES} )

# Useful macros (ressources path and asserts debug):
if(PRODUCTION_BUILD)
	# setup the ASSETS_PATH macro to be in the root folder of your exe
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") 
	# remove the option to debug asserts.
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 
else()
	# This is useful to get an ASSETS_PATH in your IDE during development
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 
endif()

# debug vs release config:
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
else()
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

if(MSVC) # If using the VS compiler...
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
	set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") #no console
  set(ADDITIONAL_DEPENDENCIES)
elseif(UNIX)
  set(ADDITIONAL_DEPENDENCIES pthread GL X11 Xrandr Xi dl)
endif()

# include my .h files:
# target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
# target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ./include)
include_directories(
  #${DEPENDENCIES_DIR}/Include
  ./include
  ${ASSIMP_INCLUDE_DIRS}
)

# link with deps + platform specific libs
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE 
  glm 
  glfw 
	glad 
  stb_image
  assimp
  imgui
  ${ADDITIONAL_DEPENDENCIES}
)
