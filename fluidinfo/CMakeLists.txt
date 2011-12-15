include_directories(${FLUIDINFO_SOURCE_DIR}/fluidinfo)
link_directories(${FLUIDINFO_BINARY_DIR}/build)

FILE(GLOB CPPSources *.cpp)
add_library(fluidinfo ${CPPSources})
target_link_libraries(fluidinfo ${JSONCPP_LIBRARY})

set(${CMAKE_CXX_FLAGS} "-D FLUID_DEBUG")
