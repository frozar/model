import random
import gym
import numpy as np
import tensorflow as tf
from collections import deque
from keras.models import Sequential
from keras.layers import Dense
import keras.optimizers as optimizers
import time

Adam = optimizers.get("Adam")
# help(Adam)

gamma = 1
alpha = 0.1
learning_rate_adam = 0.01
epsilon = 1
epsilon_decay = 0.99

env = gym.make('CartPole-v1')
env.reset()

class DQN:
    def __init__(self):
        self.epsilon = epsilon
        self.gamma = gamma
        self.alpha = alpha
        self.action_space = env.action_space.n # env.action_space.n
        self.observation_space = env.observation_space.shape[0]
        self.exploration_rate = 0.1

        self.memory = []
        self.batch_size = 32

        self.model = Sequential()
        self.model.add(Dense(24, input_shape=(self.observation_space,),
                             activation="relu"))
        self.model.add(Dense(24, activation="relu"))
        self.model.add(Dense(self.action_space, activation="linear"))
        self.model.compile(loss="mse",
                           # optimizer=Adam
                           # optimizer=optimizers.Adam(learning_rate=learning_rate_adam)
                           optimizer="adam",
                           # lr=learning_rate_adam
                           )

    def choose_action(self, state):
        if np.random.rand() < self.exploration_rate:
            return random.randrange(self.action_space)
        q_values = self.model.predict(state)
        return np.argmax(q_values[0])

    def update(self, state, action, reward, next_state, done):
        if(len(self.memory) < self.batch_size):
            return

        mem_sample = random.sample(self.memory, self.batch_size)
        for state, action, reward, next_satet, done in mem_sample:
            q_update = reward
            if not done:
                # Is not really equivalent to the Bellman equation
                q_update = self.alpha * (reward
                                         + self.gamma * np.max(self.model.predict(next_state)[0]))
            q_values = self.model.predict(state)
            q_values[0][action] = q_update
            self.model.fit(state, q_values, verbose=0)

        self.epsilon *= epsilon_decay

    def memory_update(self, state, action, reward, next_state, done):
        self.memory.append((state, action, reward, next_state, done))


def cartpole():
    env = gym.make("CartPole-v1")
    # observation_space, action_space = env.observation_space.shape[0], env.action_space.n
    observation_space = env.observation_space.shape[0]
    # dqn = DQN(observation_space, action_space)
    dqn = DQN()
    epoch = 0
    done = False

    while True:
        score = 0
        epoch += 1
        state = env.reset()
        state = np.reshape(state, [1, observation_space])
        while True:
            score += 1
            action = dqn.choose_action(state)
            next_state, reward, done, info = env.step(action)
            reward = reward if not done else -reward
            next_state = np.reshape(next_state, [1, observation_space])
            dqn.memory_update(state, action, reward, next_state, done)
            state = next_state
            if done:
                print ("Epoch: " + str(epoch) + " Score: " + str(score))
                break
            dqn.update(state, action, reward, next_state, done)


if __name__ == "__main__":
    cartpole()


# env = gym.make('CartPole-v1')
# env.reset()


# done = False

# while not done:
#     env.render()
#     # Take a random action
#     observation, reward, done, info = env.step(env.action_space.sample())
#     time.sleep(0.1)
