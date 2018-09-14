/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/pokemon_subclass.hpp>

#include <pkmn/enums/enum_to_string.hpp>

namespace pkmntest
{
    pokemon_subclass::pokemon_subclass(
        pkmn::e_species species,
        pkmn::e_game game
    ): pkmn::pokemon(),
       _database_entry(species, game, ""),
       _species(species),
       _game(game),
       _condition(pkmn::e_condition::NONE),
       _gender(pkmn::e_gender::MALE),
       _held_item(pkmn::e_item::NONE),
       _nature(pkmn::e_nature::HARDY),
       _original_trainer_gender(pkmn::e_gender::MALE),
       _language(pkmn::e_language::ENGLISH),
       _ability(pkmn::e_ability::NONE),
       _ball(pkmn::e_ball::POKE_BALL),
       _original_game(pkmn::e_game::NONE),
       _is_egg(false),
       _is_shiny(false),
       _pokerus_duration(0),
       _current_trainer_friendship(0),
       _level_met(0),
       _personality(0),
       _experience(0),
       _level(5),
       _current_hp(0),
       _nickname(pkmn::species_to_string(species)),
       _original_trainer_name("Test"),
       _location_met(),
       _location_met_as_egg(),
       _original_trainer_id({0}),
       _markings(),
       _ribbons(),
       _contest_stats(),
       _EVs(),
       _IVs(),
       _stats(),
       _moves()
    {
        // This calls an implementation-agnostic function that populates
        // values with suitable defaults to match in-game policy.
        initialize_default_values(species, game);
    }

    pkmn::pokemon::sptr pokemon_subclass::clone() const
    {
        return std::make_shared<pokemon_subclass>(*this);
    }

    pkmn::pokemon::sptr pokemon_subclass::to_game(pkmn::e_game game)
    {
        return std::make_shared<pokemon_subclass>(_species, game);
    }

    void pokemon_subclass::export_to_file(const std::string& filepath)
    {
        (void)filepath;
    }

    pkmn::e_species pokemon_subclass::get_species() const
    {
        return _species;
    }

    pkmn::e_game pokemon_subclass::get_game() const
    {
        return _game;
    }

    std::string pokemon_subclass::get_form() const
    {
        return _database_entry.get_form();
    }

    void pokemon_subclass::set_form(const std::string& form)
    {
        _database_entry.set_form(form);
    }

    bool pokemon_subclass::is_egg() const
    {
        return _is_egg;
    }

    void pokemon_subclass::set_is_egg(bool is_egg)
    {
        _is_egg = is_egg;
    }

    const pkmn::database::pokemon_entry& pokemon_subclass::get_database_entry() const
    {
        return _database_entry;
    }

    pkmn::e_condition pokemon_subclass::get_condition() const
    {
        return _condition;
    }

    void pokemon_subclass::set_condition(pkmn::e_condition condition)
    {
        _condition = condition;
    }

    std::string pokemon_subclass::get_nickname() const
    {
        return _nickname;
    }

    void pokemon_subclass::set_nickname(const std::string& nickname)
    {
        _nickname = nickname;
    }

    pkmn::e_gender pokemon_subclass::get_gender() const
    {
        return _gender;
    }

    void pokemon_subclass::set_gender(pkmn::e_gender gender)
    {
        _gender = gender;
    }

    bool pokemon_subclass::is_shiny() const
    {
        return _is_shiny;
    }

    void pokemon_subclass::set_shininess(bool is_shiny)
    {
        _is_shiny = is_shiny;
    }

    pkmn::e_item pokemon_subclass::get_held_item() const
    {
        return _held_item;
    }

    void pokemon_subclass::set_held_item(pkmn::e_item held_item)
    {
        _held_item = held_item;
    }

    pkmn::e_nature pokemon_subclass::get_nature() const
    {
        return _nature;
    }

    void pokemon_subclass::set_nature(pkmn::e_nature nature)
    {
        _nature = nature;
    }

    int pokemon_subclass::get_pokerus_duration() const
    {
        return _pokerus_duration;
    }

    void pokemon_subclass::set_pokerus_duration(int pokerus_duration)
    {
        _pokerus_duration = pokerus_duration;
    }

    std::string pokemon_subclass::get_original_trainer_name() const
    {
        return _original_trainer_name;
    }

    void pokemon_subclass::set_original_trainer_name(const std::string& original_trainer_name)
    {
        _original_trainer_name = original_trainer_name;
    }

    uint16_t pokemon_subclass::get_original_trainer_public_id() const
    {
        return _original_trainer_id.public_id;
    }

    uint16_t pokemon_subclass::get_original_trainer_secret_id() const
    {
        return _original_trainer_id.secret_id;
    }

    uint32_t pokemon_subclass::get_original_trainer_id() const
    {
        return _original_trainer_id.id;
    }

    void pokemon_subclass::set_original_trainer_public_id(
        uint16_t original_trainer_public_id
    )
    {
        _original_trainer_id.public_id = original_trainer_public_id;
    }

