#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

MACRO(ADD_QT_SOURCE class_name)
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${class_name}.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/../${class_name}.cpp
    COPYONLY)
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${class_name}.hpp
        ${PKMN_BINARY_DIR}/include/pkmn/qt/${class_name}.hpp
    COPYONLY)
    LIST(APPEND pkmn_qt_headers
        ${PKMN_BINARY_DIR}/include/pkmn/qt/${class_name}.hpp
    )
    LIST(APPEND pkmn_qt_sources
        ${CMAKE_CURRENT_BINARY_DIR}/../${class_name}.cpp
    )
ENDMACRO(ADD_QT_SOURCE)

# TODO: Spinda is in all builds.

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
    SuperTrainingMedalListComboBox
    TypeListComboBox
)

#
# Keep QT files in here for organizational purposes,
# but move them where they need to be in order to build.
#
IF(NOT PKMN_ENABLE_JAVA)
    FOREACH(class ${pkmn_qt_classes})
        ADD_QT_SOURCE(${class})
    ENDFOREACH(class ${pkmn_qt_classes})

    IF(NOT PKMN_DONT_INSTALL_HEADERS)
        INSTALL(
            FILES ${pkmn_qt_headers}
            DESTINATION ${PKMN_INCLUDE_DIR}/pkmn/qt
            COMPONENT Headers
        )
    ENDIF(NOT PKMN_DONT_INSTALL_HEADERS)
ENDIF(NOT PKMN_ENABLE_JAVA)

SET(pkmn_qt_sources ${pkmn_qt_sources} PARENT_SCOPE)
