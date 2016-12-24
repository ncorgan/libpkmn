/*
 * Copyright (c) 2015-2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
namespace PKMN {

public class PKMNHashCodes {
/// <summary>Generates a unique hash code for a given string.</summary>
/// <remarks>
/// string.GetHashCode()'s outputs are not necessarily unique to each string, as
/// explained below, so we need to do this ourselves.
///
/// "Hash codes are used to insert and retrieve keyed objects from hash tables
/// efficiently. However, hash codes do not uniquely identify strings.
/// Identical strings have equal hash codes, but the common language runtime
/// can also assign the same hash code to different strings. In addition, hash
/// codes can vary by version of the .NET Framework, by platform within a
/// single version, and by application domain. Because of this, you should not
/// serialize or persist hash code values, nor should you use them as keys
/// in a hash table or dictionary."
///
/// https://msdn.microsoft.com/en-us/library/system.string.gethashcode(v=vs.110).aspx
/// </remarks>
/// <returns>Unique hash code</returns>
    public static int StringHashCode(string str) {
        int ret = 0;

        for(int i = 0; i < str.Length; i++) {
            ret ^= (int)System.Math.Pow((int)str[i], i);
        }
        return ret;
    }
}

public partial class HiddenPower {
    public bool Equals(HiddenPower rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return (this.Type.Equals(rhs.Type) && this.BasePower == rhs.BasePower);
        }
    }

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

    public override int GetHashCode() {
        return new HashCodeBuilder<HiddenPower>(this)
                       .With(m => m.Type)
                       .With(m => m.BasePower)
                       .HashCode;
    }
}

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
            return (this.GetItemID() == rhs.GetItemID() && this.GetGameID() == rhs.GetGameID());
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
        return ((int)System.Math.Pow(this.GetItemID(), 3.0f) ^ (int)System.Math.Pow(this.GetGameID(), 6.0f));
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
        return ((int)this.Item.GetHashCode() ^ (int)System.Math.Pow(this.Amount, 5.0f));
    }
}

public partial class ItemSlotList {
    /// <summary>Generates a unique hash code for the given ItemSlotList.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        int ret = 0;

        for(int i = 0; i < this.size(); i++) {
            ret ^= (int)System.Math.Pow(this[i].GetHashCode(), (i+1));
        }

        return ret;
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
            return (this.GetMoveID() == rhs.GetMoveID() && this.GetGameID() == rhs.GetGameID());
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
        return ((int)System.Math.Pow(this.GetMoveID(), 3.0f) ^ (int)System.Math.Pow(this.GetGameID(), 6.0f));
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
            return (this.GetPokemonID() == rhs.GetPokemonID() && this.GetGameID() == rhs.GetGameID());
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
        return ((int)System.Math.Pow(this.GetPokemonID(), 3.0f) ^ (int)System.Math.Pow(this.GetGameID(), 6.0f));
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
        int ret = 0;

        ret ^= this.Move.GetHashCode();
        ret ^= (int)System.Math.Pow(this.Level, 7.0f);

        return ret;
    }
}

public partial class LevelupMoveList {
    /// <summary>Generates a unique hash code for the given LevelupMoveList.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        int ret = 0;

        for(int i = 0; i < this.size(); i++) {
            ret ^= (int)System.Math.Pow(this[i].GetHashCode(), i);
        }

        return ret;
    }
}

public partial class MoveEntryList {
    /// <summary>Generates a unique hash code for the given MoveEntryList.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        int ret = 0;

        for(int i = 0; i < this.size(); i++) {
            ret ^= (int)System.Math.Pow(this[i].GetHashCode(), i);
        }

        return ret;
    }
}

public partial class PokemonEntryList {
    /// <summary>Generates a unique hash code for the given PokemonEntryList.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        int ret = 0;

        for(int i = 0; i < this.size(); i++) {
            ret ^= (int)System.Math.Pow(this[i].GetHashCode(), i);
        }

        return ret;
    }
}

public partial class SpindaCoords {
    public bool Equals(SpindaCoords rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return (this.X == rhs.X) && (this.Y == rhs.Y);
        }
    }

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

    public override int GetHashCode() {
        return new HashCodeBuilder<SpindaCoords>(this)
                       .With(m => m.X)
                       .With(m => m.Y)
                       .HashCode;
    }
}

public partial class SpindaSpots {
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

    public override int GetHashCode() {
        return new HashCodeBuilder<SpindaSpots>(this)
                       .With(m => m.LeftEar)
                       .With(m => m.RightEar)
                       .With(m => m.LeftFace)
                       .With(m => m.RightFace)
                       .HashCode;
    }
}

public partial class StringBoolDict {
    /// <summary>Generates a unique hash code for the given StringBoolDict.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        int ret = 0;

        foreach(string key in this.Keys) {
            ret ^= (int)System.Math.Pow(PKMNHashCodes.StringHashCode(key),
                                        (this[key] ? 2.0f : 5.0f));
        }

        return ret;
    }
}

public partial class StringIntDict {
    /// <summary>Generates a unique hash code for the given StringIntDict.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        int ret = 0;

        foreach(string key in this.Keys) {
            ret ^= PKMNHashCodes.StringHashCode(key) * this[key];
        }

        return ret;
    }
}

public partial class StringList {
    /// <summary>Generates a unique hash code for the given StringList.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        int ret = 0;

        for(int i = 0; i < this.Count; i++) {
            ret ^= (int)System.Math.Pow(PKMNHashCodes.StringHashCode(this[i]), i);
        }

        return ret;
    }
}

public partial class StringPair {
    /// <summary>Compares two StringPair instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if both parts of the pair are equal.
    /// </remarks>
    /// <param name="rhs">StringPair with which to compare self</param>
    /// <returns>Whether or not StringPair instances are equal</returns>
    public bool Equals(StringPair rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return (this.First.Equals(rhs.First) &&
                    this.Second.Equals(rhs.Second));
        }
    }

    /// <summary>Compares a StringPair to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not StringPair and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        StringPair rhsList = rhs as StringPair;
        if(rhsList == null) {
            return false;
        } else {
            return this.Equals(rhsList);
        }
    }

    /// <summary>Generates a unique hash code for the given StringPair.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        int ret = 0;

        ret ^= PKMNHashCodes.StringHashCode(this.First)*37;
        ret ^= PKMNHashCodes.StringHashCode(this.Second)*29;

        return ret;
    }
}

public partial class StringStringDict {
    /// <summary>Generates a unique hash code for the given StringStringDict.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        int ret = 0;

        foreach(string key in this.Keys) {
            ret ^= ((PKMNHashCodes.StringHashCode(key)*37) ^ (PKMNHashCodes.StringHashCode(this[key])*29));
        }

        return ret;
    }
}

public partial class ItemPockets {
    /// <summary>Generates a unique hash code for the given ItemPockets.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        int ret = 0;

        foreach(string key in this.Keys) {
            ret ^= ((PKMNHashCodes.StringHashCode(key)*37) ^ this[key].GetHashCode());
        }

        return ret;
    }
}

}
