/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/levelup_move.hpp>
%}

/*
 * Doxygen documentation
 */

%javamethodmodifiers pkmn::database::levelup_move::getMove "
/**
 * Get the move that the Pokémon learns.
 */
public";

%javamethodmodifiers pkmn::database::levelup_move::getLevel "
/**
 * Get the level at which the Pokémon learns the move.
 */
public";

%javamethodmodifiers pkmn::database::levelup_move::setMove "
/**
 * Set the move that the Pokémon learns.
 *
 * @param newMove new move
 */
public";

%javamethodmodifiers pkmn::database::levelup_move::setLevel "
/**
 * Set the level at which the Pokémon learns the move.
 *
 * @param newLevel new level
 */
public";

%typemap(javacode) pkmn::database::levelup_move %{

    public boolean equals(LevelupMove other) {
        if(this == other) {
            return true;
        } else if(other == null) {
            return false;
        }

        return (this.getMove().equals(other.getMove())) &&
               (this.getLevel() == other.getLevel());
    }

    @Override
    public boolean equals(Object other) {
        if(this == other) {
            return true;
        } else if(other == null) {
            return false;
        } else if(!(other instanceof LevelupMove)) {
            return false;
        }

        return this.equals((LevelupMove)other);
    }

    @Override
    public int hashCode() {
        org.apache.commons.lang3.builder.HashCodeBuilder hashCodeBuilder = new org.apache.commons.lang3.builder.HashCodeBuilder();
        hashCodeBuilder.append(this.getMove());
        hashCodeBuilder.append(this.getLevel());

        return hashCodeBuilder.toHashCode();
    }

%}

/*
 * Manually write the get/set functions so we can individually
 * document them.
 */
namespace pkmn { namespace database {

class levelup_move {
    public:
        %extend {
            const pkmn::database::move_entry& getMove() {
                return self->move;
            }
            int getLevel() {
                return self->level;
            }

            void setMove(const pkmn::database::move_entry &newMove) {
                self->move = newMove;
            }
            void setLevel(const int newLevel) {
                self->level = newLevel;
            }
        }
};

typedef std::vector<levelup_move> levelup_moves_t;

}}

%include <java/stl_macros.i>
PKMN_JAVA_VECTOR(pkmn::database::levelup_move, LevelupMove, LevelupMoveVector)
