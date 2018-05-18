/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN
{
    public partial class PKMN
    {
        /*
         * Instantiate this class to initialize the database when the DLL is loaded
         * rather than the first time it's actually needed. We don't actually do
         * anything with this, so suppress the resulting warning.
         */
        #pragma warning disable 0414
        private static DatabaseInitializer Initializer = new DatabaseInitializer();
    }
}
