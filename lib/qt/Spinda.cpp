/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>
#include <pkmn/qt/Spinda.hpp>

#ifdef PKMN_ENABLE_QT

#include "exception_internal.hpp"
#include "utils/misc.hpp"
#include "SpindaSpotMap.hpp"

#include <pkmn/calculations/spinda_spots.hpp>
#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/utils/paths.hpp>

#include <boost/filesystem.hpp>

#include <QColor>
#include <QImage>
#include <QString>

#include <iostream>

namespace fs = boost::filesystem;

using namespace pkmn::calculations;

/*
 * Sources:
 *  * https://web.archive.org/web/20130524201028/http://www.freewebs.com/gatorshark/SpindaDocumentation.htm
 *  * https://github.com/magical/spinda
 */

typedef struct
{
    QColor color_main;
    QColor color_light;
    QColor color_dark;
} spinda_colors_t;

// Avoid compilation issues from different sizes.
static const size_t MAX_SPOT_HEIGHT = 13;

// Game Boy Advance
static const spinda_coords GBA_ORIGIN(8,6);
static const spinda_spots GBA_COORDS(
                              spinda_coords(0,0),
                              spinda_coords(24,1),
                              spinda_coords(6,18),
                              spinda_coords(18,19)
                          );

static const spinda_colors_t GBA_SPINDA_FACE_COLORS =
{
    QColor(0xE0, 0xD0, 0xA0),
    QColor(0xF0, 0xE0, 0xA8),
    QColor(0xC0, 0xB0, 0x80)
};
static const spinda_colors_t GBA_SPINDA_SPOT_COLORS =
{
    QColor(0xD8, 0x68, 0x38),
    QColor(0xD8, 0x88, 0x48),
    QColor(0xB0, 0x58, 0x28)
};
static const spinda_colors_t GBA_SPINDA_SPOT_COLORS_SHINY =
{
    QColor(0x90, 0xA0, 0x38),
    QColor(0xB0, 0xC0, 0x58),
    QColor(0x70, 0x80, 0x18)
};

// Generation IV-V
static const spinda_coords GEN4_ORIGIN(17, 7);
static const spinda_coords GEN5_ORIGIN(23, 15);
static const spinda_spots NDS_COORDS(
                              spinda_coords(0,0),
                              spinda_coords(24,2),
                              spinda_coords(3,18),
                              spinda_coords(15,18)
                          );

static const spinda_colors_t NDS_SPINDA_FACE_COLORS =
{
    QColor(0xE6, 0xD5, 0xA4),
    QColor(0xF6, 0xEE, 0xBD),
    QColor(0xCD, 0xA4, 0x73)
};
static const spinda_colors_t NDS_SPINDA_SPOT_COLORS =
{
    QColor(0xEE, 0x52, 0x4A),
    QColor(0xEE, 0x52, 0x4A), // No light color in DS sprites
    QColor(0xBD, 0x4A, 0x31)
};
static const spinda_colors_t NDS_SPINDA_SPOT_COLORS_SHINY =
{
    QColor(0xA4, 0xCD, 0x10),
    QColor(0xA4, 0xCD, 0x10), // No light color in DS sprites
    QColor(0x7B, 0x9C, 0x00)
};

namespace pkmn { namespace qt {

    static void drawSpindaSpot(
        QImage* image,
        const spinda_coords* coords,
        const char** single_spot_map,
        const spinda_colors_t* face_colors,
        const spinda_colors_t* spot_colors
    )
    {
        for(size_t i = 0; i < MAX_SPOT_HEIGHT and single_spot_map[i] != NULL; ++i)
        {
            for(size_t j = 0; j < std::strlen(single_spot_map[i]); ++j)
            {
                if(single_spot_map[i][j] == '*')
                {
                    QPoint point(
                               int(coords->x+j),
                               int(coords->y+i)
                           );
                    if(qAlpha(image->pixel(point)) > 0)
                    {
                        if(image->pixel(point) == face_colors->color_main.rgb())
                        {
                            image->setPixel(
                                point,
                                spot_colors->color_main.rgb()
                            );
                        }
                        else if(image->pixel(point) == face_colors->color_light.rgb())
                        {
                            image->setPixel(
                                point,
                                spot_colors->color_light.rgb()
                            );
                        }
                        else if(image->pixel(point) == face_colors->color_dark.rgb())
                        {
                            image->setPixel(
                                point,
                                spot_colors->color_dark.rgb()
                            );
                        }
                    }
                }
            }
        }
    }

