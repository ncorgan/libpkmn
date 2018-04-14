/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

/*
 * This file contains re-declarations of pkmn's static functions, such as sptrs'
 * make function. This prevents make from becoming a non-static member of the
 * sptr class.
 */

%{
    #include <pkmn/config.hpp>

    #include <pkmn/game_save.hpp>
    #include <pkmn/item_list.hpp>
    #include <pkmn/item_bag.hpp>
    #include <pkmn/pokedex.hpp>
    #include <pkmn/pokemon.hpp>
    #include <pkmn/pokemon_box.hpp>
    #include <pkmn/pokemon_party.hpp>
    #include <pkmn/pokemon_pc.hpp>

    #include <memory>

    inline std::shared_ptr<pkmn::item_bag> make_item_bag(
        const std::string& game
    ) {
        return pkmn::item_bag::make(game);
    }

    inline std::shared_ptr<pkmn::pokedex> make_pokedex(
        const std::string& game
    ) {
        return pkmn::pokedex::make(game);
    }

    inline std::shared_ptr<pkmn::pokemon_box> make_pokemon_box(
        const std::string& game
    ) {
        return pkmn::pokemon_box::make(game);
    }

    inline std::shared_ptr<pkmn::pokemon_party> make_pokemon_party(
        const std::string& game
    ) {
        return pkmn::pokemon_party::make(game);
    }

    inline std::shared_ptr<pkmn::pokemon_pc> make_pokemon_pc(
        const std::string& game
    ) {
        return pkmn::pokemon_pc::make(game);
    }
%}

%include <std_string.i>

/*
 * SWIG 3.0.8 introduced the SWIG_PYTHON_2_UNICODE macro, which allows the
 * Python 2 "unicode" type to be converted to a char* or std::string. There's
 * no way for a SWIG project to bring this in, so we need this ugly workaround
 * when using earlier verisons of SWIG.
 */
#if SWIGPYTHON && SWIG_VERSION < 0x030008
%include <std_wstring.i>

%{
    #include <boost/locale/encoding_utf.hpp>

    inline std::string detect_game_save_type(
        const std::wstring& filepath
    ) {
        return pkmn::game_save::detect_type(
                   boost::locale::conv::utf_to_utf<char>(filepath)
               );
    }

    inline std::shared_ptr<pkmn::game_save> make_game_save(
        const std::wstring& filepath
    ) {
        return pkmn::game_save::from_file(
            boost::locale::conv::utf_to_utf<char>(filepath)
        );
    }

    inline std::shared_ptr<pkmn::item_list> make_item_list(
        const std::wstring& name,
        const std::wstring& game
    ) {
        return pkmn::item_list::make(
            boost::locale::conv::utf_to_utf<char>(name),
            boost::locale::conv::utf_to_utf<char>(game)
        );
    }

    inline std::shared_ptr<pkmn::pokemon> make_pokemon(
        const std::wstring& species,
        const std::wstring& game,
        const std::wstring& form,
        int level
    ) {
        return pkmn::pokemon::make(
                   boost::locale::conv::utf_to_utf<char>(species),
                   boost::locale::conv::utf_to_utf<char>(game),
                   boost::locale::conv::utf_to_utf<char>(form),
                   level
               );
    }

    inline std::shared_ptr<pkmn::pokemon> make_pokemon(
        const std::wstring& filepath
    ) {
        return pkmn::pokemon::from_file(
                   boost::locale::conv::utf_to_utf<char>(filepath)
               );
    }
%}

std::string detect_game_save_type(const std::wstring& filepath);
std::shared_ptr<pkmn::game_save> make_game_save(const std::wstring& filepath);
std::shared_ptr<pkmn::item_list> make_item_list(const std::wstring& name, const std::wstring& game);
std::shared_ptr<pkmn::pokemon> make_pokemon(const std::wstring& species, const std::wstring& game,
                                             const std::wstring& form, int level);
std::shared_ptr<pkmn::pokemon> make_pokemon(const std::wstring& filepath);

#else

%{
    inline std::string detect_game_save_type(
        const std::string& filepath
    ) {
        return pkmn::game_save::detect_type(filepath);
    }

    inline std::shared_ptr<pkmn::game_save> make_game_save(
        const std::string& filepath
    ) {
        return pkmn::game_save::from_file(filepath);
    }

    inline std::shared_ptr<pkmn::item_list> make_item_list(
        const std::string& name,
        const std::string& game
    ) {
        return pkmn::item_list::make(name, game);
    }

    inline std::shared_ptr<pkmn::pokemon> make_pokemon(
        const std::string& species,
        const std::string& game,
        const std::string& form,
        int level
    ) {
        return pkmn::pokemon::make(species, game, form, level);
    }

    inline std::shared_ptr<pkmn::pokemon> make_pokemon(
        const std::string& filepath
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

#endif

std::shared_ptr<pkmn::item_bag> make_item_bag(const std::string& game);
std::shared_ptr<pkmn::pokedex> make_pokedex(const std::string& game);
std::shared_ptr<pkmn::pokemon_box> make_pokemon_box(const std::string& game);
std::shared_ptr<pkmn::pokemon_party> make_pokemon_party(const std::string& game);
std::shared_ptr<pkmn::pokemon_pc> make_pokemon_pc(const std::string& game);
