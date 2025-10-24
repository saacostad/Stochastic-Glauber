import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np



sigma_nn = 10.0
radius = np.sqrt(sigma_nn / np.pi)



def set_equal_limits(ax, x, y, z):
    min_lim = min(x.min(), y.min(), z.min())
    max_lim = max(x.max(), y.max(), z.max())
    ax.set_xlim(min_lim, max_lim)
    ax.set_ylim(min_lim, max_lim)
    ax.set_zlim(min_lim, max_lim)
    try:
        ax.set_box_aspect([1, 1, 1])
    except AttributeError:
        pass


data = np.genfromtxt("nucleus1.csv", delimiter=",")

x0 = data[:,0]
y0 = data[:,1]
z0 = data[:,2]

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.view_init(elev = 0, azim = 0)
ax.set_box_aspect([1, 1, 1])




# Generate a unit sphere
u, v = np.mgrid[0:2*np.pi:30j, 0:np.pi:15j]
x_sphere = np.cos(u) * np.sin(v)
y_sphere = np.sin(u) * np.sin(v)
z_sphere = np.cos(v)

for (xa, ya, za), r in zip(np.array([x0, y0, z0]).T, [radius]*x0.size):
    ax.plot_surface(
        xa + r * x_sphere,
        ya + r * y_sphere,
        za + r * z_sphere,
        color='r', alpha=0.5, linewidth=0
    )


# ax.scatter(x0, y0, z0, color = "blue", s=radius)  # color by c

data = np.genfromtxt("nucleus2.csv", delimiter=",")

x = data[:,0]
y = data[:,1]
z = data[:,2]

# for (xa, ya, za), r in zip(np.array([x, y, z]).T, [radius]*x0.size):
#     ax.plot_surface(
#         xa + r * x_sphere,
#         ya + r * y_sphere,
#         za + r * z_sphere,
#         color='b', alpha=0.5, linewidth=0
#     )
# ax.scatter(x, y, z, color="red", s = radius)  # color by c

ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")

set_equal_limits(ax, np.concatenate((x, x0)),np.concatenate((y, y0)),np.concatenate((z, z0)))

plt.savefig("images/nucleus.png")
plt.show()
