import numpy as np 
from scipy.integrate import simpson
import matplotlib.pyplot as plt 


sigma_nn = 41.23344641671908


data = np.genfromtxt("data2.csv", delimiter=",")


Ncolls = data[:, 1]
x = data[:, 0]
y_prev = 2 * np.pi * ( 1 - np.pow(1 - Ncolls / (197**2), 197**2) )




plt.style.use('ggplot')
plt.plot(x, y_prev * x, lw = 2.5)

plt.grid(True, linestyle='--', alpha=0.5)
plt.xlabel('b [fm]', fontsize = 18)
plt.ylabel(r'$d \sigma_{inel} \, / \, db$ [$fm^2 / fm$]', fontsize = 18)
plt.title('Inelastic Cross Section for Au79-Au79', fontsize = 24)

plt.tick_params(axis='both', which='major', labelsize=16)  # ticks font size

plt.show()




plt.plot(x, data[:,1], lw = 2.5)

plt.grid(True, linestyle='--', alpha=0.5)
plt.xlabel('b [fm]', fontsize = 18)
plt.ylabel(r'$\left< N_{col} \right>$ ', fontsize = 18)
plt.title('Average collisions for Au79-Au79', fontsize = 24)

plt.tick_params(axis='both', which='major', labelsize=16)  # ticks font size

plt.show()




integral = simpson(2 * np.pi * x * (1 - np.exp(-Ncolls)), x) * 10

print(integral)
