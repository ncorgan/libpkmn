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

}}
