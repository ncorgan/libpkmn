/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.JComboBox;

/**
 * A JComboBox filled with a list of moves available in the given game.
 */
public class MoveListComboBox extends JComboBox<String> {
    public MoveListComboBox(String game) {
        StringVector moveList = Database.getMoveList(game);
        for(long i = 0; i < moveList.size(); i++) {
            this.addItem(moveList.get((int)i));
        }
        this.setEditable(false);
    }
}
