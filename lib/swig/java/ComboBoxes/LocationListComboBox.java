/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.JComboBox;

/**
 * A JComboBox filled with a list of locations in the given game (or generation).
 */
public class LocationListComboBox extends JComboBox<String> {
    public LocationListComboBox(String game, boolean wholeGeneration) {
        StringVector locationList = Database.getLocationList(game, wholeGeneration);
        for(long i = 0; i < locationList.size(); i++) {
            this.addItem(locationList.get((int)i));
        }
        this.setEditable(false);
    }
}
