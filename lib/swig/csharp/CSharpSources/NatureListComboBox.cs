/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN.WinForms {

    public class NatureListComboBox: System.Windows.Forms.ComboBox {
        public NatureListComboBox() {
            StringList natures = Database.Lists.NatureList();
            foreach(string nature in natures) {
                this.Items.Add(nature);
            }
        }
    }

}
