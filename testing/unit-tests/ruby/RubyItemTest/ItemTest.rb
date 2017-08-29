#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require "PKMN"

require "minitest/autorun"
require "os"
require "pathname"

class ItemTest < MiniTest::Test

    @@RNG = Random.new

    @@GAME_GENERATIONS = {
        "Red" => 1,
        "Blue" => 1,
        "Yellow" => 1,
        "Gold" => 2,
        "Silver" => 2,
        "Crystal" => 2,
        "Ruby" => 3,
        "Sapphire" => 3,
        "Emerald" => 3,
        "FireRed" => 3,
        "LeafGreen" => 3,
        "Colosseum" => 3,
        "XD" => 3,
        "Diamond" => 4,
        "Pearl" => 4,
        "Platinum" => 4,
        "HeartGold" => 4,
        "SoulSilver" => 4,
        "Black" => 5,
        "White" => 5,
        "Black 2" => 5,
        "White 2" => 5,
        "X" => 6,
        "Y" => 6,
        "Omega Ruby" => 6,
        "Alpha Sapphire" => 6
    }

    def item_list_test_empty_slots(item_list)
        item_list.each do |slot|
            assert_equal("None", slot.item)
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

    def item_list_test_add_remove(items, item_names)
        assert_equal(items.num_items, 0)
        assert_equal(item_names.length, 8)

        items.add(item_names[0], 30)
        items.add(item_names[1], 99)
        items.add(item_names[2], 1)

        assert_equal(items[0].item, item_names[0])
        assert_equal(items[0].amount, 30)
        assert_equal(items[1].item, item_names[1])
        assert_equal(items[1].amount, 99)
        assert_equal(items[2].item, item_names[2])
        assert_equal(items[2].amount, 1)
        assert_equal(items.num_items, 3)

        items.add(item_names[2], 15)

        assert_equal(items[0].item, item_names[0])
        assert_equal(items[0].amount, 30)
        assert_equal(items[1].item, item_names[1])
        assert_equal(items[1].amount, 99)
        assert_equal(items[2].item, item_names[2])
        assert_equal(items[2].amount, 16)
        assert_equal(items.num_items, 3)

        items.remove(item_names[1], 20)

        assert_equal(items[0].item, item_names[0])
        assert_equal(items[0].amount, 30)
        assert_equal(items[1].item, item_names[1])
        assert_equal(items[1].amount, 79)
        assert_equal(items[2].item, item_names[2])
        assert_equal(items[2].amount, 16)
        assert_equal(items.num_items, 3)

        items.move(0, 1)

        assert_equal(items[0].item, item_names[1])
        assert_equal(items[0].amount, 79)
        assert_equal(items[1].item, item_names[0])
        assert_equal(items[1].amount, 30)
        assert_equal(items[2].item, item_names[2])
        assert_equal(items[2].amount, 16)
        assert_equal(items.num_items, 3)

        items.remove(item_names[0], 30)

        assert_equal(items[0].item, item_names[1])
        assert_equal(items[0].amount, 79)
        assert_equal(items[1].item, item_names[2])
        assert_equal(items[1].amount, 16)
        assert_equal(items[2].item, "None")
        assert_equal(items[2].amount, 0)
        assert_equal(items.num_items, 2)

        items.add(item_names[3], 90)

        assert_equal(items[0].item, item_names[1])
        assert_equal(items[0].amount, 79)
        assert_equal(items[1].item, item_names[2])
        assert_equal(items[1].amount, 16)
        assert_equal(items[2].item, item_names[3])
        assert_equal(items[2].amount, 90)
        assert_equal(items.num_items, 3)

        items.add(item_names[4], 2)

        assert_equal(items[0].item, item_names[1])
        assert_equal(items[0].amount, 79)
        assert_equal(items[1].item, item_names[2])
        assert_equal(items[1].amount, 16)
        assert_equal(items[2].item, item_names[3])
        assert_equal(items[2].amount, 90)
        assert_equal(items[3].item, item_names[4])
        assert_equal(items[3].amount, 2)
        assert_equal(items.num_items, 4)

        items.remove(item_names[1], 30)

        assert_equal(items[0].item, item_names[1])
        assert_equal(items[0].amount, 49)
        assert_equal(items[1].item, item_names[2])
        assert_equal(items[1].amount, 16)
        assert_equal(items[2].item, item_names[3])
        assert_equal(items[2].amount, 90)
        assert_equal(items[3].item, item_names[4])
        assert_equal(items[3].amount, 2)
        assert_equal(items.num_items, 4)

        items.add(item_names[5], 12)

        assert_equal(items[0].item, item_names[1])
        assert_equal(items[0].amount, 49)
        assert_equal(items[1].item, item_names[2])
        assert_equal(items[1].amount, 16)
        assert_equal(items[2].item, item_names[3])
        assert_equal(items[2].amount, 90)
        assert_equal(items[3].item, item_names[4])
        assert_equal(items[3].amount, 2)
        assert_equal(items[4].item, item_names[5])
        assert_equal(items[4].amount, 12)
        assert_equal(items.num_items, 5)

        items.remove(item_names[2], 16)

        assert_equal(items[0].item, item_names[1])
        assert_equal(items[0].amount, 49)
        assert_equal(items[1].item, item_names[3])
        assert_equal(items[1].amount, 90)
        assert_equal(items[2].item, item_names[4])
        assert_equal(items[2].amount, 2)
        assert_equal(items[3].item, item_names[5])
        assert_equal(items[3].amount, 12)
        assert_equal(items.num_items, 4)

        items.add(item_names[6], 65)

        assert_equal(items[0].item, item_names[1])
        assert_equal(items[0].amount, 49)
        assert_equal(items[1].item, item_names[3])
        assert_equal(items[1].amount, 90)
        assert_equal(items[2].item, item_names[4])
        assert_equal(items[2].amount, 2)
        assert_equal(items[3].item, item_names[5])
        assert_equal(items[3].amount, 12)
        assert_equal(items[4].item, item_names[6])
        assert_equal(items[4].amount, 65)
        assert_equal(items.num_items, 5)

        items.add(item_names[7], 6)

        assert_equal(items[0].item, item_names[1])
        assert_equal(items[0].amount, 49)
        assert_equal(items[1].item, item_names[3])
        assert_equal(items[1].amount, 90)
        assert_equal(items[2].item, item_names[4])
        assert_equal(items[2].amount, 2)
        assert_equal(items[3].item, item_names[5])
        assert_equal(items[3].amount, 12)
        assert_equal(items[4].item, item_names[6])
        assert_equal(items[4].amount, 65)
        assert_equal(items[5].item, item_names[7])
        assert_equal(items[5].amount, 6)
        assert_equal(items.num_items, 6)

        items.remove(item_names[4], 2)
        items.remove(item_names[1], 49)
        items.remove(item_names[7], 6)
        items.remove(item_names[5], 12)
        items.remove(item_names[3], 90)
        items.remove(item_names[6], 65)

        assert_equal(items.num_items, 0)
    end
end
