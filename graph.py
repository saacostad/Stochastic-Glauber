import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np


data = np.genfromtxt("data.csv", delimiter=",")

print(data)


# counts, bin_edges = np.histogram(data, bins = 100)
# print(counts)
# print(bin_edges)
#
# plt.hist(data, bins=50, density=True, color='steelblue', alpha=0.7)
# plt.xlabel('Value')
# plt.ylabel('Probability Density')
# plt.show()


r = data[:,0]
theta = data[:,1]
phi = data[:,2]

x = r * np.sin(theta) * np.cos(phi)
y = r * np.sin(theta) * np.sin(phi)
z = r * np.cos(theta)

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

sc = ax.scatter(x, y, z, cmap='viridis')  # color by c
plt.colorbar(sc, label='Color value')

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

plt.show()
