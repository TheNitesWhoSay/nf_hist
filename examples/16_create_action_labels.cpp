#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <iostream>

namespace _16
{

    enum class Descriptor
    {
        none,
        pickup_item,
        recieve_trade_item,
        drop_item,
        burn_damage_tick,
        poison_damage_tick
    };

    struct Action_descriptor
    {
        Descriptor descriptor = Descriptor::none;
        constexpr Action_descriptor() noexcept = default;
        constexpr Action_descriptor(Descriptor descriptor) : descriptor(descriptor) {}
        friend constexpr bool operator==(const Action_descriptor & lhs, const Action_descriptor & rhs) noexcept { return lhs.descriptor == rhs.descriptor; }
    };

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

    struct Tracked_npc : nf::tracked<Npc, Tracked_npc, Action_descriptor>
    {
        Tracked_npc() : tracked(this) {}

        void pickup_item(const Item & item)
        {
            auto edit = create_action(Descriptor::pickup_item);
            edit->inventory.append(item);
        }

        void recieve_trade_item(const Item & item)
        {
            auto edit = create_action(Descriptor::recieve_trade_item);
            edit->inventory.append(item);
        }

        void drop_item(std::size_t index)
        {
            if ( index < read.inventory.size() )
            {
                auto edit = create_action(Descriptor::drop_item);
                edit->inventory.remove(index);
            }
        }

        void apply_burn_tick()
        {
            auto edit = create_action(Descriptor::burn_damage_tick);
            edit->hitpoints -= 12;
        }

        void apply_poison_tick()
        {
            auto edit = create_action(Descriptor::poison_damage_tick);
            edit->hitpoints -= 20;
        }

        void process_tick()
        {
            apply_burn_tick();
            apply_poison_tick();
        }

        void after_action(std::size_t index)
        {
            auto descr = get_action_user_data(index).descriptor;
            switch ( descr ) // Derive the label from the descriptor associated with the action
            {
                case Descriptor::pickup_item: std::cout << "Picked up item\n"; break;
                case Descriptor::recieve_trade_item: std::cout << "Received item\n"; break;
                case Descriptor::drop_item: std::cout << "Dropped item\n"; break;
                case Descriptor::burn_damage_tick: std::cout << "Burn damage\n"; break;
                case Descriptor::poison_damage_tick: std::cout << "Poison damage\n"; break;
				case Descriptor::none: break;
            }

            nf::rendered_action<Action_descriptor> action {}; // Can combine with rendering to make a fuller/more informative display
            render_action(index, action, true);
            for ( auto & event : action.change_events )
                std::cout << "  " << event.summary << '\n';

            std::cout << '\n';
        }
    };

    void create_action_labels()
    {
        // By default, hist can provide you a very nice technical rendering of what changed in an action
        // But that is very different from providing a user-friendly description (/"label") for an action, which you can do with action user data
        Tracked_npc npc {};
        npc.pickup_item(Item{.label = "Bones", .value = 0.5f});
        npc.recieve_trade_item(Item{.label = "Sword", .value = 50.0f});
        npc.drop_item(0);
        npc.process_tick();
    }

}
