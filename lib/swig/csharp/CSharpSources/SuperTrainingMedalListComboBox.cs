/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN.WinForms {

    public class SuperTrainingMedalListComboBox: System.Windows.Forms.ComboBox {
        public SuperTrainingMedalListComboBox() {
            StringList superTrainingMedals = Database.Lists.SuperTrainingMedalList();
            foreach(string superTrainingMedal in superTrainingMedals) {
                this.Items.Add(superTrainingMedal);
            }
        }
    }

}
