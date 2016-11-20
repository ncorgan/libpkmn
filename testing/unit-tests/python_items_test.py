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
import pkmntest
import sys
import traceback

def gen1_item_list_test():
    try:
        items = pkmn.item_list("Items", "Red")
        pkmntest.gen1_items_tests.item_list_test(items)

        return True

    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def gen1_pc_test():
    try:
        pc = pkmn.item_list("PC", "Red")
        pkmntest.gen1_items_tests.pc_test(pc)

        return True

    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False

def gen1_item_bag_test():
    try:
        bag = pkmn.item_bag("Red")
        pkmntest.gen1_items_tests.item_bag_test(bag)

        return True

    except:
        print()
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback,
                                  limit=5, file=sys.stdout)
        return False


if __name__ == "__main__":

    successful = True
    for test in [gen1_item_list_test, gen1_pc_test, gen1_item_bag_test]:
        sys.stdout.write("Starting test %s..." % test.__name__)
        result = test()
        print("success.\n" if result else "failed.\n")
        successful = successful and result

    sys.exit(0 if successful else 1)
