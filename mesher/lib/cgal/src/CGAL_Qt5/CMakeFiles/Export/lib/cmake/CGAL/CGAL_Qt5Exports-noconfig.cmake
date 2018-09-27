#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CGAL::CGAL_Qt5" for configuration ""
set_property(TARGET CGAL::CGAL_Qt5 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(CGAL::CGAL_Qt5 PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libCGAL_Qt5.so.13.0.2"
  IMPORTED_SONAME_NOCONFIG "libCGAL_Qt5.so.13"
  )

list(APPEND _IMPORT_CHECK_TARGETS CGAL::CGAL_Qt5 )
list(APPEND _IMPORT_CHECK_FILES_FOR_CGAL::CGAL_Qt5 "${_IMPORT_PREFIX}/lib/libCGAL_Qt5.so.13.0.2" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
