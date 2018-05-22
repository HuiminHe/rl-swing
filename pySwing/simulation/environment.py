import numpy as np
import cloudpickle
from scipy import integrate
import matplotlib.pyplot as plt

# Parameters
myParam = np.array([
	0.05, 0.05, 0.05,
	1 / 12 * 0.05 * 0.1**2,
	1 / 12 * 0.05 * 0.1**2,
	1 / 12 * 0.05 * 0.1**2,
	0.15, 0.1, 0.1, 0.1,
	0.05, 0.05, 0.05,
	0.05, 0.05,
	9.8
])

# Constraints
myCon = np.array([1e4, 1, 1e-3, np.deg2rad(85)])
a_max = 100

# Config
t0 = 0
t1 = 10
dt = 0.01
t = np.arange(t0, t1, dt)
y0 = np.array([np.pi / 6, 0, 0, 0, 0, 0])

# load dynamic and kinematic functions
f = cloudpickle.load(open('./model/swing_dynamic.dll', 'rb'))
kinematic = cloudpickle.load(open('./model/swing_kinematic.dll', 'rb'))


def continuous_reward(state, action, next_state):
	reward = -np.cos(next_state[0])
	done = False
	if reward > 0:
		done = True
	return reward, done


class Swing:
	def __init__(self, ode, reward_fcn, 
				 theta0=np.pi / 6, 
				 max_steps=1000):
		self.reward_fcn = reward_fcn
		self.theta0 = theta0
		self.done = True
		self.max_steps = max_steps
		self.ode = ode
		self.sol_ = []
	def step(self, action):
		# check init
		if self.done:
			raise RuntimeError('The environment is not initialized.')
		# update
		clip_action = np.clip(
			action, -np.ones_like(action), np.ones_like(action))
		
		clip_action *= np.logical_not(np.logical_and(abs(self.state[1:3]) > myCon[-1], self.state[1:3] * clip_action > 0)).astype(np.float)
		self.torque = clip_action * a_max
		# torque *= np.logical_not(np.logical_and(abs(self.state[1:3]) > myCon[-1], self.state[1:3] * torque > 0))
		self.ode.set_f_params(self.torque, myParam, myCon)
		if self.ode.successful() and self.t < self.max_steps:
			self.ode.integrate(self.ode.t + dt)
		next_state = np.array(self.ode.y)
		# compute reward
		reward, self.done = self.reward_fcn(
			self.state, clip_action, next_state)
		# check max step
		if self.t >= self.max_steps - 1:
			self.done = True

		self.sol_.append(self.state)
		self.state = np.array(next_state)
		self.t += 1
		return (next_state, reward, self.done, None)

	def reset(self):
		self.t = 0
		self.done = False
		self.state = np.array(
			[np.random.uniform(-1, 1) * self.theta0, 0, 0, 0, 0, 0])
		self.ode.set_initial_value(self.state, t0)
		return self.state

	def seed(self, s):
		np.random.seed(s)

	def render(self):
		return

	@property
	def sol(self):
		return np.vstack(self.sol_)

def swing_plot(t, sol):
    fig, axes = plt.subplots(2, 3, figsize=(9, 7))
    axes = axes.ravel()
    for i, ax in enumerate(axes):
        ax.plot(t, sol[:, i])
        if i < 3:
            ax.plot([t[0], t[-1]], [ np.pi/2, np.pi/2], 'r--')
            ax.plot([t[0], t[-1]], [-np.pi/2,-np.pi/2], 'r--')
            ax.set_title('$q_{}$'.format(i+1))
        else:
            ax.set_title('$\dot{q}_' + '{}$'.format(i-2))
    return fig

if __name__ == '__main__':
	# ode = integrate.ode(f).set_integrator('dopris5', atol=1e-3) #, atol=1e-3, max_order_ns=12, max_order_s=5)
	ode = integrate.ode(f).set_integrator('lsoda', atol=1e-5) #, atol=1e-3, max_order_ns=12, max_order_s=5)
	env = Swing(ode, continuous_reward, max_steps=1000)
	np.random.seed(171)
	env.seed(171)
	state = env.reset()
	while not env.done:
		action = np.random.uniform(-1, 1, size=(2,))
		# action = np.zeros(2)
		state, reward, done, _ = env.step(action)
		print(env.t, np.array2string(env.torque, formatter={'float_kind':lambda x: "%.2f" % x}), np.array2string(state[:3], formatter={'float_kind':lambda x: "%.2f" % x}), np.array2string(env.torque, formatter={'float_kind':lambda x: "%.2f" % x}), reward)
	fig = swing_plot(np.arange(len(env.sol)), env.sol)
	plt.show(fig)

