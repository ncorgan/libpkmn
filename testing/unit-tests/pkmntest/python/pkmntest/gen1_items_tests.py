#!/usr/bin/env python
#
# Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn
import sys
import traceback

def __item_list_common(items):
    # Confirm errors are thrown when expected
    try:
        items.add("Potion", 0)
    except IndexError:
        pass
    except:
        raise RuntimeError("This should have thrown an IndexError.")

    try:
        items.add("Potion", 100)
    except IndexError:
        pass
    except:
        raise RuntimeError("This should have thrown an IndexError.")

    try:
        items.remove("Potion", 0)
    except IndexError:
        pass
    except:
        raise RuntimeError("This should have thrown an IndexError.")

    try:
        items.remove("Potion", 100)
    except IndexError:
        pass
    except:
        raise RuntimeError("This should have thrown an IndexError.")

    # Start adding and removing stuff, and make sure the numbers are
    # accurate. This also tests the custom __getitem__ function.

    items.add("Potion", 30)
    items.add("Great Ball", 99)
    items.add("Ether", 1)

    if items[0].item.get_name() != "Potion":
        raise RuntimeError("items[0].item.get_name() (\"{0}\") != \"Potion\"".format(items[0].item.get_name()))
    if items[0].amount != 30:
        raise RuntimeError("items[0].amount ({0}) != 30".format(items[0].amount))
    if items[1].item.get_name() != "Great Ball":
        raise RuntimeError("items[1].item.get_name() (\"{0}\") != \"Great Ball\"".format(items[1].item.get_name()))
    if items[1].amount != 99:
        raise RuntimeError("items[1].amount ({0}) != 99".format(items[1].amount))
    if items[2].item.get_name() != "Ether":
        raise RuntimeError("items[2].item.get_name() (\"{0}\") != \"Ether\"".format(items[2].item.get_name()))
    if items[2].amount != 1:
        raise RuntimeError("items[2].amount ({0}) != 1".format(items[2].amount))
    if items.get_num_items() != 3:
        raise RuntimeError("items.get_num_items() ({0}) != 3".format(items.get_num_items()))

    items.remove("Great Ball", 20)

    if items[1].item.get_name() != "Great Ball":
        raise RuntimeError("items[1].item.get_name() (\"{0}\") != \"Great Ball\"".format(items[1].item.get_name()))
    if items[1].amount != 79:
        raise RuntimeError("items[1].amount ({0}) != 79".format(items[1].amount))

    items.move(0, 1)

    if items[0].item.get_name() != "Great Ball":
        raise RuntimeError("items[0].item.get_name() (\"{0}\") != \"Great Ball\"".format(items[0].item.get_name()))
    if items[0].amount != 79:
        raise RuntimeError("items[0].amount ({0}) != 79".format(items[0].amount))
    if items[1].item.get_name() != "Potion":
        raise RuntimeError("items[1].item.get_name() (\"{0}\") != \"Potion\"".format(items[1].item.get_name()))
    if items[1].amount != 30:
        raise RuntimeError("items[1].amount ({0}) != 30".format(items[1].amount))
    if items.get_num_items() != 3:
        raise RuntimeError("items.get_num_items() ({0}) != 3".format(items.get_num_items()))

    items.remove("Potion", 30)

    if items[0].item.get_name() != "Great Ball":
        raise RuntimeError("items[0].item.get_name() (\"{0}\") != \"Great Ball\"".format(items[0].item.get_name()))
    if items[0].amount != 79:
        raise RuntimeError("items[0].amount ({0}) != 79".format(items[0].amount))
    if items[1].item.get_name() != "Ether":
        raise RuntimeError("items[1].item.get_name() (\"{0}\") != \"Ether\"".format(items[1].item.get_name()))
    if items[1].amount != 1:
        raise RuntimeError("items[1].amount ({0}) != 1".format(items[1].amount))
    if items.get_num_items() != 2:
        raise RuntimeError("items.get_num_items() ({0}) != 2".format(items.get_num_items()))

def item_list_test(items):
    # Check unchanging and initial values
    if items.get_name() != "Items":
        raise RuntimeError("items.get_name() (\"{0}\") != \"Items\"".format(items.get_name()))
    if items.get_game() != "Red":
        raise RuntimeError("items.get_game() (\"{0}\") != \"Red\"".format(items.get_game()))
    if items.get_capacity() != 20:
        raise RuntimeError("items.get_capacity() ({0}) != 20".format(items.get_capacity()))
    if items.get_num_items() != 0:
        raise RuntimeError("items.get_num_items() ({0}) != 0".format(items.get_num_items()))

    __item_list_common(items)

def pc_test(items):
    # Check unchanging and initial values
    if items.get_name() != "PC":
        raise RuntimeError("items.get_name() (\"{0}\") != \"PC\"".format(items.get_name()))
    if items.get_game() != "Red":
        raise RuntimeError("items.get_game() (\"{0}\") != \"Red\"".format(items.get_game()))
    if items.get_capacity() != 50:
        raise RuntimeError("items.get_capacity() ({0}) != 50".format(items.get_capacity()))
    if items.get_num_items() != 0:
        raise RuntimeError("items.get_num_items() ({0}) != 0".format(items.get_num_items()))

    __item_list_common(items)

def item_bag_test(bag):
    pockets = bag.get_pockets()
    if len(pockets) != 1:
        raise RuntimeError("len(pockets) ({0}) != 1".format(len(pockets)))

    item_list_test(bag["Items"])
