#include "organism.h"

Organism::Organism(const vector<DnaSlot>& dna_slots_) {
    for (const DnaSlot& dna_slot : dna_slots_) {
        dna_slots.push_back(dna_slot);
        type_last_param[dna_slot.slot_type] = dna_slot.slot_param;
    }
}

int Organism::get_upper_dna_num(const DnaSlot& dna_slot, std::map<string, int>& dna_nums, int constraint) {
    int y = 0;
    if (dna_slot.slot_type == "a") {
        for (int x = dna_nums["a"]; x>-1; x--) {
            y = dna_nums["a"] - x;
            if ( (dna_slot.slot_param * x + type_last_param["a"] * y + type_last_param["b"] * dna_nums["b"]) >= constraint )
                return x;
        }
    } else {
        for (int x = dna_nums["b"]; x>-1; x--) {
            y = dna_nums["b"] - x;
            if ( (dna_slot.slot_param * x + type_last_param["b"] * y + type_last_param["a"] * dna_nums["a"]) >= constraint )
                return x;
        }
    }
    return 0;
}

void Organism::random_init(std::map<string, int> dna_nums, int constraint) {
    int max_param = dna_slots.back().slot_param;
    int max_dna_num_of_param = 0;
    int dna_num_of_param = 0;
    for (DnaSlot& dna_slot : dna_slots) {
        max_dna_num_of_param = get_upper_dna_num(dna_slot, dna_nums, constraint);
        if (dna_slot.slot_param == type_last_param[dna_slot.slot_type]) {
            dna_num_of_param = max_dna_num_of_param;
        } else {
            dna_num_of_param = mean_random(0, max_dna_num_of_param);
        }
        dna_nums[dna_slot.slot_type] -= dna_num_of_param;
        constraint = max(constraint - dna_num_of_param * dna_slot.slot_param, 0);
        dna_slot.dna = dna_num_of_param;
    }
}

void Organism::random_init_unique(std::map<string, int> dna_nums, int constraint, std::map<vector<int>, int>& dna_cache) {
    std::vector<int> dna_key(dna_slots.size());
    random_init(dna_nums, constraint);
    for (int i = 0; i<dna_key.size(); i++) {
        dna_key[i] = dna_slots[i].dna;
    }

    int current_repeat = 0;
    while (dna_cache.find(dna_key) != dna_cache.end()) {
        repeat++;
        current_repeat += 1;
        random_init(dna_nums, constraint);
        for (int i=0; i<dna_key.size(); i++) {
            dna_key[i] = dna_slots[i].dna;
        }
    }
    std::cout << "find a unique dna_sequence" << std::endl;
    dna_cache.insert({dna_key, 1});
}

void Organism::print() {
    std::cout << "dna sequence(" << fitness["max_loss"] << "): ";
    for (const DnaSlot& dna_slot : dna_slots) {
        if (dna_slot.dna)
            std::cout << dna_slot.dna << "(" << dna_slot.slot_type << " " << dna_slot.slot_param << "),";
    }
    std::cout << std::endl;
}