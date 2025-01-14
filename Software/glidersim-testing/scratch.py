import numpy as np

def as_to_lines(delta_a, delta_s):
    delta_1 = delta_s + ( 0.5*delta_a )
    delta_2 = delta_s - ( 0.5*delta_a )
    return delta_1, delta_2

def lines_to_as(delta_1, delta_2):        
    delta_a = delta_1 - delta_2
    delta_s = np.average([delta_1, delta_2])
    return delta_a, delta_s 

delta_a = 4
delta_s = 0

# d1, d2 = as_to_lines(delta_a, delta_s)
# print(d1, d2)

delta_1 = [0, 1, 2, 3]
delta_2 = [0, 0, 0, 0]

for i in range(len(delta_1)):
    da, ds = lines_to_as(delta_1[i], delta_2[i])
    print(da, ds)

# print(da, ds)
