/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

package nc.PKMN;

import javax.swing.JComboBox;

/**
 * A JComboBox filled with a list of abilities in a given generation.
 */
public class AbilityListComboBox extends JComboBox<String> {
    public AbilityListComboBox(int generation) {
        StringVector abilityList = Database.getAbilityList(generation);
        for(long i = 0; i < abilityList.size(); i++) {
            this.addItem(abilityList.get((int)i));
        }
        this.setEditable(false);
    }
}
