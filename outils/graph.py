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


data1 = np.genfromtxt("systems/Eliptic/200/targets.csv", delimiter=",", max_rows=197)
data2 = np.genfromtxt("systems/Eliptic/200/projectiles.csv", delimiter=",", max_rows=197)

x0 = data1[:,0]
y0 = data1[:,1]
z0 = data1[:,2]

x1 = data2[:,0]
y1 = data2[:,1]
z1 = data2[:,2] + 15.0



""" PYVISTA """
plotter = pv.Plotter()

for (x, y, z), r in zip(np.array([x0, y0, z0]).T, [radius]*x0.size):
    sphere = pv.Sphere(radius=r, center=(x, y, z))
    plotter.add_mesh(sphere, color='blue', opacity=0.3)

for (x, y, z), r in zip(np.array([x1, y1, z1]).T, [radius]*x0.size):
    sphere = pv.Sphere(radius=r, center=(x, y, z))
    plotter.add_mesh(sphere, color='red', opacity=0.3)



# Add axes widget and coordinate frame
plotter.add_axes(interactive=True)
plotter.show_axes()

# Add a bounding box for context
plotter.show_bounds(grid='front', location='outer', color='gray')
plotter.show()


""" plotly """
# def sphere(x0, y0, z0, r, color):
#     u = np.linspace(0, 2 * np.pi, 50)
#     v = np.linspace(0, np.pi, 25)
#     x = x0 + r * np.outer(np.cos(u), np.sin(v))
#     y = y0 + r * np.outer(np.sin(u), np.sin(v))
#     z = z0 + r * np.outer(np.ones_like(u), np.cos(v))
#     return go.Surface(x=x, y=y, z=z, colorscale=[[0, color], [1, color]], showscale=False, opacity=0.8)
#
# fig = go.Figure()
#
#
# for (xa, ya, za), r in zip(np.array([x0, y0, z0]).T, [radius]*x0.size):
#     fig.add_trace(sphere(xa, ya, za, r, 'blue'))
#
#
# for (xa, ya, za), r in zip(np.array([x, y, z]).T, [radius]*x0.size):
#     fig.add_trace(sphere(xa, ya, za, r, 'red'))
#
#
# fig.update_layout(scene=dict(aspectmode='data'), title="Multiple Spheres")
# fig.show()



""" Matplotlib """
# def set_equal_limits(ax, x, y, z):
#     min_lim = min(x.min(), y.min(), z.min())
#     max_lim = max(x.max(), y.max(), z.max())
#     ax.set_xlim(min_lim, max_lim)
#     ax.set_ylim(min_lim, max_lim)
#     ax.set_zlim(min_lim, max_lim)
#     try:
#         ax.set_box_aspect([1, 1, 1])
#     except AttributeError:
#         pass
#
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
# ax.view_init(elev = 0, azim = 0)
# ax.set_box_aspect([1, 1, 1])
#
# # Generate a unit sphere
# u, v = np.mgrid[0:2*np.pi:30j, 0:np.pi:15j]
# x_sphere = np.cos(u) * np.sin(v)
# y_sphere = np.sin(u) * np.sin(v)
# z_sphere = np.cos(v)
#
# # for (xa, ya, za), r in zip(np.array([x0, y0, z0]).T, [radius]*x0.size):
# #     ax.plot_surface(
# #         xa + r * x_sphere,
# #         ya + r * y_sphere,
# #         za + r * z_sphere,
# #         color='r', alpha=0.5, linewidth=0
# #     )
#
#
# ax.scatter(x0, y0, z0, color = "blue")  # color by c
#
# data = np.genfromtxt("systems/pGraph/200/projectile.csv", delimiter=",", max_rows=197)
#
# x = data2[:,0]
# y = data2[:,1]
# z = data2[:,2] + 8.0
#
# # for (xa, ya, za), r in zip(np.array([x, y, z]).T, [radius]*x0.size):
# #     ax.plot_surface(
# #         xa + r * x_sphere,
# #         ya + r * y_sphere,
# #         za + r * z_sphere,
# #         color='b', alpha=0.5, linewidth=0
# #     )
# ax.scatter(x, y, z, color="red")  # color by c
#
# ax.set_xlabel("X")
# ax.set_ylabel("Y")
# ax.set_zlabel("Z")
#
# set_equal_limits(ax, np.concatenate((x, x0)),np.concatenate((y, y0)),np.concatenate((z, z0)))
#
# plt.savefig("images/One Single Attom.png")
# plt.show()
