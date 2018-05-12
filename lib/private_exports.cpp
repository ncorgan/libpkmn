/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "private_exports.hpp"

#include "database/database_common.hpp"

#include "types/rng.hpp"

#include <pkmn/config.hpp>
#include <pkmn/utils/paths.hpp>

#include <boost/filesystem.hpp>

#ifdef PKMN_ENABLE_QT
#include <QImage>
#include <QString>
#endif

#ifdef PKMN_PLATFORM_WIN32
#    include <windows.h>
#    define GET_PROCESS_ID() GetCurrentProcessId()
#else
#    include <unistd.h>
#    define GET_PROCESS_ID() getpid()
#endif

namespace fs = boost::filesystem;

namespace pkmn { namespace priv {

    int game_name_to_generation(
        const std::string& game
    )
    {
        return pkmn::database::game_name_to_generation(game);
    }

    void initialize_database_connection()
    {
        pkmn::database::initialize_connection();
    }

    std::string convert_image_for_mono(
        const std::string& filepath
    )
    {
#ifdef PKMN_ENABLE_QT
        static const fs::path TMP_DIR(pkmn::get_tmp_dir());

        QImage input_image(QString::fromStdString(filepath));
        QImage output_image = input_image.convertToFormat(QImage::Format_ARGB32);

        fs::path output_path(TMP_DIR);

        std::string output_filename(std::to_string(GET_PROCESS_ID()));
        output_filename.append(std::to_string(pkmn::rng<uint32_t>().rand()));
        output_filename.append(".png");

        output_path /= output_filename;

        output_image.save(QString::fromStdString(output_path.string()));

        return output_path.string();
#else
        return filepath;
#endif
    }
}}
