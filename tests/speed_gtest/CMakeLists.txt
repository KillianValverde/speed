project(speed_gtest)

include_directories(${PROJECT_SOURCE_DIR}/../../src)

set(GTEST_LIBRARIES "gtest" "gtest_main")

set(SPEED_ALGORITHM_TEST_SOURCE_FILES
        algorithm_test/operations_test.cpp
)

set(SPEED_ARGPARSE_TEST_SOURCE_FILES
        argparse_test/arg_parser_test.cpp
)

set(SPEED_CONTAINERS_TEST_SOURCE_FILES
        containers_test/circular_doubly_linked_lists_test.cpp
        containers_test/flags_test.cpp
        containers_test/static_cache_test.cpp
)

set(SPEED_FILESYSTEM_TEST_SOURCE_FILES
        filesystem_test/directory_iteration_test.cpp
        filesystem_test/valid_path_test.cpp
)

set(SPEED_IOSTREAM_TEST_SOURCE_FILES
        iostream_test/indentation_test.cpp
)

set(SPEED_LOWLEVEL_TEST_SOURCE_FILES
        lowlevel_test/enum_bitwise_operators_test.cpp
        lowlevel_test/operations_test.cpp
)

set(SPEED_SAFETY_TEST_SOURCE_FILES
        safety_test/operations_test.cpp
)

set(SPEED_SCALARS_TEST_SOURCE_FILES
        scalars_test/scalar_test.cpp
        scalars_test/operations_test.cpp
)

set(SPEED_STRINGUTILS_TEST_SOURCE_FILES
        stringutils_test/operations.cpp
)

set(SPEED_SYSTEM_TEST_SOURCE_FILES
        system_test/filesystem_test.cpp
        system_test/process_test.cpp
        system_test/terminal_test.cpp
        system_test/time_test.cpp
)

set(SPEED_TIME_TEST_SOURCE_FILES
        time_test/cpu_chono_test.cpp
        time_test/monotonic_chrono_test.cpp
)

set(SPEED_TYPE_CASTING_TEST_SOURCE_FILES
        type_casting_test/try_type_cast_test.cpp
        type_casting_test/type_cast_test.cpp
)

set(SPEED_TYPE_TRAITS_TEST_SOURCE_FILES
        type_traits_test/operations_test.cpp
)

add_executable(speed_algorithm_test main.cpp ${SPEED_ALGORITHM_TEST_SOURCE_FILES})
add_executable(speed_argparse_test main.cpp ${SPEED_ARGPARSE_TEST_SOURCE_FILES})
add_executable(speed_containers_test main.cpp ${SPEED_CONTAINERS_TEST_SOURCE_FILES})
add_executable(speed_filesystem_test main.cpp ${SPEED_FILESYSTEM_TEST_SOURCE_FILES})
add_executable(speed_iostream_test main.cpp ${SPEED_IOSTREAM_TEST_SOURCE_FILES})
add_executable(speed_lowlevel_test main.cpp ${SPEED_LOWLEVEL_TEST_SOURCE_FILES})
add_executable(speed_safety_test main.cpp ${SPEED_SAFETY_TEST_SOURCE_FILES})
add_executable(speed_scalars_test main.cpp ${SPEED_SCALARS_TEST_SOURCE_FILES})
add_executable(speed_stringutils_test main.cpp ${SPEED_STRINGUTILS_TEST_SOURCE_FILES})
add_executable(speed_system_test main.cpp ${SPEED_SYSTEM_TEST_SOURCE_FILES})
add_executable(speed_time_test main.cpp ${SPEED_TIME_TEST_SOURCE_FILES})
add_executable(speed_type_casting_test main.cpp ${SPEED_TYPE_CASTING_TEST_SOURCE_FILES})
add_executable(speed_type_traits_test main.cpp ${SPEED_TYPE_TRAITS_TEST_SOURCE_FILES})

add_executable(speed_test
        main.cpp
        ${SPEED_ALGORITHM_TEST_SOURCE_FILES}
        ${SPEED_ARGPARSE_TEST_SOURCE_FILES}
        ${SPEED_CONTAINERS_TEST_SOURCE_FILES}
        ${SPEED_FILESYSTEM_TEST_SOURCE_FILES}
        ${SPEED_IOSTREAM_TEST_SOURCE_FILES}
        ${SPEED_LOWLEVEL_TEST_SOURCE_FILES}
        ${SPEED_SAFETY_TEST_SOURCE_FILES}
        ${SPEED_SCALARS_TEST_SOURCE_FILES}
        ${SPEED_STRINGUTILS_TEST_SOURCE_FILES}
        ${SPEED_SYSTEM_TEST_SOURCE_FILES}
        ${SPEED_TIME_TEST_SOURCE_FILES}
        ${SPEED_TYPE_CASTING_TEST_SOURCE_FILES}
        ${SPEED_TYPE_TRAITS_TEST_SOURCE_FILES}
)

target_link_libraries(speed_algorithm_test speed_algorithm ${GTEST_LIBRARIES})
target_link_libraries(speed_argparse_test speed_argparse ${GTEST_LIBRARIES})
target_link_libraries(speed_containers_test speed_containers ${GTEST_LIBRARIES})
target_link_libraries(speed_iostream_test speed_iostream ${GTEST_LIBRARIES})
target_link_libraries(speed_lowlevel_test speed_lowlevel ${GTEST_LIBRARIES})
target_link_libraries(speed_safety_test speed_safety ${GTEST_LIBRARIES})
target_link_libraries(speed_scalars_test speed_scalars ${GTEST_LIBRARIES})
target_link_libraries(speed_stringutils_test speed_stringutils ${GTEST_LIBRARIES})
target_link_libraries(speed_system_test speed_system ${GTEST_LIBRARIES})
target_link_libraries(speed_time_test speed_time ${GTEST_LIBRARIES})
target_link_libraries(speed_type_casting_test speed_type_casting ${GTEST_LIBRARIES})
target_link_libraries(speed_type_traits_test speed_type_traits ${GTEST_LIBRARIES})
target_link_libraries(speed_test speed ${GTEST_LIBRARIES})
