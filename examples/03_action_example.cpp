#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <cassert>
#include <iostream>
#include <string>

namespace _03
{

    struct Npc
    {
        std::string name = "";
        int hitpoints = 0;

        REFLECT(Npc, name, hitpoints)
    };

    // Usually you want to create a tracked version of your structure by extending nf::tracked<SOURCE_DATA_TYPE, TRACKED_TYPE>
    struct Tracked_npc : nf::tracked<Npc, Tracked_npc>
    {
        Tracked_npc() : tracked(this) {} // Pass your "this" pointer to nf::tracked (allows nf_hist to call notification methods, covered later) 

        void generate_character() // Now you can add methods to your tracked type
        {
            auto edit = create_action(); // Create an action which groups together some data changes
            edit->name = "jj";
            edit->hitpoints = rand()%100+1;
            assert(read.name == "jj" && read.hitpoints > 0); // Note that the data changes happen instantly
            // The action, however, isn't submitted to the change history until the action ("edit") goes out of scope
        }

    };

    void action_example()
    {
        Tracked_npc npc {};
        npc.generate_character();
        npc.generate_character();
        npc.print_change_history(std::cout);
        // Note that you're now adding a single action to the change history for every call to generate_character
        // And the two data change events involved in generate_character get added under one action
        // This saves a little time & space over having separate actions for ever data change
        // And as we'll see in the future, this also makes them a single, undoable, label-able unit of work
    }

}
