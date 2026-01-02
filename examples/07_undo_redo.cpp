#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <cassert>

namespace _07
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

    void undo_redo()
    {
        Tracked_npc npc {};
        npc.full_restore(); // 100
        npc.hit(5); // 95
        npc.hit(4); // 91
        assert(npc->hitpoints == 91);
        
        npc.undo_action(); // You can undo any action made by nf_hist, returning your data to a previous state
        assert(npc->hitpoints == 95);
        
        npc.redo_action(); // You can redo actions you've recently undone, restoring the state you were at
        assert(npc->hitpoints == 91);
        
        npc.undo_action(); // 95
        npc.undo_action(); // 100
        assert(npc->hitpoints == 100); // You can go back or forward multiple steps
        npc.redo_action(); // 95
        npc.redo_action(); // 91
        assert(npc->hitpoints == 91);

        npc.hit(20);
        assert(npc->hitpoints == 71);
    }

}
