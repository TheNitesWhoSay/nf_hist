#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <cstdint>
#include <iostream>

namespace _17
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

    struct OptimizedNpc
    {
        std::string name = "";
        int hitpoints = 50;

        NOTE(inventory, nf::index_size<std::uint8_t>)
        std::vector<Item> inventory {};

        REFLECT(OptimizedNpc, name, hitpoints, inventory)
    };

    static std::size_t sum_history_size(auto & tracked)
    {
        std::size_t total_size = 0;
        auto action_count = tracked.total_actions();
        for ( std::size_t i=0; i<action_count; ++i )
        {
            nf::rendered_action<> action {};
            tracked.render_action(i, action);
            total_size += action.byte_count;
        }
        return total_size;
    }

    void optimize_history_size()
    {
        auto npc = nf::make_tracked<Npc>(Npc{.inventory {
            Item { .label = "Bow", .value = 40.0f },
            Item { .label = "Arrows x50", .value = 10.0f }
        }});
        
        npc()->inventory[0].value *= 0.80f;
        npc()->inventory[1].value *= 0.75f;

        npc.print_change_history(std::cout);
        // If you peer into the bytes, you'll see that 8 bytes (or maybe 4/otherwise depending on your system/compilation settings)
        // Are being used to store the indexes [0] and [1] used in the actions above
        // This can add up to a lot, and unnecessarily if you know there is some maximum amount of items the inventory can hold (like 100)...
        // When that's the case you can use a smaller index (supplied in the NOTE on inventory using nf::index_size)
        std::cout << "Total size: " << sum_history_size(npc) << "\n\n";

        auto optimized_npc = nf::make_tracked<OptimizedNpc>(OptimizedNpc{.inventory {
            Item { .label = "Bow", .value = 40.0f },
            Item { .label = "Arrows x50", .value = 10.0f }
        }});
        
        optimized_npc()->inventory[0].value *= 0.80f;
        optimized_npc()->inventory[1].value *= 0.75f;

        optimized_npc.print_change_history(std::cout);
        // This, combined with removing redundant fields and using smaller size types in general, can greatly reduce your history size
        std::cout << "Total size: " << sum_history_size(optimized_npc) << '\n';
    }

}
