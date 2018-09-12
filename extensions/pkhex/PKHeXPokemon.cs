/*
 * Copyright (C) 2018 Nicholas Corgan
 *
 * This file is part of LibPKMN: PKHeX Extension.
 *
 * LibPKMN: PKHeX Extension is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LibPKMN: PKHeX Extension is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LibPKMN: PKHeX Extension.  If not, see <https://www.gnu.org/licenses/>.
 */

class PKHeXPokemon: PKMN.Pokemon
{
    // Member variables

    private PKHeX.Core.PKM _pkhexPKM;

    public PKHeXPokemon(
        PKHeX.Core.PKM pkhexPKM
    ): base()
    {
        _pkhexPKM = pkhexPKM;
    }

    // Override protected PKMN.Pokemon functions. These will be accessed via
    // PKMN.Pokemon's properties.

    override protected string GetNickname()
    {
        return _pkhexPKM.Nickname;
    }

    override protected void SetNickname(string nickname)
    {
        _pkhexPKM.Nickname = nickname;
    }
}
