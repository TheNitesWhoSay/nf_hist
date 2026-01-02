#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <iostream>

namespace _12
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
        int hitpoints = 50;
        std::vector<Item> inventory {};

        REFLECT(Npc, name, hitpoints, inventory)
    };

    struct Tracked_npc : nf::tracked<Npc, Tracked_npc>
    {
        Tracked_npc() : tracked(this) {}

        // Hist uses the C.A.R.M.S. data change notifications - change, add, remove, move and sel update; separately there is the after_action notification
        //   add, remove, and move notifications apply to container-mutating operations, while change applies to set operations
        //   including changing regular fields as well as elements within containers
        // You opt into a notification simply by adding the appropriate method, these methods have the signature and are made specific to fields using paths
        
        // void value_changed(PATH, OLD_VALUE, NEW_VALUE);
        // void element_added(PATH, INDEX);
        // void element_removed(PATH, INDEX);
        // void element_moved(PATH, OLD_INDEX, NEW_INDEX);
        // void selections_changed(PATH);
        // void after_action(size_t action_index);

        // You can supply the path inline (you also don't need to name the variable if it contains no random access indexes/keys you need to use)...
        void value_changed(NF_PATH(root->hitpoints), int old_hitpoints, int new_hitpoints) // Sent when the hitpoints value changes
        {
            std::cout << "Hitpoints changed from " << old_hitpoints << " to " << new_hitpoints << '\n';
        }

        // But it can be cleaner & better for multiple uses to provide an alias for paths
        using inventory_path = NF_PATH(root->inventory);
        using item_value_path = NF_PATH(root->inventory[0].value); // Reminder: the value given to array accesses in paths is meaningless

        void value_changed(item_value_path item, float old_value, float new_value)
        {
            auto item_index = item.index<0>();
            std::cout << "Item value at inventory index[" << item_index << "] changed from " << old_value << " to " << new_value << '\n';
        }

        void element_added(inventory_path, std::size_t index) // Sent when an Item is added to the inventory vector
        {
            const auto & item = read.inventory[index]; // Element at index is guarentee'd to still be available while this notification is processed
            std::cout << "Item added to inventory at index[" << index << "]: " << Json::out(item) << '\n';
        }

        void element_removed(inventory_path, std::size_t index) // Sent when an Item is removed from the inventory vector
        {
            const auto & item = read.inventory[index]; // Element will be removed *after* this notification is processed
            std::cout << "Removing item from inventory at index[" << index << "]: " << Json::out(item) << '\n';
        }

        void element_moved(inventory_path, std::size_t old_index, std::size_t new_index) // Sent when an Item is moved within the inventory vector
        {
            // Note that while the notifications come after the move, for better batch performance the order you get move notifications is not assured
            // Strongly consider using attached data and not notifications if your use case is covered by having a parallel array
            std::cout << "Inventory item moved from index " << old_index << " to " << new_index << '\n';
        }

        void selections_changed(inventory_path) // Sent when the items selected in the inventory change
        {
            std::cout << "Inventory selections changed: " << Json::out(view.inventory.sel()) << '\n';
            // Note that you may receive these notifications from ops that have potential to change selection, even if there were no substantive changes
        }

        void after_action(std::size_t action_index) // Sent after an action is submitted (when the action goes out of scope)
        {
            std::cout << "Action submitted: [" << action_index << "]\n";
            // This notification is especially useful together with action rendering & history trimming
        }

    };

    void all_change_notifications()
    {
        Tracked_npc npc {};
        npc.init_data(Npc{});

        npc()->hitpoints -= 20;
        npc()->inventory.append(Item{.label = "Bow", .value = 40.0f});
        npc()->inventory.append(Item{.label = "Arrows x50", .value = 10.0f});
        npc()->inventory.append(Item{.label = "Sword", .value = 50.0f});

        npc()->inventory.remove(1); // Remove arrows, note that you also get an element_moved notification, cause the sword moved down from index 2 to 1
        npc()->inventory.move_down(0); // Move the bow down 1 (and consequently the sword up 1)
        npc()->inventory.select(0); // Select the sword
    }

}
