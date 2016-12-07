/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <pkmn_shared_ptr.i>

%include <types/pkmn_std_map.i>
%include <types/pkmn_std_pair.i>
%include <types/pkmn_std_vector.i>

%define PKMN_JAVA_MAP(cpp_key, cpp_val, java_key, java_val, java_name)

    %typemap(javaimports) std::map<cpp_key, cpp_val > "
/**
 * A map with keys of type java_key and values of type java_val.
 *
 * Matches a subset of the java.util.HashMap API.
 */";

    %javamethodmodifiers std::map<cpp_key, cpp_val >::size "
/**
 * Returns the number of key-value mappings in this map.
 */
public";

    %javamethodmodifiers std::map<cpp_key, cpp_val >::empty const "
/**
 * Returns true if this map contains no key-value mappings.
 */
public";

    %javamethodmodifiers std::map<cpp_key, cpp_val >::clear "
/**
 * Removes all of the mappings from this map.
 */
public";

    %javamethodmodifiers std::map<cpp_key, cpp_val >::get "
/**
 * Returns the value to which the specified key is mapped.
 *
 * @param key the key whose associated value is to be returned
 * @throw IllegalArgumentException if invalid key is given
 */
public";

    %javamethodmodifiers std::map<cpp_key, cpp_val >::put "
/**
 * Associates the specified value with the specified key in this map.
 *
 * @param key key with which the specified value is to be associated
 * @param value value to be associated with the specified key
 */
public";

    %javamethodmodifiers std::map<cpp_key, cpp_val >::remove "
/**
 * Removes the mapping for the specified key from this map if present.
 *
 * @param key key whose mapping is to be removed from the map
 * @throw IllegalArgumentException if invalid key is given
 */
public";

    %javamethodmodifiers std::map<cpp_key, cpp_val >::containsKey "
/**
 * Returns true if this map contains a mapping for the specified key.
 *
 * @param key The key whose presence in this map is to be tested
 */
public";

    %javamethodmodifiers std::map<cpp_key, cpp_val >::containsValue "
/**
 * Returns true if this map maps one or more keys to the specified value.
 *
 * @param value value whose presence in this map is to be tested
 */
public";

    %javamethodmodifiers std::map<cpp_key, cpp_val >::keySet "
/**
 * Returns a vector of the keys contained in this map.
 */
public";

    %template(java_name) std::map<cpp_key, cpp_val >;
%enddef

%define PKMN_JAVA_PAIR(cpp_type1, cpp_type2, java_type1, java_type2, java_name)

    %typemap(javaimports) std::pair<cpp_type1, cpp_type2> "
/**
 * A pair whose first value is of type java_type1 and whose second value is of type java_type2.
 */";

    %javamethodmodifiers std::pair<cpp_type1, cpp_type2>::getFirst "
/**
 * Returns the first value in the pair.
 */
public";

    %javamethodmodifiers std::pair<cpp_type1, cpp_type2>::getSecond "
/**
 * Returns the second value in the pair.
 */
public";

    %javamethodmodifiers std::pair<cpp_type1, cpp_type2>::setFirst "
/**
 * Sets the first value in the pair.
 */
public";

    %javamethodmodifiers std::pair<cpp_type1, cpp_type2>::setSecond "
/**
 * Sets the second value in the pair.
 */
public";

    %template(java_name) std::pair<cpp_type1, cpp_type2>;
%enddef

