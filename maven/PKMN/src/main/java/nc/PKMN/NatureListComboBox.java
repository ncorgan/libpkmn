/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.JComboBox;

/**
 * A JComboBox filled with a list of natures.
 */
public class NatureListComboBox extends JComboBox<String> {
    public NatureListComboBox() {
        StringVector natureList = Database.getNatureList();
        for(long i = 0; i < natureList.size(); i++) {
            this.addItem(natureList.get((int)i));
        }
        this.setEditable(false);
    }
}
