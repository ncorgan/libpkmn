/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.JComboBox;

/**
 * A JComboBox filled with a list of games in (or up to) a given generation.
 */
public class GameListComboBox extends JComboBox<String> {
    public GameListComboBox(int generation, boolean includePrevious) {
        StringVector gameList = Database.getGameList(generation, includePrevious);
        for(long i = 0; i < gameList.size(); i++) {
            this.addItem(gameList.get((int)i));
        }
        this.setEditable(false);
    }
}
