cmake_minimum_required( VERSION 3.6.2 )
project( VLight )

set( VLIGHT_MAJOR_VERSION 1 )
set( VLIGHT_MINOR_VERSION 1 )
set( VLIGHT_PATCH_VERSION 0 )

find_package( SDL REQUIRED)
find_package( OpenGL REQUIRED )
find_package( GLEW REQUIRED )

add_compile_options( -Wall -O0 -g -std=c++11 )

add_subdirectory( fonts )
add_subdirectory( src )

configure_file( README.md README.md COPYONLY )
configure_file( LICENSE LICENSE COPYONLY )
configure_file( vertexShader.vert vertexShader.vert COPYONLY )
configure_file( fragmentShader.frag fragmentShader.frag COPYONLY )
