/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.JComboBox;

/**
 * A JComboBox filled with a list of items in the given game.
 */
public class ItemListComboBox extends JComboBox<String> {
    public ItemListComboBox(String game) {
        StringVector itemList = Database.getItemList(game);
        for(long i = 0; i < itemList.size(); i++) {
            this.addItem(itemList.get((int)i));
        }
        this.setEditable(false);
    }
}
