#
# Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

SET(csharp_files
    AbilityListComboBox.cs
    AssemblyInfo.cs
    BuildInfo.cs
    DatabaseProperties.cs
    GameListComboBox.cs
    Equality.cs
    HashCodeBuilder.cs
    ItemListComboBox.cs
    LocationListComboBox.cs
    MoveListComboBox.cs
    NatureListComboBox.cs
    PokemonListComboBox.cs
    RegionListComboBox.cs
    RibbonListComboBox.cs
    SpindaOperators.cs
    SuperTrainingMedalListComboBox.cs
    TypeListComboBox.cs
)

FOREACH(cs ${csharp_files})
    CONFIGURE_FILE(
        ${CMAKE_CURRENT_SOURCE_DIR}/${cs}
        ${CMAKE_CURRENT_BINARY_DIR}/${cs}
    @ONLY)
ENDFOREACH(cs ${csharp_files})
