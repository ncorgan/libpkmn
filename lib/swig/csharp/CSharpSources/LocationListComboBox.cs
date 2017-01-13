/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN {

    public class LocationListComboBox: System.Windows.Forms.ComboBox {
        public LocationListComboBox(string game, bool wholeGeneration) {
            StringList locations = Database.GetLocationList(game, wholeGeneration);
            foreach(string location in locations) {
                this.Items.Add(location);
            }
        }
    }

}
