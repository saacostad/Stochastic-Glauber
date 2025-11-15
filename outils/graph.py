import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import csv
import plotly.graph_objects as go
import pyvista as pv

sigma_nn = {}

with open("systems/NNCrossSection.csv", newline='') as f:
    reader = csv.reader(f)
    for key, value in reader:        # no header, so read directly
        sigma_nn[float(key)] = float(value)  # or convert if you know it’s numeric

radius = np.sqrt(sigma_nn[200] / np.pi)

nP = 238
nT = 238

# data1 = np.genfromtxt("systems/Au79-Au79/200/targets.csv", delimiter=",", max_rows=197)
# data2 = np.genfromtxt("systems/Au79-Au79/200/projectiles.csv", delimiter=",", max_rows=197)

# data1 = np.genfromtxt("systems/Eliptic/200/targets.csv", delimiter=",", max_rows=197)
# data2 = np.genfromtxt("systems/Eliptic/200/projectiles.csv", delimiter=",", max_rows=197)

data1 = np.genfromtxt("systems/test_uranium/200/targets.csv", delimiter=",", max_rows=2 * nT)
data2 = np.genfromtxt("systems/test_uranium/200/projectiles.csv", delimiter=",", max_rows=nP)


x0 = data1[:nT,0]
y0 = data1[:nT,1]
z0 = data1[:nT,2]

#
# x1 = data1[nT:2 * nT,0]
# y1 = data1[nT:2 * nT,1] 
# z1 = data1[nT:2 * nT,2] + 25.0


x1 = data2[:nP,0] 
y1 = data2[:nP,1] + 6.0
z1 = data2[:nP,2] + 25.0


""" PYVISTA """
plotter = pv.Plotter()

for (x, y, z), r in zip(np.array([x0, y0, z0]).T, [radius]*x0.size):
    sphere = pv.Sphere(radius=r, center=(x, y, z))
    plotter.add_mesh(sphere, color=(1.0, 149 / 245, 204 / 245), opacity=0.3)

for (x, y, z), r in zip(np.array([x1, y1, z1]).T, [radius]*x0.size):
    sphere = pv.Sphere(radius=r, center=(x, y, z))
    plotter.add_mesh(sphere, color=(1.0, 149 / 245, 204 / 245), opacity=0.3)



# Add axes widget and coordinate frame
plotter.add_axes(interactive=True)
plotter.show_axes()

# Add a bounding box for context
plotter.show_bounds(grid='front', location='outer', color='gray')
plotter.show()



# # """ Matplotlib """
# fig, ax = plt.subplots()
#
# rad = 15
#
# for (x, y), r in zip(np.array([x1, y1]).T, [radius]*x0.size):
#     circle = plt.Circle((x, y), r, fill=True, color='blue', lw=1.5,
#                         transform=ax.transData,
#                         alpha = 0.2)  # ✅ ensures "data units"
#     ax.add_patch(circle)
# for (x, y), r in zip(np.array([x0, y0]).T, [radius]*x0.size):
#     circle = plt.Circle((x, y), r, fill=True, color='red', lw=1.5,
#                         transform=ax.transData,
#                         alpha = 0.2)  # ✅ ensures "data units"
#     ax.add_patch(circle)
# # Equal aspect ratio so circles look like circles
# ax.set_aspect('equal')
#
# ax.set_xlim(-rad, rad + 3)
# ax.set_ylim(-rad, rad + 6)
# plt.xlabel('x [fm]', fontsize = 18)
# plt.ylabel('y [fm]', fontsize = 18)
# plt.tick_params(axis='both', which='major', labelsize=16)  # ticks font size
# plt.grid(True, linestyle='--', alpha=0.5)
# plt.show()
