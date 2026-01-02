#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <iostream>

namespace _11
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

    struct Tracked_npc : nf::tracked<Npc, Tracked_npc> // Note: passing Tracked_npc allows nf to perform a compile-time check for methods like value_changed
    {
        Tracked_npc() : tracked(this) {} // Note: passing this allows nf to call member methods on this without polymorphism
        
        // You can create a representation of the path from the "root" of your data to some field using NF_PATH (auto-complete friendly)
        using name_path = NF_PATH(root->name);
        using hitpoints_path = NF_PATH(root->hitpoints);
        using inventory_path = NF_PATH(root->inventory);

        // Paths can go down levels further levels, when you pass through arrays/collections you put an index
        // What index you use doesn't matter, it's just syntactically representative of an array access operator
        using item_value_path = NF_PATH(root->inventory[0].value);

        // NF_PATH is an easier/more preferred way of writing nf::make_path
        using same_item_value_path = nf::make_path<decltype(root->inventory[0].value)>;

        // Paths are used primarily to receive change notifications, e.g.
        void element_added(inventory_path, std::size_t index) // nf can sense and call this method automatically (it's NOT using polymorphism)
        {
            std::cout << "item added to inventory: " << Json::out(read.inventory[index]) << '\n';
        }

        void value_changed(item_value_path path, const float & old_value, const float & new_value)
        {
            auto inventory_index = path.index<0>(); // If your path passes through arrays/collections you can get the index used from path
            std::cout << "inventory[" << inventory_index << "].value changed from " << old_value << " to " << new_value << '\n';
        }
    };

    void change_notification_paths()
    {
        Tracked_npc npc {};
        npc()->inventory.append(Item{.label = "Sword", .value = 40.0f});
        npc()->inventory.append(Item{.label = "Axe", .value = 31.5f});

        npc()->inventory[1].value = 25.0f;
    }

}
