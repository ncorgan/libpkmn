/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN
{
    internal class DatabaseInitializer
    {
        public DatabaseInitializer()
        {
            // Initialize on import instead of on the first call. This adds a small
            // amount of import time but increases overall application efficiency.
            PKMN.InitializeDatabaseConnection();
        }
    }
}
