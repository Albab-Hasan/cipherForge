enable_testing()

include(GoogleTest)

add_executable(cipherforge_tests test_aes.cpp test_modes.cpp)

target_link_libraries(cipherforge_tests PRIVATE cipherforge gtest_main)

gtest_discover_tests(cipherforge_tests)
