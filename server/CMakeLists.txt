SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES("${SOURCE_DIR}")

find_package( Boost REQUIRED COMPONENTS system thread)
include_directories( ${Boost_INCLUDE_DIRS} )

FILE(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.h)

add_executable (server ${SOURCES})
target_link_libraries(server ${Boost_LIBRARIES})
set_property(TARGET server PROPERTY CXX_STANDARD 11)

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
#target_include_directories (server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
