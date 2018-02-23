/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

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
        const std::wstring &filepath
    ) {
        return pkmn::game_save::detect_type(
                   boost::locale::conv::utf_to_utf<char>(filepath)
               );
    }

    inline std::shared_ptr<pkmn::game_save> make_game_save(
        const std::wstring &filepath
    ) {
        return pkmn::game_save::from_file(
            boost::locale::conv::utf_to_utf<char>(filepath)
        );
    }

    inline std::shared_ptr<pkmn::item_list> make_item_list(
        const std::wstring &name,
        const std::wstring &game
    ) {
        return pkmn::item_list::make(
            boost::locale::conv::utf_to_utf<char>(name),
            boost::locale::conv::utf_to_utf<char>(game)
        );
    }

    inline std::shared_ptr<pkmn::pokemon> make_pokemon(
        const std::wstring &species,
        const std::wstring &game,
        const std::wstring &form,
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
        const std::wstring &filepath
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

#endif // SWIGPYTHON && SWIG_VERSION < 0x030008
