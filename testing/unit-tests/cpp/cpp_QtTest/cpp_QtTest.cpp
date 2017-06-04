/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "env.hpp"

#include "cpp_QtTest.hpp"

#include <pkmn/build_info.hpp>

#include <pkmn/database/pokemon_entry.hpp>

#include <pkmn/qt/AbilityListComboBox.hpp>
#include <pkmn/qt/GameListComboBox.hpp>
#include <pkmn/qt/GamecubeShadowPokemonListComboBox.hpp>
#include <pkmn/qt/ItemListComboBox.hpp>
#include <pkmn/qt/LocationListComboBox.hpp>
#include <pkmn/qt/MoveListComboBox.hpp>
#include <pkmn/qt/NatureListComboBox.hpp>
#include <pkmn/qt/PokemonListComboBox.hpp>
#include <pkmn/qt/RegionListComboBox.hpp>
#include <pkmn/qt/RibbonListComboBox.hpp>
#include <pkmn/qt/Spinda.hpp>
#include <pkmn/qt/SuperTrainingMedalListComboBox.hpp>
#include <pkmn/qt/TypeListComboBox.hpp>

#include <pkmn/utils/paths.hpp>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#ifdef PKMN_QT4
#include <QtCore/QString>
#include <QtGui/QImage>
#else
#include <QImage>
#include <QString>
#endif

namespace fs = boost::filesystem;

void QtTest::testGetQtVersion() {
    QString versionFromLibPKMN(pkmn::build_info::get_qt_version().c_str());
    QString versionFromQt(qVersion());
    QCOMPARE(versionFromLibPKMN, versionFromQt);
}

void QtTest::testAbilityListComboBox() {
    try {
        pkmn::qt::AbilityListComboBox abilities(6, nullptr);
        QCOMPARE(abilities.count(), 191);

        abilities.setCurrentIndex(0);
        QCOMPARE(abilities.currentText(), QString("Adaptability"));
        abilities.setCurrentIndex(190);
        QCOMPARE(abilities.currentText(), QString("Zen Mode"));
    } catch(const std::exception &e) {
        QFAIL(e.what());
    }
}

void QtTest::testGameListComboBox() {
    try {
        pkmn::qt::GameListComboBox games(6, true, nullptr);
        QCOMPARE(games.count(), 26);

        games.setCurrentIndex(0);
        QCOMPARE(games.currentText(), QString("Red"));
        games.setCurrentIndex(25);
        QCOMPARE(games.currentText(), QString("Alpha Sapphire"));
    } catch(const std::exception &e) {
        QFAIL(e.what());
    }
}

void QtTest::testGamecubeShadowPokemonListComboBox()
{
    try
    {
        pkmn::qt::GamecubeShadowPokemonListComboBox colosseumShadowPokemon(true, nullptr);
        QCOMPARE(colosseumShadowPokemon.count(), 48);

        pkmn::qt::GamecubeShadowPokemonListComboBox xdShadowPokemon(false, nullptr);
        QCOMPARE(xdShadowPokemon.count(), 83);
    }
    catch(const std::exception &e)
    {
        QFAIL(e.what());
    }
}

void QtTest::testItemListComboBox() {
    try {
        pkmn::qt::ItemListComboBox items(QString("HeartGold"), nullptr);
        QCOMPARE(items.count(), 513);

        items.setCurrentIndex(0);
        QCOMPARE(items.currentText(), QString("Adamant Orb"));
        items.setCurrentIndex(512);
        QCOMPARE(items.currentText(), QString("Zoom Lens"));
    } catch(const std::exception &e) {
        QFAIL(e.what());
    }
}

void QtTest::testLocationListComboBox() {
    try {
        pkmn::qt::LocationListComboBox locations(QString("Emerald"), false, nullptr);
        QCOMPARE(locations.count(), 106);

        locations.setCurrentIndex(0);
        QCOMPARE(locations.currentText(), QString("Abandoned Ship"));
        locations.setCurrentIndex(105);
        QCOMPARE(locations.currentText(), QString("Victory Road"));
    } catch(const std::exception &e) {
        QFAIL(e.what());
    }
}

void QtTest::testMoveListComboBox() {
    try {
        pkmn::qt::MoveListComboBox moves(QString("Red"), nullptr);
        QCOMPARE(moves.count(), 165);

        moves.setCurrentIndex(0);
        QCOMPARE(moves.currentText(), QString("Pound"));
        moves.setCurrentIndex(164);
        QCOMPARE(moves.currentText(), QString("Struggle"));
    } catch(const std::exception &e) {
        QFAIL(e.what());
    }
}

void QtTest::testNatureListComboBox() {
    try {
        pkmn::qt::NatureListComboBox natures(nullptr);
        QCOMPARE(natures.count(), 25);

        natures.setCurrentIndex(0);
        QCOMPARE(natures.currentText(), QString("Hardy"));
        natures.setCurrentIndex(24);
        QCOMPARE(natures.currentText(), QString("Quirky"));
    } catch(const std::exception &e) {
        QFAIL(e.what());
    }
}

