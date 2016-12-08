/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
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
