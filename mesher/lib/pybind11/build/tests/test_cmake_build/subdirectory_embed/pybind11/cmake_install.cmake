# Install script for directory: /home/admin-suz/Documents/Code/Interface_CGAL_python/mesher/lib/pybind11

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/admin-suz/Documents/Code/Interface_CGAL_python/mesher/lib/pybind11/include/pybind11")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/pybind11" TYPE FILE FILES
    "/home/admin-suz/Documents/Code/Interface_CGAL_python/mesher/lib/pybind11/build/tests/test_cmake_build/subdirectory_embed/pybind11/pybind11Config.cmake"
    "/home/admin-suz/Documents/Code/Interface_CGAL_python/mesher/lib/pybind11/build/tests/test_cmake_build/subdirectory_embed/pybind11/pybind11ConfigVersion.cmake"
    "/home/admin-suz/Documents/Code/Interface_CGAL_python/mesher/lib/pybind11/tools/FindPythonLibsNew.cmake"
    "/home/admin-suz/Documents/Code/Interface_CGAL_python/mesher/lib/pybind11/tools/pybind11Tools.cmake"
    )
endif()

