#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "CGAL::CGAL" for configuration ""
set_property(TARGET CGAL::CGAL APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(CGAL::CGAL PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libCGAL.so.13.0.2"
  IMPORTED_SONAME_NOCONFIG "libCGAL.so.13"
  )

list(APPEND _IMPORT_CHECK_TARGETS CGAL::CGAL )
list(APPEND _IMPORT_CHECK_FILES_FOR_CGAL::CGAL "${_IMPORT_PREFIX}/lib/libCGAL.so.13.0.2" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
