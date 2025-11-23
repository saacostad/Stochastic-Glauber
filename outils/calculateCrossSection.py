import numpy as np 
from scipy.integrate import simpson
import matplotlib.pyplot as plt 
from uncertainties import ufloat
from uncertainties import unumpy as unp


sigma_nn = 41.23344641671908
n = 238
# n = 197

collision_name = "U238-U238"

data = np.genfromtxt("rotated_uranium.csv", delimiter=",")
# data = np.genfromtxt("data_test.csv", delimiter=",")


x = data[:, 0]          # Impact parameter


Ncolls = data[:, 1]     # Collisions 
Ncolls_std = data[:, 2] # Uncertainty collisions 

Ncolls_u = unp.uarray(Ncolls, Ncolls_std)

# Stuff that makes the predictions
y_prev = 2 * np.pi * ( 1 - unp.pow(1 - Ncolls_u / (n**2), n**2) ) * x


y_c = unp.nominal_values(y_prev)
y_e = unp.std_devs(y_prev)


NO_part = data[:, 3]        # Participants  
NO_part_std = data[:, 4]    # STD Participants  


plt.style.use('ggplot')
plt.plot(x, y_c, lw = 1.0)
plt.fill_between(x, y_c + y_e, y_c - y_e, alpha=0.3, color = "red")


plt.grid(True, linestyle='--', alpha=0.5)
plt.xlabel('b [fm]', fontsize = 18)
plt.ylabel(r'$d \sigma_{inel} \, / \, db$ [$fm^2 / fm$]', fontsize = 18)
plt.title('Inelastic Cross Section for ' + collision_name, fontsize = 24)

plt.tick_params(axis='both', which='major', labelsize=16)  # ticks font size

plt.show()




plt.plot(x, Ncolls, lw = 1.0, label = r"$N_{col}$", c = "red")
plt.fill_between(x, Ncolls - Ncolls_std, Ncolls + Ncolls_std, alpha=0.3, color = "red")


plt.plot(x, NO_part, lw = 1.0, label = r"$N_{part}$", c = "black")
plt.fill_between(x, NO_part - NO_part_std, NO_part + NO_part_std, alpha=0.3, color = "black")


plt.grid(True, linestyle='--', alpha=0.5)
plt.xlabel('b [fm]', fontsize = 18)
plt.ylabel(r'$\left< N_{col} \right>$ ', fontsize = 18)
plt.title('Average collisions for ' + collision_name, fontsize = 24)
plt.legend(fontsize = 16)
plt.tick_params(axis='both', which='major', labelsize=16)  # ticks font size

plt.show()

integral_c = simpson(y_c, x) * 10
integral_u = simpson(y_c+y_e, x) * 10
integral_d = simpson(y_c-y_e, x) * 10

print(f"Calculated Cross Section = {integral_c} +- {integral_u - integral_c}")
