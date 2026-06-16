#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/game.h"

TEST_CASE("Оценка качества: разные сценарии") {
    // Великая держава: 200 золота, 150 еды, 80 счастья, 80 армии => 2+2+2+2 = 8
    Nation n1(200, 150, 80, 80);
    CHECK(n1.getQualityRating() == "Великая держава!");

    // Процветающее: 100 золота, 100 еды, 60 счастья, 40 армии => 1+1+1+1 = 4 -> "Среднее королевство"
    // Немного подкорректируем, чтобы получить 5
    Nation n2(150, 80, 60, 80); // gold>=100(1), food>=80(1), happiness>=50(1), army>=80(2) = 5 -> "Процветающее"
    CHECK(n2.getQualityRating() == "Процветающее государство.");

    // Упадок: 10,10,10,10
    Nation n3(10, 10, 10, 10);
    CHECK(n3.getQualityRating() == "Упадок и разруха.");
}

TEST_CASE("Война: победа и поражение") {
    // Победа: армия 50 > враг 30
    Nation nWin(100, 50, 50, 50);
    nWin.warAction(30);   // фиксированная сила врага
    std::string stats = nWin.getFinalStats();
    // После победы должно быть +20 золота, +10 еды, -5 армии, +5 счастья
    CHECK(stats.find("Золото: 120") != std::string::npos);   // 100+20=120
    CHECK(stats.find("Еда: 60") != std::string::npos);       // 50+10=60
    CHECK(stats.find("Армия: 45") != std::string::npos);     // 50-5=45
    CHECK(stats.find("Счастье: 55%") != std::string::npos);  // 50+5=55

    // Поражение: армия 20 < враг 80
    Nation nLose(100, 50, 50, 20);
    nLose.warAction(80);
    stats = nLose.getFinalStats();
    CHECK(stats.find("Золото: 90") != std::string::npos);    // 100-10=90
    CHECK(stats.find("Армия: 10") != std::string::npos);     // 20-10=10
    CHECK(stats.find("Счастье: 40%") != std::string::npos);  // 50-10=40
}

TEST_CASE("Исключения при нехватке ресурсов") {
    Nation n(3, 10, 50, 10);
    CHECK_THROWS_AS(n.buildAction(), std::runtime_error);   // нужно 10 золота, есть 3
    CHECK_THROWS_AS(n.entertainmentAction(), std::runtime_error); // 5 золота
    CHECK_THROWS_AS(n.trainAction(), std::runtime_error);   // 8 золота
}

TEST_CASE("Исключение при войне без армии") {
    Nation n(100, 50, 50, 0);
    CHECK_THROWS_AS(n.warAction(30), std::runtime_error);
}