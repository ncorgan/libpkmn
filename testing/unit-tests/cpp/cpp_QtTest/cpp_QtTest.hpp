/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_QTWIDGETSTEST_HPP
#define CPP_QTWIDGETSTEST_HPP

#include <QtTest/QtTest>

class QtTest: public QObject
{
    Q_OBJECT

    private slots:
        void testGetQtVersion();
        void testAbilityListComboBox();
        void testGameListComboBox();
        void testGamecubeShadowPokemonListComboBox();
        void testItemListComboBox();
        void testLocationListComboBox();
        void testMoveListComboBox();
        void testNatureListComboBox();
        void testPokemonListComboBox();
        void testRegionListComboBox();
        void testRibbonListComboBox();
        void testSuperTrainingMedalListComboBox();
        void testTypeListComboBox();
};

#endif /* CPP_QTWIDGETSTEST_HPP */
