# """
# ==================
# Animated line plot
# ==================

# """

# import numpy as np
# import matplotlib.pyplot as plt
# import matplotlib.animation as animation

# fig, ax = plt.subplots()
# # help(ax)
# # help(fig)
# # exit()

# # x = np.arange(0, 2*np.pi, 0.01)
# # x = np.arange(0, 2*np.pi, 1.)
# x = np.arange(0, 5, 1.)
# print("x * x ", x * x)
# line, = ax.plot(x, x * x, 'rx', animated=True)
# # exit()

# def init():  # only required for blitting to give a clean slate.
#     line.set_ydata([np.nan] * len(x))
#     return line,


# def animate(i):
#     global x
#     # line.set_ydata(np.sin(x + i / 100.))  # update the data.
#     x = x + 1
#     line.set_xdata(x)  # update the data.
#     line.set_ydata(x * x)  # update the data.
#     # help(line)
#     # ax.set_ylim(max(x * x))
#     # ax.autoscale_view()
#     fig.autoscale_view()
#     print("max(x)", max(x * x))
#     # exit()
#     # line.y_lim
#     print("i: ", i)
#     print("x: ", x)
#     # print("x + i / 100.: ", x + i / 100.)
#     print("x + 1: ", x + 1)
#     print("x * x ", x * x)
#     return line,


# ani = animation.FuncAnimation(
#     fig, animate, init_func=init,
#     interval=500., blit=True,
#     # save_count=50)
#     save_count=50)

# # To save the animation, use e.g.
# #
# # ani.save("movie.mp4")
# #
# # or
# #
# # from matplotlib.animation import FFMpegWriter
# # writer = FFMpegWriter(fps=15, metadata=dict(artist='Me'), bitrate=1800)
# # ani.save("movie.mp4", writer=writer)

# plt.show()


import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math


def data_gen(t=0):
    import random as rd
    
    cnt = 0
    pt_a = np.array([0., 0.])
    pt_b = np.array([1., 0.])
    pt_c = np.array([1/2., math.sqrt(3) / 2])
    pt_center = np.array([1/2., math.sqrt(3) / 6])

    pt_a -= pt_center
    pt_b -= pt_center
    pt_c -= pt_center
    triangle = [pt_a, pt_b, pt_c]

    pt_seed = np.array([rd.random(), rd.random()])
    # pt_seed = np.array([0.6, 0.6])
    pt_square_center = np.array([0.5, 0.5])
    pt_seed -= pt_square_center
    if (pt_seed[1] > -pt_seed[0]):
        B = np.array([[0., -1.], [-1., 0.]])
        print("B", B)
        pt_seed = B.dot(pt_seed)
        print("0 pt_seed", pt_seed)
    pt_seed += pt_square_center
    print("1 pt_seed", pt_seed)

    A = np.array([[1, np.cos(np.pi/3)], [0, np.sin(np.pi/3)]])
    pt_seed = A.dot(pt_seed)

    pt_cur = pt_seed - pt_center
    print("2 pt_seed", pt_seed)
    # while cnt < 1000:
    while True:
        # cnt += 1
        # t += 0.5
        # # yield t, np.sin(2*np.pi*t) * np.exp(-t/10.)
        # yield t, t * t

        yield pt_cur[0], pt_cur[1]

        rand_int = rd.randint(0, 2)
        pt_corner = triangle[rand_int]
        pt_cur = 0.5 * (pt_cur + pt_corner)

        # yield pt_cur[0], pt_cur[1]

def init():
    # ax.set_ylim(-0.1, 10.)
    # ax.set_xlim(0, 10)
    del xdata[:]
    del ydata[:]
    line.set_data(xdata, ydata)
    return line,

fig, ax = plt.subplots()
# line, = ax.plot([], [], lw=3)
# line, = ax.plot([], [], 'ro', lw=1)
line, = ax.plot([], [], 'x', lw=1)
# ax.set_ylim(-0.1, 10.)
# ax.set_xlim(0, 10)
offset = 0.1
wide = 1.
height = math.sqrt(3)/2
ax.set_xlim(-wide / 2 - offset, wide / 2 + offset)
ax.set_ylim(-height / 3 - offset, 2 * height / 3 + offset)
ax.grid()
xdata, ydata = [], []


# def run(data):
#     # update the data
#     t, y = data
#     xdata.append(t)
#     ydata.append(y)
#     xmin, xmax = ax.get_xlim()
#     ymin, ymax = ax.get_ylim()

#     # if t >= xmax:
#     #     ax.set_xlim(xmin, 2*xmax)
#     #     ax.figure.canvas.draw()

#     if max(xdata) >= xmax:
#         ax.set_xlim(xmin, 1.1 * max(xdata))
#         ax.figure.canvas.draw()
#     if max(ydata) >= ymax:
#         ax.set_ylim(ymin, 1.1 * max(ydata))
#         ax.figure.canvas.draw()

#     # ax.set_xlim(xmin, xmax)
#     # ax.set_ylim(ymin, ymax)
#     # ax.figure.canvas.draw()
#     line.set_data(xdata, ydata)

#     return line,

idx_color_switch = 0

def run_wk(data):
    global idx_color_switch
    # update the data
    t, y = data
    xdata = [t]
    ydata = [y]
    # print("t", t)
    # print("y", y)

    color = None
    if (idx_color_switch % 2 == 0):
        # color = "red"
        color = "#FF0000"
    else:
        color = "#0000FF"

    idx_color_switch = (idx_color_switch + 1) % 2
    line, = ax.plot([t], [y], 'x', color=color, lw=3)

    xmin, xmax = ax.get_xlim()
    ymin, ymax = ax.get_ylim()

    if max(xdata) >= xmax:
        ax.set_xlim(xmin, 1.1 * max(xdata))
        ax.figure.canvas.draw()
    if max(ydata) >= ymax:
        ax.set_ylim(ymin, 1.1 * max(ydata))
        ax.figure.canvas.draw()

    return line,

ani = animation.FuncAnimation(fig,
                              # run,
                              run_wk,
                              data_gen, blit=False,
                              # interval=500,
                              # interval=5,
                              interval=1,
                              repeat=False, init_func=init)
plt.show()
