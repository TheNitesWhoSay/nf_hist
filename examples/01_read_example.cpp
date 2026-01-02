#include <nf/hist.h>
#include <rarecpp/reflect.h>
#include <cassert>
#include <iostream>
#include <string>

namespace _01
{

    struct Npc // Create your own data structure
    {
        std::string name = "";
        int hitpoints = 0;

        REFLECT(Npc, name, hitpoints) // Before C++26, you'll need to reflect your struct using its name & field names
    };
    //REFLECT_PRIVATE(Npc, name, hitpoints) // If needed, you can reflect from outside a structure instead of inside

    void read_example()
    {
        auto jj = nf::make_tracked(Npc{.name = "jj", .hitpoints = 120}); // Make a tracked version of your data structure
        assert(jj.read.hitpoints == 120); // Read using the const & "read"
        assert(jj->hitpoints == 120); // Or using pointer semantics
        std::cout << "Hitpoints: " << jj->hitpoints << '\n';
    }

}