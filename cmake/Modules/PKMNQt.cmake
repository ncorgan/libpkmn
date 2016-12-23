#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(DEFINED __INCLUDED_PKMNQT_CMAKE)
    RETURN()
ENDIF(DEFINED __INCLUDED_PKMNQT_CMAKE)
SET(__INCLUDED_PKMNQT_CMAKE TRUE)

IF(DESIRED_QT_VERSION MATCHES 5)
    FIND_PACKAGE(Qt5Widgets)
    FIND_PACKAGE(Qt5Test)
    IF(Qt5Widgets_FOUND AND Qt5Test_FOUND)
        SET(QT_FOUND TRUE)

        SET(PKMN_QT4 FALSE CACHE BOOL "Using Qt4")
        SET(PKMN_QT5 TRUE CACHE BOOL "Using Qt5")

        SET(QTx_INCLUDE_DIRS
            ${Qt5Widgets_INCLUDE_DIRS}
            ${Qt5Test_INCLUDE_DIRS}
        )
        SET(QTx_LIBRARIES
            ${Qt5Widgets_LIBRARIES}
            ${Qt5Test_LIBRARIES}
        )

        SET(QTx_RUNTIME_DIR "${_qt5Core_install_prefix}/bin")
    ENDIF(Qt5Widgets_FOUND AND Qt5Test_FOUND)
ELSE()
    FIND_PACKAGE(Qt4)
    SET(QT_FOUND ${Qt4_FOUND})
    IF(QT_FOUND)
        INCLUDE(UseQt4)

        SET(PKMN_QT4 TRUE CACHE BOOL "Using Qt4")
        SET(PKMN_QT5 FALSE CACHE BOOL "Using Qt5")

        SET(QTx_INCLUDE_DIRS
            ${QT_INCLUDE_DIR}
        )
        SET(QTx_LIBRARIES
            ${QT_LIBRARIES}
            ${QT_QTMAIN_LIBRARY}
            ${QT_QTTEST_LIBRARY}
        )

        GET_FILENAME_COMPONENT(QTx_RUNTIME_DIR ${QT_QMAKE_EXECUTABLE} DIRECTORY)
    ENDIF(QT_FOUND)
ENDIF(DESIRED_QT_VERSION MATCHES 5)