%define PKMN_JAVA_INCLUDE_AND_SPTR(cpp_type, java_name)
    %rename(cpp_type ## _base) pkmn:: ## cpp_type;

    %javamethodmodifiers pkmn::shared_ptr<pkmn:: ## cpp_type>::__cptr "
    private";
    %javamethodmodifiers pkmn::shared_ptr<pkmn:: ## cpp_type>::__sptr_eq "
    private";

    %typemap(javacode) pkmn::shared_ptr<pkmn:: ## cpp_type> %{
        /**
         * Compares two java_name instances to determine value equality.
         *
         * Returns true if the internal shared_ptrs' pointers are equal.
         *
         * @param rhs java_name with which to compare self
         * @return Whether or not java_name instances are equal
         */
        public boolean equals(java_name rhs) {
            if(rhs == null) {
                return false;
            } else if(this == rhs) {
                return true;
            } else {
                return this.__sptr_eq(rhs);
            }
        }

        /**
         * Compares an instance of java_name to a Java object.
         *
         * @param rhs Object with which to compare self
         * @return Whether or not java_name and Object are equal
         */
        @Override
        public boolean equals(Object rhs) {
            if(rhs == null) {
                return false;
            } else if(!(rhs instanceof java_name)) {
                return false;
            }

            return (this.equals(((java_name)rhs)));
        }

        /**
         * Generates a unique hashcode for the given java_name.
         * @return Unique hash code
         */
        @Override
        public int hashCode() {
            return this.__cptr().hashCode();
        }
    %}

    %include <pkmn/ ## cpp_type ## .hpp>
    %template(java_name) pkmn::shared_ptr<pkmn:: ## cpp_type>;
%enddef

%define PKMN_JAVA_VECTOR(cpp_type, java_type, java_name)

    %typemap(javaimports) std::vector<cpp_type> "
/**
 * A vector of type java_type.
 *
 * Matches a subset of the java.util.Vector API.
 */";

    %javamethodmodifiers std::vector<cpp_type>::size "
/**
 * Returns the number of components in this vector.
 */
public";

    %javamethodmodifiers std::vector<cpp_type>::capacity "
/**
 * Returns the current capacity of this vector.
 */
public";

    %javamethodmodifiers std::vector<cpp_type>::reserve "
/**
 * Increases the capacity of this vector, if necessary.
 *
 * Ensures that it can hold at least the number of components specified by
 * the minimum capacity argument.
 *
 * @param minCapacity the desired minimum capacity
 */
public";

    %javamethodmodifiers std::vector<cpp_type>::resize "
/**
 * Sets the size of this vector.
 *
 * If the new size is greater than the current size, new items, instantiated
 * with the default constructor, are added to the end of the vector. If the
 * new size is less than the current size, all components at index newSize and
 * greater are discarded.
 *
 * @param newSize the new size of this vector
 */
public";

    %javamethodmodifiers std::vector<cpp_type>::empty "
/**
 * Tests if this vector has no components.
 */
public";

    %javamethodmodifiers std::vector<cpp_type>::clear "
/**
 * Removes all of the elements from this Vector.
 *
 * The Vector will be empty after this call returns (unless it throws an exception).
 */
public";

    %javamethodmodifiers std::vector<cpp_type>::add "
/**
 * Appends the specified element to the end of this Vector.
 *
 * @param e element to be appended to this Vector
 */
public";

    %javamethodmodifiers std::vector<cpp_type>::set "
/**
 * Replaces the element at the specified position in this Vector with the specified element.
 *
 * @param index index of the element to replace
 * @param element element to be stored at the specified position
 * @throws IllegalArgumentException if index is out of range
 * @returns the element previously at the specified position
 */
public";

    %javamethodmodifiers std::vector<cpp_type>::remove "
/**
 * Removes the element at the specified position in this Vector.
 *
 * Shifts any subsequent elements to the left (subtracts one from their indices). Returns the element that was removed from the Vector.
 *
 * @param index the index of the element to be removed
 * @throws IllegalArgumentException if index is out of range
 * @returns element that was removed
 */
public";

    %javamethodmodifiers std::vector<cpp_type>::get "
/**
 * Returns the element at the specified position in this Vector.
 *
 * @param index index of the element to return
 * @throws IllegalArgumentException if index is out of range
 * @returns object at the specified index
 */
public";

    %javamethodmodifiers std::vector<cpp_type>::firstElement "
/**
 * Returns the first component (the item at index 0) of this Vector.
 *
 * @returns the first component of this vector
 * @throws RuntimeException if vector is empty
 */
public";

    %javamethodmodifiers std::vector<cpp_type>::lastElement "
/**
 * Returns the last component of this Vector.
 *
 * @returns the last component of the vector, i.e., the component at index size() - 1.
 * @throws RuntimeException if vector is empty
 */
public";

    %template(java_name) std::vector<cpp_type>;
%enddef
