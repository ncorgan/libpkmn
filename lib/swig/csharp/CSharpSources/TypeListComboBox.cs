/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN {

    public class TypeListComboBox: System.Windows.Forms.ComboBox {
        public TypeListComboBox(string game) {
            StringList types = Database.Lists.TypeList(game);
            foreach(string type in types) {
                this.Items.Add(type);
            }
        }
    }

}
