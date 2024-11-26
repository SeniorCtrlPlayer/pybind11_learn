#include "organism.h"

// template <class T> void swap (T& a, T& b)
// {
//   T c(std::move(a)); a=std::move(b); b=std::move(c);
// }

class GA {
public:
    void Init(vector<DnaSlot> dna_slots_, int pop_size_, int rest_pop_size_, map<string, int> resources_map_, vector<pair<int, double>> target_) {
        pop_size = pop_size_;
        rest_pop_size = rest_pop_size_;
        for (const DnaSlot& dna_slot : dna_slots_) {
            dna_slots.emplace_back(dna_slot);
        }
        target = target_;
        resources_map = resources_map_;

        for (int i=0; i<pop_size; i++) {
            pops.emplace_back(dna_slots);
        }
        // for (const Organism& organism : pops) {
        for (Organism& organism : pops) {
            organism.random_init_unique(resources_map, target.size(), dna_cache);
        }
    }
    
    void translate_dna(const Organism& organism, vector<int> &behaviors) {
        behaviors.clear();
        int attribute_of_dna = 0;
        for (const DnaSlot& dna_slot : organism.dna_slots) {
            if ( dna_slot.dna == 0 )
                continue;
            attribute_of_dna = dna_slot.slot_param * dna_slot.dna;
            behaviors.insert(behaviors.end(), attribute_of_dna, dna_slot.slot_param);
        }


    }

    void calculate_fitness() {
        vector<int> behaviors;
        vector<int> dna_key(dna_slots.size());
        // for (const Organism& organism : pops) {
        for (Organism& organism : pops) {
            translate_dna(organism, behaviors);
            double max_loss = -100000;
            double error = 0;
            for (int i=0; i<target.size(); i++) {
                error = target[i].second - behaviors[i];
                max_loss = max(max_loss, error);
            }
            organism.fitness["max_loss"] = max_loss;
            organism.fitness["capacity"] = behaviors.size();
            for (int i=0; i<dna_key.size(); i++)
                dna_key[i] = organism.dna_slots[i].dna;
            dna_cache.insert({dna_key, 1});
        }
    }
    
    void evolve() {
        int constraint = target.size();
        // for (const Organism& organism : pops)
        for (Organism& organism : pops)
            organism.random_init_unique(resources_map, constraint, dna_cache);
        calculate_fitness();
    }

    void assignment() {
        std::cout << "repeat time: " << repeat << std::endl;
    }

    vector<DnaSlot> dna_slots;
    vector<pair<int, double>> target;
    int pop_size;
    int rest_pop_size;

    vector<Organism> pops;
    map<string, int> resources_map;
    map<vector<int>, int> dna_cache;
};

int main() {
    vector<int> candidate_dna_set_a = {2};
    vector<int> candidate_dna_set_b;
    for (int i=8; i<128; i+=8)
        candidate_dna_set_a.push_back(i);
    for (int i=32; i<512; i+=32)
        candidate_dna_set_b.push_back(i);
    vector<DnaSlot> dna_slots;
    for (const int& a : candidate_dna_set_a)
        dna_slots.emplace_back("a", a, a * 0.625 + 30);
    for (const int& b : candidate_dna_set_b)
        dna_slots.emplace_back("b", b, b * 0.078125 + 35);
    std::cout << "=====================" << std::endl;
    std::stable_sort(
        dna_slots.begin(), dna_slots.end(),
        [](const DnaSlot& a, const DnaSlot& b) {
            if (a.slot_delay == b.slot_delay)
                return a.slot_param < b.slot_param;
            return a.slot_delay < b.slot_delay;
        }
    );

    map<string, int> resources_map = {{"a", 23}, {"b", 4}};
    
    int pop_size = 50;
    vector<std::pair<int, double>> target;
    int i=0;
    for (; i<111; )
        target.push_back(std::pair<int, double>(i++, 4.025));
    target.push_back(std::pair<int, double>(i++, 4.16731));
    target.push_back(std::pair<int, double>(i++, 4.92429));
    target.push_back(std::pair<int, double>(i++, 4.92429));
    target.push_back(std::pair<int, double>(i++, 6.40026));
    target.push_back(std::pair<int, double>(i++, 6.40755));
    target.push_back(std::pair<int, double>(i++, 6.41138));
    target.push_back(std::pair<int, double>(i++, 6.41139));
    for (int j=0; j<8; j++)
        target.push_back(std::pair<int, double>(i++, 6.41535));
    for (int j=0; j<18; j++)
        target.push_back(std::pair<int, double>(i++, 6.41943));
    for (int j=0; j<83; j++)
        target.push_back(std::pair<int, double>(i++, 6.42362));

    GA gaEngine;
    gaEngine.Init(dna_slots, pop_size, 1, resources_map, target);
    int generation = 10;
    for (int i=0; i<generation; i++)
        gaEngine.evolve();
    gaEngine.assignment();
}