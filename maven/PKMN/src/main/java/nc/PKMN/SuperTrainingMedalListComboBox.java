/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.JComboBox;

/**
 * A JComboBox filled with a list of Super Training Medals.
 */
public class SuperTrainingMedalListComboBox extends JComboBox<String> {
    public SuperTrainingMedalListComboBox() {
        StringVector superTrainingMedalList = Database.getSuperTrainingMedalList();
        for(long i = 0; i < superTrainingMedalList.size(); i++) {
            this.addItem(superTrainingMedalList.get((int)i));
        }
        this.setEditable(false);
    }
}
