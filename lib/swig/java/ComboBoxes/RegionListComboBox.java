/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.JComboBox;

/**
 * A JComboBox filled with a list of regions in the main series and Gamecube games.
 */
public class RegionListComboBox extends JComboBox<String> {
    public RegionListComboBox() {
        StringVector regionList = Database.getRegionList();
        for(long i = 0; i < regionList.size(); i++) {
            this.addItem(regionList.get((int)i));
        }
        this.setEditable(false);
    }
}
