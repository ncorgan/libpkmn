/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/pokemon_box.hpp>

#if SWIG_VERSION < 0x030008
    #include <boost/locale/encoding_utf.hpp>
#endif

%}

#if SWIG_VERSION < 0x030008
%ignore pkmn::shared_ptr<pkmn::pokemon_box>::set_name;
#endif

%extend pkmn::shared_ptr<pkmn::pokemon_box> {

/*
 * SWIG 3.0.8 introduced the SWIG_PYTHON_2_UNICODE macro, which allows the
 * Python 2 "unicode" type to be converted to a char* or std::string. There's
 * no way for a SWIG project to bring this in, so we need this ugly workaround
 * when using earlier versions of SWIG.
 */
#if SWIG_VERSION < 0x030008
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
