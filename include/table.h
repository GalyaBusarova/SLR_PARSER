#include <vector>
#include <map>

#include "lr0.h"
#include "grammar.h"

// тип action
enum ActionType {
    SHIFT,
    REDUCE,
    ACCEPT,
    ERROR
};

struct Action {
    ActionType type;
    int value;  // для shift: номер состояния, для reduce: номер правила
    
    Action(ActionType t = ERROR, int v = 0) : type(t), value(v) {}
};

// создание таблицы action: [состояние][терминал] -> действие
std::vector<std::vector<Action>> make_action_table(const std::vector<lr0_state>& lr0_automation, const std::vector<Production>& ExtendedGrammar);

// функция для построения goto-таблицы, для каждого состояния и нетерминала: GOTO[i][B] = nextStateIndex
std::vector<std::map<std::string, int>> make_goto_table(const std::vector<lr0_state>& lr0_automation, const std::vector<Production>& ExtendedGrammar);