/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%rename(pokemon_base) pkmn::pokemon;

%javamethodmodifiers pkmn::shared_ptr<pkmn::pokemon>::__cptr "
private";
%javamethodmodifiers pkmn::shared_ptr<pkmn::pokemon>::__sptr_eq "
private";

%typemap(javacode) pkmn::shared_ptr<pkmn::pokemon> %{
    /**
     * Compares two Pokemon instances to determine value equality.
     *
     * Returns true if the internal shared_ptrs' pointers are equal.
     *
     * @param rhs Pokemon with which to compare self
     * @return Whether or not Pokemon instances are equal
     */
    public boolean equals(Pokemon rhs) {
        if(rhs == null) {
            return false;
        } else if(this == rhs) {
            return true;
        } else {
            return this.__sptr_eq(rhs);
        }
    }

    /**
     * Compares an instance of Pokemon to a Java object.
     *
     * @param rhs Object with which to compare self
     * @return Whether or not Pokemon and Object are equal
     */
    @Override
    public boolean equals(Object rhs) {
        if(rhs == null) {
            return false;
        } else if(!(rhs instanceof Pokemon)) {
            return false;
        }

        return (this.equals(((Pokemon)rhs)));
    }

    /**
     * Generates a unique hashcode for the given Pokemon.
     * @return Unique hash code
     */
    @Override
    public int hashCode() {
        return this.__cptr().hashCode();
    }

    public String getIconFilepath() throws java.io.IOException {
        String game = this.getGame();
        PokemonEntry databaseEntry = this.getDatabaseEntry();

        if(game.equals("Red") || game.equals("Blue") || game.equals("Yellow")) {
            return databaseEntry.getIconFilepath(false);
        } else {
            return databaseEntry.getIconFilepath(this.getGender().equals("Female"));
        }
    }

    public String getSpriteFilepath() throws java.io.IOException {
        String game = this.getGame();
        PokemonEntry databaseEntry = this.getDatabaseEntry();

        if(game.equals("Red") || game.equals("Blue") || game.equals("Yellow")) {
            return databaseEntry.getSpriteFilepath(false, false);
        } else {
            return databaseEntry.getSpriteFilepath(
                       this.getGender().equals("Female"),
                       this.isShiny()
                   );
        }
    }

    public java.awt.image.BufferedImage getIcon() throws java.io.IOException {
        return javax.imageio.ImageIO.read(new java.io.File(this.getIconFilepath()));
    }

    public java.awt.image.BufferedImage getSprite() throws java.io.IOException {
        return javax.imageio.ImageIO.read(new java.io.File(this.getSpriteFilepath()));
    }
%}

%ignore LIBPKMN_OT_ID;
%ignore LIBPKMN_OT_NAME;
%ignore from_file;
%ignore get_native_pc_data;
%ignore get_native_party_data;
%ignore get_icon_filepath;
%ignore get_sprite_filepath;
%include <pkmn/pokemon.hpp>
%template(Pokemon) pkmn::shared_ptr<pkmn::pokemon>;
