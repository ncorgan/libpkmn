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
    class PKHeXPokemon: Pokemon
    {
        private static PKHeX.Core.GameStrings _gameStrings = PKHeX.Core.GameInfo.GetStrings("en");

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

        // TODO: status, half-done in PKHeX

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
    }
}
