#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>

namespace _13
{

    struct Item
    {
        std::string label = "";
        float value = 0.0f;

        REFLECT(Item, label, value)
    };

    struct Item_rendering
    {
        int frame = 0;
        int x_offset = 0;
        int y_offset = 0;
        std::vector<std::uint8_t> texture {};

        ~Item_rendering()
        {
            //destroy_texture(*this);
            std::cout << "Graphics data cleaned up\n";
        }

        REFLECT(Item_rendering, frame, x_offset, y_offset, texture)
    };

    struct Npc_data
    {
        std::string name = "";
        int hitpoints = 50;

        // You can use annotations (described in RareCpp's documentation) to indicate (at compile time) that you want this field treated somehow differently
        NOTE(inventory, nf::attach_data<std::unique_ptr<Item_rendering>>) // Attach_data instructs nf to maintain a parallel array of type, to inventory
        std::vector<Item> inventory {};

        REFLECT(Npc_data, name, hitpoints, inventory)
    };

    struct Npc : nf::tracked<Npc_data, Npc>
    {
        Npc() : tracked(this) {}

        void element_added(NF_PATH(root->inventory), std::size_t index)
        {
            const Item & item = read.inventory[index]; // This is a parallel array, so the item at index...
            std::unique_ptr<Item_rendering> & rendering = view.inventory.attached_data(index); // Matches the attached data at index
            rendering = std::make_unique<Item_rendering>();
            rendering->frame = rand();
            rendering->x_offset = int(index)*2;
            rendering->y_offset = 0;
            //load_texture(rendering.texture);
            std::cout << "Initialized graphics for " << item.label << "\n";
        }
    };
    
    void attach_data()
    {
        Npc npc {};
        auto edit = npc.create_action();
        edit->inventory.append(Item{.label = "Sword"});
        edit->inventory.append(Item{.label = "Bow"});
        edit->inventory.append(Item{.label = "Axe"});
        
        assert(npc->inventory[0].label == "Sword" && npc.view.inventory.read_attached_data()[0]->x_offset == 0);
        assert(npc->inventory[1].label == "Bow" && npc.view.inventory.read_attached_data()[1]->x_offset == 2);
        assert(npc->inventory[2].label == "Axe" && npc.view.inventory.read_attached_data()[2]->x_offset == 4);
        
        std::cout << "Removing item\n";
        edit->inventory.remove(1); // Remove the bow, axe and its attached data should fall down to index 1, maintaining their parallel-array relationship
        std::cout << "Removal complete\n";
        assert(npc->inventory[0].label == "Sword" && npc.view.inventory.read_attached_data()[0]->x_offset == 0);
        assert(npc->inventory[1].label == "Axe" && npc.view.inventory.read_attached_data()[1]->x_offset == 4);
        std::cout << "Example function finished/remaining items going out of scope\n";
    }

}
