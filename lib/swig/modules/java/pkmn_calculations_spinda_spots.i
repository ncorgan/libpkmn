/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/calculations/spinda_spots.hpp>
%}

%include <std_string.i>

/*
 * Doxygen documentation
 */

%javamethodmodifiers pkmn::calculations::spinda_coords::getXCoord "
/**
 * Get the X coordinate.
 */
public";

%javamethodmodifiers pkmn::calculations::spinda_coords::getYCoord "
/**
 * Get the Y coordinate.
 */
public";

%javamethodmodifiers pkmn::calculations::spinda_coords::setXCoord "
/**
 * Set the X coordinate.
 *
 * @param newXCoord new X coordinate
 */
public";

%javamethodmodifiers pkmn::calculations::spinda_coords::setYCoord "
/**
 * Set the Y coordinate
 *
 * @param newYCoord new Y coordinate
 */
public";

%javamethodmodifiers pkmn::calculations::spinda_spots::getLeftEar "
/**
 * Get the left ear spot.
 */
public";

%javamethodmodifiers pkmn::calculations::spinda_spots::getRightEar "
/**
 * Get the right ear spot.
 */
public";
%javamethodmodifiers pkmn::calculations::spinda_spots::getLeftFace "
/**
 * Get the left face spot.
 */
public";

%javamethodmodifiers pkmn::calculations::spinda_spots::getRightFace "
/**
 * Get the right face spot.
 */
public";

%javamethodmodifiers pkmn::calculations::spinda_spots::setLeftEar "
/**
 * Set the left ear.
 *
 * @param newLeftEar new left ear spot
 */
public";

%javamethodmodifiers pkmn::calculations::spinda_spots::setRightEar "
/**
 * Set the right ear spot
 *
 * @param newRightEar new right ear spot
 */
public";
%javamethodmodifiers pkmn::calculations::spinda_spots::setLeftFace "
/**
 * Set the left face spot.
 *
 * @param newLeftFace new left face spot
 */
public";

%javamethodmodifiers pkmn::calculations::spinda_spots::setRightFace "
/**
 * Set the right face spot
 *
 * @param newRightFace new right face spot
 */
public";

%typemap(javacode) pkmn::calculations::spinda_coords %{

public boolean equals(SpindaCoords other) {
    if(this == other) {
        return true;
    }

    return (this.getXCoord() == other.getXCoord()) &&
           (this.getYCoord() == other.getYCoord());
}

@Override
public boolean equals(Object other) {
    if(this == other) {
        return true;
    } else if(!(other instanceof SpindaCoords)) {
        return false;
    }

    return this.equals((SpindaCoords)other);
}

@Override
public int hashCode() {
    org.apache.commons.lang3.builder.HashCodeBuilder hashCodeBuilder = new org.apache.commons.lang3.builder.HashCodeBuilder();
    hashCodeBuilder.append(this.getXCoord());
    hashCodeBuilder.append(this.getYCoord());
    return hashCodeBuilder.toHashCode();
}

%}

%typemap(javacode) pkmn::calculations::spinda_spots %{

public boolean equals(SpindaSpots other) {
    if(this == other) {
        return true;
    }

    return (this.getLeftEar().equals(other.getLeftEar())) &&
           (this.getRightEar().equals(other.getRightEar())) &&
           (this.getLeftFace().equals(other.getLeftFace())) &&
           (this.getRightFace().equals(other.getRightFace()));
}

@Override
public boolean equals(Object other) {
    if(this == other) {
        return true;
    } else if(!(other instanceof SpindaSpots)) {
        return false;
    }

    return this.equals((SpindaSpots)other);
}

@Override
public int hashCode() {
    org.apache.commons.lang3.builder.HashCodeBuilder hashCodeBuilder = new org.apache.commons.lang3.builder.HashCodeBuilder();
    hashCodeBuilder.append(this.getLeftEar().hashCode());
    hashCodeBuilder.append(this.getRightEar().hashCode());
    hashCodeBuilder.append(this.getLeftFace().hashCode());
    hashCodeBuilder.append(this.getRightFace().hashCode());
    return hashCodeBuilder.toHashCode();
}

%}

/*
 * Manually write the get/set functions so we can individually
 * document them.
 */
namespace pkmn { namespace calculations {

class spinda_coords {
    public:
        %extend {
            int getXCoord() {
                return self->x;
            }
            int getYCoord() {
                return self->y;
            }

            void setXCoord(int newXCoord) {
                self->x = newXCoord;
            }
            void setYCoord(int newYCoord) {
                self->y = newYCoord;
            }
        }
};

class spinda_spots {
    public:
        %extend {
            const spinda_coords& getLeftEar() {
                return self->left_ear;
            }
            const spinda_coords& getRightEar() {
                return self->right_ear;
            }
            const spinda_coords& getLeftFace() {
                return self->left_face;
            }
            const spinda_coords& getRightFace() {
                return self->right_face;
            }

            void setLeftEar(const spinda_coords& newLeftEar) {
                self->left_ear = newLeftEar;
            }
            void setRightEar(const spinda_coords& newRightEar) {
                self->right_ear = newRightEar;
            }
            void setLeftFace(const spinda_coords& newLeftFace) {
                self->left_face = newLeftFace;
            }
            void setRightFace(const spinda_coords& newRightFace) {
                self->right_face = newRightFace;
            }
        }
};

}}

%ignore spinda_coords;
%ignore spinda_spots;
%include <pkmn/calculations/spinda_spots.hpp>