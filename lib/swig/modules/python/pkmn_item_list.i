/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_list.hpp>
%}

%rename(as_list) as_vector;
%include <pkmn/item_list.hpp>

%extend pkmn::shared_ptr<pkmn::item_list> {

    const pkmn::item_slot& __getitem__(
        int index
    ) {
        return self->get()->as_vector()[index];
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
%template(item_list_sptr) pkmn::shared_ptr<pkmn::item_list>;
