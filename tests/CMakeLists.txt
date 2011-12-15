include_directories(${FLUIDINFO_SOURCE_DIR}/fluidinfo)
link_directories(${FLUIDINFO_BINARY_DIR}/build)

add_executable(TestObject ${CORE_CPP}/generic_stuff.cpp ${CORE_CPP}/fluid_ns.cpp ${CORE_CPP}/fluid_object.cpp ${CORE_CPP}/fluid_session.cpp ${CORE_CPP}/fluid_session_details.cpp ${CORE_CPP}/fluid_tag.cpp test_object.cpp)
target_link_libraries(TestObject ${CURL_LIBRARY} json_linux-gcc-4.5.2_libmt)
set(CMAKE_CXX_FLAGS "-D FLUID_DEBUG")

add_executable(TestNs ${CORE_CPP}/generic_stuff.cpp ${CORE_CPP}/fluid_ns.cpp ${CORE_CPP}/fluid_session.cpp ${CORE_CPP}/fluid_session_details.cpp ${CORE_CPP}/fluid_tag.cpp test_ns.cpp)
target_link_libraries(TestNs ${CURL_LIBRARY} json_linux-gcc-4.5.2_libmt)
set(CMAKE_CXX_FLAGS "-D FLUID_DEBUG")