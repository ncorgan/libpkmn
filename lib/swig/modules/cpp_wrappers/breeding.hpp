/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_BREEDING_HPP
#define CPP_WRAPPERS_BREEDING_HPP

#include "pokemon.hpp"

#include <pkmn/config.hpp>

/*
 * We want to hide these functions from the preprocessor when SWIG is run,
 * but not during compilation itself.
 */
#ifdef SWIG
#undef SWIG
#endif

#include <pkmn/breeding/child_info.hpp>

#define SWIG

#include <boost/assert.hpp>

namespace pkmn { namespace swig { namespace breeding {

    inline std::vector<std::string> get_child_moves(
        const pkmn::swig::pokemon& mother,
        const pkmn::swig::pokemon& father,
        const std::string& child_species
    )
    {
        BOOST_ASSERT(mother.get_internal() != nullptr);
        BOOST_ASSERT(father.get_internal() != nullptr);

        return pkmn::breeding::get_child_moves(
                   mother.get_internal(),
                   father.get_internal(),
                   child_species
               );
    }

    inline std::map<std::string, int> get_ideal_child_IVs(
        const pkmn::swig::pokemon& mother,
        const pkmn::swig::pokemon& father,
        pkmn::e_gender child_gender
    )
    {
        BOOST_ASSERT(mother.get_internal() != nullptr);
        BOOST_ASSERT(father.get_internal() != nullptr);

        return pkmn::breeding::get_ideal_child_IVs(
                   mother.get_internal(),
                   father.get_internal(),
                   child_gender
               );
    }

}}}

#endif /* CPP_WRAPPERS_BREEDING_HPP */
