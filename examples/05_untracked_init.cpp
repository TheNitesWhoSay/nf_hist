#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <cassert>

namespace _05
{

    struct Npc
    {
        std::string name = "";
        int hitpoints = 0;

        REFLECT(Npc, name, hitpoints)
    };

    void untracked_init()
    {
        auto npc = nf::make_tracked(Npc{});
        npc()->name = "Jack";
        assert(npc->name == "Jack" && npc->hitpoints == 0 && npc.total_actions() == 1);
        npc.clear_history(); // If edit history is not empty you cannot use init_data, clear_history can be used if needed

        npc.init_data<false>(Npc{.name = "Bill", .hitpoints = 50}); // init_data<false> performs an untracked (re)initialization
        assert(npc->name == "Bill" && npc->hitpoints == 50 && npc.total_actions() == 0);
    }

}
