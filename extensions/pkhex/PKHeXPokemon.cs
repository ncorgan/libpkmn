/*
 * Copyright (C) 2018 Nicholas Corgan
 *
 * This file is part of PKMN.PKHeX.
 *
 * PKMN.PKHeX is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PKMN.PKHeX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PKMN.PKHeX.  If not, see <https://www.gnu.org/licenses/>.
 */

namespace PKMN.Extensions
{
    public class PKHeXPokemon: Pokemon
    {
        protected static PKHeX.Core.GameStrings _gameStrings = PKHeX.Core.GameInfo.GetStrings("en");

        // Member variables
        private PKHeX.Core.PKM _pkm;

        public PKHeXPokemon(
            PKHeX.Core.PKM pkm
        ): base()
        {
            _pkm = pkm;
        }

        // TODO: ToGame(), behavior is generation-dependent

        override public Pokemon Clone()
        {
            return new PKHeXPokemon(_pkm.Clone());
        }

        // Utility functions

        // See: https://github.com/ncorgan/pksav/blob/master/lib/common/stats.c
        // TODO: upstream this functionality
        protected void SetGBIV(Stat stat, int value, ref ushort rawIV)
        {
            ushort[] ivMasks =
            {
                0xFFFF, // None
                0x1111, // HP
                0xF000, // Attack
                0x0F00, // Defense
                0x00F0, // Speed
                0x000F, // Special
            };
            ushort[] ivOffsets =
            {
                0,  // None
                0,  // HP
                12, // Attack
                8,  // Defense
                4,  // Speed
                0,  // Special
            };

            // For some reason, ~ casts to int.
            rawIV &= (ushort)(~ivMasks[(int)stat]);

            // This is a reference to the abstraction dict.
            StatDict IVs = GetIVsMapInternal();
            IVs[stat] = value;

            // The HP IV is formed taking a single bit on other stats' bitfields,
            // so any change affects at least one other IV.
            if(stat == Stat.HP)
            {
                rawIV = (ushort)((rawIV & ~ivMasks[(int)Stat.HP])
                      | ((value & 0x08) << 9)
                      | ((value & 0x04) << 6)
                      | ((value & 0x02) << 3)
                      |  (value & 0x01));

                // Given that we know what stats there are, don't bother going
                // into the C++ layer to get the map keys.
                Stat[] ivStats =
                {
                    Stat.ATTACK,
                    Stat.DEFENSE,
                    Stat.SPEED,
                    Stat.SPECIAL
                };

                foreach(Stat ivStat in ivStats)
                {
                    IVs[ivStat] = (rawIV & ivMasks[(int)ivStat]) >> ivOffsets[(int)ivStat];
                }
            }
            else
            {
                rawIV |= (ushort)(value << ivMasks[(int)stat]);

                IVs[Stat.HP] = (IVs[Stat.SPECIAL] & 0x01)
                             | ((IVs[Stat.SPEED] & 0x01) << 1)
                             | ((IVs[Stat.DEFENSE] & 0x01) << 2)
                             | ((IVs[Stat.ATTACK] & 0x01) << 3);
            }
        }

        // Override protected PKMN.Pokemon functions. These will be accessed via
        // PKMN.Pokemon's properties.

        override protected bool GetIsEgg()
        {
            return _pkm.IsEgg;
        }

        override protected void SetIsEgg(bool isEgg)
        {
            _pkm.IsEgg = isEgg;
        }

        override protected string GetNickname()
        {
            return _pkm.Nickname;
        }

        override protected void SetNickname(string nickname)
        {
            _pkm.Nickname = nickname;
        }

        override protected Gender GetGender()
        {
            // TODO: likely not how it's done
            return (Gender)_pkm.Gender;
        }

        override protected void SetGender(Gender gender)
        {
            // TODO: likely not how it's done
            _pkm.Gender = (int)gender;
        }

        // TODO: shininess, is generation-dependent

        // TODO: bounds check
        override protected Item GetHeldItem()
        {
            return PKMN.StringToItem(
                       _gameStrings.itemlist[_pkm.HeldItem]
                   );
        }

