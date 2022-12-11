# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

cmake_minimum_required(VERSION 3.1...3.15)

project( optimal_polygon )


# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  

endif()


# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()  

endif()

# include for local directory
add_subdirectory(parsing_input)
add_subdirectory(polygonization_algorithms)
add_subdirectory(calculate_convex_hull)
add_subdirectory(write_results)
add_subdirectory(polygon_optimization_algorithms)
list(APPEND EXTRA_LIBS parsing_input polygonization_algorithms calculate_convex_hull write_results polygon_optimization_algorithms)
# include for local package


# Creating entries for target: optimal_polygon
# ############################

add_executable( optimal_polygon  main.cpp )

add_to_cached_list( CGAL_EXECUTABLE_TARGETS optimal_polygon )

# Link the executable to CGAL and third-party libraries
#target_link_libraries(optimal_polygon PRIVATE CGAL::CGAL )
target_link_libraries(optimal_polygon PRIVATE CGAL::CGAL ${EXTRA_LIBS})
