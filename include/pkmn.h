/*
 * Copyright (c) 2015-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_H
#define PKMN_H

#include <pkmn-c/build_info.h>
#include <pkmn-c/config.h>
#include <pkmn-c/error.h>
#include <pkmn-c/game_save.h>
#include <pkmn-c/item_bag.h>
#include <pkmn-c/item_list.h>
#include <pkmn-c/pokedex.h>
#include <pkmn-c/pokemon.h>
#include <pkmn-c/pokemon2.h>
#include <pkmn-c/pokemon_box.h>
#include <pkmn-c/pokemon_party.h>
#include <pkmn-c/pokemon_pc.h>
#include <pkmn-c/version.h>

#include <pkmn-c/calculations/form.h>
#include <pkmn-c/calculations/gender.h>
#include <pkmn-c/calculations/nature.h>
#include <pkmn-c/calculations/personality.h>
#include <pkmn-c/calculations/shininess.h>
#include <pkmn-c/calculations/size.h>
#include <pkmn-c/calculations/spinda_spots.h>
#include <pkmn-c/calculations/stats.h>

#include <pkmn-c/calculations/moves/critical_hit.h>
#include <pkmn-c/calculations/moves/damage.h>
#include <pkmn-c/calculations/moves/hidden_power.h>
#include <pkmn-c/calculations/moves/modifiers.h>
#include <pkmn-c/calculations/moves/natural_gift.h>
#include <pkmn-c/calculations/moves/power.h>

#include <pkmn-c/database/item_entry.h>
#include <pkmn-c/database/lists.h>
#include <pkmn-c/database/move_entry.h>
#include <pkmn-c/database/pokemon_entry.h>

#include <pkmn-c/gui/spinda.h>

#include <pkmn-c/types/gender.h>
#include <pkmn-c/types/int_pair.h>
#include <pkmn-c/types/item_slot.h>
#include <pkmn-c/types/hidden_power.h>
#include <pkmn-c/types/levelup_move.h>
#include <pkmn-c/types/move_slot.h>
#include <pkmn-c/types/pokemon_info.h>
#include <pkmn-c/types/pokemon_list.h>
#include <pkmn-c/types/pokemon_box_list.h>
#include <pkmn-c/types/stats.h>
#include <pkmn-c/types/string_types.h>
#include <pkmn-c/types/trainer_id.h>
#include <pkmn-c/types/trainer_info.h>

#include <pkmn-c/utils/paths.h>

#endif /* PKMN_H */
