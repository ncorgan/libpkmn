/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/database/pokemon_entry.hpp>
%}

%typemap(javacode) pkmn::database::pokemon_entry %{

    public boolean equals(PokemonEntry other) {
        if(this == other) {
            return true;
        } else if(other == null) {
            return false;
        }

        return (this.getName().equals(other.getName())) &&
               (this.getGame().equals(other.getGame())) &&
               (this.getGame().equals(other.getForm()));
    }

    @Override
    public boolean equals(Object other) {
        if(this == other) {
            return true;
        } else if(other == null) {
            return false;
        } else if(!(other instanceof PokemonEntry)) {
            return false;
        }

        return this.equals((PokemonEntry)other);
    }

    @Override
    public int hashCode() {
        org.apache.commons.lang3.builder.HashCodeBuilder hashCodeBuilder = new org.apache.commons.lang3.builder.HashCodeBuilder();
        hashCodeBuilder.append(this.getName());
        hashCodeBuilder.append(this.getGame());
        hashCodeBuilder.append(this.getForm());

        return hashCodeBuilder.toHashCode();
    }

    // TODO: extract images from JAR

    public java.awt.image.BufferedImage getIcon(
        boolean female
    ) throws java.io.IOException {
        return javax.imageio.ImageIO.read(new java.io.File(this.getIconFilepath(female)));
    }

    public java.awt.image.BufferedImage getSprite(
        boolean female, boolean shiny
    ) throws java.io.IOException {
        return javax.imageio.ImageIO.read(new java.io.File(this.getSpriteFilepath(female, shiny)));
    }

%}

%include <pkmn/database/pokemon_entry.hpp>
