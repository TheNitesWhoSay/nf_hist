#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <cassert>
#include <iostream>

namespace _08
{
    
    struct Npc
    {
        std::string name = "";
        int hitpoints = 0;
        int mana = 0;

        REFLECT(Npc, name, hitpoints, mana)
    };

    struct Tracked_npc : nf::tracked<Npc, Tracked_npc>
    {
        Tracked_npc() : tracked(this) {}

        void full_restore()
        {
            auto edit = create_action();
            edit->hitpoints = 100;
            edit->mana = 100;
        }

        void hit(int damage)
        {
            create_action()->hitpoints = damage > read.hitpoints ? 0 : read.hitpoints - damage;
        }
    };

    void redo_elision()
    {
        Tracked_npc npc {};
        npc.full_restore(); // 100
        npc.hit(5); // 95
        npc.hit(4); // 91
        assert(npc->hitpoints == 91);

        npc.undo_action(); // 95
        npc.undo_action(); // 100
        assert(npc->hitpoints == 100);

        // If redos are still available when you perform a new action...
        npc.hit(20);
        assert(npc->hitpoints == 80);

        // The redos are "elided" - made unavailable for access by undos and redos
        npc.redo_action(); // No redos are available, so no change
        assert(npc->hitpoints == 80);

        // Those elided actions are still part of the change history (which is append-only), just no longer directly accessible via undo-redo
        npc.print_change_history(std::cout);
    }

}
