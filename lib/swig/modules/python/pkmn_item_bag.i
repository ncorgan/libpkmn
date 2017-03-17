/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_bag.hpp>

    #include <iostream>
    #include <string>
%}

%include <std_string.i>

%include <pkmn/item_bag.hpp>

#ifdef PKMN_WSTRING_WORKAROUND
%warnfilter(508) pkmn::shared_ptr<pkmn::item_bag>;
#endif

%extend pkmn::shared_ptr<pkmn::item_bag> {

    %pythoncode %{
        def __eq__(self, rhs):
            if not isinstance(rhs, (item_bag_sptr)):
                return False
            else:
                return self.__sptr_eq(rhs)

        def __ne__(self, rhs):
            return not (self == rhs)
    %}

#ifdef PKMN_WSTRING_WORKAROUND
    std::map<std::wstring, pkmn::item_list::sptr> get_pockets() {
        const pkmn::item_pockets_t& pockets = self->get()->get_pockets();
        std::map<std::wstring, pkmn::item_list::sptr> ret;

        for(auto iter = pockets.begin(); iter != pockets.end(); ++iter) {
            ret[boost::locale::conv::utf_to_utf<wchar_t>(iter->first)] = iter->second;
        }

        return ret;
    }

    std::vector<std::wstring> get_pocket_names() {
        std::vector<std::string> pocket_names = self->get()->get_pocket_names();
        std::vector<std::wstring> ret;
        for(size_t i = 0; i < pocket_names.size(); ++i) {
            ret.emplace_back(
               boost::locale::conv::utf_to_utf<wchar_t>(pocket_names[i])
            );
        }

        return ret;
    }

    pkmn::item_list::sptr __getitem__(
        const std::wstring &key
    ) {
        return self->get()->get_pocket(
                   boost::locale::conv::utf_to_utf<char>(key)
               );
    }

    pkmn::shared_ptr<pkmn::item_list> get_pocket(
        const std::wstring &name
    ) {
        return self->get()->get_pocket(
                   boost::locale::conv::utf_to_utf<char>(name)
               );
    }

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
#else
    pkmn::item_list::sptr __getitem__(
        const std::string &key
    ) {
        return self->get()->get_pocket(key);
    }
#endif

    int __len__() {
        return int(self->get()->get_pocket_names().size());
    }
}
%template(item_bag_sptr) pkmn::shared_ptr<pkmn::item_bag>;
