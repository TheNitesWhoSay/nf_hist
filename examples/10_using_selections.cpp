#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <cassert>

namespace _10
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
        int hitpoints = 0;
        std::vector<Item> inventory {};

        REFLECT(Npc, name, hitpoints, inventory)
    };

    struct Tracked_npc : nf::tracked<Npc, Tracked_npc>
    {
        Tracked_npc() : tracked(this) {}
    };

    void using_selections()
    {
        Tracked_npc npc {};
        npc.init_data(Npc{.inventory {
            Item { .label = "Bow", .value = 40.0f },
            Item { .label = "Arrows x50", .value = 10.0f },
            Item { .label = "Sword", .value = 50.0f }
        }});

        auto edit = npc.create_action();
        edit->inventory.select({0, 2}); // Items in arrays/collections can be "selected", selection changes are tracked as part of history
        const auto expected_sel = std::vector<std::size_t>{0, 2};

        assert(npc.view.inventory.sel() == expected_sel); // You can read which items are selected using .view.path.to.elem.sel()

        // You can modify selections with a variety of methods
        edit->inventory.clear_selections();
        edit->inventory.select_all();
        edit->inventory.toggle_selected(1);
        edit->inventory.deselect(2);
        edit->inventory.sort_selection();
        assert(npc.view.inventory.sel() == std::vector<std::size_t>{0});

        // You can make perform assignment on all the items in a selection
        edit->inventory.selection().value = 5.0f;

        // Or make other changes
        edit->inventory.move_selections_bottom(); // Move the selected items to the "bottom" of the list/container
        edit->inventory.remove_selection(); // Delete selected items

        // Adding/removing/moving other items to/from/in a container, or undoing/redoing such actions automatically syncs such changes to the selection
        // Use selections when "selecting" something is meaningful for your application, e.g. selecting a bunch of items for bulk purchase/summing values
        // nf's selections are sometimes cheaper (in runtime/hist size) and ~always easier than floating your own (and manually syncing changes/undos/redos)
    }

}