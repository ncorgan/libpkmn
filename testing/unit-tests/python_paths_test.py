#!/usr/bin/env python
#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import os
import platform
import pkmn
import sys
import traceback

def appdata_dir_test():
    try:
        if platform.system() == "Windows":
            appdata_dir = "C:\\libpkmn\\appdata\\dir"
        else:
            appdata_dir = "/libpkmn/appdata/dir"

        os.putenv("PKMN_APPDATA_DIR", appdata_dir)
        pkmn_appdata_dir = pkmn.get_appdata_dir()
        if pkmn_appdata_dir != appdata_dir:
            raise RuntimeError("pkmn_appdata_dir (\"{0}\") != \"{1}\"".format(pkmn_appdata_dir, appdata_dir))

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def database_path_test():
    try:
        #
        # When this unit test is run, the PKMN_DATABASE_PATH environment variable
        # is set to a valid value, so just make sure the call doesn't fail.
        #
        pkmn.get_database_path()

        # Cause a failure with an existing file that isn't a database
        os.putenv("PKMN_DATABASE_PATH", os.path.realpath(__file__))
        try:
            pkmn.get_database_path()
        except RuntimeError:
            pass
        except:
            raise RuntimeError("The path \"{0}\" should not be considered a valid database.".format(os.path.realpath(__file__)))

        # Cause a failure with a non-existent file
        if platform.system() == "Windows":
            database_path = "C:\\libpkmn\\database\\path"
        else:
            database_path = "/libpkmn/database/path"

        os.putenv("PKMN_DATABASE_PATH", database_path)
        try:
            pkmn.get_database_path()
        except RuntimeError:
            pass
        except:
            raise RuntimeError("The path \"{0}\" should not be considered a valid database.".format(database_path))

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def images_dir_test():
    try:
        if platform.system() == "Windows":
            images_dir = "C:\\libpkmn\\images\\dir"
        else:
            images_dir = "/libpkmn/images/dir"

        os.putenv("PKMN_IMAGES_DIR", images_dir)
        pkmn_images_dir = pkmn.get_images_dir()
        if pkmn_images_dir != images_dir:
            raise RuntimeError("pkmn_images_dir (\"{0}\") != \"{1}\"".format(pkmn_images_dir, images_dir))

        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

# TODO
def tmp_dir_test():
    try:
        return True
    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

if __name__ == "__main__":

    successful = True
    for test in [appdata_dir_test, database_path_test, images_dir_test, tmp_dir_test]:
        sys.stdout.write("Starting test %s..." % test.__name__)
        result = test()
        print("success.\n" if result else "failed.\n")
        successful = successful and result

    sys.exit(0 if successful else 1)
