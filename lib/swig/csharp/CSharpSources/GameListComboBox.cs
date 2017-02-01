/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN {

    public class GameListComboBox: System.Windows.Forms.ComboBox {
        public GameListComboBox(int generation, bool includePrevious) {
            StringList games = Database.Lists.GameList(generation, includePrevious);
            foreach(string game in games) {
                this.Items.Add(game);
            }
        }
    }

}
