/**
 * @file game.cpp
 * @brief Реализация методов класса Nation.
 */

#include "game.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

// --------------------------------------------------------------
Nation::Nation(int startGold, int startFood, int startHappiness, int startArmy)
    : gold(startGold), food(startFood), happiness(startHappiness), army(startArmy),
      infrastructure(1), day(0),
      purchaseCount(0), buildCount(0), entertainmentCount(0), trainCount(0), warCount(0),
      startGold(startGold), startFood(startFood), startHappiness(startHappiness), startArmy(startArmy),
      rng(std::random_device{}())   // <-- исправлено: seed из random_device
{
    clampResources();
    log("Государство создано.");
}

void Nation::log(const std::string& msg) {
    actionLog.push_back(msg);
}

void Nation::clampResources() {
    if (gold < 0) gold = 0;
    if (food < 0) food = 0;
    if (happiness < 0) happiness = 0;
    if (happiness > 100) happiness = 100;
    if (army < 0) army = 0;
    if (infrastructure < 0) infrastructure = 0;
}

void Nation::displayResources() const {
    std::cout << "\n========== ДЕНЬ " << day+1 << "/10 ==========\n"   // день для пользователя с 1
              << "💰 Золото: " << gold << "\n"
              << "🍞 Еда: " << food << "\n"
              << "😊 Счастье: " << happiness << "%\n"
              << "⚔️ Армия: " << army << "\n"
              << "🏗️ Инфраструктура: " << infrastructure << "\n"
              << "================================\n";
}

void Nation::purchaseAction() {
    std::cout << "Сколько золота потратить на еду? (1 золото = 2 еды): ";
    int spend;
    std::cin >> spend;
    if (std::cin.fail() || spend <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::runtime_error("Некорректная сумма.");
    }
    if (spend > gold) {
        throw std::runtime_error("Недостаточно золота!");
    }
    gold -= spend;
    food += spend * 2;
    purchaseCount++;
    log("Покупка: потрачено " + std::to_string(spend) + " золота, получено " + std::to_string(spend*2) + " еды.");
    std::cout << "✅ Покупка совершена.\n";
}

void Nation::buildAction() {
    const int cost = 10;
    if (gold < cost) {
        throw std::runtime_error("Недостаточно золота для строительства (нужно 10).");
    }
    gold -= cost;
    // Удвоение инфраструктуры, кроме начального шага
    if (infrastructure == 1) infrastructure = 2;
    else infrastructure *= 2;
    buildCount++;
    log("Строительство: уровень инфраструктуры увеличен до " + std::to_string(infrastructure) + ".");
    std::cout << "🏗️ Построено! Уровень инфраструктуры: " << infrastructure << "\n";
}

void Nation::entertainmentAction() {
    const int cost = 5;
    if (gold < cost) {
        throw std::runtime_error("Недостаточно золота для мероприятия (нужно 5).");
    }
    gold -= cost;
    happiness += 10;
    if (happiness > 100) happiness = 100;
    entertainmentCount++;
    log("Мероприятие: счастье повышено до " + std::to_string(happiness) + "%.");
    std::cout << "🎉 Счастье народа повышено! +10%\n";
}

void Nation::trainAction() {
    const int cost = 8;
    if (gold < cost) {
        throw std::runtime_error("Недостаточно золота для тренировки (нужно 8).");
    }
    gold -= cost;
    army += 10;
    trainCount++;
    log("Тренировка: армия увеличена до " + std::to_string(army) + ".");
    std::cout << "⚔️ Армия пополнена на 10 бойцов.\n";
}

void Nation::warAction(int enemyForce) {
    if (army <= 0) {
        throw std::runtime_error("Невозможно воевать без армии!");
    }

    int enemy = enemyForce;
    if (enemy < 0) {   // случайный враг
        std::uniform_int_distribution<int> dist(20, 100);
        enemy = dist(rng);
    }

    std::cout << "⚔️ Враг с силой " << enemy << " атакует!\n";
    if (army > enemy) {
        // Победа
        int lootGold = 20;
        int lootFood = 10;
        int losses = 5;
        gold += lootGold;
        food += lootFood;
        army -= losses;
        happiness += 5;
        if (happiness > 100) happiness = 100;
        log("Война: ПОБЕДА! +" + std::to_string(lootGold) + " золота, +" + std::to_string(lootFood) +
            " еды, потери " + std::to_string(losses) + " воинов.");
        std::cout << "🏆 Победа! Получено: " << lootGold << " золота, " << lootFood << " еды.\n"
                  << "Потери: " << losses << " воинов.\n";
    } else {
        // Поражение
        int losses = 10;
        int goldLoss = 10;
        army -= losses;
        gold -= goldLoss;
        happiness -= 10;
        if (happiness < 0) happiness = 0;
        log("Война: ПОРАЖЕНИЕ! Потери " + std::to_string(losses) + " воинов, потеряно " +
            std::to_string(goldLoss) + " золота.");
        std::cout << "💔 Поражение! Потеряно " << losses << " воинов и " << goldLoss << " золота.\n";
    }
    clampResources();
    warCount++;
}

