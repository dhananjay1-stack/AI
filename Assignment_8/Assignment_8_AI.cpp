#include <bits/stdc++.h>
using namespace std;

struct Rule {
    vector<string> premises;
    string conclusion;
};

bool backwardChaining(string goal, vector<Rule>& rules, set<string>& facts, set<string>& visited) {
    if (facts.count(goal)) return true;
    if (visited.count(goal)) return false;
    visited.insert(goal);
    for (auto& rule : rules) {
        if (rule.conclusion == goal) {
            bool ok = true;
            for (auto& p : rule.premises) {
                if (!backwardChaining(p, rules, facts, visited)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                facts.insert(goal);
                return true;
            }
        }
    }
    return false;
}

int main() {
    vector<Rule> rules = {
        {{"A","B"}, "C"},
        {{"C","D"}, "E"},
        {{"B"}, "D"}
    };
    set<string> facts = {"A","B"};
    string goal = "E";
    set<string> visited;
    if (backwardChaining(goal, rules, facts, visited))
        cout << "Goal " << goal << " is proven.\n";
    else
        cout << "Goal " << goal << " cannot be proven.\n";
}
