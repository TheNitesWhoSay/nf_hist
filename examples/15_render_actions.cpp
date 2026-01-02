#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <iostream>

namespace _15
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
    
    void render_actions()
    {
        auto npc = nf::make_tracked<Npc>({});
        npc()->hitpoints = 40;
        npc()->hitpoints = 30;

        npc.print_change_history(std::cout); // You can print your change history.. but this is mainly for debugging purposes
        // In a real application you probably don't want to rely on nfs rigid formatting as well as always including every action
        
        auto action_count = npc.total_actions();
        for ( std::size_t i=0; i<action_count; ++i )
        {
            nf::rendered_action<> action {};
            npc.render_action(i, action, true); // "Rendering" gives you control over which actions/events to render and lets you browse details
            
            switch ( action.status )
            {
                case nf::action_status::undoable: std::cout << "Action[" << i << "] (undoable)\n"; break;
                case nf::action_status::redoable: std::cout << "Action[" << i << "] (redoable)\n"; break;
                case nf::action_status::elided_redo: std::cout << "Action[" << i << "] (elided_redo)\n"; break;
				case nf::action_status::unknown: break;
            }
            std::cout << "  byte_count: " << action.byte_count << '\n';
            std::cout << "  user_data: " << Json::out(action.user_data) << '\n';
            std::cout << "  events: {\n";
            for ( const auto & event : action.change_events )
            {
                std::cout << "    op: " << int(event.operation) << '\n';
                std::cout << "    summary: " << event.summary << "\n  }\n";
            }
        }
    }

}
