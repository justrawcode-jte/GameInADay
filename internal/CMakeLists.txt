cmake_minimum_required( VERSION 4.0.0 )
project( NumberDungeon VERSION 1.0.0 )
set( PROJECT_VERSION_STRING "\"v${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}\"" )
cmake_policy( SET CMP0115 NEW )
cmake_policy( SET CMP0177 NEW )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE )
set( CMAKE_CXX_EXTENSIONS ON )

option( GAME_DIR "game directory to compile" "" )

set( SDL3_DIR ${PROJECT_SOURCE_DIR}/deps/sdl3/lib/cmake/SDL3 )
set( SDL3_image_DIR ${PROJECT_SOURCE_DIR}/deps/sdl3_image/lib/cmake/SDL3_image )
find_package( SDL3 REQUIRED )
find_package( SDL3_image REQUIRED )
set( SDL3_DIR ${PROJECT_SOURCE_DIR}/deps/sdl3 )
set( SDL3_image_DIR ${PROJECT_SOURCE_DIR}/deps/sdl3_image )

file( GLOB_RECURSE CPP_SRC_FILES ${PROJECT_SOURCE_DIR}/../${GAME_DIR}/src/*.cpp )

add_executable( app ${CPP_SRC_FILES} )

include_directories( app PRIVATE
	${SDL3_DIR}/include
	${SDL3_image_DIR}/include
)

target_include_directories( app PRIVATE
	${PROJECT_SOURCE_DIR}/../${GAME_DIR}/src/
)
target_link_libraries( app PRIVATE
	SDL3::SDL3
	SDL3_image::SDL3_image
)

install( TARGETS app DESTINATION ./bin )

install( DIRECTORY ${PROJECT_SOURCE_DIR}/../${GAME_DIR}/assets/ DESTINATION ./assets )
if( APPLE )
	set_target_properties( app PROPERTIES INSTALL_RPATH "@loader_path/../lib" )
	install( FILES
		${SDL3_DIR}/lib/libSDL3.0.dylib
		${SDL3_DIR}/lib/libSDL3.dylib
		${SDL3_image_DIR}/lib/libSDL3_image.0.3.0.dylib
		${SDL3_image_DIR}/lib/libSDL3_image.0.dylib
		${SDL3_image_DIR}/lib/libSDL3_image.dylib
		DESTINATION ./lib
	)
elseif( WIN32 )
	set_target_properties( app PROPERTIES INSTALL_RPATH "$ORIGIN/../lib" )
	install( FILES $<TARGET_RUNTIME_DLLS:run> TYPE BIN )
endif()