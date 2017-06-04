/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN {

    /// <summary>Information about this build of LibPKMN.</summary>
    public class BuildInfo {

        /// <summary>The version of Boost used to build the LibPKMN core library.</summary>
        public static string BoostVersion {
            get {
                return PKMN.GetBoostVersion();
            }
        }

        /// <summary>The version of LibPkmGC used to build the LibPKMN core library.</summary>
        public static string LibPkmGCVersion {
            get {
                return PKMN.GetLibPkmGCVersion();
            }
        }

        /// <summary>The version of PKSav used to build the LibPKMN core library.</summary>
        public static string PKSavVersion {
            get {
                return PKMN.GetPKSavVersion();
            }
        }

        /// <summary>The version of Qt used to build the LibPKMN core library.</summary>
        public static string QtVersion {
            get {
                return PKMN.GetQtVersion();
            }
        }

        /// <summary>The version of SQLite3 used to build the LibPKMN core library.</summary>
        public static string SQLite3Version {
            get {
                return PKMN.GetSQLite3Version();
            }
        }

        /// <summary>The version of SQLiteCpp used to build the LibPKMN core library.</summary>
        public static string SQLiteCppVersion {
            get {
                return PKMN.GetSQLiteCppVersion();
            }
        }

        /// <summary>The version of SWIG used to build the LibPKMN C# wrapper.</summary>
        public static string SWIGVersion {
            get {
                return PKMN.GetSWIGVersion();
            }
        }

    }
}
