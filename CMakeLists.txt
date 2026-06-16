cmake_minimum_required(VERSION 3.14)
project(KingdomGame LANGUAGES CXX)

# Устанавливаем стандарт C++17 (нужен для std::filesystem)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем тестирование
enable_testing()

# Основная исполняемая цель
add_executable(kingdom_game
    src/main.cpp
    src/game.cpp
    src/game.h
)

# Добавляем подкаталог с тестами
add_subdirectory(tests)