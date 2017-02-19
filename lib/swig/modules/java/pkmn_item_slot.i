/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include <pkmn/item_slot.hpp>
%}

/*
 * Doxygen documentation
 */

%javamethodmodifiers pkmn::item_slot::getItem "
/**
 * Get the item in this slot.
 */
public";

%javamethodmodifiers pkmn::item_slot::getAmount "
/**
 * Get the amount of the item in this slot.
 */
public";

%javamethodmodifiers pkmn::item_slot::setItem "
/**
 * Set the item in this slot.
 *
 * @param newItem new item
 */
public";

%javamethodmodifiers pkmn::item_slot::setAmount "
/**
 * Set the amount of the item in this slot.
 *
 * @param newAmount new amount
 */
public";

%typemap(javacode) pkmn::database::levelup_move %{

    public boolean equals(ItemSlot other) {
        if(this == other) {
            return true;
        } else if(other == null) {
            return false;
        }

        return (this.getItem().equals(other.getItem())) &&
               (this.getAmount() == other.getAmount());
    }

    @Override
    public boolean equals(Object other) {
        if(this == other) {
            return true;
        } else if(other == null) {
            return false;
        } else if(!(other instanceof ItemSlot)) {
            return false;
        }

        return this.equals((ItemSlot)other);
    }

    @Override
    public int hashCode() {
        org.apache.commons.lang3.builder.HashCodeBuilder hashCodeBuilder = new org.apache.commons.lang3.builder.HashCodeBuilder();
        hashCodeBuilder.append(this.getItem());
        hashCodeBuilder.append(this.getAmount());

        return hashCodeBuilder.toHashCode();
    }

%}

/*
 * Manually write the get/set functions so we can individually
 * document them.
 */
namespace pkmn {

class item_slot {
    public:
        %extend {
            const pkmn::database::item_entry& getItem() {
                return self->item;
            }
            int getAmount() {
                return self->amount;
            }

            void setItem(const pkmn::database::item_entry &newItem) {
                self->item = newItem;
            }
            void setAmount(const int newAmount) {
                self->amount = newAmount;
            }
        }
};

typedef std::vector<item_slot> item_slots_t;

}

%include <java/stl_macros.i>
PKMN_JAVA_VECTOR(pkmn::item_slot, ItemSlot, ItemSlotVector)
