include_directories(${FLUIDINFO_SOURCE_DIR}/fluidinfo)
link_directories(${FLUIDINFO_BINARY_DIR}/build)

set(${CMAKE_CXX_FLAGS} "-D FLUID_DEBUG")