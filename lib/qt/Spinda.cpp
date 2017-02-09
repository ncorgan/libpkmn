/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../misc_common.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/calculations/spinda_spots.hpp>
#include <pkmn/database/pokemon_entry.hpp>
#include <pkmn/qt/Spinda.hpp>
#include <pkmn/utils/paths.hpp>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

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

    void GenerateSpindaSprite(
        int generation,
        uint32_t personality,
        bool shiny,
        QImage* imageOut
    ) {
        if(generation < 3 or generation > 5) {
            throw pkmn::range_error("personality", 3, 5);
        } else if(game_is_gamecube(generation)) {
            throw std::invalid_argument("No Gamecube support.");
        } else if(imageOut == nullptr) {
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
        pkmn::calculations::spinda_spots spot_offset = pkmn::calculations::spinda_spot_offset(
                                                           personality
                                                       );
        (void)spot_offset;

        if(!imageOut->load(QString::fromStdString(input_path.string()))) {
            throw std::runtime_error("Failed to load base Spinda sprite.");
        }
    }

}}
