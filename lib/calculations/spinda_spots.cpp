/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/calculations/spinda_spots.hpp>

#include <pksav/math/endian.h>

#include <boost/config.hpp>

/*
 * Source: http://web.archive.org/web/20141110213341/http://www.freewebs.com/gatorshark/SpindaDocumentation.htm
 */

namespace pkmn { namespace calculations {

    /*
     * Helper functions
     */

    BOOST_STATIC_CONSTEXPR spinda_coords operator+(
        const spinda_coords &lhs,
        const spinda_coords &rhs
    ) {
        return spinda_coords(
                   (lhs.x + rhs.x),
                   (lhs.y + rhs.y)
               );
    }

    BOOST_STATIC_CONSTEXPR spinda_spots operator+(
        const spinda_spots &lhs,
        const spinda_spots &rhs
    ) {
        return spinda_spots(
                   spinda_coords(lhs.left_ear + rhs.left_ear),
                   spinda_coords(lhs.right_ear + rhs.right_ear),
                   spinda_coords(lhs.left_face + rhs.left_face),
                   spinda_coords(lhs.right_face + rhs.right_face)
               );
    }

    BOOST_STATIC_CONSTEXPR spinda_spots get_offset(
        uint32_t personality
    ) {
        return spinda_spots(
                   spinda_coords(
                       int( personality & 0x0000000F),
                       int((personality & 0x000000F0) >> 4)
                   ),
                   spinda_coords(
                       int((personality & 0x0000F000) >> 8),
                       int((personality & 0x00000F00) >> 12)
                   ),
                   spinda_coords(
                       int((personality & 0x000F0000) >> 16),
                       int((personality & 0x00F00000) >> 20)
                   ),
                   spinda_coords(
                       int((personality & 0x0F000000) >> 24),
                       int((personality & 0xF0000000) >> 28)
                   )
               );
    }

    /*
     * Spot origins
     */

    BOOST_STATIC_CONSTEXPR spinda_spots gen3_origin(
        spinda_coords(32,9),  spinda_coords(8,8),
        spinda_coords(26,27), spinda_coords(14,26)
    );

    BOOST_STATIC_CONSTEXPR spinda_spots nds_origin(
        spinda_coords(31,9),  spinda_coords(7,7),
        spinda_coords(22,25), spinda_coords(10,25)
    );

    /*
     * Main functions
     */

    spinda_spots gen3_spinda_spots(
        uint32_t personality
    ) {
        uint32_t pid_le32 = pksav_littleendian32(personality);
        return (gen3_origin + get_offset(pid_le32));
    }

    spinda_spots nds_spinda_spots(
        uint32_t personality
    ) {
        uint32_t pid_le32 = pksav_littleendian32(personality);
        return (nds_origin + get_offset(pid_le32));
    }

}}
