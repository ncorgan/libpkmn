/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN.WinForms {

    public class ItemListComboBox: System.Windows.Forms.ComboBox {
        public ItemListComboBox(string game) {
            StringList items = Database.Lists.ItemList(game);
            foreach(string item in items) {
                this.Items.Add(item);
            }
        }
    }

}
