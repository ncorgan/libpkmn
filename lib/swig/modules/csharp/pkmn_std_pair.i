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

namespace std {

  template<class T, class U> struct pair {

    pair();
    pair(T t, U u);

    template <class U1, class U2> pair(const pair<U1, U2> &p);

    T first;
    U second;
  };

}
