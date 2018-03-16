/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/item_bag.hpp"

#if defined(SWIGPYTHON) && SWIG_VERSION < 0x030008
#    include <boost/locale/encoding_utf.hpp>
#endif
%}

%include <attribute.i>

%ignore pkmn::swig::item_bag::item_bag();
%ignore pkmn::swig::item_bag::item_bag(const pkmn::item_bag::sptr&);
%ignore pkmn::swig::item_bag::cptr();
%ignore pkmn::swig::item_bag::get_pocket(const std::string&);

// Convert getter/setter functions into attributes for more idiomatic Python.

%attributestring(pkmn::swig::item_bag, std::string, game, get_game);
%attributeval(pkmn::swig::item_bag, %arg(std::vector<std::string>), pocket_names, get_pocket_names);

%extend pkmn::swig::item_bag
{
/*
 * SWIG 3.0.8 introduced the SWIG_PYTHON_2_UNICODE macro, which allows the
 * Python 2 "unicode" type to be converted to a char* or std::string. There's
 * no way for a SWIG project to bring this in, so we need this ugly workaround
 * when using earlier verisons of SWIG.
 */
#if defined(SWIGPYTHON) && SWIG_VERSION < 0x030008
    pkmn::swig::item_list __getitem__(
        const std::wstring& pocket_name
    )
    {
        return self->get_pocket(
                   boost::locale::conv::utf_to_utf<char>(pocket_name)
               );
    }
#else
    pkmn::swig::item_list __getitem__(
        const std::string& pocket_name
    )
    {
        return self->get_pocket(pocket_name);
    }
#endif

    size_t __len__()
    {
        return self->get_pocket_names().size();
    }
}

%include "cpp_wrappers/item_bag.hpp"
