/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <std_string.i>

%{
    inline std::string detect_game_save_type(
        const std::string &filepath
    ) {
        return pkmn::game_save::detect_type(filepath);
    }

    inline std::shared_ptr<pkmn::game_save> make_game_save(
        const std::string &filepath
    ) {
        return pkmn::game_save::from_file(filepath);
    }

    inline std::shared_ptr<pkmn::item_list> make_item_list(
        const std::string &name,
        const std::string &game
    ) {
        return pkmn::item_list::make(name, game);
    }

    inline std::shared_ptr<pkmn::pokemon> make_pokemon(
        const std::string &species,
        const std::string &game,
        const std::string &form,
        int level
    ) {
        return pkmn::pokemon::make(species, game, form, level);
    }

    inline std::shared_ptr<pkmn::pokemon> make_pokemon(
        const std::string &filepath
    ) {
        return pkmn::pokemon::from_file(filepath);
    }
%}

std::string detect_game_save_type(const std::string& filepath);
std::shared_ptr<pkmn::game_save> make_game_save(const std::string& filepath);
std::shared_ptr<pkmn::item_list> make_item_list(const std::string& name, const std::string& game);
std::shared_ptr<pkmn::pokemon> make_pokemon(const std::string& species, const std::string& game,
                                             const std::string& form, int level);
std::shared_ptr<pkmn::pokemon> make_pokemon(const std::string& filepath);
