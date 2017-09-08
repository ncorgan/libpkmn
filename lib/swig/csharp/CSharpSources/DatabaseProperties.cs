/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN { namespace Database {

public partial class ItemEntry {
    public string Name {
        get { return GetName(); }
    }

    public string Game {
        get { return GetGame(); }
    }

    public string Category {
        get { return GetCategory(); }
    }

    public string Pocket {
        get { return GetPocket(); }
    }

    public string Description {
        get { return GetDescription(); }
    }

    public int Cost {
        get { return GetCost(); }
    }

    public bool IsHoldable {
        get { return Holdable(); }
    }

    public int FlingPower {
        get { return GetFlingPower(); }
    }

    public string FlingEffect {
        get { return GetFlingEffect(); }
    }
}

public partial class MoveEntry {
    public string Name {
        get { return GetName(); }
    }

    public string Game {
        get { return GetGame(); }
    }

    public string MoveType {
        get { return GetMoveType(); }
    }

    public string Description {
        get { return GetDescription(); }
    }

    public string Target {
        get { return GetTarget(); }
    }

    public string DamageClass {
        get { return GetDamageClass(); }
    }

    public int BasePower {
        get { return GetBasePower(); }
    }

    public float Accuracy {
        get { return GetAccuracy(); }
    }

    public int Priority {
        get { return GetPriority(); }
    }

    public string Effect {
        get { return GetEffect(); }
    }

    public string ContestType {
        get { return GetContestType(); }
    }

    public string ContestEffect {
        get { return GetContestEffect(); }
    }

    public string SuperContestEffect {
        get { return GetSuperContestEffect(); }
    }
}

public partial class PokemonEntry {
    public string Name {
        get { return GetName(); }
    }

    public string Game {
        get { return GetGame(); }
    }

    public string Species {
        get { return GetSpecies(); }
    }

    public string PokedexEntry {
        get { return GetPokedexEntry(); }
    }

    public string Form {
        get { return GetForm(); }
        set { SetForm(value); }
    }

    public float Height {
        get { return GetHeight(); }
    }

    public float Weight {
        get { return GetWeight(); }
    }

    public float ChanceMale {
        get { return GetChanceMale(); }
    }

    public float ChanceFemale {
        get { return GetChanceFemale(); }
    }

    public bool HasGenderDifferences {
        get { return HasGenderDifferencesFunction(); }
    }

    public int BaseFriendship {
        get { return GetBaseFriendship(); }
    }

    public StringPair Types {
        get { return GetTypes(); }
    }

    public StringPair Abilities {
        get { return GetAbilities(); }
    }

    public string HiddenAbility {
        get { return GetHiddenAbility(); }
    }

    public StringPair EggGroups {
        get { return GetEggGroups(); }
    }

    public StringIntDict BaseStats {
        get { return GetBaseStats(); }
    }

    public StringIntDict EVYields {
        get { return GetEVYields(); }
    }

    public int ExperienceYield {
        get { return GetExperienceYield(); }
    }

    public LevelupMoveList LevelupMoves {
        get { return GetLevelupMoves(); }
    }

    public MoveEntryList TMHMMoves {
        get { return GetTMHMMoves(); }
    }

    public MoveEntryList EggMoves {
        get { return GetEggMoves(); }
    }

    public MoveEntryList TutorMoves {
        get { return GetTutorMoves(); }
    }

    public StringList Forms {
        get { return GetForms(); }
    }

    public PokemonEntryList Evolutions {
        get { return GetEvolutions(); }
    }
}

}}
