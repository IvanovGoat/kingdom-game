/**
 * @file game.h
 * @brief Класс, моделирующий государство и игровую логику.
 */

#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <random>

/**
 * @class Nation
 * @brief Хранит ресурсы, выполняет действия, обрабатывает ходы и события.
 */
class Nation {
public:
    /**
     * @brief Конструктор, задающий начальные ресурсы.
     * @param startGold      Начальное золото.
     * @param startFood      Начальная еда.
     * @param startHappiness Начальное счастье (0–100).
     * @param startArmy      Начальный размер армии.
     */
    Nation(int startGold, int startFood, int startHappiness, int startArmy);

    /**
     * @brief Отображает текущие ресурсы на экране.
     */
    void displayResources() const;

    /**
     * @brief Действие: покупка еды за золото.
     * @throws std::runtime_error При некорректной сумме или нехватке золота.
     */
    void purchaseAction();

    /**
     * @brief Действие: строительство инфраструктуры (10 золота).
     * @throws std::runtime_error Если золота меньше 10.
     */
    void buildAction();

    /**
     * @brief Действие: мероприятие для народа (5 золота, +10% счастья).
     * @throws std::runtime_error Если золота меньше 5.
     */
    void entertainmentAction();

    /**
     * @brief Действие: тренировка бойцов (8 золота, +10 армии).
     * @throws std::runtime_error Если золота меньше 8.
     */
    void trainAction();

    /**
     * @brief Действие: война. Если enemyForce < 0, сила врага определяется случайно.
     * @param enemyForce Фиксированная сила врага, либо -1 для случайной.
     * @throws std::runtime_error Если армия равна 0.
     */
    void warAction(int enemyForce = -1);

    /**
     * @brief Обрабатывает один игровой ход: отображение, выбор действия, случайное событие.
     */
    void processTurn();

    /**
     * @brief Завершена ли игра (прошло 10 дней).
     * @return true, если game over.
     */
    bool isGameOver() const;

    /**
     * @brief Формирует строку с итоговой статистикой и оценкой.
     * @return Итоговая статистика.
     */
    std::string getFinalStats() const;

    /**
     * @brief Качественная оценка правления.
     * @return Строка с описанием (Великая держава и т.п.).
     */
    std::string getQualityRating() const;

    /**
     * @brief Сохраняет статистику в файл game_stats.txt в указанной директории.
     * @param directory Путь к каталогу (будет создан при необходимости).
     * @throws std::runtime_error Если не удалось создать каталог или файл.
     */
    void saveStatsToFile(const std::string& directory) const;

private:
    // Ресурсы
    int gold;
    int food;
    int happiness;       ///< 0..100
    int army;
    int infrastructure;  ///< Уровень развития (1,2,4,8,...)
    int day;             ///< Текущий день (0-9)

    // Счётчики действий
    int purchaseCount;
    int buildCount;
    int entertainmentCount;
    int trainCount;
    int warCount;

    // Начальные значения для итоговой статистики
    int startGold;
    int startFood;
    int startHappiness;
    int startArmy;

    /// Генератор случайных чисел (Mersenne Twister)
    mutable std::mt19937 rng;

    /// Журнал событий (для отладки, может быть расширен)
    std::vector<std::string> actionLog;

    /**
     * @brief Приводит ресурсы к допустимым границам (обнуление отрицательных и т.д.).
     */
    void clampResources();

    /**
     * @brief Добавляет запись в журнал.
     * @param msg Сообщение для логирования.
     */
    void log(const std::string& msg);

    /**
     * @brief Применяет одно случайное событие (изменяет ресурсы).
     */
    void applyRandomEvent();

    /**
     * @brief Доход от инфраструктуры + случайное событие.
     */
    void randomEvent();
};

#endif // GAME_H