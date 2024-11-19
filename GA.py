import numpy as np
import random
random.seed(9638)
np.random.seed(9638)
import pandas as pd

candidate_ratios = [2] + list(range(8, 128, 8))

# sig_ratio = np.array([
sig_ratio = \
    [4.025] * 111 + \
    [4.16731, 4.92429, 4.92429, 6.40026, 6.40755, 6.41138, 6.41139] + \
    [6.41535] * 8 + \
    [6.41943] * 18 + \
    [6.42362] * 83
# ])
# mean_ratio = sig_ratio.mean()
# print(mean_ratio)

sig_num = len(sig_ratio)

pop_size = 5
channel_num = 46

# print(mean_ratio, sig_num / channel_num)

def get_upper_channel_num(a, b, channel_num, sig_num):
    # ax + by >= sig_num
    # x + y = channel_num
    # get max a
    for x in range(channel_num, -1, -1):
        y = channel_num - x
        if (a * x + b * y) >= sig_num:
            return x
    return 0

pops = []
def my_random(candidate_ratios, sig_num, channel_num):
    channel_nums = []
    for candidate_ratio in candidate_ratios[:-1]:
        max_channel_num_of_ratio = get_upper_channel_num(candidate_ratio, candidate_ratios[-1], channel_num, sig_num)
        channel_num_of_ratio = np.random.choice(range(0, max_channel_num_of_ratio + 1))
        channel_num -= channel_num_of_ratio
        sig_num -= channel_num_of_ratio * candidate_ratio
        channel_nums.append(channel_num_of_ratio)
    # last_channel_num = channel_num - sum(channel_nums)
    if channel_num >= 0:
        channel_nums.append(channel_num)
    else:
        raise "Error"
    return channel_nums

def calculate_loss(target_ratios, channel_nums, candidate_ratios):
    ratios = []
    for i, channel_num in enumerate(channel_nums):
        ratios += [candidate_ratios[i]] * (candidate_ratios[i] * channel_num)
    ratios = ratios[:len(target_ratios)]
    loss = np.array(ratios) - np.array(target_ratios)
    # print(loss)
    return loss

def evolve(pops):
    pass

target_ratios = [2] * 28 + [8] * 14 + [16] * 4
target_ratios = [0] * (128//8)
target_ratios[:3] = [28, 14, 4]

for _ in range(50):
    pops.append(my_random(candidate_ratios, sig_num, channel_num))
pops.append(target_ratios)

loss = []
mean_loss = []
max_loss = []
for pop in pops:
    loss = calculate_loss(sig_ratio, pop, candidate_ratios)
    mean_loss.append(loss.mean())
    max_loss.append(loss.max())
# print(loss)
df = pd.DataFrame(pops, columns=candidate_ratios)
df['mean_loss'] = mean_loss
df['max_loss'] = max_loss
print(df.sort_values(by=['max_loss', 'mean_loss']))