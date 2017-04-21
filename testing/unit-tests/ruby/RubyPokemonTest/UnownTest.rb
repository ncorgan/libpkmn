#!/usr/bin/ruby
#
# Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

require_relative "PokemonTest"

require "pathname"

class UnownTest < PokemonTest
    def _test_gen2_unown(game)
        unown_forms = PKMN::Database::PokemonEntry.new("Unown", game, "").forms
        assert_equal(26, unown_forms.length)
        unown = 0 # So the variable exists

        unown_forms.each do |form|
            unown = PKMN::Pokemon.new("Unown", game, form, 5)
            assert_equal(form, unown.form)

            # Make sure IVs are properly set
            form_from_IVs = PKMN::Calculations::gen2_unown_form(
                                unown.IVs["Attack"],
                                unown.IVs["Defense"],
                                unown.IVs["Speed"],
                                unown.IVs["Special"]
                            )
            assert_equal(form_from_IVs, unown.form)
            assert(Pathname.new(unown.icon_filepath).exist?)
            assert(Pathname.new(unown.sprite_filepath).exist?)
        end

        # Make sure setting IVs properly changes the form.
        unown.set_IV("Attack", 10)
        unown.set_IV("Defense", 9)
        unown.set_IV("Speed", 1)
        unown.set_IV("Special", 14)
        assert_equal("G", unown.form)
    end

    def _test_gen3_unown(game)
        unown_forms = PKMN::Database::PokemonEntry.new("Unown", game, "").forms
        assert_equal(28, unown_forms.length)
        unown = 0 # So the variable exists

        unown_forms.each do |form|
            unown = PKMN::Pokemon.new("Unown", game, form, 5)
            assert_equal(form, unown.form)
            assert(Pathname.new(unown.icon_filepath).exist?)
            assert(Pathname.new(unown.sprite_filepath).exist?)
        end

        # Make sure setting the personality properly sets the form.
        unown.personality = 0x4C07DE71
        assert_equal("B", unown.form)

        # Make sure setting the form properly sets the personality.
        unown_forms.each do |form|
            unown.form = form
            assert_equal(form, unown.form)
            form_from_personality = PKMN::Calculations::gen3_unown_form(unown.personality)
            assert_equal(unown.form, form_from_personality)
        end
    end

    GEN2_PARAMS = ["Gold", "Silver", "Crystal"]
    GEN3_PARAMS = ["Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen"]

    GEN2_PARAMS.each do |game|
        define_method("test_gen2_unown_#{game}") do
            _test_gen2_unown(game)
        end
    end
    GEN3_PARAMS.each do |game|
        define_method("test_gen3_unown_#{game}") do
            _test_gen3_unown(game)
        end
    end
end
