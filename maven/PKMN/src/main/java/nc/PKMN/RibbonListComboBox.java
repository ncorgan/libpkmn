/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.JComboBox;

/**
 * A JComboBox filled with a list of ribbons available in the given generation.
 */
public class RibbonListComboBox extends JComboBox<String> {
    public RibbonListComboBox(int generation) {
        StringVector ribbonList = Database.getRibbonList(generation);
        for(long i = 0; i < ribbonList.size(); i++) {
            this.addItem(ribbonList.get((int)i));
        }
        this.setEditable(false);
    }
}
