#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <cassert>
#include <iostream>

namespace _18
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

        std::size_t max_action_count = 10;
        std::size_t max_hist_bytes = 300;
        std::size_t running_byte_count = 0; // You can keep track of how much space hist is using

        void after_action(std::size_t index)
        {
            nf::rendered_action<> action {};
            render_action(index, action, false);
            running_byte_count += action.byte_count;
            if ( running_byte_count > max_hist_bytes ) // If it exceeds your size limit
                running_byte_count = trim_history_to_size(max_hist_bytes*4/5); // Trim it to a smaller size (use a ratio to not run this after *every* action)
            else if ( total_actions() > max_action_count ) // If it exceeds your action count limit
            {
                trim_history(max_action_count/5); // Trim to a smaller action count
                running_byte_count = 0;
                auto action_count = total_actions();
                for ( std::size_t i=0; i<action_count; ++i )
                {
                    nf::rendered_action<> act {};
                    render_action(i, act, false);
                    running_byte_count += act.byte_count;
                }
            }

            // Trimming will remove your earliest actions first and will always remove elided actions in blocks
        }
    };

    void cap_history_size()
    {
        Tracked_npc npc {};
        npc()->hitpoints += 1;
        npc()->hitpoints += 1;
        npc()->hitpoints += 1;
        npc()->hitpoints += 1;
        npc()->hitpoints += 1;
        npc()->hitpoints += 1;
        npc()->hitpoints += 1;
        npc()->hitpoints += 1;
        npc()->hitpoints += 1;
        npc()->hitpoints += 1;
        assert(npc.total_actions() == 10);
        npc()->hitpoints += 1;
        assert(npc.total_actions() == 9);
        npc()->hitpoints += 1;
        assert(npc.total_actions() == 10);
        npc()->hitpoints += 1;
        assert(npc.total_actions() == 9);
        npc()->inventory.append(std::vector<Item>{Item{}, Item{}, Item{}, Item{}, Item{}, Item{}, Item{}, Item{}, Item{}, Item{}, Item{}});
        assert(npc.total_actions() < 9); // Should have exceeded size limit
        npc.print_change_history(std::cout);
    }

}
