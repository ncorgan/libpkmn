/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/stats.hpp>

namespace pkmn { namespace calculations {

    int get_gb_stat(
        const std::string &stat,
        int level,
        int base_stat,
        int EV,
        int IV
    ) {
        (void)stat;
        (void)level;
        (void)base_stat;
        (void)EV;
        (void)IV;
        return 0;
    }

    int get_modern_stat(
        const std::string &stat,
        int level,
        float nature_modifier,
        int base_stat,
        int EV,
        int IV
    ) {
        (void)stat;
        (void)level;
        (void)nature_modifier;
        (void)base_stat;
        (void)EV;
        (void)IV;
        return 0;
    }

}}
