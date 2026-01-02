#include <nf/hist.h>
#include <rarecpp/json.h>
#include <rarecpp/reflect.h>
#include <cassert>
#include <iostream>

namespace _04
{

    struct Npc
    {
        std::string name = "George"; // Inline initializers are one way of initializing your source data
        int hitpoints = 100;

        REFLECT(Npc, name, hitpoints)
    };

    struct Item
    {
        std::string label = "";
        int damage = 0;
        int hitcount = 0;

        Item() : label("Sword"), damage(12), hitcount(0) {} // Default constructors are another way

        REFLECT(Item, label, damage, hitcount)
    };

    struct Tracked_npc : nf::tracked<Npc, Tracked_npc>
    {
        Tracked_npc() : tracked(this) {}
    };

    struct Tracked_item : nf::tracked<Item, Tracked_item>
    {
        Tracked_item() : tracked(this)
        {
            // Before running actions you can instruct hist to remember inline/ctor initialization (or *gasp* undefined initial state) if you have a need to
            record_init(); // e.g. to ensure replays are valid even if initializers have changed in the code
        }
    };

    void default_init()
    {
        Tracked_npc npc {};
        assert(npc->name == "George" && npc->hitpoints == 100 && // Source data is initialized via inline initializers
            npc.total_actions() == 0); // Using inline initializers is untracked
        std::cout << Json::out(*npc) << '\n';

        Tracked_item item {}; // Source data is initialized via ctor (untracked)
        assert(item->label == "Sword" && item->damage == 12 && item->hitcount == 0 && // Source data is initialized via ctor
            item.total_actions() == 1); // Using ctor for initialization is untracked (but Tracked_items ctor explicitly calls "record_init")
        std::cout << Json::out(*item) << '\n';
        item.print_change_history(std::cout);
    }

}
