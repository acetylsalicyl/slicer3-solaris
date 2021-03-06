project(ITKCommandIO)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)
MARK_AS_ADVANCED(CMAKE_BACKWARDS_COMPATIBILITY)

 
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

if(NOT DEFINED BUILD_SHARED_LIBS)
  option(BUILD_SHARED_LIBS "Build with shared libraries." ON)
endif(NOT DEFINED BUILD_SHARED_LIBS)
 
# --------------------------------------------------------------------------
# Include dirs

set(include_dirs
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

foreach(package 
    SlicerIO
    )
  # This is mostly to test building this lib as standalone outside Slicer3
  if(NOT DEFINED ${package}_SOURCE_DIR)
    set(${package}_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../${package}")
  endif(NOT DEFINED ${package}_SOURCE_DIR)
  if(NOT DEFINED ${package}_BINARY_DIR)
    string(REPLACE ${PROJECT_NAME} ${package} 
      ${package}_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    link_directories(${${package}_BINARY_DIR})
  endif(NOT DEFINED ${package}_BINARY_DIR)

  set(include_dirs ${include_dirs}
    ${${package}_SOURCE_DIR}
    ${${package}_BINARY_DIR}
    )
endforeach(package)

include_directories(${include_dirs})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/itkCommandIOConfigure.h.in 
  ${CMAKE_CURRENT_BINARY_DIR}/itkCommandIOConfigure.h
  )

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES 
  ${headers} "${CMAKE_CURRENT_BINARY_DIR}/itkCommandIOConfigure.h" 
  DESTINATION include/${PROJECT_NAME} COMPONENT Development
  )

# --------------------------------------------------------------------------
# Sources

set(ITKCOMMANDIO_SRCS 
  itkCommandIO.cxx 
  itkCommandIOFactory.cxx
  )

# --------------------------------------------------------------------------
# Build and install the library

set(lib_name CommandIO)
add_library(${lib_name} 
  ${ITKCOMMANDIO_SRCS}
  )
target_link_libraries(${lib_name} 
  ITKIO
  )

install(TARGETS ${lib_name}  
  RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
  LIBRARY DESTINATION lib/${PROJECT_NAME} COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION lib/${PROJECT_NAME} COMPONENT Development
  )

# Shared library that when placed in ITK_AUTOLOAD_PATH, will add
# MRMLIDImageIO as an ImageIOFactory.  Need to have separate shared
# library for each new format. Note that the plugin library is placed
# in a special directory to speed up the searching for ImageIO
# factories (which improves the speed at which plugins run).

if(NOT DEFINED ITKCommandIO_INSTALL_ITKFACTORIES_DIR)
  set(ITKCommandIO_INSTALL_ITKFACTORIES_DIR lib/ITKFactories)
endif(NOT DEFINED ITKCommandIO_INSTALL_ITKFACTORIES_DIR)

add_library(CommandIOPlugin SHARED 
  itkCommandIOPlugin.cxx
  )
set_target_properties(CommandIOPlugin 
  PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY 
  "${CMAKE_BINARY_DIR}/${ITKCommandIO_INSTALL_ITKFACTORIES_DIR}"
  LIBRARY_OUTPUT_DIRECTORY 
  "${CMAKE_BINARY_DIR}/${ITKCommandIO_INSTALL_ITKFACTORIES_DIR}"
  ARCHIVE_OUTPUT_DIRECTORY 
  "${CMAKE_BINARY_DIR}/${ITKCommandIO_INSTALL_ITKFACTORIES_DIR}"
  )
target_link_libraries(CommandIOPlugin 
  ${lib_name}  
  ITKAlgorithms
  ITKNumerics 
  ITKStatistics
  ITKBasicFilters
  ITKIO
  ITKDICOMParser
  slicerio 
  ITKNrrdIO
)

# Note CommandIO and CommandIOPlugin are installed in different locations

# Apply user-defined properties to the library target.
IF(Slicer3_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(${lib_name} PROPERTIES
     ${Slicer3_LIBRARY_PROPERTIES}
  )
ENDIF(Slicer3_LIBRARY_PROPERTIES)

install(TARGETS CommandIOPlugin
  RUNTIME DESTINATION ${ITKCommandIO_INSTALL_ITKFACTORIES_DIR} COMPONENT RuntimeLibraries
  LIBRARY DESTINATION ${ITKCommandIO_INSTALL_ITKFACTORIES_DIR} COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION lib/${PROJECT_NAME} COMPONENT Development
  )

# make a copy named '.so' for compatibility with the expectations
# of the ITK Factory mechanism on macs

if(APPLE)
  add_custom_command(
    TARGET CommandIOPlugin
    DEPENDS "${CMAKE_BINARY_DIR}/${ITKCommandIO_INSTALL_ITKFACTORIES_DIR}/libCommandIOPlugin.dylib"
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_BINARY_DIR}/${ITKCommandIO_INSTALL_ITKFACTORIES_DIR}/libCommandIOPlugin.dylib"
    "${CMAKE_BINARY_DIR}/${ITKCommandIO_INSTALL_ITKFACTORIES_DIR}/libCommandIOPlugin.so"
    VERBATIM
    )
  install(FILES
    "${CMAKE_BINARY_DIR}/${ITKCommandIO_INSTALL_ITKFACTORIES_DIR}/libCommandIOPlugin.so"
    DESTINATION ${ITKCommandIO_INSTALL_ITKFACTORIES_DIR}
    )
endif(APPLE)
