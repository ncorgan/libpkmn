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
        SET(QTx_INCLUDE_DIRS
            ${Qt5Widgets_INCLUDE_DIRS}
            ${Qt5Test_INCLUDE_DIRS}
        )
        SET(QTx_LIBRARIES
            ${Qt5Widgets_LIBRARIES}
            ${Qt5Test_LIBRARIES}
        )
    ENDIF(Qt5Widgets_FOUND AND Qt5Test_FOUND)
ELSE()
    FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtTest)
    SET(QT_FOUND ${Qt4_FOUND})
    SET(QTx_INCLUDE_DIRS
        ${QT_INCLUDE_DIR}
    )
    SET(QTx_LIBRARIES
        ${QT_LIBRARIES}
        ${QT_QTMAIN_LIBRARY}
    )
ENDIF(DESIRED_QT_VERSION MATCHES 5)
