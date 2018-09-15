/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest
{

public class PokemonSubclass: PKMN.Pokemon
{
    public PokemonSubclass(
        PKMN.Species species,
        PKMN.Game game
    ): base()
    {
        InitializeDefaultValues(species, game);

        _species = species;
        _game = game;
    }

    override public PKMN.Pokemon Clone()
    {
        return new PokemonSubclass(this.Species, this.Game);
    }

    override public PKMN.Pokemon ToGame(PKMN.Game game)
    {
        return new PokemonSubclass(this.Species, game);
    }

    override public void ExportToFile(string filepath)
    {
    }

    override protected PKMN.Species GetSpecies()
    {
        return _species;
    }

    override protected PKMN.Game GetGame()
    {
        return _game;
    }

    override protected string GetForm()
    {
        return GetDatabaseEntryInternal().Form;
    }

    override protected void SetForm(string form)
    {
        GetDatabaseEntryInternal().Form = form;
    }

    override protected bool GetIsEgg()
    {
        return _isEgg;
    }

    override protected void SetIsEgg(bool isEgg)
    {
        _isEgg = isEgg;
    }

    override protected PKMN.Condition GetCondition()
    {
        return _condition;
    }

    override protected void SetCondition(PKMN.Condition condition)
    {
        _condition = condition;
    }

    override protected string GetNickname()
    {
        return _nickname;
    }

    override protected void SetNickname(string nickname)
    {
        _nickname = nickname;
    }

    override protected PKMN.Gender GetGender()
    {
        return _gender;
    }

    override protected void SetGender(PKMN.Gender gender)
    {
        _gender = gender;
    }

    override protected bool GetIsShiny()
    {
        return _isShiny;
    }

    override protected void SetShininess(bool isShiny)
    {
        _isShiny = isShiny;
    }

    override protected PKMN.Item GetHeldItem()
    {
        return _heldItem;
    }

    override protected void SetHeldItem(PKMN.Item heldItem)
    {
        _heldItem = heldItem;
    }

    override protected PKMN.Nature GetNature()
    {
        return _nature;
    }

    override protected void SetNature(PKMN.Nature nature)
    {
        _nature = nature;
    }

    override protected int GetPokerusDuration()
    {
        return _pokerusDuration;
    }

    override protected void SetPokerusDuration(int pokerusDuration)
    {
        _pokerusDuration = pokerusDuration;
    }

    override protected string GetOriginalTrainerName()
    {
        return _originalTrainerName;
    }

    override protected void SetOriginalTrainerName(string originalTrainerName)
    {
        _originalTrainerName = originalTrainerName;
    }

    override protected uint GetOriginalTrainerID()
    {
        return _originalTrainerID;
    }

    override protected void SetOriginalTrainerID(uint originalTrainerID)
    {
        _originalTrainerID = originalTrainerID;
    }

    override protected ushort GetOriginalTrainerPublicID()
    {
        return (ushort)(_originalTrainerID & 0xFFFF);
    }

    override protected void SetOriginalTrainerPublicID(ushort originalTrainerPublicID)
    {
        _originalTrainerID &= 0xFFFF0000;
        _originalTrainerID |= (uint)originalTrainerPublicID;
    }

    override protected ushort GetOriginalTrainerSecretID()
    {
        return (ushort)(_originalTrainerID >> 16);
    }

    override protected void SetOriginalTrainerSecretID(ushort originalTrainerSecretID)
    {
        _originalTrainerID &= 0xFFFF;
        _originalTrainerID |= ((uint)(originalTrainerSecretID) << 16);
    }

    override protected PKMN.Gender GetOriginalTrainerGender()
    {
        return _originalTrainerGender;
    }

    override protected void SetOriginalTrainerGender(PKMN.Gender originalTrainerGender)
    {
        _originalTrainerGender = originalTrainerGender;
    }

    override protected PKMN.Language GetLanguage()
    {
        return _language;
    }

    override protected void SetLanguage(PKMN.Language language)
    {
        _language = language;
    }

    override protected int GetCurrentTrainerFriendship()
    {
        return _currentTrainerFriendship;
    }

    override protected void SetCurrentTrainerFriendship(int currentTrainerFriendship)
    {
        _currentTrainerFriendship = currentTrainerFriendship;
    }

    override protected PKMN.Ability GetAbility()
    {
        return _ability;
    }

    override protected void SetAbility(PKMN.Ability ability)
    {
        _ability = ability;
    }

    override protected PKMN.Ball GetBall()
    {
        return _ball;
    }

    override protected void SetBall(PKMN.Ball ball)
    {
        _ball = ball;
    }

    override protected int GetLevelMet()
    {
        return _levelMet;
    }

    override protected void SetLevelMet(int levelMet)
    {
        _levelMet = levelMet;
    }

    override protected string GetLocationMet()
    {
        return _locationMet;
    }

    override protected void SetLocationMet(string locationMet)
    {
        _locationMet = locationMet;
    }

    override protected string GetLocationMetAsEgg()
    {
        return _locationMetAsEgg;
    }

    override protected void SetLocationMetAsEgg(string locationMetAsEgg)
    {
        _locationMetAsEgg = locationMetAsEgg;
    }

    override protected PKMN.Game GetOriginalGame()
    {
        return _originalGame;
    }

    override protected void SetOriginalGame(PKMN.Game originalGame)
    {
        _originalGame = originalGame;
    }

    override protected uint GetPersonality()
    {
        return _personality;
    }

    override protected void SetPersonality(uint personality)
    {
        _personality = personality;
    }

    override protected int GetExperience()
    {
        return _experience;
    }

    override protected void SetExperience(int experience)
    {
        _experience = experience;
    }

    override protected int GetLevel()
    {
        return _level;
    }

    override protected void SetLevel(int level)
    {
        _level = level;
    }

    override protected int GetCurrentHP()
    {
        return _currentHP;
    }

    override protected void SetCurrentHP(int currentHP)
    {
        _currentHP = currentHP;
    }

    // Just set the abstraction.
    override protected void SetMarking(PKMN.Marking marking, bool value)
    {
        SetMarkingsMapValueInternal(marking, value);
    }

    // Just set the abstraction.
    override protected void SetRibbon(string ribbon, bool value)
    {
        SetRibbonsMapValueInternal(ribbon, value);
    }

    // Just set the abstraction.
    override protected void SetContestStat(
        PKMN.ContestStat contestStat,
        int value
    )
    {
        SetContestStatsMapValueInternal(contestStat, value);
    }

    // Just set the abstraction.
    override protected void SetMove(
        PKMN.Move move,
        int index
    )
    {
        SetMovesListMoveValueInternal(index, move);
    }

    // Just set the abstraction.
    override protected void SetMovePP(
        int index,
        int PP
    )
    {
        SetMovesListPPValueInternal(index, PP);
    }

    // Just set the abstraction.
    override protected void SetEV(
        PKMN.Stat stat,
        int value
    )
    {
        SetEVsMapValueInternal(stat, value);
    }

    // Just set the abstraction.
    override protected void SetIV(
        PKMN.Stat stat,
        int value
    )
    {
        SetIVsMapValueInternal(stat, value);
    }

    // Internals
    private PKMN.Species _species;
    private PKMN.Game _game;
    private bool _isEgg;
    private PKMN.Condition _condition;
    private string _nickname;
    private PKMN.Gender _gender;
    private bool _isShiny;
    private PKMN.Item _heldItem;
    private PKMN.Nature _nature;
    private int _pokerusDuration;
    private string _originalTrainerName;
    private uint _originalTrainerID;
    private PKMN.Gender _originalTrainerGender;
    private PKMN.Language _language;
    private int _currentTrainerFriendship;
    private PKMN.Ability _ability;
    private PKMN.Ball _ball;
    private int _levelMet;
    private string _locationMet;
    private string _locationMetAsEgg;
    private PKMN.Game _originalGame;
    private uint _personality;
    private int _experience;
    private int _level;
    private int _currentHP;
}

}
