cmake_minimum_required(VERSION 3.12)
project(GLStuff)

set(CMAKE_CXX_STANDARD 11)

# Vérifier le système d'exploitation
if(UNIX AND NOT APPLE)
    message(STATUS "Configuring for Linux")
    set(DEPENDENCIES_DIR deps)
    add_subdirectory(deps/glfw)
    set(LINUX_DEPENDENCIES pthread GL X11 Xrandr Xi dl)
# set(LINUX_DEPENDENCIES pthread GL pthread glfw3-wayland)

elseif(WIN32)
    message(STATUS "Configuring for Windows")
    set(DEPENDENCIES_DIR "C:/Users/Naim/Code/Dependencies")
    set(WINDOWS_DEPENDENCIES )

else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

# Ajouter les fichiers source de votre projet
file(GLOB SOURCES
    src/glad/glad.c
    src/*.cpp
)

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
)
FetchContent_MakeAvailable(glm)

# Ajouter les répertoires d'inclusion
include_directories(
  #${DEPENDENCIES_DIR}/Include
  ./include
  ${OPENGL_INCLUDE_DIRS} 
)

# Same for linking
link_directories(${DEPENDENCIES_DIR}/Libs)

# Options de compilation
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Variables pour les répertoires de sortie
set(DEBUG_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/debug)
set(RELEASE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/release)

# Créer les dossiers bin/debug et bin/release s'ils n'existent pas
file(MAKE_DIRECTORY ${DEBUG_OUTPUT_PATH})
file(MAKE_DIRECTORY ${RELEASE_OUTPUT_PATH})

# Configuration pour le mode Debug
add_executable(GLStuffDebug ${SOURCES} )
set_target_properties(GLStuffDebug PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${DEBUG_OUTPUT_PATH}
)

# Liens avec les bibliothèques
target_link_libraries(GLStuffDebug
    PRIVATE
    glfw
    glm::glm
    ${LINUX_DEPENDENCIES}
    ${WINDOWS_DEPENDENCIES}
)

# Configuration pour le mode Release
add_executable(GLStuffRelease ${SOURCES} )
set_target_properties(GLStuffRelease PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${RELEASE_OUTPUT_PATH}
)

# Liens avec les bibliothèques
target_link_libraries(GLStuffRelease
    PRIVATE
    glfw
    glm::glm
    ${LINUX_DEPENDENCIES}
    ${WINDOWS_DEPENDENCIES}
)
