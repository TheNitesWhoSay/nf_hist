#include <nf/hist.h>
#include <rarecpp/json.h>
#include <rarecpp/reflect.h>
#include <cassert>
#include <iostream>

namespace _04
{

    struct Npc_data
    {
        std::string name = "George"; // Inline initializers are one way of initializing your source data
        int hitpoints = 100;

        REFLECT(Npc_data, name, hitpoints)
    };

    struct Item_data
    {
        std::string label = "";
        int damage = 0;
        int hitcount = 0;

        Item_data() : label("Sword"), damage(12), hitcount(0) {} // Default constructors are another way

        REFLECT(Item_data, label, damage, hitcount)
    };

    struct Npc : nf::tracked<Npc_data, Npc>
    {
        Npc() : tracked(this) {}
    };

    struct Item : nf::tracked<Item_data, Item>
    {
        Item() : tracked(this)
        {
            // Before running actions you can instruct hist to remember inline/ctor initialization (or *gasp* undefined initial state) if needed
            record_init(); // e.g. to ensure replays are valid even if initializers have changed in the code
        }
    };

    void default_init()
    {
        Npc npc {};
        assert(npc->name == "George" && npc->hitpoints == 100 && // Source data is initialized via inline initializers
            npc.total_actions() == 0); // Using inline initializers is untracked
        std::cout << Json::out(*npc) << '\n';

        Item item {}; // Source data is initialized via ctor
        assert(item->label == "Sword" && item->damage == 12 && item->hitcount == 0 && // Source data is initialized via ctor
            item.total_actions() == 1); // Using ctor for initialization is untracked (but Item's ctor explicitly calls "record_init")
        std::cout << Json::out(*item) << '\n';
        item.print_change_history(std::cout);
    }

}
