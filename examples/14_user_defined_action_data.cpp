#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <iostream>

namespace _14
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

    struct Notes // Define some data structure you want attached to each action
    {
        std::string value;

        bool operator==(const Notes & other) const { return value == other.value; } // Need an operator== overload if your type doesn't automatically have one
    };

    struct Tracked_npc : nf::tracked<Npc, Tracked_npc, Notes> // Provide this data structure as the third template argument to nf::tracked
    {
        Tracked_npc() : tracked(this) {}

        void hit(int damage, std::string source)
        {
            auto edit = create_action({"Npc is getting hit by " + source});
            edit->hitpoints -= damage;
        }
    };

    void user_defined_action_data()
    {
        Tracked_npc npc {};
        npc.hit(12, "burn");
        npc.hit(20, "poison");
        
        for ( std::size_t i=0; i<npc.total_actions(); ++i )
            std::cout << npc.get_action_user_data(i).value << '\n';

        // This user data is associated with the action and can be accessed whenever via the action index (perhaps from an after_action notification)
        // It is strongly recommended to minimize the data you attach to actions as it will be present on every action for that tracked type
        // e.g. you might only attach an enum value instead of a string, and use that to give your action a user-friendly description
        // Otherwise attached data will become unnecessary size, if not also unnecessary time overhead
    }

}
