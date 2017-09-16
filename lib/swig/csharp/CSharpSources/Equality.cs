/*
 * Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN {

public partial class HiddenPower {
    /// <summary>Compares two HiddenPower instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if their type and base power are equal.
    /// </remarks>
    /// <param name="rhs">HiddenPower with which to compare self</param>
    /// <returns>Whether or not HiddenPower instances are equal</returns>
    public bool Equals(HiddenPower rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return (this.Type.Equals(rhs.Type) && this.BasePower == rhs.BasePower);
        }
    }

    /// <summary>Compares a HiddenPower to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not HiddenPower and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        HiddenPower rhsHiddenPower = rhs as HiddenPower;
        if(rhsHiddenPower == null) {
            return false;
        } else {
            return this.Equals(rhsHiddenPower);
        }
    }

    /// <summary>Generates a unique hash code for the given HiddenPower.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<string>(this.Type)
                                       .AddValue<int>(this.BasePower)
                                       .ToHashCode();
    }
}

public partial class ItemSlot {
    /// <summary>Compares two ItemSlot instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if they represent the same item and have
    /// the same amount.
    /// </remarks>
    /// <param name="rhs">ItemSlot with which to compare self</param>
    /// <returns>Whether or not ItemSlot instances are equal</returns>
    public bool Equals(ItemSlot rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return (this.Item.Equals(rhs.Item) && (this.Amount == rhs.Amount));
        }
    }

    /// <summary>Compares a ItemSlot to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not ItemSlot and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        ItemSlot rhsEntry = rhs as ItemSlot;
        if(rhsEntry == null) {
            return false;
        } else {
            return this.Equals(rhsEntry);
        }
    }

    /// <summary>Generates a unique hash code for the given ItemSlot.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<string>(this.Item)
                                       .AddValue<int>(this.Amount)
                                       .ToHashCode();
    }
}

public partial class MoveSlot {
    /// <summary>Compares two MoveSlot instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if they represent the same move and have
    /// the same PP.
    /// </remarks>
    /// <param name="rhs">MoveSlot with which to compare self</param>
    /// <returns>Whether or not MoveSlot instances are equal</returns>
    public bool Equals(MoveSlot rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return (this.Move.Equals(rhs.Move) && (this.PP == rhs.PP));
        }
    }

    /// <summary>Compares a MoveSlot to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not MoveSlot and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        MoveSlot rhsEntry = rhs as MoveSlot;
        if(rhsEntry == null) {
            return false;
        } else {
            return this.Equals(rhsEntry);
        }
    }

    /// <summary>Generates a unique hash code for the given MoveSlot.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<string>(this.Move)
                                       .AddValue<int>(this.PP)
                                       .ToHashCode();
    }
}

public partial class NaturalGift {
    /// <summary>Compares two NaturalGift instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if their type and base power are equal.
    /// </remarks>
    /// <param name="rhs">NaturalGift with which to compare self</param>
    /// <returns>Whether or not NaturalGift instances are equal</returns>
    public bool Equals(NaturalGift rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return (this.Type.Equals(rhs.Type) && this.BasePower == rhs.BasePower);
        }
    }

    /// <summary>Compares a NaturalGift to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not NaturalGift and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        NaturalGift rhsNaturalGift = rhs as NaturalGift;
        if(rhsNaturalGift == null) {
            return false;
        } else {
            return this.Equals(rhsNaturalGift);
        }
    }

    /// <summary>Generates a unique hash code for the given NaturalGift.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<string>(this.Type)
                                       .AddValue<int>(this.BasePower)
                                       .ToHashCode();
    }
}

public partial class SpindaCoords {
    /// <summary>Compares two SpindaCoords instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if each respective coordinate is equal.
    /// </remarks>
    /// <param name="rhs">SpindaCoords with which to compare self</param>
    /// <returns>Whether or not SpindaCoords instances are equal</returns>
    public bool Equals(SpindaCoords rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return (this.X == rhs.X) && (this.Y == rhs.Y);
        }
    }

    /// <summary>Compares a SpindaCoords to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not SpindaCoords and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        SpindaCoords rhsSpindaCoords = rhs as SpindaCoords;
        if(rhsSpindaCoords == null) {
            return false;
        } else {
            return this.Equals(rhsSpindaCoords);
        }
    }

    /// <summary>Generates a unique hash code for the given SpindaCoords.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<int>(this.X)
                                       .AddValue<int>(this.Y)
                                       .ToHashCode();
    }
}

public partial class SpindaSpots {
    /// <summary>Compares two SpindaSpots instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if their each corresponding spot is equal.
    /// </remarks>
    /// <param name="rhs">SpindaSpots with which to compare self</param>
    /// <returns>Whether or not SpindaSpots instances are equal</returns>
    public bool Equals(SpindaSpots rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return (this.LeftEar.Equals(rhs.LeftEar) &&
                    this.RightEar.Equals(rhs.RightEar) &&
                    this.LeftFace.Equals(rhs.LeftFace) &&
                    this.RightFace.Equals(rhs.RightFace));
        }
    }

    /// <summary>Compares a SpindaSpots to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not SpindaSpots and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        SpindaSpots rhsSpindaSpots = rhs as SpindaSpots;
        if(rhsSpindaSpots == null) {
            return false;
        } else {
            return this.Equals(rhsSpindaSpots);
        }
    }

    /// <summary>Generates a unique hash code for the given SpindaSpots.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<SpindaCoords>(this.LeftEar)
                                       .AddValue<SpindaCoords>(this.RightEar)
                                       .AddValue<SpindaCoords>(this.LeftFace)
                                       .AddValue<SpindaCoords>(this.RightFace)
                                       .ToHashCode();
    }
}

namespace Database {

public partial class ItemEntry {
    /// <summary>Compares two ItemEntry instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if they represent the same item from
    /// the same game.
    /// </remarks>
    /// <param name="rhs">ItemEntry with which to compare self</param>
    /// <returns>Whether or not ItemEntry instances are equal</returns>
    public bool Equals(ItemEntry rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return this.GetName().Equals(rhs.GetName()) &&
                   this.GetGame().Equals(rhs.GetGame());
        }
    }

    /// <summary>Compares an ItemEntry to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not ItemEntry and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        ItemEntry rhsEntry = rhs as ItemEntry;
        if(rhsEntry == null) {
            return false;
        } else {
            return this.Equals(rhsEntry);
        }
    }

    /// <summary>Generates a unique hash code for the given ItemEntry.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<string>(this.GetName())
                                       .AddValue<string>(this.GetGame())
                                       .ToHashCode();
    }
}

public partial class LevelupMove {
    /// <summary>Compares two LevelupMove instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if their MoveEntry and level are
    /// equal.
    /// </remarks>
    /// <param name="rhs">LevelupMove with which to compare self</param>
    /// <returns>Whether or not LevelupMove instances are equal</returns>
    public bool Equals(LevelupMove rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return (this.Move.Equals(rhs.Move) &&
                    this.Level == rhs.Level);
        }
    }

    /// <summary>Compares a LevelupMove to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not LevelupMove and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        LevelupMove rhsList = rhs as LevelupMove;
        if(rhsList == null) {
            return false;
        } else {
            return this.Equals(rhsList);
        }
    }

    /// <summary>Generates a unique hash code for the given LevelupMove.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<MoveEntry>(this.Move)
                                       .AddValue<int>(this.Level)
                                       .ToHashCode();
    }
}

public partial class MoveEntry {
    /// <summary>Compares two MoveEntry instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if they represent the same move from
    /// the same game.
    /// </remarks>
    /// <param name="rhs">MoveEntry with which to compare self</param>
    /// <returns>Whether or not MoveEntry instances are equal</returns>
    public bool Equals(MoveEntry rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return this.GetName().Equals(rhs.GetName()) &&
                   this.GetGame().Equals(rhs.GetGame());
        }
    }

    /// <summary>Compares a MoveEntry to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not MoveEntry and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        MoveEntry rhsEntry = rhs as MoveEntry;
        if(rhsEntry == null) {
            return false;
        } else {
            return this.Equals(rhsEntry);
        }
    }

    /// <summary>Generates a unique hash code for the given MoveEntry.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<string>(this.GetName())
                                       .AddValue<string>(this.GetGame())
                                       .ToHashCode();
    }
}

public partial class PokemonEntry {
    /// <summary>Compares two PokemonEntry instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if they represent the same Pokémon, in
    /// the same form, in the same game.
    /// </remarks>
    /// <param name="rhs">PokemonEntry with which to compare self</param>
    /// <returns>Whether or not PokemonEntry instances are equal</returns>
    public bool Equals(PokemonEntry rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return this.GetName().Equals(rhs.GetName()) &&
                   this.GetGame().Equals(rhs.GetGame()) &&
                   this.GetForm().Equals(rhs.GetForm());
        }
    }

    /// <summary>Compares a PokemonEntry to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not PokemonEntry and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        PokemonEntry rhsEntry = rhs as PokemonEntry;
        if(rhsEntry == null) {
            return false;
        } else {
            return this.Equals(rhsEntry);
        }
    }

    /// <summary>Generates a unique hash code for the given PokemonEntry.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue<string>(this.GetName())
                                       .AddValue<string>(this.GetGame())
                                       .AddValue<string>(this.GetForm())
                                       .ToHashCode();
    }
}

}}
