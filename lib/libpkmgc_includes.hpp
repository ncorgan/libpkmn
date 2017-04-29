/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_LIBPKMGC_POKEMON_HPP
#define INCLUDED_LIBPKMGC_POKEMON_HPP

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

#include <LibPkmGC/GC/SaveEditing/Save.h>
#include <LibPkmGC/Colosseum/SaveEditing/Save.h>
#include <LibPkmGC/XD/SaveEditing/Save.h>

#include <LibPkmGC/GC/Common/BagData.h>
#include <LibPkmGC/Colosseum/Common/BagData.h>
#include <LibPkmGC/XD/Common/BagData.h>

#include <LibPkmGC/GC/Common/Pokemon.h>
#include <LibPkmGC/Colosseum/Common/Pokemon.h>
#include <LibPkmGC/XD/Common/Pokemon.h>

#include <LibPkmGC/GC/Common/PokemonBox.h>
#include <LibPkmGC/Colosseum/Common/PokemonBox.h>
#include <LibPkmGC/XD/Common/PokemonBox.h>

#ifdef __clang__
#pragma GCC diagnostic pop
#endif

#endif /* INCLUDED_LIBPKMGC_POKEMON_HPP */
