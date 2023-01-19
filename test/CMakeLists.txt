file(GLOB cpp_server_test_sources "${PROJECT_SOURCE_DIR}/test/*.cpp")
######################################################################################################################
# DEPENDENCIES
######################################################################################################################

######################################################################################################################
# MAKE TARGETS
######################################################################################################################

##########################################
# "make check-tests"
##########################################
add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

##########################################
# "make server client ..."
##########################################
foreach (cpp_server_test_source ${cpp_server_test_sources})
    # Create a human readable name.
    get_filename_component(cpp_server_test_filename ${cpp_server_test_source} NAME)
    string(REPLACE ".cpp" "" cpp_server_test_name ${cpp_server_test_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${cpp_server_test_name} EXCLUDE_FROM_ALL ${cpp_server_test_source} chat_client.cpp chat_server.cpp)
    add_dependencies(build-tests ${cpp_server_test_name})
    add_dependencies(check-tests ${cpp_server_test_name})

    target_link_libraries(${cpp_server_test_name} cpp_server_shared)

    # Set test target properties and dependencies.
    set_target_properties(${cpp_server_test_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            COMMAND ${cpp_server_test_name}
            )

endforeach(cpp_server_test_source ${cpp_server_test_sources})