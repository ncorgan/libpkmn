/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN.WinForms {

    public class MoveListComboBox: System.Windows.Forms.ComboBox {
        public MoveListComboBox(string game) {
            StringList moves = Database.Lists.MoveList(game);
            foreach(string move in moves) {
                this.Items.Add(move);
            }
        }
    }

}
