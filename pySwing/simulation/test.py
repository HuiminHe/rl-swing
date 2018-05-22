import os
import sys
from time import sleep
import torch
import cloudpickle
from torch.autograd import Variable

from environment import *
from models import *
from running_state import ZFilter
import matplotlib.pyplot as plt
import cloudpickle

torch.utils.backcompat.broadcast_warning.enabled = True
torch.utils.backcompat.keepdim_warning.enabled = True

torch.set_default_tensor_type('torch.DoubleTensor')


def select_action(state):
    state = torch.from_numpy(state).unsqueeze(0)
    action_mean, _, action_std = policy_net(Variable(state))
    action = torch.normal(action_mean, action_std)
    return action


class Animation:
    def __init__(self, dir_name, savefig=False):
        self.savefig = savefig
        self.rendered = False
        self.step = 0
        self.ims = []
        self.dir_name = dir_name
        os.makedirs(dir_name + 'anim', exist_ok=True)

    def __call__(self, state, reward):
        if not self.rendered:
            self.kinematic = cloudpickle.load(open('./model/swing_kinematic.dll', 'rb'))
            self.fig, self.ax = plt.subplots(ncols=1, figsize=(6, 6))
            self.ax.set_xlim([-0.3, 0.3])
            self.ax.set_ylim([-0.3, 0.15])
            self.ax.set_xlabel('X')
            self.ax.set_ylabel('Y')
            # self.ax[0].set_xlim([-0.3, 0.3])
            # self.ax[0].set_ylim([-0.3, 0.3])
            # self.ax[0].set_xlabel('X')
            # self.ax[0].set_ylabel('Y')
            # self.ax[1].set_xlim([-2, 2])
            # self.ax[1].set_ylim([-15, 15])
            # self.ax[1].set_xlabel('$q_1$')
            # self.ax[1].set_ylabel('$\dot{q_1}$')

            self.line1, = self.ax.plot([], [], lw=2, color='k', linestyle='-', marker='o', ms=10)
            self.line2, = self.ax.plot([], [], lw=4, color='b', linestyle='-', marker='o', ms=6)

            # self.line1, = self.ax[0].plot([], [], lw=1, color='k', linestyle='-', marker='o', ms=5)
            # self.line2, = self.ax[0].plot([], [], lw=2, color='b', linestyle='-', marker='o', ms=3)
            # self.tau,   = self.ax[1].plot([], [], color='b', marker='o', ms=3)
            # self.traj = None
            # self.time_text = self.ax.text(-0.2, 0.23, '')
            # self.reward_text = self.ax.text(-0.2, 0.2, '')

            # self.time_text = self.ax[0].text(-0.2, 0.23, '')
            # self.reward_text = self.ax[0].text(-0.2, 0.2, '')
            self.fig.canvas.draw()
            self.rendered = True
            if not self.savefig:
                plt.show(block=False)
        else:
            p1, p2, p3, p21, p31 = kinematic(next_state, myParam)
            # if self.traj is None:
            #     self.traj = np.expand_dims(next_state[[0, 3]], axis=0)
            # else:
            #     self.traj = np.vstack([self.traj, next_state[[0, 3]]])
            self.line1.set_data([0, p1[0]], [0, p1[1]])
            self.line2.set_data([p21[0], p2[0], p3[0], p31[0]], [p21[1], p2[1], p3[1], p31[1]])
            # self.tau.set_data(self.traj[:, 0], self.traj[:, 1])
            # self.time_text.set_text('t = %.2f'%(self.step/100.0))
            # self.reward_text.set_text('r = %.2f'%reward)

            # update canvas
            self.ax.draw_artist(self.ax.patch)
            #self.ax.draw_artist(self.ax[1].patch)
            self.ax.draw_artist(self.line1)
            self.ax.draw_artist(self.line2)
            # self.ax.draw_artist(self.tau)
            # self.ax.draw_artist(self.time_text)
            # self.ax.draw_artist(self.reward_text)

            # self.ax[0].draw_artist(self.ax[0].patch)
            # self.ax[1].draw_artist(self.ax[1].patch)
            # self.ax[0].draw_artist(self.line1)
            # self.ax[0].draw_artist(self.line2)
            # self.ax[1].draw_artist(self.tau)
            # self.ax[0].draw_artist(self.time_text)
            # self.ax[0].draw_artist(self.reward_text)
            self.fig.canvas.flush_events()
            self.fig.canvas.update()
        plt.axis('off')
        if self.savefig:
            self.fig.savefig(self.dir_name + '/anim/{}.png'.format(self.step))
        self.step += 1
        return self.fig

def trajectory():
    return

if __name__ == '__main__':
    # init agent
    num_inputs = 6
    num_actions = 2

    policy_net = Policy(num_inputs, num_actions)
    value_net = Value(num_inputs)

    policy_net.load_state_dict(torch.load(sys.argv[1] + 'model/policy_ep' + sys.argv[2] + '.pth'))
    value_net.load_state_dict(torch.load(sys.argv[1] + 'model/value_ep' + sys.argv[2] + '.pth'))

    # init env
    ode = integrate.ode(f).set_integrator('lsoda', atol=1e-5)
    env = Swing(ode, continuous_reward, max_steps=1000)

    # env.seed(117)
    # init stat
    running_state = ZFilter((num_inputs,), clip=5)
    running_reward = ZFilter((1,), demean=False, clip=10)
    sol = []
    state = env.reset()
    sol.append(np.array(state))
    state = running_state(state)
    
    # init animation
    anim = Animation(sys.argv[1], savefig=True)
    # run test
    reward_sum = 0
    for step in range(1000):
        action = select_action(state)
        action = action.data[0].numpy()
        next_state, reward, done, _ = env.step(action)
        reward_sum += reward
        fig = anim(next_state, reward)

        sol.append(np.array(next_state))
        # render(step, next_state, reward)
        next_state = running_state(next_state)
        
        
        if done:
            print('task completed')
            np.save(open('sol.npy', 'wb'), np.array(sol))
            # fig.savefig('./trajectory.png')
            exit(0)

        state = next_state



