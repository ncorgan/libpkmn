/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/pokemon_box.hpp>
%}

#ifdef PKMN_WSTRING_WORKAROUND
%warnfilter(508) pkmn::shared_ptr<pkmn::pokemon_box>;
#endif

%extend pkmn::shared_ptr<pkmn::pokemon_box> {

#ifdef PKMN_WSTRING_WORKAROUND
    void set_name(
        const std::wstring& name
    ) {
        self->get()->set_name(boost::locale::conv::utf_to_utf<char>(name));
    }
#endif

    pkmn::pokemon::sptr __getitem__(
        int index
    ) {
        return self->get()->get_pokemon(index);
    }

    void __setitem__(
        int index,
        pkmn::pokemon::sptr new_pokemon
    ) {
        self->get()->set_pokemon(
            index,
            new_pokemon
        );
    }

    int __len__() {
        return self->get()->get_capacity();
    }
}

%include <pkmn/pokemon_box.hpp>
PKMN_PYTHON_SPTR(pokemon_box)
PKMN_PYTHON_VECTOR(pkmn::pokemon_box::sptr, pokemon_box_list)
