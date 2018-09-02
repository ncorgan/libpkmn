/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
using System;
using System.Drawing;
using System.IO;

namespace PKMN
{
    public partial class GUI
    {
        public static Image GenerateSpindaSprite(
            int generation,
            uint personality,
            bool shiny
        )
        {
            string tmpDir = Paths.GetTmpDir();
            string filename = String.Format(
                                  "spinda_{0}_{1}.png",
                                  System.Diagnostics.Process.GetCurrentProcess().Id,
                                  new Random().Next()
                              );
            string filepath = Path.GetFullPath(Path.Combine(tmpDir, filename));

            GenerateSpindaSpriteAtFilepath(
                generation,
                personality,
                shiny,
                filepath
            );

            Image ret = Image.FromFile(filepath);
            File.Delete(filepath);
            return ret;
        }

        public class AbilityListComboBox: System.Windows.Forms.ComboBox
        {
            public AbilityListComboBox(int generation)
            {
                StringList abilities = Database.Lists.AbilityNameList(generation);
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
                StringList games = Database.Lists.GameNameList(generation, includePrevious);
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
                Game gameEnum = PKMN.StringToGame(game);

                StringList items = Database.Lists.ItemNameList(gameEnum);
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
                Game gameEnum = PKMN.StringToGame(game);

                StringList locations = Database.Lists.LocationNameList(gameEnum, wholeGeneration);
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
                Game gameEnum = PKMN.StringToGame(game);

                StringList moves = Database.Lists.MoveNameList(gameEnum);
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
                StringList natures = Database.Lists.NatureNameList();
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
                StringList pokemon = Database.Lists.PokemonNameList(generation, wholeGeneration);
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
                StringList regions = Database.Lists.RegionNameList();
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
                StringList ribbons = Database.Lists.RibbonNameList(generation);
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
                StringList superTrainingMedals = Database.Lists.SuperTrainingMedalNameList();
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
                Game gameEnum = PKMN.StringToGame(game);

                StringList types = Database.Lists.TypeNameList(gameEnum);
                foreach(string type in types)
                {
                    this.Items.Add(type);
                }
            }
        }
    }
}
