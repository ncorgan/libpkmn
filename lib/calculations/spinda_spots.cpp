/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
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

    inline spinda_spots _spinda_spot_offset(
        uint32_t personality
    ) {
        return spinda_spots(
                   spinda_coords(
                       int( personality & 0x0000000F),
                       int((personality & 0x000000F0) >> 4)
                   ),
                   spinda_coords(
                       int((personality & 0x00000F00) >> 8),
                       int((personality & 0x0000F000) >> 12)
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

    spinda_spots spinda_spot_offset(
        uint32_t personality
    ) {
        uint32_t pid_le32 = pksav_littleendian32(personality);
        return _spinda_spot_offset(pid_le32);
    }

}}
