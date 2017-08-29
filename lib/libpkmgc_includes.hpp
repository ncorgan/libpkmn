/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef INCLUDED_LIBPKMGC_INCLUDES_HPP
#define INCLUDED_LIBPKMGC_INCLUDES_HPP

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

/*
 * LibPkmGC stores some values in arrays, with little indication as to what each
 * index actually corresponds to, so these enums make things easier.
 */

typedef enum {
    LIBPKMGC_STAT_HP = 0,
    LIBPKMGC_STAT_ATTACK,
    LIBPKMGC_STAT_DEFENSE,
    LIBPKMGC_STAT_SPATK,
    LIBPKMGC_STAT_SPDEF,
    LIBPKMGC_STAT_SPEED
} libpkmgc_stat_t;

typedef enum {
    LIBPKMGC_CONTEST_STAT_COOL = 0,
    LIBPKMGC_CONTEST_STAT_BEAUTY,
    LIBPKMGC_CONTEST_STAT_CUTE,
    LIBPKMGC_CONTEST_STAT_SMART,
    LIBPKMGC_CONTEST_STAT_TOUGH
} libpkmgc_contest_stat_t;

typedef enum {
    LIBPKMGC_RIBBON_CHAMPION = 0,
    LIBPKMGC_RIBBON_WINNING,
    LIBPKMGC_RIBBON_VICTORY,
    LIBPKMGC_RIBBON_ARTIST,
    LIBPKMGC_RIBBON_EFFORT,
    LIBPKMGC_RIBBON_MARINE,
    LIBPKMGC_RIBBON_LAND,
    LIBPKMGC_RIBBON_SKY,
    LIBPKMGC_RIBBON_COUNTRY,
    LIBPKMGC_RIBBON_NATIONAL,
    LIBPKMGC_RIBBON_EARTH,
    LIBPKMGC_RIBBON_WORLD
} libpkmgc_ribbon_t;

#endif /* INCLUDED_LIBPKMGC_INCLUDES_HPP */