void Nation::applyRandomEvent() {
    std::uniform_int_distribution<int> dist(0, 5);
    int event = dist(rng);
    std::string eventMsg;
    switch (event) {
        case 0:
            eventMsg = "🌾 Хороший урожай! +15 еды.";
            food += 15;
            break;
        case 1:
            eventMsg = "🌊 Наводнение! -10 еды.";
            food -= 10;
            break;
        case 2:
            eventMsg = "💰 Налоговый сбор. +10 золота.";
            gold += 10;
            break;
        case 3:
            eventMsg = "👑 Коррупция. -8 золота.";
            gold -= 8;
            break;
        case 4:
            eventMsg = "🇵 Патриотический подъём! +5 армии.";
            army += 5;
            break;
        case 5:
            eventMsg = "🦠 Эпидемия. Счастье -10%.";
            happiness -= 10;
            break;
    }
    clampResources();
    log("Событие: " + eventMsg);
    std::cout << "📢 " << eventMsg << "\n";
}

void Nation::randomEvent() {
    // Доход от инфраструктуры
    if (infrastructure > 0) {
        gold += infrastructure;
        log("Доход от инфраструктуры: +" + std::to_string(infrastructure) + " золота.");
        std::cout << "🏭 Инфраструктура принесла " << infrastructure << " золота.\n";
    }
    applyRandomEvent();
}

bool Nation::isGameOver() const {
    return day >= 10;
}

void Nation::processTurn() {
    while (!isGameOver()) {
        displayResources();
        std::cout << "\nВыберите действие:\n"
                  << "1. Покупка еды\n"
                  << "2. Строительство (10 золота)\n"
                  << "3. Мероприятие для народа (5 золота)\n"
                  << "4. Тренировка бойцов (8 золота)\n"
                  << "5. Война\n"
                  << "Ваш выбор: ";

        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::runtime_error("Введите число от 1 до 5.");
        }

        try {
            switch (choice) {
                case 1: purchaseAction(); break;
                case 2: buildAction(); break;
                case 3: entertainmentAction(); break;
                case 4: trainAction(); break;
                case 5: warAction(); break;
                default: throw std::runtime_error("Неверный пункт меню.");
            }
        } catch (const std::exception& e) {
            std::cout << "❌ Ошибка: " << e.what() << "\n";
            continue; // повторяем выбор действия в этот же день
        }

        randomEvent();
        day++;
        clampResources();

        if (isGameOver()) {
            std::cout << "\n🏁 ИГРА ЗАВЕРШЕНА 🏁\n";
            break;
        }
    }
}

std::string Nation::getFinalStats() const {
    std::string stats = "\n========== ИТОГОВАЯ СТАТИСТИКА ==========\n";
    stats += "Начальные ресурсы:\n";
    stats += "  Золото: " + std::to_string(startGold) + "\n";
    stats += "  Еда: " + std::to_string(startFood) + "\n";
    stats += "  Счастье: " + std::to_string(startHappiness) + "%\n";
    stats += "  Армия: " + std::to_string(startArmy) + "\n";
    stats += "Конечные ресурсы:\n";
    stats += "  Золото: " + std::to_string(gold) + "\n";
    stats += "  Еда: " + std::to_string(food) + "\n";
    stats += "  Счастье: " + std::to_string(happiness) + "%\n";
    stats += "  Армия: " + std::to_string(army) + "\n";
    stats += "Инфраструктура: " + std::to_string(infrastructure) + "\n";
    stats += "Выполнено действий:\n";
    stats += "  Покупок: " + std::to_string(purchaseCount) + "\n";
    stats += "  Построек: " + std::to_string(buildCount) + "\n";
    stats += "  Мероприятий: " + std::to_string(entertainmentCount) + "\n";
    stats += "  Тренировок: " + std::to_string(trainCount) + "\n";
    stats += "  Войн: " + std::to_string(warCount) + "\n";
    stats += "Качественная оценка: " + getQualityRating() + "\n";
    stats += "===============================================\n";
    return stats;
}

std::string Nation::getQualityRating() const {
    int score = 0;
    if (gold >= 200) score += 2;
    else if (gold >= 100) score += 1;
    if (food >= 150) score += 2;
    else if (food >= 80) score += 1;
    if (happiness >= 80) score += 2;
    else if (happiness >= 50) score += 1;
    if (army >= 80) score += 2;
    else if (army >= 40) score += 1;

    if (score >= 7) return "Великая держава!";
    if (score >= 5) return "Процветающее государство.";
    if (score >= 3) return "Среднее королевство.";
    return "Упадок и разруха.";
}

void Nation::saveStatsToFile(const std::string& directory) const {
    fs::path dirPath(directory);
    if (!dirPath.empty() && !fs::exists(dirPath)) {
        if (!fs::create_directories(dirPath)) {
            throw std::runtime_error("Не удалось создать директорию: " + directory);
        }
    }
    fs::path filePath = dirPath / "game_stats.txt";
    std::ofstream out(filePath);
    if (!out.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filePath.string());
    }
    out << getFinalStats();
    out.close();
    std::cout << "📄 Статистика сохранена в " << filePath.string() << "\n";
}