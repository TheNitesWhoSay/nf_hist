#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <cassert>
#include <iostream>

namespace _06
{

    struct Npc
    {
        std::string name = "";
        int hitpoints = 0;

        REFLECT(Npc, name, hitpoints)
    };

    void tracked_init()
    {
        auto npc = nf::make_tracked(Npc{});
        npc.init_data<true>(Npc{.name = "Jill", .hitpoints = 60}); // init_data<true> performs a tracked (re)initialization
        assert(npc->name == "Jill" && npc->hitpoints == 60 && npc.total_actions() == 1);

        npc.print_change_history(std::cout); // Tracked initializations remember the initial value in the change history
        // this isn't always necessary, e.g. live undo-redos will work fine without
        // but may be needed for something like data-history replays to start at the correct value after code-changes have occured
    }

}
