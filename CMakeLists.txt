
file(GLOB srcfile ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(msgpackunpack ${srcfile})
target_link_libraries(msgpackunpack Juce msgpack11)
