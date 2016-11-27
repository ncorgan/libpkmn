/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_QT5WIDGETSTEST_HPP
#define CPP_QT5WIDGETSTEST_HPP

#include <QtTest/QtTest>

class Qt5WidgetsTest: public QObject
{
    Q_OBJECT

    private slots:
        void testAbilityListComboBox();
        void testGameListComboBox();
        void testItemListComboBox();
};

#endif /* CPP_QT5WIDGETSTEST_HPP */