void QtTest::testPokemonListComboBox() {
    try {
        pkmn::qt::PokemonListComboBox pokemon(1, true, nullptr);
        QCOMPARE(pokemon.count(), 151);

        pokemon.setCurrentIndex(0);
        QCOMPARE(pokemon.currentText(), QString("Bulbasaur"));
        pokemon.setCurrentIndex(150);
        QCOMPARE(pokemon.currentText(), QString("Mew"));
    } catch(const std::exception &e) {
        QFAIL(e.what());
    }
}

void QtTest::testRegionListComboBox() {
    try {
        pkmn::qt::RegionListComboBox regions(nullptr);
        QCOMPARE(regions.count(), 7);

        regions.setCurrentIndex(0);
        QCOMPARE(regions.currentText(), QString("Kanto"));
        regions.setCurrentIndex(6);
        QCOMPARE(regions.currentText(), QString("Kalos"));
    } catch(const std::exception &e) {
        QFAIL(e.what());
    }
}

void QtTest::testRibbonListComboBox() {
}

void QtTest::testSuperTrainingMedalListComboBox() {
    try {
        pkmn::qt::SuperTrainingMedalListComboBox superTrainingMedals(nullptr);
        QCOMPARE(superTrainingMedals.count(), 30);

        superTrainingMedals.setCurrentIndex(0);
        QCOMPARE(superTrainingMedals.currentText(), QString("Sp. Atk Level 1"));
        superTrainingMedals.setCurrentIndex(29);
        QCOMPARE(superTrainingMedals.currentText(), QString("The Battle for the Best!"));
    } catch(const std::exception &e) {
        QFAIL(e.what());
    }
}

void QtTest::testSpinda()
{
    fs::path LIBPKMN_TEST_FILES(pkmn_getenv("LIBPKMN_TEST_FILES"));
    fs::path PKMN_TMP_DIR(pkmn::get_tmp_dir());
    boost::format SPINDA_FORMAT("spinda_%d_%d_%u.png");

    const uint32_t personality       = 0x88888888;
    const uint32_t personality_shiny = 0xF81C8021;

    for(int generation = 3; generation <= 5; ++generation)
    {
        std::string testFilesSpindaFilepath = fs::path(
                                                  LIBPKMN_TEST_FILES /
                                                  "spinda" /
                                                  str(SPINDA_FORMAT % generation % 0 % personality)
                                              ).string();
        QVERIFY(fs::exists(testFilesSpindaFilepath));
        QImage testFilesSpindaImage(QString::fromStdString(testFilesSpindaFilepath));

        std::string testSpindaFilepath = fs::path(
                                             PKMN_TMP_DIR /
                                             str(SPINDA_FORMAT % generation % 0 % personality)
                                         ).string();
        pkmn::qt::GenerateSpindaSpriteAtFilepath(
            generation,
            personality,
            false,
            testSpindaFilepath
        );
        QVERIFY(fs::exists(testSpindaFilepath));
        QImage testSpindaImage(QString::fromStdString(testSpindaFilepath));
        std::remove(testSpindaFilepath.c_str());
        QCOMPARE(testFilesSpindaImage, testSpindaImage);

        testFilesSpindaFilepath = fs::path(
                                      LIBPKMN_TEST_FILES /
                                      "spinda" /
                                      str(SPINDA_FORMAT % generation % 1 % personality_shiny)
                                  ).string();
        QVERIFY(fs::exists(testFilesSpindaFilepath));
        testFilesSpindaImage = QImage(QString::fromStdString(testFilesSpindaFilepath));

        testSpindaFilepath = fs::path(
                                 PKMN_TMP_DIR /
                                 str(SPINDA_FORMAT % generation % 1 % personality_shiny)
                             ).string();
        pkmn::qt::GenerateSpindaSpriteAtFilepath(
            generation,
            personality_shiny,
            true,
            testSpindaFilepath
        );
        QVERIFY(fs::exists(testSpindaFilepath));
        testSpindaImage = QImage(QString::fromStdString(testSpindaFilepath));
        std::remove(testSpindaFilepath.c_str());
        QCOMPARE(testFilesSpindaImage, testSpindaImage);
    }
}

void QtTest::testTypeListComboBox() {
    try {
        pkmn::qt::TypeListComboBox types(QString("Alpha Sapphire"), nullptr);
        QCOMPARE(types.count(), 18);

        types.setCurrentIndex(0);
        QCOMPARE(types.currentText(), QString("Normal"));
        types.setCurrentIndex(17);
        QCOMPARE(types.currentText(), QString("Fairy"));
    } catch(const std::exception &e) {
        QFAIL(e.what());
    }
}

QTEST_MAIN(QtTest)
#include "../moc_cpp_QtTest.cpp"
