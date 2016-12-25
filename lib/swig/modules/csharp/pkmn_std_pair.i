/* -----------------------------------------------------------------------------
 * pkmn_std_pair.i
 *
 * SWIG typemaps for std::pair
 * ----------------------------------------------------------------------------- */

%include <std_common.i>
%include <exception.i>

%rename(First) first;
%rename(Second) second;

// ------------------------------------------------------------------------
// std::pair
// ------------------------------------------------------------------------

%{
#include <utility>
%}

%csmethodmodifiers std::pair::first "
/// <summary>The first member of the pair.</summary>
public";

%csmethodmodifiers std::pair::second "
/// <summary>The second member of the pair.</summary>
public";

%typemap(cscode) std::pair %{
    /// <summary>Compares two $csclassname instances to determine value equality.</summary>
    /// <remarks>
    /// Two instances are determined to be equal if their respective fields are equal.
    /// </remarks>
    /// <param name="rhs">$csclassname with which to compare self</param>
    /// <returns>Whether or not $csclassname instances are equal</returns>
    public bool Equals($csclassname rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        }

        return (this.First.Equals(rhs.First) && this.Second.Equals(rhs.Second));
    }

    /// <summary>Compares a $csclassname to a C# object.</summary>
    /// <param name="rhs">Object with which to compare self</param>
    /// <returns>Whether or not $csclassname and Object are equal</returns>
    public override bool Equals(System.Object rhs) {
        if(rhs == null) {
            return false;
        }

        $csclassname rhsList = rhs as $csclassname;
        if(rhsList == null) {
            return false;
        } else {
            return this.Equals(rhsList);
        }
    }

    /// <summary>Returns a hash code unique to the given object.</summary>
    /// <returns>Unique hash code</returns>
    public override int GetHashCode() {
        return HashCodeBuilder.Create().AddValue(this.First)
                                       .AddValue(this.Second)
                                       .ToHashCode();
    }
%}

namespace std {

  template<class T, class U> struct pair {

    pair();
    pair(T t, U u);

    T first;
    U second;
  };

}
