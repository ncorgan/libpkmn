/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_bag.hpp>
%}

%include <std_string.i>

%include <pkmn/item_bag.hpp>

%extend std::shared_ptr<pkmn::item_bag> {

    %pythoncode %{
        def __eq__(self, rhs):
            if not isinstance(rhs, (item_bag_sptr)):
                return False
            else:
                return self.__sptr_eq(rhs)

        def __ne__(self, rhs):
            return not (self == rhs)
    %}

    pkmn::item_list::sptr __getitem__(
        const std::string &key
    ) {
        return self->get()->get_pocket(key);
    }

    int __len__() {
        return int(self->get()->get_pocket_names().size());
    }
}
%template(item_bag_sptr) std::shared_ptr<pkmn::item_bag>;
