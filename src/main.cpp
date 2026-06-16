/**
 * @file main.cpp
 * @brief Точка входа в игру "Королевство".
 */

#include <iostream>
#include <string>
#include "game.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);       // вывод в UTF-8
    SetConsoleCP(CP_UTF8);             // ввод в UTF-8 (на всякий случай)
#endif
    // Если передан аргумент командной строки, используем его как директорию для сохранения
    std::string saveDir;
    if (argc >= 2) {
        saveDir = argv[1];
    } else {
        std::cout << "Введите путь для сохранения статистики (или нажмите Enter для пропуска): ";
        std::getline(std::cin, saveDir);
        if (saveDir.empty()) {
            std::cout << "Статистика не будет сохранена в файл.\n";
        }
    }

    try {
        Nation myNation(100, 50, 50, 20);  // стартовые ресурсы
        myNation.processTurn();

        std::cout << myNation.getFinalStats();

        if (!saveDir.empty()) {
            myNation.saveStatsToFile(saveDir);
        }
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}