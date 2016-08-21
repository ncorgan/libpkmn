/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <csharp/pkmn_std_map.i>
%include <csharp/pkmn_std_pair.i>
%include <pkmn_shared_ptr.i>
%include <csharp/pkmn_std_vector.i>

%define PKMN_CSHARP_MAP(cpp_key, cpp_val, csharp_key, csharp_val, csharp_name)
    %typemap(csimports) std::map<cpp_key, cpp_val > "
using System;
using System.Runtime.InteropServices;
/// <summary>An IDictionary<csharp_key, csharp_val> implementation.</summary>";

    %template(csharp_name) std::map<cpp_key, cpp_val >;
%enddef

%define PKMN_CSHARP_PAIR(cpp_type1, cpp_type2, csharp_type1, csharp_type2, csharp_name)
    %typemap(csimports) std::pair<cpp_type1, cpp_type2> "
using System;
using System.Runtime.InteropServices;
/// <summary>A pair of values, the first being a csharp_type1 and the second being a csharp_type2.</summary>";

    %template(csharp_name) std::pair<cpp_type1, cpp_type2>;
%enddef

%define PKMN_CSHARP_INCLUDE_AND_SPTR(cpp_type, csharp_name)
    %rename(cpp_type ## _base) pkmn:: ## cpp_type;

    %csmethodmodifiers pkmn::shared_ptr<pkmn:: ## cpp_type>::__cptr "
    private";
    %csmethodmodifiers pkmn::shared_ptr<pkmn:: ## cpp_type>::__sptr_eq "
    private";

    %typemap(cscode) pkmn::shared_ptr<pkmn:: ## cpp_type> %{
        /// <summary>Compares two csharp_name instances to determine value equality.</summary>
        /// <remarks>
        /// Returns true if the internal shared_ptrs' pointers are equal.
        /// </remarks>
        /// <param name="rhs">csharp_name with which to compare self</param>
        /// <returns>Whether or not csharp_name instances are equal</returns>
        /// </remarks>
        public bool Equals(csharp_name rhs) {
            if(rhs == null) {
                return false;
            } else if(this == rhs) {
                return true;
            } else {
                return this.__sptr_eq(rhs);
            }
        }

        /// <summary>Compares an instance of csharp_name to a C# object.</summary>
        /// <param name="rhs">Object with which to compare self</param>
        /// <returns>Whether or not csharp_name and Object are equal</returns>
        public override bool Equals(System.Object rhs) {
            if(rhs == null) {
                return false;
            }

            csharp_name rhsSptr = rhs as csharp_name;
            if(rhsSptr == null) {
                return false;
            } else {
                return this.Equals(rhsSptr);
            }
        }

        /// <summary>Generates a unique hash code for the given csharp_name.</summary>
        /// <returns>Unique hash code</returns>
        public override int GetHashCode() {
            ulong cptr = __cptr();
            while(cptr > global::System.Int32.MaxValue) {
                cptr = (ulong)global::System.Math.Sqrt(cptr);
            }
            return (int)cptr;
        }
    %}


    %include <pkmn/ ## cpp_type ## .hpp>
    %template(csharp_name) pkmn::shared_ptr<pkmn:: ## cpp_type>;
%enddef

%define PKMN_CSHARP_VECTOR(cpp_type, csharp_type, csharp_name)
    %typemap(csimports) std::vector<cpp_type> "
using System;
using System.Runtime.InteropServices;
/// <summary>An IList<csharp_type> implementation.</summary>";

    SWIG_STD_VECTOR_ENHANCED(cpp_type);
    %template(csharp_name) std::vector<cpp_type>;
%enddef
