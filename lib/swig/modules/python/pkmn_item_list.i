/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_list.hpp>

    #include <boost/locale/encoding_utf.hpp>

    #include <string>
%}

%include <std_string.i>

%rename(as_list) as_vector;
%include <pkmn/item_list.hpp>

#if SWIG_VERSION < 0x030008
%include <std_wstring.i>
%warnfilter(508) pkmn::shared_ptr<pkmn::item_list>;
#endif

%extend pkmn::shared_ptr<pkmn::item_list> {

    const pkmn::item_slot& __getitem__(
        int index
    ) {
        return self->get()->as_vector().at(index);
    }

    %pythoncode %{
        def __eq__(self, rhs):
            if not isinstance(rhs, (item_list_sptr)):
                return False
            else:
                return self.__sptr_eq(rhs)

        def __ne__(self, rhs):
            return not (self == rhs)
    %}

/*
 * SWIG 3.0.8 introduced the SWIG_PYTHON_2_UNICODE macro, which allows the
 * Python 2 "unicode" type to be converted to a char* or std::string. There's
 * no way for a SWIG project to bring this in, so we need this ugly workaround
 * when using earlier versions of SWIG.
 */
#if SWIG_VERSION < 0x030008

    void add(
        const std::wstring &item_name,
        int amount
    ) {
        self->get()->add(
            boost::locale::conv::utf_to_utf<char>(item_name),
            amount
        );
    }

    void remove(
        const std::wstring &item_name,
        int amount
    ) {
        self->get()->remove(
            boost::locale::conv::utf_to_utf<char>(item_name),
            amount
        );
    }

#endif

}
%template(item_list_sptr) pkmn::shared_ptr<pkmn::item_list>;

#if SWIG_VERSION < 0x030008
PKMN_PYTHON_MAP(std::wstring, pkmn::item_list::sptr, item_pockets);
#else
PKMN_PYTHON_MAP(std::string, pkmn::item_list::sptr, item_pockets);
#endif
