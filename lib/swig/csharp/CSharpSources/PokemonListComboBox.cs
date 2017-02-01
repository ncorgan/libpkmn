/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN {

    public class PokemonListComboBox: System.Windows.Forms.ComboBox {
        public PokemonListComboBox(int generation, bool wholeGeneration) {
            StringList pokemon = Database.Lists.PokemonList(generation, wholeGeneration);
            foreach(string pkmn in pokemon) {
                this.Items.Add(pkmn);
            }
        }
    }

}
