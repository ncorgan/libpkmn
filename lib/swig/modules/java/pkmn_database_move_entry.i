/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/move_entry.hpp>
%}

%typemap(javacode) pkmn::database::move_entry %{

    public boolean equals(MoveEntry other) {
        if(this == other) {
            return true;
        } else if(other == null) {
            return false;
        }

        return (this.getName().equals(other.getName())) &&
               (this.getGame().equals(other.getGame()));
    }

    @Override
    public boolean equals(Object other) {
        if(this == other) {
            return true;
        } else if(other == null) {
            return false;
        } else if(!(other instanceof MoveEntry)) {
            return false;
        }

        return this.equals((MoveEntry)other);
    }

    @Override
    public int hashCode() {
        org.apache.commons.lang3.builder.HashCodeBuilder hashCodeBuilder = new org.apache.commons.lang3.builder.HashCodeBuilder();
        hashCodeBuilder.append(this.getName());
        hashCodeBuilder.append(this.getGame());

        return hashCodeBuilder.toHashCode();
    }

%}

%include <pkmn/database/move_entry.hpp>
