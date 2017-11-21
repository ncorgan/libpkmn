#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

IF(PKMN_ENABLE_QT)
    INCLUDE_DIRECTORIES(
        ${CMAKE_CURRENT_SOURCE_DIR}/..
    )

    SET(pkmn_qt_classes
        AbilityListComboBox
        GameListComboBox
        GamecubeShadowPokemonListComboBox
        ItemListComboBox
        LocationListComboBox
        MoveListComboBox
        NatureListComboBox
        PokemonListComboBox
        RegionListComboBox
        RibbonListComboBox
        SuperTrainingMedalListComboBox
        TypeListComboBox
    )

    #
    # Keep qt files in here for organizational purposes,
    # but move them where they need to be in order to build.
    #
    FOREACH(class ${pkmn_qt_classes})
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/${class}.cpp
            ${CMAKE_CURRENT_BINARY_DIR}/../${class}.cpp
        COPYONLY)
        CONFIGURE_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/${class}.hpp
            ${PKMN_BINARY_DIR}/include/pkmn/qt/${class}.hpp
        COPYONLY)
        LIST(APPEND pkmn_qt_headers
            ${PKMN_BINARY_DIR}/include/pkmn/qt/${class}.hpp
        )
        LIST(APPEND pkmn_qt_sources
            ${CMAKE_CURRENT_BINARY_DIR}/../${class}.cpp
        )
    ENDFOREACH(class ${pkmn_qt_classes})

    IF(NOT PKMN_DONT_INSTALL_HEADERS)
        INSTALL(
            FILES ${pkmn_qt_headers}
            DESTINATION ${PKMN_INCLUDE_DIR}/pkmn/qt
            COMPONENT Headers
        )
    ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)
ENDIF(PKMN_ENABLE_QT)

#
# As the Spinda sprite functionality isn't a Qt class, it doesn't
# need all the MOC stuff.
#
LIST(APPEND pkmn_qt_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/Spinda.cpp
)
SET(pkmn_qt_sources ${pkmn_qt_sources} PARENT_SCOPE)
