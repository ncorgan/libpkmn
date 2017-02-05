#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

SET(pkmn_qt_classes
    AbilityListComboBox
    GameListComboBox
    ItemListComboBox
    LocationListComboBox
    MoveListComboBox
    NatureListComboBox
    PokemonListComboBox
    RegionListComboBox
    RibbonListComboBox
    Spinda
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

SET(pkmn_qt_sources ${pkmn_qt_sources} PARENT_SCOPE)

IF(NOT PKMN_DONT_INSTALL_HEADERS)
    INSTALL(
        FILES ${pkmn_qt_headers}
        DESTINATION ${PKMN_INCLUDE_DIR}/pkmn/qt
        COMPONENT Headers
    )
ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)
