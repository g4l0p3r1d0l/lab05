add_executable(check
    ${CMAKE_CURRENT_SOURCE_DIR}/test_account.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test_transaction.cpp
)

# Линкуем тесты с нашей библиотекой, gtest и gmock
target_link_libraries(check banking gtest_main gmock_main)

add_test(NAME check COMMAND check)
