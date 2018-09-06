/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMNTEST_UTIL_HPP
#define PKMNTEST_UTIL_HPP

#include <pkmn/enums/game.hpp>

#include <vector>

namespace pkmntest
{

static const std::vector<pkmn::e_game> GEN1_GAMES =
{
    pkmn::e_game::RED,
    pkmn::e_game::BLUE,
    pkmn::e_game::YELLOW,
};

static const std::vector<pkmn::e_game> GEN2_GAMES =
{
    pkmn::e_game::GOLD,
    pkmn::e_game::SILVER,
    pkmn::e_game::CRYSTAL,
};

static const std::vector<pkmn::e_game> GEN3_GAMES =
{
    pkmn::e_game::RUBY,
    pkmn::e_game::SAPPHIRE,
    pkmn::e_game::EMERALD,
    pkmn::e_game::FIRERED,
    pkmn::e_game::LEAFGREEN,
    pkmn::e_game::COLOSSEUM,
    pkmn::e_game::XD,
};

static const std::vector<pkmn::e_game> GBA_GAMES =
{
    pkmn::e_game::RUBY,
    pkmn::e_game::SAPPHIRE,
    pkmn::e_game::EMERALD,
    pkmn::e_game::FIRERED,
};

static const std::vector<pkmn::e_game> GCN_GAMES =
{
    pkmn::e_game::COLOSSEUM,
    pkmn::e_game::XD,
};

static const std::vector<pkmn::e_game> GEN4_GAMES =
{
    pkmn::e_game::DIAMOND,
    pkmn::e_game::PEARL,
    pkmn::e_game::PLATINUM,
    pkmn::e_game::HEARTGOLD,
    pkmn::e_game::SOULSILVER,
};

static const std::vector<pkmn::e_game> GEN5_GAMES =
{
    pkmn::e_game::BLACK,
    pkmn::e_game::WHITE,
    pkmn::e_game::BLACK2,
    pkmn::e_game::WHITE2,
};

static const std::vector<pkmn::e_game> GEN6_GAMES =
{
    pkmn::e_game::X,
    pkmn::e_game::Y,
    pkmn::e_game::OMEGA_RUBY,
    pkmn::e_game::ALPHA_SAPPHIRE,
};

// Games with a full pokemon subclass
static const std::vector<pkmn::e_game> SUPPORTED_GAMES =
{
    pkmn::e_game::RED,
    pkmn::e_game::BLUE,
    pkmn::e_game::YELLOW,
    pkmn::e_game::GOLD,
    pkmn::e_game::SILVER,
    pkmn::e_game::CRYSTAL,
    pkmn::e_game::RUBY,
    pkmn::e_game::SAPPHIRE,
    pkmn::e_game::EMERALD,
    pkmn::e_game::FIRERED,
    pkmn::e_game::LEAFGREEN,
    pkmn::e_game::COLOSSEUM,
    pkmn::e_game::XD,
};

static const std::vector<pkmn::e_game> ALL_GAMES =
{
    pkmn::e_game::RED,
    pkmn::e_game::BLUE,
    pkmn::e_game::YELLOW,
    pkmn::e_game::GOLD,
    pkmn::e_game::SILVER,
    pkmn::e_game::CRYSTAL,
    pkmn::e_game::RUBY,
    pkmn::e_game::SAPPHIRE,
    pkmn::e_game::EMERALD,
    pkmn::e_game::FIRERED,
    pkmn::e_game::LEAFGREEN,
    pkmn::e_game::COLOSSEUM,
    pkmn::e_game::XD,
    pkmn::e_game::DIAMOND,
    pkmn::e_game::PEARL,
    pkmn::e_game::PLATINUM,
    pkmn::e_game::HEARTGOLD,
    pkmn::e_game::SOULSILVER,
    pkmn::e_game::BLACK,
    pkmn::e_game::WHITE,
    pkmn::e_game::BLACK2,
    pkmn::e_game::WHITE2,
    pkmn::e_game::X,
    pkmn::e_game::Y,
    pkmn::e_game::OMEGA_RUBY,
    pkmn::e_game::ALPHA_SAPPHIRE,
};

}

#endif /* PKMNTEST_UTIL_HPP */
