from bfv_rns import *
import pickle
import datetime
import gc
import copy
# import defs
import sys
import numpy as np
from multiprocessing import Pool
import socket
from util import *

def center(a, b):
    if a > b//2:
        a -= b
    elif a < -b//2:
        a += b
    return a

R = 2**32
"""
MOD = [2147565569, 2148155393, 2148384769, 2148728833, \
       2148794369, 2149072897, 2149171201, 1, 2149466113, \
       2149662721, 2149810177, 2150072321, 2150301697, \
       2150318081, 2150563841, 2150612993]
"""
with open("test.txt", "rb") as fp:   # Unpickling
    context = pickle.load(fp)

# with open("encrypt8192.txt", "rb") as fp:   # Unpickling
#     ct_ = pickle.load(fp)

# # print((context.encryptor[0].rlk[0][0][0].poly[0]))
# # print((ct_[0][0][0][0].poly[0]))
# # print((context.encryptor[0].galois[0][0][0][0].poly[0]))

# # static const int rlk[4][7][2][7][8192]
# rlk = []
# for i in range(4):
#     for j in range(7):
#         for k in range(2):
#             for l in range(7):
#                 for x in range(8192):
#                     rlk.append(context.encryptor[i].rlk[j][k][l].poly[x])


# # static const int ct[25][4][2][7][8192]
# ct = []
# for i in range(25):
#     for j in range(4):
#         for k in range(2):
#             for l in range(7):            
#                 for x in range(8192):
#                     ct.append(ct_[i][j][k][l].poly[x])

# # static const int gk[4][25][7][2][7][8192]
# gk = []
# for i in range(4):
#     for j in range(25):
#         for k in range(7):
#             for l in range(2):
#                 for x in range(7):
#                     for y in range(8192):
#                         gk.append(context.encryptor[i].galois[j][k][l][x].poly[y])

# with open("rlk_data.txt", "wb") as fp:
#     pickle.dump(rlk, fp)
    
# with open("ct_data.txt", "wb") as fp:
#     pickle.dump(ct, fp)

# with open("gk_data.txt", "wb") as fp:
#     pickle.dump(gk, fp)


with open("rlk.txt", "rb") as fp:
    rlk = pickle.load(fp)

with open("ct.txt", "rb") as fp:
    ct = pickle.load(fp)

with open("gk.txt", "rb") as fp:
    gk = pickle.load(fp)

# send to server
print('Transforming data to C array ...')

rlk_arr = List2CArray(rlk)
ct_arr  = List2CArray(ct)
gk_arr  = List2CArray(gk)

print('Connecting to server ...')

ip, port = '140.112.31.159', 17777
sckt = startSocket(ip, port)

print('Start sending data to server ...')
sendData(sckt, rlk_arr)
sendData(sckt, ct_arr)
sendData(sckt, gk_arr)

ret_bytes = receiveData(sckt, num_int=4*2*7*8192)
print('Receive result from server!')
print('Transforming result to Python list ...')

# note: ret is a 1D list
ret = CArray2List(ret_bytes, num_int=4*2*7*8192)


m = np.reshape(ret, (4,2,7,8192))

### Test Units ###

# Kmov
# for x in range(7):
#   for i, j in zip(m[0][0][x], m[0][1][x]):
#     print(i, j)

# Add_0
# for x in range(7):
#   for i, j in zip(m[0][0][x], m[0][1][x]):
#     ti = center(i, R)
#     tj = center(j, R)
#     ans = center((center(i, R) + center(i, R)) % MOD[x], MOD[x])

#     if (tj != ans):
#       print(ti, tj, ans)

# Add_1
# for x in range(7):
#   for i, j in zip(m[0][0][x], m[0][1][x]):
#     ti = center(i, R)
#     tj = center(j, R)
#     ans = center((center(i, R) + center(i, R)) % MOD[x+8], MOD[x+8])

#     if (tj != ans):
#       print(ti, tj, ans)

# MulWise_0
# for x in range(7):
#   for i, j in zip(m[0][0][x], m[0][1][x]):
#     ti = center(i, R)
#     tj = center(j, R)
#     ans = center(center(i, R) * center(i, R) % MOD[x], MOD[x])

#     if (tj != ans):
#       print(ti, tj, ans)

# MulWise_1
# for x in range(7):
#   for i, j in zip(m[0][0][x], m[0][1][x]):
#     ti = center(i, R)
#     tj = center(j, R)
#     ans = center(center(i, R) * center(i, R) % MOD[x+8], MOD[x+8])

#     if (tj != ans):
#       print(ti, tj, ans)

# MulConst_0
# const_val = 2**17
# for x in range(7):
#   for i, j in zip(m[0][0][x], m[0][1][x]):
#     ti = center(i, R)
#     tj = center(j, R)
#     ans = center(ti * const_val % MOD[x], MOD[x])

#     if (tj != ans):
#       print(ti, tj, ans)

# MulConst_1
# const_val = 2**17
# for x in range(7):
#   for i, j in zip(m[0][0][x], m[0][1][x]):
#     ti = center(i, R)
#     tj = center(j, R)
#     ans = center(center(i, R) * const_val % MOD[x+8], MOD[x+8])

#     if (tj != ans):
#       print(ti, tj, ans)

# NTT
# for x in range(7):
#   for i, j in zip(m[0][0][x], m[0][1][x]):
#     print(center(i, R), center(j, R), x)

# Lift
# for x in range(7):
#   for i, j in zip(m[0][0][x], m[0][1][x]):
#     print(center(i, R), center(j, R))

# Scale
# for x in range(7):
#   for i, j in zip(m[0][0][x], m[0][1][x]):
#     print(center(i, R), center(j, R))

# Galois
#for x in range(7):
#  for i, j in zip(m[0][0][x], m[0][1][x]):
#    print(center(i, R), center(j, R))

closeSocket(sckt)
#exit()

# packed into ct form
ct_ret = []
for i in m:
    temp1 = []
    for j in i:
        temp2 = []
        for k, p in zip(j, context.encryptor[0].q_i):
            pp = []
            for ppp in k:
                pp.append(int(center(ppp, R)))
            temp2.append(polynomial(pp, p))
        temp1.append(temp2)
    ct_ret.append(temp1)

# 10 min decrypt and decode
pt  = context.decrypt(ct_ret)
out = context.decode_and_reconstruct(pt)

print(out)
result = []
for i in range(845):
    result.append(out[i]/(1<<9))
print(result)
print(result.index(max(result)))
