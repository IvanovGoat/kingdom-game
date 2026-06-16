# Подключаем doctest как header-only библиотеку (можно просто указать путь к include)
add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE ${CMAKE_SOURCE_DIR}/external)

# Исполняемый файл тестов
add_executable(test_kingdom
    test_game.cpp
    ${CMAKE_SOURCE_DIR}/src/game.cpp   # подключаем реализацию Nation, не компилируя main.cpp
    ${CMAKE_SOURCE_DIR}/src/game.h
)

target_link_libraries(test_kingdom PRIVATE doctest)
target_include_directories(test_kingdom PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Регистрируем тест для ctest
add_test(NAME KingdomTests COMMAND test_kingdom)