        // TODO: check for mismatches, invalid cases
        override protected void SetHeldItem(Item heldItem)
        {
            _pkm.HeldItem = System.Array.IndexOf(
                                _gameStrings.itemlist,
                                PKMN.ItemToString(heldItem)
                            );
        }

        override protected Nature GetNature()
        {
            return PKMN.StringToNature(
                       _gameStrings.natures[_pkm.Nature]
                   );
        }

        override protected void SetNature(Nature nature)
        {
            _pkm.Nature = System.Array.IndexOf(
                              _gameStrings.natures,
                              PKMN.NatureToString(nature)
                          );
        }

        override protected int GetPokerusDuration()
        {
            return _pkm.PKRS_Days;
        }

        override protected void SetPokerusDuration(int pokerusDuration)
        {
            // TODO: bounds checking, set strain
            _pkm.PKRS_Days = pokerusDuration;
        }

        override protected string GetOriginalTrainerName()
        {
            return _pkm.OT_Name;
        }

        override protected void SetOriginalTrainerName(string originalTrainerName)
        {
            _pkm.OT_Name = originalTrainerName;
        }

        override protected ushort GetOriginalTrainerPublicID()
        {
            return (ushort)_pkm.TID;
        }

        override protected void SetOriginalTrainerPublicID(
            ushort originalTrainerPublicID
        )
        {
            _pkm.TID = (int)originalTrainerPublicID;
        }

        override protected ushort GetOriginalTrainerSecretID()
        {
            return (ushort)_pkm.SID;
        }

        override protected void SetOriginalTrainerSecretID(
            ushort originalTrainerSecretID
        )
        {
            _pkm.SID = (int)originalTrainerSecretID;
        }

        override protected uint GetOriginalTrainerID()
        {
            return (uint)((ushort)_pkm.TID | ((ushort)_pkm.SID << 16));
        }

        override protected void SetOriginalTrainerID(uint originalTrainerID)
        {
            _pkm.TID = (ushort)(originalTrainerID & 0xFFFF);
            _pkm.SID = (ushort)((originalTrainerID & 0xFFFF0000) >> 16);
        }

        // TODO: language

        override protected int GetCurrentTrainerFriendship()
        {
            return _pkm.CurrentFriendship;
        }

        override protected void SetCurrentTrainerFriendship(
            int currentTrainerFriendship
        )
        {
            _pkm.CurrentFriendship = currentTrainerFriendship;
        }

        // Ability indices have been pretty constant, so this should be fine.

        override protected Ability GetAbility()
        {
            return (Ability)_pkm.Ability;
        }

        override protected void SetAbility(Ability ability)
        {
            _pkm.Ability = (int)ability;
        }

        // TODO: ball

        override protected int GetLevelMet()
        {
            return _pkm.Met_Level;
        }

        override protected void SetLevelMet(int levelMet)
        {
            _pkm.Met_Level = levelMet;
        }

        // TODO: location met, needs LibPKMN Game <-> PKHeX Version conversion
        // for parameter

        // TODO: original game

        override protected uint GetPersonality()
        {
            return _pkm.PID;
        }

        override protected void SetPersonality(uint personality)
        {
            _pkm.PID = personality;

            // TODO: Unown form, gender, etc
        }

        override protected int GetExperience()
        {
            return (int)_pkm.EXP;
        }

        override protected void SetExperience(int experience)
        {
            _pkm.EXP = (uint)experience;
        }

        override protected int GetLevel()
        {
            return _pkm.CurrentLevel;
        }

        override protected void SetLevel(int level)
        {
            _pkm.CurrentLevel = level;
        }

        override protected int GetCurrentHP()
        {
            return _pkm.Stat_HPCurrent;
        }

        override protected void SetCurrentHP(int currentHP)
        {
            _pkm.Stat_HPCurrent = currentHP;
        }

        // TODO: setting EVs+IVs for Generation III+, override for I-II
    }
}
