/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_list.hpp>

    #include <string>
%}

%include <std_string.i>

%rename(as_list) as_vector;
%include <pkmn/item_list.hpp>

#ifdef PKMN_WSTRING_WORKAROUND
%warnfilter(508) pkmn::shared_ptr<pkmn::item_list>;
#endif

%extend pkmn::shared_ptr<pkmn::item_list> {

    int __len__() {
        return self->get()->get_capacity();
    }

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

#ifdef PKMN_WSTRING_WORKAROUND
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

#ifdef PKMN_WSTRING_WORKAROUND
PKMN_PYTHON_MAP(std::wstring, pkmn::item_list::sptr, item_pockets);
#else
PKMN_PYTHON_MAP(std::string, pkmn::item_list::sptr, item_pockets);
#endif
