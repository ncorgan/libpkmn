/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN {

    public class AbilityListComboBox: System.Windows.Forms.ComboBox {
        public AbilityListComboBox(int generation) {
            StringList abilities = Database.GetAbilityList(generation);
            foreach(string ability in abilities) {
                this.Items.Add(ability);
            }
        }
    }

}
