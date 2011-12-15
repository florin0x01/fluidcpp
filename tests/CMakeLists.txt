include_directories(${FLUIDINFO_SOURCE_DIR}/fluidinfo)
link_directories(${FLUIDINFO_BINARY_DIR}/build)

add_executable(TestObject test_object.cpp)
target_link_libraries(TestObject ${CURL_LIBRARY} fluidinfo)

add_executable(TestNs test_ns.cpp)
target_link_libraries(TestNs ${CURL_LIBRARY} fluidinfo)
