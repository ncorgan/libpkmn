/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/spinda_spots.hpp>

#include <boost/config.hpp>

/*
 * Source: http://web.archive.org/web/20141110213341/http://www.freewebs.com/gatorshark/SpindaDocumentation.htm
 */

namespace pkmn { namespace calculations {

    spinda_spots gen3_spinda_spots(
        uint32_t personality
    ) {
        (void)personality;
        return spinda_spots();
    }

    spinda_spots nds_spinda_spots(
        uint32_t personality
    ) {
        (void)personality;
        return spinda_spots();
    }

}}
