import numpy as np 
from scipy.integrate import simpson
import matplotlib.pyplot as plt 
# from outils.getNNCrossSection import Ecm
from uncertainties import ufloat
from uncertainties import unumpy as unp

A = 197
B = 197
A_el = "Au"
B_el = "Au"

energy = 200
fileName = "data.csv"


collision_name = f"{A_el}{A}-{B_el}{B}"
system = collision_name




data = np.genfromtxt(f"systems/{system}/{energy}/{fileName}", delimiter=",")


x = data[:, 0]          # Impact parameter

Ncolls = data[:, 1]     # Collisions 
Ncolls_std = data[:, 2] # Uncertainty collisions 

Ncolls_u = unp.uarray(Ncolls, Ncolls_std)

# Stuff that makes the predictions
y_prev = 2 * np.pi * ( 1 - unp.pow(1 - Ncolls_u / (A*B), A*B) ) * x


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
plt.savefig(f"systems/{system}/{energy}/{fileName[0:-4]}_dsdb.png")
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

plt.savefig(f"systems/{system}/{energy}/{fileName[0:-4]}_colls.png")
plt.show()

integral_c = simpson(y_c, x) * 10
integral_u = simpson(y_c+y_e, x) * 10
integral_d = simpson(y_c-y_e, x) * 10

print(f"Calculated Cross Section for {system} at Ecm {energy} GeV = {integral_c} +- {integral_u - integral_c}")
