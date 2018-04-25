import numpy as np

#pythran export my_dot(float64[:,:],float64[:,:],float64[:,:])
def my_dot(A, B, C):
    C = np.dot(A, B)
    return C

# To generate the pythran module:
# $ pythran dot.py
