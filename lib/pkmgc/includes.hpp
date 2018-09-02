/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_PKMN_PKMGC_INCLUDES_HPP
#define INCLUDED_PKMN_PKMGC_INCLUDES_HPP

/*
 * When using -Wall and -Wextra, LibPkmGC's headers cause warnings in Clang.
 * This file includes those files surrounded by pragmas to suppress the warnings.
 */

#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif

#include <LibPkmGC/Core/Localization.h>
#include <LibPkmGC/Core/PokemonInfo.h>

#include <LibPkmGC/GC/SaveEditing/Save.h>
#include <LibPkmGC/Colosseum/SaveEditing/Save.h>
#include <LibPkmGC/XD/SaveEditing/Save.h>

#include <LibPkmGC/GC/Common/BagData.h>
#include <LibPkmGC/Colosseum/Common/BagData.h>
#include <LibPkmGC/XD/Common/BagData.h>

#include <LibPkmGC/GC/Common/DaycareData.h>
#include <LibPkmGC/Colosseum/Common/DaycareData.h>
#include <LibPkmGC/XD/Common/DaycareData.h>

#include <LibPkmGC/GC/Common/PCData.h>
#include <LibPkmGC/Colosseum/Common/PCData.h>
#include <LibPkmGC/XD/Common/PCData.h>

#include <LibPkmGC/GC/Common/Pokemon.h>
#include <LibPkmGC/Colosseum/Common/Pokemon.h>
#include <LibPkmGC/XD/Common/Pokemon.h>

#include <LibPkmGC/GC/Common/PokemonBox.h>
#include <LibPkmGC/Colosseum/Common/PokemonBox.h>
#include <LibPkmGC/XD/Common/PokemonBox.h>

#ifdef __clang__
#pragma GCC diagnostic pop
#endif

#endif /* INCLUDED_PKMN_PKMGC_INCLUDES_HPP */
