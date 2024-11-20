import numpy as np
import random
random.seed(9638)
np.random.seed(9638)
import pandas as pd
from copy import deepcopy

repeat = 0

# sig_ratio = np.array([
sig_ratio = \
    [4.025] * 111 + \
    [4.16731, 4.92429, 4.92429, 6.40026, 6.40755, 6.41138, 6.41139] + \
    [6.41535] * 8 + \
    [6.41943] * 18 + \
    [6.42362] * 83
# ])

class DnaSlot:
    def __init__(self, ctype_, ratio_, delay_):
        self.ctype = ctype_
        self.ratio = ratio_
        self.delay = delay_
        self.dna: int = 0

class Organism:
    def __init__(self, dna_slots_):
        self.dna_slots = dna_slots_
        self.dna = []
        self.fitness = {}
        self.ctype_max_ratio = {}
        for dna_slot in self.dna_slots:
            self.ctype_max_ratio[dna_slot.ctype] = dna_slot.ratio
    def get_upper_channel_num(self, dna_slot_, a, b, s):
        y = 0
        if dna_slot_.ctype == 'a':
            for x in range(a, -1, -1):
                y = a - x
                if (dna_slot_.ratio * x + self.ctype_max_ratio['a'] * y + self.ctype_max_ratio['b'] * b) >= s:
                    return x
        else:
            for x in range(b, -1, -1):
                y = b - x
                if (dna_slot_.ratio * x + self.ctype_max_ratio['b'] * y + self.ctype_max_ratio['a'] * a) >= s:
                    return x
        return 0

    def random_init(self, resources, s):
        ctype_nums = deepcopy(resources)
        max_channel_num_of_ratio = 0
        channel_num_of_ratio = 0
        for dna_slot in self.dna_slots:
            max_channel_num_of_ratio = self.get_upper_channel_num(dna_slot, ctype_nums['a'], ctype_nums['b'], s)
            if (dna_slot.ratio == self.ctype_max_ratio[dna_slot.ctype]):
                channel_num_of_ratio = max_channel_num_of_ratio
            else:
                channel_num_of_ratio = int(np.random.choice(range(0, max_channel_num_of_ratio + 1)))
            ctype_nums[dna_slot.ctype] -= channel_num_of_ratio
            s = max(s - channel_num_of_ratio * dna_slot.ratio, 0)
            dna_slot.dna = channel_num_of_ratio

    def random_init_unique(self, ctype_nums, s, dna_cache):
        self.random_init(ctype_nums, s)
        dna_key = tuple([dna_slot.dna for dna_slot in self.dna_slots])
        repeat = 0
        while dna_key in dna_cache:
            repeat += 1
            self.random_init(ctype_nums, s)
            dna_key = tuple([dna_slot.dna for dna_slot in self.dna_slots])
        dna_cache[dna_key] = 1

    def print(self):
        print(f"dna sequence( {self.fitness} ): ", end='')
        for dna_slot in self.dna_slots:
            if dna_slot.dna:
                print(f"{dna_slot.dna}({dna_slot.ctype} {dna_slot.ratio}),", end='')
        print("")

class GA:
    def __init__(self, dna_slots_ : list[DnaSlot], pop_size_ : int, rest_pop_size_ : int, resources_, target_ : list[tuple]):
        self.init = True
        self.target = target_
        self.pop_size = pop_size_
        self.rest_pop_size = rest_pop_size_
        self.loser_start = self.pop_size - self.rest_pop_size
        self.dna_slots = dna_slots_
        self.resources = resources_
        self.pops : list[Organism] = []
        self.dna_cache = {}
        for i in range(self.pop_size):
            self.pops.append(Organism(self.dna_slots))
        for i in range(self.pop_size):
            self.pops[i].random_init_unique(self.resources, len(self.target), self.dna_cache)
        self.calculate_fitness()
        self.pops.sort(key=lambda organism: (organism.fitness['max_loss'], organism.fitness['capacity']))

    def translate_dna(self, organism, behavior_ratios):
        behavior_ratios.clear()
        sig_num_of_ratio = 0
        for dna_slot in organism.dna_slots:
            if dna_slot.dna == 0:
                continue
            sig_num_of_ratio = dna_slot.ratio * dna_slot.dna
            behavior_ratios.extend([dna_slot.ratio] * sig_num_of_ratio)

    def calculate_fitness(self):
        behavior_ratios = []
        pops = self.pops if self.init else self.pops[self.loser_start:]
        self.init = False if self.init else False

        for organism in pops:
            self.translate_dna(organism, behavior_ratios)
            max_loss = -100000
            error = 0
            for i, (origin_i, t) in enumerate(self.target):
                error = t - behavior_ratios[i]
                max_loss = max(max_loss, error)
            organism.fitness['max_loss'] = max_loss
            organism.fitness['capacity'] = len(behavior_ratios)
            dna_key = tuple([dna_slot.dna for dna_slot in self.dna_slots])
            self.dna_cache[dna_key] = 1

    def evolve(self):
        s = len(self.target)
        for organism in self.pops[self.loser_start:]:
            organism.random_init_unique(self.resources, s, self.dna_cache)
        self.calculate_fitness()
        self.pops.sort(key=lambda organism: (organism.fitness['max_loss'], organism.fitness['capacity']))
        self.pops[0].print()

    def assignment(self):
        print(f"repeat time: {repeat}")
        pass

candidate_ratios_a = [2] + list(range(8, 128, 8))
candidate_ratios_b = list(range(32, 512, 32))
dna_slots = []
for ratio in candidate_ratios_a:
    dna_slots.append(DnaSlot('a', ratio, ratio * 0.625 + 30))
for ratio in candidate_ratios_b:
    dna_slots.append(DnaSlot('b', ratio, ratio * 0.078125 + 35))
dna_slots.sort(key=lambda x:(x.delay, x.ratio))

pop_size = 50
sig_ratio = list(enumerate(sig_ratio))
gaEngine = GA(dna_slots, pop_size, pop_size//2, {"a":10, "b":5}, sig_ratio)
generation = 10
for i in range(generation):
    gaEngine.evolve()
gaEngine.assignment()