    void pokemon_subclass::set_original_trainer_secret_id(
        uint16_t original_trainer_secret_id
    )
    {
        _original_trainer_id.secret_id = original_trainer_secret_id;
    }

    void pokemon_subclass::set_original_trainer_id(
        uint32_t original_trainer_id
    )
    {
        _original_trainer_id.id = original_trainer_id;
    }

    pkmn::e_gender pokemon_subclass::get_original_trainer_gender() const
    {
        return _original_trainer_gender;
    }

    void pokemon_subclass::set_original_trainer_gender(
        pkmn::e_gender original_trainer_gender
    )
    {
        _original_trainer_gender = original_trainer_gender;
    }

    pkmn::e_language pokemon_subclass::get_language() const
    {
        return _language;
    }

    void pokemon_subclass::set_language(pkmn::e_language language)
    {
        _language = language;
    }

    int pokemon_subclass::get_current_trainer_friendship() const
    {
        return _current_trainer_friendship;
    }

    void pokemon_subclass::set_current_trainer_friendship(int current_trainer_friendship)
    {
        _current_trainer_friendship = current_trainer_friendship;
    }

    pkmn::e_ability pokemon_subclass::get_ability() const
    {
        return _ability;
    }

    void pokemon_subclass::set_ability(pkmn::e_ability ability)
    {
        _ability = ability;
    }

    pkmn::e_ball pokemon_subclass::get_ball() const
    {
        return _ball;
    }

    void pokemon_subclass::set_ball(pkmn::e_ball ball)
    {
        _ball = ball;
    }

    int pokemon_subclass::get_level_met() const
    {
        return _level_met;
    }

    void pokemon_subclass::set_level_met(int level_met)
    {
        _level_met = level_met;
    }

    std::string pokemon_subclass::get_location_met(bool as_egg) const
    {
        return as_egg ? _location_met_as_egg : _location_met;
    }

    void pokemon_subclass::set_location_met(
        const std::string& location,
        bool as_egg
    )
    {
        if(as_egg)
        {
            _location_met_as_egg = location;
        }
        else
        {
            _location_met = location;
        }
    }

    pkmn::e_game pokemon_subclass::get_original_game() const
    {
        return _original_game;
    }

    void pokemon_subclass::set_original_game(pkmn::e_game original_game)
    {
        _original_game = original_game;
    }

    uint32_t pokemon_subclass::get_personality() const
    {
        return _personality;
    }

    void pokemon_subclass::set_personality(uint32_t personality)
    {
        _personality = personality;
    }

    int pokemon_subclass::get_experience() const
    {
        return _experience;
    }

    void pokemon_subclass::set_experience(int experience)
    {
        _experience = experience;
    }

    int pokemon_subclass::get_level() const
    {
        return _level;
    }

    void pokemon_subclass::set_level(int level)
    {
        _level = level;
    }

    const std::map<pkmn::e_marking, bool>& pokemon_subclass::get_markings() const
    {
        return _markings;
    }

    void pokemon_subclass::set_marking(
        pkmn::e_marking marking,
        bool value
    )
    {
        _markings[marking] = value;
    }

    const std::map<std::string, bool>& pokemon_subclass::get_ribbons() const
    {
        return _ribbons;
    }

    void pokemon_subclass::set_ribbon(
        const std::string& ribbon,
        bool value
    )
    {
        _ribbons[ribbon] = value;
    }

    const std::map<pkmn::e_contest_stat, int>& pokemon_subclass::get_contest_stats() const
    {
        return _contest_stats;
    }

    void pokemon_subclass::set_contest_stat(
        pkmn::e_contest_stat stat,
        int value
    )
    {
        _contest_stats[stat] = value;
    }

    const pkmn::move_slots_t& pokemon_subclass::get_moves() const
    {
        return _moves;
    }

    void pokemon_subclass::set_move(
        pkmn::e_move move,
        int index
    )
    {
        _moves[index].move = move;
    }

    void pokemon_subclass::set_move_pp(
        int index,
        int pp
    )
    {
        _moves[index].pp = pp;
    }

    const std::map<pkmn::e_stat, int>& pokemon_subclass::get_EVs() const
    {
        return _EVs;
    }

    void pokemon_subclass::set_EV(
        pkmn::e_stat stat,
        int value
    )
    {
        _EVs[stat] = value;
    }

    const std::map<pkmn::e_stat, int>& pokemon_subclass::get_IVs() const
    {
        return _IVs;
    }

    void pokemon_subclass::set_IV(
        pkmn::e_stat stat,
        int value
    )
    {
        _IVs[stat] = value;
    }

    int pokemon_subclass::get_current_hp() const
    {
        return _current_hp;
    }

    void pokemon_subclass::set_current_hp(int current_hp)
    {
        _current_hp = current_hp;
    }

    const std::map<pkmn::e_stat, int>& pokemon_subclass::get_stats() const
    {
        return _stats;
    }

    std::string pokemon_subclass::get_icon_filepath() const
    {
        return "";
    }

    std::string pokemon_subclass::get_sprite_filepath() const
    {
        return "";
    }

    const void* pokemon_subclass::get_native() const
    {
        return nullptr;
    }
}
