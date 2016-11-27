/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_Qt5WidgetsTest.hpp"

#include <pkmn/qt5/AbilityListComboBox.hpp>
#include <pkmn/qt5/GameListComboBox.hpp>
#include <pkmn/qt5/ItemListComboBox.hpp>

void Qt5WidgetsTest::testAbilityListComboBox() {
    try {
        pkmn::qt5::AbilityListComboBox abilities(6, Q_NULLPTR);
        QCOMPARE(abilities.count(), 191);

        abilities.setCurrentIndex(0);
        QCOMPARE(abilities.currentText(), QString("Adaptability"));
        abilities.setCurrentIndex(190);
        QCOMPARE(abilities.currentText(), QString("Zen Mode"));
    } catch(const std::exception &e) {
        QFAIL(e.what());
    }
}

void Qt5WidgetsTest::testGameListComboBox() {
    try {
        pkmn::qt5::GameListComboBox games(6, true, Q_NULLPTR);
        QCOMPARE(games.count(), 26);

        games.setCurrentIndex(0);
        QCOMPARE(games.currentText(), QString("Red"));
        games.setCurrentIndex(25);
        QCOMPARE(games.currentText(), QString("Alpha Sapphire"));
    } catch(const std::exception &e) {
        QFAIL(e.what());
    }
}

void Qt5WidgetsTest::testItemListComboBox() {
    try {
        pkmn::qt5::ItemListComboBox items(QString("HeartGold"), Q_NULLPTR);
        QCOMPARE(items.count(), 513);

        items.setCurrentIndex(0);
        QCOMPARE(items.currentText(), QString("Adamant Orb"));
        items.setCurrentIndex(512);
        QCOMPARE(items.currentText(), QString("Zoom Lens"));
    } catch(const std::exception &e) {
        QFAIL(e.what());
    }
}

QTEST_MAIN(Qt5WidgetsTest)
#include "moc_cpp_Qt5WidgetsTest.cpp"
