/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN
{
    public partial class GUI
    {
        public class AbilityListComboBox: System.Windows.Forms.ComboBox
        {
            public AbilityListComboBox(int generation)
            {
                StringList abilities = Database.Lists.AbilityList(generation);
                foreach(string ability in abilities)
                {
                    this.Items.Add(ability);
                }
            }
        }

        public class GameListComboBox: System.Windows.Forms.ComboBox
        {
            public GameListComboBox(int generation, bool includePrevious)
            {
                StringList games = Database.Lists.GameList(generation, includePrevious);
                foreach(string game in games)
                {
                    this.Items.Add(game);
                }
            }
        }

        public class ItemListComboBox: System.Windows.Forms.ComboBox
        {
            public ItemListComboBox(string game)
            {
                StringList items = Database.Lists.ItemList(game);
                foreach(string item in items)
                {
                    this.Items.Add(item);
                }
            }
        }

        public class LocationListComboBox: System.Windows.Forms.ComboBox
        {
            public LocationListComboBox(string game, bool wholeGeneration)
            {
                StringList locations = Database.Lists.LocationList(game, wholeGeneration);
                foreach(string location in locations)
                {
                    this.Items.Add(location);
                }
            }
        }

        public class MoveListComboBox: System.Windows.Forms.ComboBox
        {
            public MoveListComboBox(string game)
            {
                StringList moves = Database.Lists.MoveList(game);
                foreach(string move in moves)
                {
                    this.Items.Add(move);
                }
            }
        }

        public class NatureListComboBox: System.Windows.Forms.ComboBox
        {
            public NatureListComboBox()
            {
                StringList natures = Database.Lists.NatureList();
                foreach(string nature in natures)
                {
                    this.Items.Add(nature);
                }
            }
        }

        public class PokemonListComboBox: System.Windows.Forms.ComboBox
        {
            public PokemonListComboBox(int generation, bool wholeGeneration)
            {
                StringList pokemon = Database.Lists.PokemonList(generation, wholeGeneration);
                foreach(string pkmn in pokemon)
                {
                    this.Items.Add(pkmn);
                }
            }
        }

        public class RegionListComboBox: System.Windows.Forms.ComboBox
        {
            public RegionListComboBox()
            {
                StringList regions = Database.Lists.RegionList();
                foreach(string region in regions)
                {
                    this.Items.Add(region);
                }
            }
        }

        public class RibbonListComboBox: System.Windows.Forms.ComboBox
        {
            public RibbonListComboBox(int generation)
            {
                StringList ribbons = Database.Lists.RibbonList(generation);
                foreach(string ribbon in ribbons)
                {
                    this.Items.Add(ribbon);
                }
            }
        }

        public class SuperTrainingMedalListComboBox: System.Windows.Forms.ComboBox
        {
            public SuperTrainingMedalListComboBox()
            {
                StringList superTrainingMedals = Database.Lists.SuperTrainingMedalList();
                foreach(string superTrainingMedal in superTrainingMedals)
                {
                    this.Items.Add(superTrainingMedal);
                }
            }
        }

        public class TypeListComboBox: System.Windows.Forms.ComboBox
        {
            public TypeListComboBox(string game)
            {
                StringList types = Database.Lists.TypeList(game);
                foreach(string type in types)
                {
                    this.Items.Add(type);
                }
            }
        }
    }
}
