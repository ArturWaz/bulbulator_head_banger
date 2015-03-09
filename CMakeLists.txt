CMAKE_MINIMUM_REQUIRED( VERSION 3.1.2 )

get_filename_component(SOURCE_DIRECTORY_NAME ${CMAKE_SOURCE_DIR} NAME)



# Section for project specific settings

set( PROJ_NAME "bulbulator" )

set( BOOST_PATH F:/_compilers/_lib/boost_1_57_0 )

set( BOOST_INCLUDES ${BOOST_PATH}/include/ )
set( BOOST_LIBRARIES ${BOOST_PATH}/lib/libboost_system-mgw49-mt-1_57.a ${BOOST_PATH}/lib/libboost_thread-mgw49-mt-1_57.a ${BOOST_PATH}/lib/libboost_date_time-mgw49-mt-1_57.a ${BOOST_PATH}/lib/libboost_regex-mgw49-mt-1_57.a ${BOOST_PATH}/lib/libboost_serialization-mgw49-mt-1_57.a )


set( PROJ_LIBRARIES ${BOOST_LIBRARIES} "ws2_32" "wsock32" )
set( PROJ_INCLUDES ${BOOST_INCLUDES} lib src )
file( GLOB_RECURSE PROJ_SOURCES *.cpp lib/serial/*.cpp lib/serial/*.c )

# End of special section


set( CMAKE_CXX_FLAGS "-std=c++11" )
if ( UNIX )
    set( CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}" )
endif ( UNIX )


set(CMAKE_SHARED_LINKER_FLAGS "-static")
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../___bin___/${SOURCE_DIRECTORY_NAME}___${CMAKE_BUILD_TYPE}/" )
project( ${PROJ_NAME} )
include_directories( ${PROJ_INCLUDES} )
add_executable( ${PROJ_NAME} ${PROJ_SOURCES} )
target_link_libraries( ${PROJ_NAME} ${PROJ_LIBRARIES} )
