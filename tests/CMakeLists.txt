include_directories(${FLUIDINFO_SOURCE_DIR}/fluidinfo)
link_directories(${FLUIDINFO_BINARY_DIR}/build)
add_executable(TestObject ${CORE_CPP}/fluid_object.cpp ${CORE_CPP}/fluid_session.cpp ${CORE_CPP}/fluid_session_details.cpp test_object.cpp)
target_link_libraries(TestObject ${CURL_LIBRARY} json_linux-gcc-4.4.5_libmt)
