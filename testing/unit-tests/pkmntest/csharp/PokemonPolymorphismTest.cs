/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest
{

public class PokemonPolymorphismTest
{
    static private PKMN.Species species = PKMN.Species.BULBASAUR;

    public static void SettingPokemonInBoxTest(PKMN.Game game)
    {
        PokemonSubclass outsidePokemon = new PokemonSubclass(species, game);

        PKMN.PokemonBox pokemonBox = new PKMN.PokemonBox(game);
        pokemonBox[0] = outsidePokemon;
    }

    public static void SettingPokemonInPartyTest(PKMN.Game game)
    {
        PokemonSubclass outsidePokemon = new PokemonSubclass(species, game);

        PKMN.PokemonParty pokemonParty = new PKMN.PokemonParty(game);
        pokemonParty[0] = outsidePokemon;
    }

    public static void SettingPokemonInDaycareTest(PKMN.Game game)
    {
        PokemonSubclass outsidePokemon = new PokemonSubclass(species, game);

        PKMN.Daycare daycare = new PKMN.Daycare(game);

        daycare.LevelupPokemon[0] = outsidePokemon;
        if(daycare.CanBreedPokemon)
        {
            daycare.BreedingPokemon[0] = outsidePokemon;
        }
    }
}

}
