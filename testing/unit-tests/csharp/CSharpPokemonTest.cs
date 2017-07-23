/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpPokemonTest {

    /*
     * Invalid Pokémon test
     */
    [TestCase("Red")]
    [TestCase("Blue")]
    [TestCase("Yellow")]
    [TestCase("Gold")]
    [TestCase("Silver")]
    [TestCase("Crystal")]
    [TestCase("Ruby")]
    [TestCase("Sapphire")]
    [TestCase("Emerald")]
    [TestCase("FireRed")]
    [TestCase("LeafGreen")]
    [TestCase("Colosseum")]
    [TestCase("XD")]
    public void InvalidPokemonTest(
        string version
    )
    {
        PKMNTest.PokemonTestsCommon.InvalidPokemonTest(version);
    }

    /*
     * Gender test
     */
    [TestCase("Gold")]
    [TestCase("Silver")]
    [TestCase("Crystal")]
    [TestCase("Ruby")]
    [TestCase("Sapphire")]
    [TestCase("Emerald")]
    [TestCase("FireRed")]
    [TestCase("LeafGreen")]
    [TestCase("Colosseum")]
    [TestCase("XD")]
    public void GenderTest(
        string version
    )
    {
        PKMNTest.PokemonTestsCommon.GenderTest(version);
    }

    /*
     * Generation I friendship test
     */
    [TestCase("Red")]
    [TestCase("Blue")]
    [TestCase("Yellow")]
    public void Gen1FriendshipTest(
        string version
    )
    {
        PKMNTest.Gen1PokemonTests.FriendshipTest(version);
    }

    /*
     * Generation I Pokémon test
     */
    [TestCase("Red")]
    [TestCase("Blue")]
    [TestCase("Yellow")]
    public void Gen1PokemonTest(
        string version
    )
    {
        PKMNTest.Gen1PokemonTests.PokemonTest(version);
    }

    /*
     * Generation II Unown test
     */
    [TestCase("Gold")]
    [TestCase("Silver")]
    [TestCase("Crystal")]
    public void Gen2UnownFormTest(
        string version
    )
    {
        PKMNTest.Gen2PokemonTests.UnownFormTest(version);
    }

    /*
     * Generation II Pokémon test
     */
    [TestCase("Gold")]
    [TestCase("Silver")]
    [TestCase("Crystal")]
    public void Gen2PokemonTest(
        string version
    )
    {
        PKMNTest.Gen2PokemonTests.PokemonTest(version);
    }

    /*
     * Generation III Unown test
     */
    [TestCase("Ruby")]
    [TestCase("Sapphire")]
    [TestCase("Emerald")]
    [TestCase("FireRed")]
    [TestCase("LeafGreen")]
    [TestCase("Colosseum")]
    [TestCase("XD")]
    public void Gen3UnownFormTest(
        string version
    )
    {
        PKMNTest.Gen3PokemonTests.UnownFormTest(version);
    }

    /*
     * Generation III Pokémon test
     */
    [TestCase("Ruby")]
    [TestCase("Sapphire")]
    [TestCase("Emerald")]
    [TestCase("FireRed")]
    [TestCase("LeafGreen")]
    [TestCase("Colosseum")]
    [TestCase("XD")]
    public void Gen3PokemonTest(
        string version
    )
    {
        PKMNTest.Gen3PokemonTests.PokemonTest(version);
    }

    /*
     * I/O
     */
    [Test]
    public void _3GPKMTest() {
        PKMNTest.PokemonIOTests.Test3GPKM();
    }

    /*
     * Conversions
     */
    // Generation I -> Generation I
    [TestCase("Bulbasaur", "", "Red", "Yellow")]

    // Generation I -> Generation II
    [TestCase("Squirtle", "", "Blue", "Gold")]

    // Generation II -> Generation II
    [TestCase("Cyndaquil", "", "Gold", "Crystal")]
    [TestCase("Totodile", "", "Crystal", "Gold")]

    // Generation II -> Generation I
    [TestCase("Charmander", "", "Silver", "Blue")]

    // GBA -> GBA
    [TestCase("Torchic", "", "Ruby", "Sapphire")]
    [TestCase("Mudkip", "", "Ruby", "Emerald")]
    [TestCase("Treecko", "", "Ruby", "FireRed")]
    [TestCase("Torchic", "", "Emerald", "Sapphire")]
    [TestCase("Mudkip", "", "Emerald", "Emerald")]
    [TestCase("Treecko", "", "Emerald", "FireRed")]
    [TestCase("Charmander", "", "FireRed", "Sapphire")]
    [TestCase("Squirtle", "", "FireRed", "Emerald")]
    [TestCase("Bulbasaur", "", "FireRed", "FireRed")]

    // GBA -> GCN
    [TestCase("Eevee", "", "Ruby", "Colosseum")]
    [TestCase("Espeon", "", "Emerald", "Colosseum")]
    [TestCase("Umbreon", "", "FireRed", "Colosseum")]
    [TestCase("Eevee", "", "Ruby", "XD")]
    [TestCase("Espeon", "", "Emerald", "XD")]
    [TestCase("Umbreon", "", "FireRed", "XD")]

    // GCN -> GBA
    [TestCase("Eevee", "", "Colosseum", "Sapphire")]
    [TestCase("Espeon", "", "Colosseum", "Emerald")]
    [TestCase("Umbreon", "", "Colosseum", "LeafGreen")]
    [TestCase("Eevee", "", "XD", "Sapphire")]
    [TestCase("Espeon", "", "XD", "Emerald")]
    [TestCase("Umbreon", "", "XD", "LeafGreen")]

    // GCN -> GCN
    [TestCase("Vaporeon", "", "Colosseum", "XD")]
    [TestCase("Vaporeon", "", "XD", "Colosseum")]
    public void PokemonConversionsTest(
        string species,
        string form,
        string originGame,
        string destGame
    )
    {
        PKMNTest.PokemonConversionsTest.ConversionsTest(
            species,
            form,
            originGame,
            destGame
        );
    }
}
