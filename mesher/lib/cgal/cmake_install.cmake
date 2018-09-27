# Install script for directory: /home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1

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
    set(CMAKE_INSTALL_CONFIG_NAME "")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/CGAL-4.12.1" TYPE FILE FILES
    "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/AUTHORS"
    "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/CHANGES.md"
    "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/LICENSE"
    "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/LICENSE.FREE_USE"
    "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/LICENSE.GPL"
    "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/LICENSE.LGPL"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/include/CGAL" REGEX "/\\.svn$" EXCLUDE)
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/include/CGAL" REGEX "/\\.svn$" EXCLUDE)
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES
    "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/scripts/cgal_create_CMakeLists"
    "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/scripts/cgal_create_cmake_script"
    "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/scripts/cgal_make_macosx_app"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/CGAL" TYPE DIRECTORY FILES "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/cmake/modules/")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/CGAL" TYPE FILE FILES "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/cmake/modules/UseCGAL.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/CGAL" TYPE FILE FILES
    "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/CGALConfigVersion.cmake"
    "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/config/CGALConfig.cmake"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/auxiliary/cgal_create_cmake_script.1")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/src/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/admin-suz/Documents/Code/CGAL_TEST/CGAL-4.12.1/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
