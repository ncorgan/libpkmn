/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN.WinForms {

    public class RegionListComboBox: System.Windows.Forms.ComboBox {
        public RegionListComboBox() {
            StringList regions = Database.Lists.RegionList();
            foreach(string region in regions) {
                this.Items.Add(region);
            }
        }
    }

}
