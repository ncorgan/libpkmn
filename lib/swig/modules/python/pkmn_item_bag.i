/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_bag.hpp>
%}

%include <std_string.i>

%include <pkmn/item_bag.hpp>

%extend pkmn::shared_ptr<pkmn::item_bag> {

    pkmn::item_list::sptr __getitem__(
        const std::string &key
    ) {
        return self->get()->get_pocket(key);
    }

    %pythoncode %{
        def __eq__(self, rhs):
            if not isinstance(rhs, (item_bag_sptr)):
                return False
            else:
                return self.__sptr_eq(rhs)

        def __ne__(self, rhs):
            return not (self == rhs)
    %}
}
%template(item_bag_sptr) pkmn::shared_ptr<pkmn::item_bag>;
