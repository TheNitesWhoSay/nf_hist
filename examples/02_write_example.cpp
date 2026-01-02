#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <cassert>
#include <iostream>
#include <string>

namespace _02
{

    struct Npc
    {
        std::string name = "";
        int hitpoints = 0;

        REFLECT(Npc, name, hitpoints)
    };

    void write_example()
    {
        auto smith = nf::make_tracked(Npc{.name = "jj", .hitpoints = 120});
        assert(smith->name == "jj" && smith->hitpoints == 120);
        smith()->name = "aj"; // You can make simple tracked changes using the ()-> syntax
        smith()->hitpoints = 150;
        assert(smith->name == "aj" && smith->hitpoints == 150);
        smith.print_change_history(std::cout); // You can see all data changes using print_change_history (or with more control via the renderer described later)
        // Note that standalone ()-> usage is often not best practice, mutators & explicit actions will cleanup syntax, group actions, and cut your overhead
    }

}
