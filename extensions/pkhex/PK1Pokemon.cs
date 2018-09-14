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
    public sealed class PK1Pokemon: PKHeXPokemon
    {
        // Member variables
        PKHeX.Core.PK1 _pk1;

        public PK1Pokemon(
            PKHeX.Core.PK1 pk1
        ): base(pk1)
        {
            _pk1 = pk1;
        }

        // Override protected PKMN.Pokemon functions. These will be accessed via
        // PKMN.Pokemon's properties.

        // Level is stored twice in Generation I.
        override protected void SetLevel(int level)
        {
            _pk1.Stat_Level = level;
            //_pk1.Stat_LevelBox = level;
        }

        // TODO: port translation from PKSav
        override protected Condition GetCondition()
        {
            return (Condition)_pk1.Status_Condition;
        }

        // TODO: port translation from PKSav
        override protected void SetCondition(Condition condition)
        {
            _pk1.Status_Condition = (int)condition;
        }

        // No shininess in Generation I
        override protected bool GetIsShiny()
        {
            return false;
        }

        override protected void SetShininess(bool isShiny)
        {
            // TODO: throw?
        }

        // The Special stat is not exposed in the PKM abstraction layer,
        // so we must reimplement this function here.
        override protected void SetEV(Stat stat, int value)
        {
            switch(stat)
            {
                case Stat.HP:
                    _pk1.EV_HP = value;
                    break;

                case Stat.ATTACK:
                    _pk1.EV_ATK = value;
                    break;

                case Stat.DEFENSE:
                    _pk1.EV_DEF = value;
                    break;

                case Stat.SPEED:
                    _pk1.EV_SPE = value;
                    break;

                case Stat.SPECIAL:
                    _pk1.EV_SPC = value;
                    break;

                default:
                    // Per PKHeX behavior, redirect to Special.
                    _pk1.EV_SPC = value;
                    break;
            }

            // This gets a reference to the abstraction map.
            GetEVsMapInternal()[stat] = value;

            CalculateStats();
        }

        // PKHeX doesn't expose setting individual IVs, so we need
        // to implement that logic on our layer and pass the whole value.
        override protected void SetIV(Stat stat, int value)
        {
            ushort DV16 = _pk1.DV16;

            SetGBIV(stat, value, ref DV16);

            _pk1.DV16 = DV16;

            CalculateStats();
        }

        //
        // Helpers
        // TODO: does PKHeX have something like this?
        //
        private void CalculateStats()
        {
            int level = this.Level;
            StatDict baseStats = this.DatabaseEntry.BaseStats;

            // These are references to the internal abstraction dicts.
            StatDict EVs = GetEVsMapInternal();
            StatDict IVs = GetIVsMapInternal();
            StatDict stats = GetStatsMapInternal();

            Stat[] statList =
            {
                Stat.HP,
                Stat.ATTACK,
                Stat.DEFENSE,
                Stat.SPEED,
                Stat.SPECIAL
            };
            foreach(Stat stat in statList)
            {
                stats[stat] = Calculations.GetGBStat(
                                  stat,
                                  level,
                                  baseStats[stat],
                                  EVs[stat],
                                  IVs[stat]
                              );
            }

            _pk1.Stat_HPCurrent = stats[Stat.HP];
            _pk1.Stat_HPMax = _pk1.Stat_HPCurrent;
            _pk1.Stat_ATK = stats[Stat.ATTACK];
            _pk1.Stat_DEF = stats[Stat.DEFENSE];
            _pk1.Stat_SPD = stats[Stat.SPEED];
            _pk1.Stat_SPC = stats[Stat.SPECIAL];
        }
    }

}
