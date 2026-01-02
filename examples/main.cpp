#include <cassert>
#include <iostream>
#include <nf/hist.h>

namespace _01 { void read_example(); }
namespace _02 { void write_example(); }
namespace _03 { void action_example(); }
namespace _04 { void default_init(); }
namespace _05 { void untracked_init(); }
namespace _06 { void tracked_init(); }
namespace _07 { void undo_redo(); }
namespace _08 { void redo_elision(); }
namespace _09 { void edit_sub_elements(); }
namespace _10 { void using_selections(); }
namespace _11 { void change_notification_paths(); }
namespace _12 { void all_change_notifications(); }
namespace _13 { void attach_data(); }
namespace _14 { void user_defined_action_data(); }
namespace _15 { void render_actions(); }
namespace _16 { void create_action_labels(); }
namespace _17 { void optimize_history_size(); }
namespace _18 { void cap_history_size(); }

/*
    - 01 Create a tracked type and read from it
    - 02 Write changed to a tracked type & print change history
    - 03 Write changes to a tracked type using an action method
    - 04 Leverage default initialization
    - 05 Perform an untracked initialization
    - 06 Perform a tracked initialization
    - 07 Undo & redo actions
    - 08 Redo elision
    - 09 Editing sub-elements
    - 10 Using selections
    - 11 Simple path & change notification
    - 12 All change notifications
    - 13 Attach vector parallel to source data
    - 14 Add data to actions
    - 15 Render actions
    - 16 Create user-friendly action labels
    - 17 Optimize history size
    - 18 Cap history size

*/

int main()
{
    std::cout << "\n____ read example ____\n\n";
    _01::read_example();
    std::cout << "\n____ write example ____\n\n";
    _02::write_example();
    std::cout << "\n____ action example ____\n\n";
    _03::action_example();
    std::cout << "\n____ default init ____\n\n";
    _04::default_init();
    std::cout << "\n____ untracked init ____\n\n";
    _05::untracked_init();
    std::cout << "\n____ tracked init ____\n\n";
    _06::tracked_init();
    std::cout << "\n____ undo redo ____\n\n";
    _07::undo_redo();
    std::cout << "\n____ redo elision ____\n\n";
    _08::redo_elision();
    std::cout << "\n____ edit sub elements ____\n\n";
    _09::edit_sub_elements();
    std::cout << "\n____ using selections ____\n\n";
    _10::using_selections();
    std::cout << "\n____ change notification paths ____\n\n";
    _11::change_notification_paths();
    std::cout << "\n____ all change notifications ____\n\n";
    _12::all_change_notifications();
    std::cout << "\n____ attach data ____\n\n";
    _13::attach_data();
    std::cout << "\n____ user-defined action data ____\n\n";
    _14::user_defined_action_data();
    std::cout << "\n____ render actions ____\n\n";
    _15::render_actions();
    std::cout << "\n____ create action labels ____\n\n";
    _16::create_action_labels();
    std::cout << "\n____ optimize history size ____\n\n";
    _17::optimize_history_size();
    std::cout << "\n____ cap history size ____\n\n";
    _18::cap_history_size();
}
