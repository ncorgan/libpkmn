/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"
#include "SpindaSpotMap.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/spinda_spots.hpp>
#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/qt/Spinda.hpp>
#include <pkmn/utils/paths.hpp>

#include <boost/filesystem.hpp>

#include <QColor>

#include <iostream>

namespace fs = boost::filesystem;

using namespace pkmn::calculations;

/*
 * Sources:
 *  * https://web.archive.org/web/20130524201028/http://www.freewebs.com/gatorshark/SpindaDocumentation.htm
 *  * https://github.com/magical/spinda
 */

// Game Boy Advance
BOOST_STATIC_CONSTEXPR spinda_coords GBA_ORIGIN(8,6);
BOOST_STATIC_CONSTEXPR spinda_spots GBA_COORDS(
                                        spinda_coords(0,0),
                                        spinda_coords(24,1),
                                        spinda_coords(6,18),
                                        spinda_coords(18,19)
                                    );
BOOST_STATIC_CONSTEXPR size_t GBA_SPOT_HEIGHT = 13;

// Colors

typedef struct {
    QColor color_main;
    QColor color_light;
    QColor color_dark;
} spinda_colors_t;

static const spinda_colors_t GBA_SPINDA_SPOT_COLORS = {
    QColor(0xD8, 0x68, 0x38),
    QColor(0xD8, 0x88, 0x48),
    QColor(0xB0, 0x58, 0x28)
};


namespace pkmn { namespace qt {

    bool GenerateSpindaSpriteAtPath(
        int generation,
        uint32_t personality,
        bool shiny,
        const QString &filePath
    ) {
        QImage outputImage;

        GenerateSpindaSprite(
            generation,
            personality,
            shiny,
            &outputImage
        );

        return outputImage.save(filePath);
    }

    static void drawSpindaSpot(
        QImage* image,
        const spinda_coords* coords,
        const char** single_spot_map,
        const spinda_colors_t* spot_colors,
        size_t height
    ) {
        for(size_t i = 0; i < height && single_spot_map[i] != NULL; ++i) {
            for(size_t j = 0; j < std::strlen(single_spot_map[i]); ++j) {
                if(single_spot_map[i][j] == '*') {
                    QPoint point(
                               coords->x+j,
                               coords->y+i
                           );
                    if(qAlpha(image->pixel(point)) > 0) {
                        image->setPixel(
                            point,
                            spot_colors->color_main.rgb()
                        );
                    }
                }
            }
        }
    }

    void GenerateSpindaSprite(
        int generation,
        uint32_t personality,
        bool shiny,
        QImage* imageOut
    ) {
        if(generation < 3 or generation > 5) {
            throw pkmn::range_error("generation", 3, 5);
        } else if(game_is_gamecube(generation)) {
            throw std::invalid_argument("No Gamecube support.");
        } else if(!imageOut) {
            throw std::invalid_argument("Null pointer passed into imageOut");
        }

        // There is no variation in sprite within a generation, so the generation is enough.
        fs::path input_path;
        pkmn::database::pokemon_entry spinda_entry;
        switch(generation) {
            case 3:
                spinda_entry = pkmn::database::pokemon_entry("Spinda", "Ruby", "");
                break;

            case 4:
                spinda_entry = pkmn::database::pokemon_entry("Spinda", "Diamond", "");
                break;

            case 5:
                spinda_entry = pkmn::database::pokemon_entry("Spinda", "Black", "");
                break;
        }
        input_path = fs::path(spinda_entry.get_sprite_filepath(false, shiny));

        // Get spot offsets.
        spinda_spots spot_offset = spinda_spot_offset(
                                       personality
                                   );
        spinda_spots final_spot_coords;

        if(!imageOut->load(QString::fromStdString(input_path.string()))) {
            throw std::runtime_error("Failed to load base Spinda sprite.");
        }
        *imageOut = imageOut->convertToFormat(QImage::Format_ARGB32);

        const spinda_colors_t* spot_colors = NULL;
        const char* (*spot_map)[GBA_SPOT_HEIGHT] = {0};

        switch(generation) {
            case 3:
                final_spot_coords = GBA_COORDS + spot_offset;
                final_spot_coords += GBA_ORIGIN;

                spot_colors = &GBA_SPINDA_SPOT_COLORS;
                spot_map = GBA_SPINDA_SPOT_MAP;
                break;

            default:
                throw pkmn::unimplemented_error();
        }

        drawSpindaSpot(
            imageOut,
            &final_spot_coords.left_ear,
            spot_map[0],
            spot_colors,
            GBA_SPOT_HEIGHT
        );
        drawSpindaSpot(
            imageOut,
            &final_spot_coords.right_ear,
            spot_map[1],
            spot_colors,
            GBA_SPOT_HEIGHT
        );
        drawSpindaSpot(
            imageOut,
            &final_spot_coords.left_face,
            spot_map[2],
            spot_colors,
            GBA_SPOT_HEIGHT
        );
        drawSpindaSpot(
            imageOut,
            &final_spot_coords.right_face,
            spot_map[3],
            spot_colors,
            GBA_SPOT_HEIGHT
        );
    }

}}
