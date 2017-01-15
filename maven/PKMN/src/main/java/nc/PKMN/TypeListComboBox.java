/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.JComboBox;

/**
 * A JComboBox filled with a list of types available in a given game.
 */
public class TypeListComboBox extends JComboBox<String> {
    public TypeListComboBox(String game) {
        StringVector typeList = Database.getTypeList(game);
        for(long i = 0; i < typeList.size(); i++) {
            this.addItem(typeList.get((int)i));
        }
        this.setEditable(false);
    }
}
