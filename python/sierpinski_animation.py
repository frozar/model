import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math

pt_a = np.array([0., 0.])
pt_b = np.array([1., 0.])
pt_c = np.array([1/2., math.sqrt(3) / 2])
pt_center = np.array([1/2., math.sqrt(3) / 6])
pt_a -= pt_center
pt_b -= pt_center
pt_c -= pt_center
triangle = [pt_a, pt_b, pt_c]

def data_gen(t=0):
    import random as rd
    
    pt_seed = np.array([rd.random(), rd.random()])
    pt_square_center = np.array([0.5, 0.5])
    pt_seed -= pt_square_center
    if (pt_seed[1] > -pt_seed[0]):
        B = np.array([[0., -1.], [-1., 0.]])
        pt_seed = B.dot(pt_seed)

    pt_seed += pt_square_center

    A = np.array([[1, np.cos(np.pi/3)], [0, np.sin(np.pi/3)]])
    pt_seed = A.dot(pt_seed)

    pt_cur = pt_seed - pt_center

    while True:
        yield pt_cur

        rand_int = rd.randint(0, 2)
        pt_corner = triangle[rand_int]
        pt_cur = 0.5 * (pt_cur + pt_corner)


def init():
    xdata, ydata = [], []
    line.set_data(xdata, ydata)
    return line,


fig, ax = plt.subplots()
line, = ax.plot([], [], 'x', lw=1)

offset = 0.1
wide = 1.
height = math.sqrt(3)/2
ax.set_xlim(-wide / 2 - offset, wide / 2 + offset)
ax.set_ylim(-height / 3 - offset, 2 * height / 3 + offset)

ax.grid()


idx_color_switch = 0

def run(data):
    global idx_color_switch
    # update the data
    pt_cur = data
    xdata = pt_cur[0]
    ydata = pt_cur[1]

    pt_a, pt_b, pt_c = triangle

    l_weight = [np.linalg.norm(pt - pt_cur) for pt in triangle]
    l_weight = [w / sum(l_weight) for w in l_weight]
    eps = 1e-10
    r, g, b = [int(w * (256 - eps)) for w in l_weight]

    hex_color = ''.join('%02X'%i for i in [r, g, b]).upper()
    color = '#' + hex_color

    idx_color_switch = (idx_color_switch + 1) % 2
    line, = ax.plot(xdata, ydata, '.', color=color, lw=3)

    xmin, xmax = ax.get_xlim()
    ymin, ymax = ax.get_ylim()

    if xdata >= xmax:
        ax.set_xlim(xmin, 1.1 * max(xdata))
        ax.figure.canvas.draw()
    if ydata >= ymax:
        ax.set_ylim(ymin, 1.1 * max(ydata))
        ax.figure.canvas.draw()

    return line,


ani = animation.FuncAnimation(fig,
                              run,
                              data_gen, blit=False,
                              interval=1,
                              repeat=False, init_func=init)
plt.show()

