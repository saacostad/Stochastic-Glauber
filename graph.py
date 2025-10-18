import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np


data = np.genfromtxt("data.csv", delimiter="\n")

print(data)


counts, bin_edges = np.histogram(data, bins = 100)
print(counts)
print(bin_edges)

plt.hist(data, bins=50, density=True, color='steelblue', alpha=0.7)
plt.xlabel('Value')
plt.ylabel('Probability Density')
plt.show()
# x = data[:,0]
# y = data[:,1]
# z = data[:,2]
# c = data[:,3]
#
#
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
#
# sc = ax.scatter(x, y, z, c=c, cmap='viridis')  # color by c
# plt.colorbar(sc, label='Color value')
#
# ax.set_xlabel('X')
# ax.set_ylabel('Y')
# ax.set_zlabel('Z')
#
# plt.show()
