import gym
import time

env = gym.make("Taxi-v3")
state = env.reset()
# print(state)
env.render()
reward = 0
count = 0
while reward != 20:
    observation, reward, done, info = env.step(env.action_space.sample())
    count += 1
    # time.sleep(1.0)
# print("step_res {} {} {} {}".format(observation, reward, done, info))

env.render()
print("count {}".format(count))
# print("Action Space {}".format(env.observation_space))
# print("State Space {}".format(env.action_space))
