import numpy as np 
from scipy.integrate import simpson
import matplotlib.pyplot as plt 


sigma_nn = 41.23344641671908


data = np.genfromtxt("data2.csv", delimiter=",")


Ncolls = data[:, 1]
x = data[:, 0]
y_prev = 2 * np.pi * ( 1 - np.pow(1 - Ncolls / (197**2), 197**2) )


plt.plot(x, y_prev * x)
plt.show()




integral = simpson(2 * np.pi * x * (1 - np.exp(-Ncolls)), x) * 10

print(integral)
