#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"
require "PKMNTest"

require "minitest/autorun"
require "os"
require "pathname"

class ItemTest < PKMNTest

    @@RNG = Random.new

    def item_list_test_empty_slots(item_list)
        item_list.each do |slot|
            assert_equal(PKMN::Item::NONE, slot.item)
            assert_equal(0, slot.amount)
        end
    end

    def item_list_test_out_of_range(item_list, item_name)
        assert_raises IndexError do
            item_list.add(item_name, 0)
        end
        assert_raises IndexError do
            item_list.add(item_name, 100)
        end
        assert_raises IndexError do
            item_list.remove(item_name, 0)
        end
        assert_raises IndexError do
            item_list.remove(item_name, 100)
        end
    end

    def item_class_test_invalid_items(item_class, invalid_items)
        invalid_items.each do |item|
            assert_raises ArgumentError do
                item_class.add(item, 1)
            end
        end
    end

    def item_list_test_setting_items(item_list, items)
        assert_equal(item_list.num_items, 0)
        assert_equal(items.length, 8)

        # Make sure invalid indices are caught.
        assert_raises IndexError do
            item_list[-1].item = items[0]
        end
        assert_raises IndexError do
            item_list[item_list.length].item = items[0]
        end

        item_list[0].item = items[0]
        item_list[0].amount = 50
        item_list[1].item = items[1]
        item_list[1].amount = 40
        item_list[2].item = items[2]
        item_list[2].amount = 30

        assert_equal(item_list.num_items, 3)
        assert_equal(item_list[0].item, items[0])
        assert_equal(item_list[0].amount, 50)
        assert_equal(item_list[1].item, items[1])
        assert_equal(item_list[1].amount, 40)
        assert_equal(item_list[2].item, items[2])
        assert_equal(item_list[2].amount, 30)

        # Make sure the item list being contiguous is enforced.
        assert_raises IndexError do
            item_list[4].item = items[3]
        end

        item_list[2].item = PKMN::Item::NONE

        assert_equal(item_list.num_items, 2)
        assert_equal(item_list[0].item, items[0])
        assert_equal(item_list[0].amount, 50)
        assert_equal(item_list[1].item, items[1])
        assert_equal(item_list[1].amount, 40)
        assert_equal(item_list[2].item, PKMN::Item::NONE)
        assert_equal(item_list[2].amount, 0)

        item_list[0].item = PKMN::Item::NONE
        item_list[0].item = PKMN::Item::NONE

        (0..2).each do |index|
            assert_equal(item_list[index].item, PKMN::Item::NONE)
            assert_equal(item_list[index].amount, 0)
        end
    end

    def item_list_test_add_remove(item_list, items)
        assert_equal(item_list.num_items, 0)
        assert_equal(items.length, 8)

        item_list.add(items[0], 30)
        item_list.add(items[1], 99)
        item_list.add(items[2], 1)

        assert_equal(item_list[0].item, items[0])
        assert_equal(item_list[0].amount, 30)
        assert_equal(item_list[1].item, items[1])
        assert_equal(item_list[1].amount, 99)
        assert_equal(item_list[2].item, items[2])
        assert_equal(item_list[2].amount, 1)
        assert_equal(item_list.num_items, 3)

        item_list.add(items[2], 15)

        assert_equal(item_list[0].item, items[0])
        assert_equal(item_list[0].amount, 30)
        assert_equal(item_list[1].item, items[1])
        assert_equal(item_list[1].amount, 99)
        assert_equal(item_list[2].item, items[2])
        assert_equal(item_list[2].amount, 16)
        assert_equal(item_list.num_items, 3)

        item_list.remove(items[1], 20)

        assert_equal(item_list[0].item, items[0])
        assert_equal(item_list[0].amount, 30)
        assert_equal(item_list[1].item, items[1])
        assert_equal(item_list[1].amount, 79)
        assert_equal(item_list[2].item, items[2])
        assert_equal(item_list[2].amount, 16)
        assert_equal(item_list.num_items, 3)

        item_list.move(0, 1)

        assert_equal(item_list[0].item, items[1])
        assert_equal(item_list[0].amount, 79)
        assert_equal(item_list[1].item, items[0])
        assert_equal(item_list[1].amount, 30)
        assert_equal(item_list[2].item, items[2])
        assert_equal(item_list[2].amount, 16)
        assert_equal(item_list.num_items, 3)

        item_list.remove(items[0], 30)

        assert_equal(item_list[0].item, items[1])
        assert_equal(item_list[0].amount, 79)
        assert_equal(item_list[1].item, items[2])
        assert_equal(item_list[1].amount, 16)
        assert_equal(item_list[2].item, PKMN::Item::NONE)
        assert_equal(item_list[2].amount, 0)
        assert_equal(item_list.num_items, 2)

        item_list.add(items[3], 90)

        assert_equal(item_list[0].item, items[1])
        assert_equal(item_list[0].amount, 79)
        assert_equal(item_list[1].item, items[2])
        assert_equal(item_list[1].amount, 16)
        assert_equal(item_list[2].item, items[3])
        assert_equal(item_list[2].amount, 90)
        assert_equal(item_list.num_items, 3)

        item_list.add(items[4], 2)

        assert_equal(item_list[0].item, items[1])
        assert_equal(item_list[0].amount, 79)
        assert_equal(item_list[1].item, items[2])
        assert_equal(item_list[1].amount, 16)
        assert_equal(item_list[2].item, items[3])
        assert_equal(item_list[2].amount, 90)
        assert_equal(item_list[3].item, items[4])
        assert_equal(item_list[3].amount, 2)
        assert_equal(item_list.num_items, 4)

        item_list.remove(items[1], 30)

        assert_equal(item_list[0].item, items[1])
        assert_equal(item_list[0].amount, 49)
        assert_equal(item_list[1].item, items[2])
        assert_equal(item_list[1].amount, 16)
        assert_equal(item_list[2].item, items[3])
        assert_equal(item_list[2].amount, 90)
        assert_equal(item_list[3].item, items[4])
        assert_equal(item_list[3].amount, 2)
        assert_equal(item_list.num_items, 4)

        item_list.add(items[5], 12)

        assert_equal(item_list[0].item, items[1])
        assert_equal(item_list[0].amount, 49)
        assert_equal(item_list[1].item, items[2])
        assert_equal(item_list[1].amount, 16)
        assert_equal(item_list[2].item, items[3])
        assert_equal(item_list[2].amount, 90)
        assert_equal(item_list[3].item, items[4])
        assert_equal(item_list[3].amount, 2)
        assert_equal(item_list[4].item, items[5])
        assert_equal(item_list[4].amount, 12)
        assert_equal(item_list.num_items, 5)

        item_list.remove(items[2], 16)

        assert_equal(item_list[0].item, items[1])
        assert_equal(item_list[0].amount, 49)
        assert_equal(item_list[1].item, items[3])
        assert_equal(item_list[1].amount, 90)
        assert_equal(item_list[2].item, items[4])
        assert_equal(item_list[2].amount, 2)
        assert_equal(item_list[3].item, items[5])
        assert_equal(item_list[3].amount, 12)
        assert_equal(item_list.num_items, 4)

        item_list.add(items[6], 65)

        assert_equal(item_list[0].item, items[1])
        assert_equal(item_list[0].amount, 49)
        assert_equal(item_list[1].item, items[3])
        assert_equal(item_list[1].amount, 90)
        assert_equal(item_list[2].item, items[4])
        assert_equal(item_list[2].amount, 2)
        assert_equal(item_list[3].item, items[5])
        assert_equal(item_list[3].amount, 12)
        assert_equal(item_list[4].item, items[6])
        assert_equal(item_list[4].amount, 65)
        assert_equal(item_list.num_items, 5)

        item_list.add(items[7], 6)

        assert_equal(item_list[0].item, items[1])
        assert_equal(item_list[0].amount, 49)
        assert_equal(item_list[1].item, items[3])
        assert_equal(item_list[1].amount, 90)
        assert_equal(item_list[2].item, items[4])
        assert_equal(item_list[2].amount, 2)
        assert_equal(item_list[3].item, items[5])
        assert_equal(item_list[3].amount, 12)
        assert_equal(item_list[4].item, items[6])
        assert_equal(item_list[4].amount, 65)
        assert_equal(item_list[5].item, items[7])
        assert_equal(item_list[5].amount, 6)
        assert_equal(item_list.num_items, 6)

        item_list.remove(items[4], 2)
        item_list.remove(items[1], 49)
        item_list.remove(items[7], 6)
        item_list.remove(items[5], 12)
        item_list.remove(items[3], 90)
        item_list.remove(items[6], 65)

        assert_equal(item_list.num_items, 0)
    end
end
