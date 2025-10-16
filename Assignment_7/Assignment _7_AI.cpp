#include <bits/stdc++.h>
using namespace std;

// A rule: antecedents -> consequent
struct Rule {
    vector<string> antecedents;
    string consequent;
};

bool forward_chaining(const unordered_set<string>& initial_facts,
                      const vector<Rule>& rules,
                      const string& query,
                      unordered_set<string>& derived_facts) {
    // Map each proposition to list of rules indices where it appears as an antecedent
    unordered_map<string, vector<int>> prop_to_rules;
    vector<int> count; // count[r] = number of antecedents of rule r not yet proven

    count.reserve(rules.size());
    for (int i = 0; i < (int)rules.size(); ++i) {
        count.push_back((int)rules[i].antecedents.size());
        for (const auto& a : rules[i].antecedents) {
            prop_to_rules[a].push_back(i);
        }
    }

    // facts known so far (start with initial facts)
    unordered_set<string> known = initial_facts;

    // agenda: facts to process (queue)
    queue<string> agenda;
    for (const auto &f : initial_facts) agenda.push(f);

    // Derived facts output
    derived_facts = known;

    while (!agenda.empty()) {
        string p = agenda.front(); agenda.pop();

        // For every rule that has p as an antecedent
        auto it = prop_to_rules.find(p);
        if (it == prop_to_rules.end()) continue;
        for (int r_index : it->second) {
            // decrement outstanding antecedents for rule r_index
            if (count[r_index] > 0) {
                --count[r_index];
            }
            // if all antecedents satisfied and consequent not yet known
            if (count[r_index] == 0) {
                const string &c = rules[r_index].consequent;
                if (known.find(c) == known.end()) {
                    // add new fact
                    known.insert(c);
                    derived_facts.insert(c);
                    agenda.push(c);
                    // If it's the query, success
                    if (c == query) return true;
                }
            }
        }
    }
    // query was not proved
    return (known.find(query) != known.end());
}

int main() {
    // Example: knowledge base
    // Facts: P, Q
    // Rules:
    // 1) P & Q -> R
    // 2) R -> S
    // 3) Q -> T
    unordered_set<string> facts = {"P", "Q"};

    vector<Rule> rules = {
        {{"P","Q"}, "R"},
        {{"R"}, "S"},
        {{"Q"}, "T"},
        {{"T","S"}, "U"} // extra rule to show chaining further
    };

    string query = "U"; // change query to test different goals

    unordered_set<string> derived;
    bool entailed = forward_chaining(facts, rules, query, derived);

    cout << "Derived facts:\n";
    for (const auto &f : derived) cout << "- " << f << '\n';

    cout << "\nIs \"" << query << "\" entailed by the KB? "
         << (entailed ? "YES" : "NO") << '\n';

    return 0;
}
