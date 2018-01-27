/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_list.hpp>
%}

%include <std_string.i>

%rename(as_list) as_vector;
%include <pkmn/item_list.hpp>

%extend std::shared_ptr<pkmn::item_list> {

    int __len__()
    {
        return self->get()->get_capacity();
    }

    const pkmn::item_slot& __getitem__(
        int index
    )
    {
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
}
%template(item_list_sptr) std::shared_ptr<pkmn::item_list>;

PKMN_PYTHON_MAP(std::string, pkmn::item_list::sptr, item_pockets);
