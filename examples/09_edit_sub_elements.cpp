#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <cassert>

namespace _09
{

    struct Item
    {
        std::string label = "";
        float value = 0.0f;

        REFLECT(Item, label, value)
    };

    struct Npc
    {
        std::string name = "";
        int hitpoints = 0;
        std::vector<Item> inventory {};

        REFLECT(Npc, name, hitpoints, inventory)
    };

    struct Tracked_npc : nf::tracked<Npc, Tracked_npc>
    {
        Tracked_npc() : tracked(this) {}

        // If you need mutable, non-const sub-elements from your root data...
        struct Edit_item : nf::tracked_element<Item, NF_PATH(root->inventory[0])> // Create a type representing a sub-element of your data
        {
            using tracked_element::tracked_element;

            void decrease_value(float percentage) // Add some non-const mutator methods
            {
                edit.label += "*";
                edit.value -= read.value * percentage;
            }
        };

        Edit_item edit_item(std::size_t index) // Add a method to get a particular sub-element
        {
            return Edit_item(this, view.inventory[index]);
        }
    };
    
    void edit_sub_elements()
    {
        Tracked_npc npc {};
        npc.init_data(Npc{.name = "Shopkeeper", .hitpoints = 99, .inventory {
            Item { .label = "Bow", .value = 40.0f },
            Item { .label = "Arrows x50", .value = 10.0f }
        }});

        // When you have a tree of items... it's easy to read from items further down with the .read const ref
        assert(npc->inventory[0].label == "Bow");
        
        // And not hard to write to the individual fields that are further down
        npc()->inventory[0].label = "Willow Bow";
        assert(npc->inventory[0].label == "Willow Bow");

        // However, writing out the full path from your data root and making changes to individual fields is not always reasonable
        // When data gets complicated enough, being able to grab one non-const element, maybe pass it around, and use mutator methods is essential
        // This is the use-case for nf::sub_elements
        auto bow = npc.edit_item(0);
        auto arrows = npc.edit_item(1);
        
        assert(!npc->inventory[0].label.ends_with("*") && npc->inventory[0].value == 40.0f);
        assert(!npc->inventory[1].label.ends_with("*") && npc->inventory[1].value == 10.0f);
        bow.decrease_value(0.02f);
        arrows.decrease_value(0.033f);
        assert(npc->inventory[0].label.ends_with("*") && npc->inventory[0].value == 40.0f - 40.0f*0.02f);
        assert(npc->inventory[1].label.ends_with("*") && npc->inventory[1].value == 10.0f - 10.0f*0.033f);
    }

}
