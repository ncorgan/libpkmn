/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/calculations/hidden_power.hpp>
%}

%include <std_string.i>

/*
 * Doxygen documentation
 */

%javamethodmodifiers pkmn::calculations::hidden_power::getType "
/**
 * Get what type Hidden Power has when this Pokémon uses it.
 */
public";

%javamethodmodifiers pkmn::calculations::hidden_power::getBasePower "
/**
 * Get the base power.
 */
public";

%javamethodmodifiers pkmn::calculations::hidden_power::setType "
/**
 * Set the type.
 *
 * @param newType new type
 */
public";

%javamethodmodifiers pkmn::calculations::hidden_power::setBasePower "
/**
 * Set the base power.
 *
 * @param newBasePower new base power
 */
public";

%typemap(javacode) pkmn::calculations::hidden_power %{

public boolean equals(HiddenPower other) {
    if(this == other) {
        return true;
    }

    return (this.getType().equals(other.getType())) &&
           (this.getBasePower() == other.getBasePower());
}

@Override
public boolean equals(Object other) {
    if(this == other) {
        return true;
    } else if(!(other instanceof HiddenPower)) {
        return false;
    }

    return this.equals((HiddenPower)other);
}

@Override
public int hashCode() {
    org.apache.commons.lang3.builder.HashCodeBuilder hashCodeBuilder = new org.apache.commons.lang3.builder.HashCodeBuilder();
    hashCodeBuilder.append(this.getType());
    hashCodeBuilder.append(this.getBasePower());
    return hashCodeBuilder.toHashCode();
}

%}

/*
 * Manually write the get/set functions so we can individually
 * document them.
 */
namespace pkmn { namespace calculations {

class hidden_power {
    public:
        %extend {
            const std::string& getType() {
                return self->type;
            }
            int getBasePower() {
                return self->base_power;
            }

            void setType(const std::string &newType) {
                self->type = newType;
            }
            void setBasePower(const int newBasePower) {
                self->base_power = newBasePower;
            }
        }
};

typedef std::vector<pkmn::calculations::hidden_power> calculations::hidden_powers_t;

}}

%include <java/stl_macros.i>
PKMN_JAVA_VECTOR(pkmn::calculations::hidden_power, HiddenPower, HiddenPowerVector)

%ignore hidden_power;
%include <pkmn/calculations/hidden_power.hpp>