    static void GenerateSpindaSprite(
        int generation,
        uint32_t personality,
        bool shiny,
        QImage* imageOut
    )
    {
        pkmn::enforce_bounds("generation", generation, 3, 5);

        if(game_is_gamecube(generation))
        {
            throw std::invalid_argument("No Gamecube support.");
        }
        else if(!imageOut)
        {
            throw std::invalid_argument("Null pointer passed into imageOut");
        }

        // There is no variation in sprite within a generation, so the generation is enough.
        fs::path input_path;
        pkmn::database::pokemon_entry spinda_entry;
        switch(generation)
        {
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

        if(!imageOut->load(QString::fromStdString(input_path.string())))
        {
            throw std::runtime_error("Failed to load base Spinda sprite.");
        }
        *imageOut = imageOut->convertToFormat(QImage::Format_ARGB32);

        const spinda_colors_t* face_colors = NULL;
        const spinda_colors_t* spot_colors = NULL;
        const char* (*spot_map)[MAX_SPOT_HEIGHT] = {0};

        switch(generation)
        {
            case 3:
                final_spot_coords = GBA_COORDS + spot_offset;
                final_spot_coords += GBA_ORIGIN;

                face_colors = &GBA_SPINDA_FACE_COLORS;
                spot_colors = shiny ? &GBA_SPINDA_SPOT_COLORS_SHINY : &GBA_SPINDA_SPOT_COLORS;
                spot_map = GBA_SPINDA_SPOT_MAP;
                break;

            // Generation already validated
            default:
                final_spot_coords = NDS_COORDS + spot_offset;
                final_spot_coords += (generation == 4) ? GEN4_ORIGIN : GEN5_ORIGIN;

                face_colors = &NDS_SPINDA_FACE_COLORS;
                spot_colors = shiny ? &NDS_SPINDA_SPOT_COLORS_SHINY : &NDS_SPINDA_SPOT_COLORS;
                spot_map = NDS_SPINDA_SPOT_MAP;
                break;

        }

        drawSpindaSpot(
            imageOut,
            &final_spot_coords.left_ear,
            spot_map[0],
            face_colors,
            spot_colors
        );
        drawSpindaSpot(
            imageOut,
            &final_spot_coords.right_ear,
            spot_map[1],
            face_colors,
            spot_colors
        );
        drawSpindaSpot(
            imageOut,
            &final_spot_coords.left_face,
            spot_map[2],
            face_colors,
            spot_colors
        );
        drawSpindaSpot(
            imageOut,
            &final_spot_coords.right_face,
            spot_map[3],
            face_colors,
            spot_colors
        );
    }

    void GenerateSpindaSpriteAtFilepath(
        int generation,
        uint32_t personality,
        bool shiny,
        const std::string& filepath
    )
    {
        QImage outputImage;

        GenerateSpindaSprite(
            generation,
            personality,
            shiny,
            &outputImage
        );

        bool successful = outputImage.save(QString::fromStdString(filepath));
        if(not successful)
        {
            throw std::runtime_error("Failed to generate Spinda sprite.");
        }
    }

}}

#else

namespace pkmn { namespace qt {

    void GenerateSpindaSpriteAtFilepath(
        PKMN_UNUSED(int generation),
        PKMN_UNUSED(uint32_t personality),
        PKMN_UNUSED(bool shiny),
        PKMN_UNUSED(const std::string& filepath)
    )
    {
        throw pkmn::feature_not_in_build_error("Qt support");
    }

}}

#endif
