#
# Provides a command to run all tests, since CLion can't do that on it's own.
# https://stackoverflow.com/questions/733475/cmake-ctest-make-test-doesnt-build-tests#736838
#
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -V -C $<CONFIG>)

enable_testing()

#
# Creates a unit test and add it to our registered list, since CLion doesn't have a RUN ALL GOOGLE TESTS build setting.
# Format:
#   unit_test(NAME test_name SOURCES a.cpp b.cpp)
#   unit_test(NAME test_name SOURCES a.cpp b.cpp LIBS GL)
# https://stackoverflow.com/questions/733475/cmake-ctest-make-test-doesnt-build-tests#736838
#
# Example Usage:
# add_unit_test(NAME test_singly_linked_list SOURCES test_singly_linked_list.cpp LIBS gw)
#
function(add_unit_test)
    set(options "")
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES LIBS)
    cmake_parse_arguments(UTEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    add_executable("${UTEST_NAME}" ${UTEST_SOURCES})

    # Generate coverage data only under GCC
    if(CMAKE_BUILD_TYPE MATCHES Debug AND CMAKE_COMPILER_IS_GNUCC)
        target_link_libraries(${UTEST_NAME} catch gcov)
        set_target_properties(${UTEST_NAME} PROPERTIES COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
        set_target_properties(${UTEST_NAME} PROPERTIES LINK_FLAGS "-fprofile-arcs -ftest-coverage")
    else()
        # Linking to catch reduces build time significantly.
        target_link_libraries(${UTEST_NAME} catch)
    endif()
    target_link_libraries(${UTEST_NAME} ${UTEST_LIBS})
    add_test(NAME "${UTEST_NAME}" COMMAND "${UTEST_NAME}" --durations=yes)
    list(APPEND UNIT_TEST_REGISTER ${UTEST_NAME})
    add_dependencies(check ${UTEST_NAME})
    message("Adding ${UTEST_NAME} to unit tests.")
endfunction()

add_library(catch catch.cpp)

add_unit_test(NAME sample SOURCES test_sample.cpp)
