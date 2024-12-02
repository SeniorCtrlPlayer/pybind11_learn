// #pragma once
#ifndef GA_PARAM_
#define GA_PARAM_

#include <iostream>
#include <string>
#include <random>
#include <map>
#include <algorithm>
using namespace std;

unsigned static seed = 9638;
std::mt19937 static gen(seed);
int static repeat = 0;

int static mean_random(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

template<typename T>
void test(T a) {
    int b = 1;
}

class DnaSlot {
public:
    DnaSlot(string slot_type_, int slot_param_, double slot_delay_) : slot_type(slot_type_), slot_param(slot_param_), slot_delay(slot_delay_) {}
    
    DnaSlot(const DnaSlot& dna_slot) {
        std::cout << "copy constructor" << std::endl;
        slot_type = dna_slot.slot_type;
        slot_param = dna_slot.slot_param;
        slot_delay = dna_slot.slot_delay;
    }

    // DnaSlot(DnaSlot&& dna_slot) noexcept {
    //     slot_type = std::move(dna_slot.slot_type);
    //     std::cout << "move constructor" << std::endl;
    // }

    // DnaSlot& operator=(DnaSlot&&) = default;

    string slot_type;
    int slot_param;
    double slot_delay;
    int dna = 0;
};

class Organism {
public:
    Organism(const vector<DnaSlot>& dna_slots_);
    
    int get_upper_dna_num(const DnaSlot& dna_slot, std::map<string, int>& dna_nums, int constraint);
    
    void random_init(std::map<string, int> dna_nums, int constraint);
    
    void random_init_unique(
        std::map<string, int> dna_nums, int constraint,
        std::map<vector<int>, int>& dna_cache
    );
    
    void print();

    vector<DnaSlot> dna_slots;
    vector<int> dna;
    map<string, double> fitness;
    map<string, int> type_last_param;
};
#endif