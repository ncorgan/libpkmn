/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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

    #include <pkmn/item_list.hpp>
    #include <pkmn/item_bag.hpp>
    #include <pkmn/pokemon.hpp>

    #include <pkmn/types/shared_ptr.hpp>

    PKMN_INLINE pkmn::shared_ptr<pkmn::item_bag> make_item_bag(
        const std::string &game
    ) {
        return pkmn::item_bag::make(game);
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

    PKMN_INLINE pkmn::shared_ptr<pkmn::item_list> make_item_list(
        const std::wstring &name,
        const std::wstring &game
    ) {
        return pkmn::item_list::make(
            boost::locale::conv::utf_to_utf<char>(name),
            boost::locale::conv::utf_to_utf<char>(game)
        );
    }
%}

pkmn::shared_ptr<pkmn::item_list> make_item_list(const std::wstring& name, const std::wstring& game);
#else
%{
    PKMN_INLINE pkmn::shared_ptr<pkmn::item_list> make_item_list(
        const std::string &name,
        const std::string &game
    ) {
        return pkmn::item_list::make(name, game);
    }

    PKMN_INLINE pkmn::shared_ptr<pkmn::pokemon> make_pokemon(
        const std::string &species,
        const std::string &game,
        const std::string &form,
        int level
    ) {
        return pkmn::pokemon::make(species, game, form, level);
    }
%}

pkmn::shared_ptr<pkmn::item_list> make_item_list(const std::string& name, const std::string& game);
#endif

pkmn::shared_ptr<pkmn::item_bag> make_item_bag(const std::string& game);
pkmn::shared_ptr<pkmn::pokemon> make_pokemon(const std::string& species, const std::string& game,
                                             const std::string& form, int level);
