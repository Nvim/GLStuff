cmake_minimum_required(VERSION 3.12)
project(GLStuff)

set(CMAKE_CXX_STANDARD 11)

# Vérifier le système d'exploitation
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Configuring for Linux")
    set(DEPENDENCIES_DIR "/home/naim/Code/Dependencies")
    set(LINUX_DEPENDENCIES pthread GL X11 Xrandr Xi dl glfw3)
# set(LINUX_DEPENDENCIES pthread GL pthread glfw3-wayland)

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Configuring for Windows")
    set(DEPENDENCIES_DIR "C:/Users/Naim/Code/Dependencies")
    set(WINDOWS_DEPENDENCIES )

else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()

find_package( OpenGL REQUIRED )


# Ajouter les fichiers source de votre projet
file(GLOB SOURCES
    src/glad/glad.c
    src/*.cpp
)

# Ajouter les répertoires d'inclusion
include_directories(
  ${DEPENDENCIES_DIR}/Include
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
    ${LINUX_DEPENDENCIES}
    ${WINDOWS_DEPENDENCIES}
)
