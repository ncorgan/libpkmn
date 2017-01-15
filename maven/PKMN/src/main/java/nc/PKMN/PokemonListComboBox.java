/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.JComboBox;

/**
 * A JComboBox filled with a list of Pokémon in (and up to) a given generation.
 */
public class PokemonListComboBox extends JComboBox<String> {
    public PokemonListComboBox(int generation, boolean includePrevious) {
        StringVector pokemonList = Database.getPokemonList(generation, includePrevious);
        for(long i = 0; i < pokemonList.size(); i++) {
            this.addItem(pokemonList.get((int)i));
        }
        this.setEditable(false);
    }
}
