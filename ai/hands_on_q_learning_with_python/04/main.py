import gym
import numpy as np
import time

# create environment
env = gym.make("Taxi-v3")
# print(state)

def run():
    # initialize Q-table
    Q = np.zeros([env.observation_space.n, env.action_space.n])

    # initialize hyperparameters
    gamma = 0.6
    alpha = 0.1
    epsilon = 0.1
    epsilon_decay = 0.99

    total_epochs = 0
    episodes = 1000

    for episode in range(episodes):
        # initialize environment
        state = env.reset()

        reward = 0
        epochs = 0
        epsilon *= epsilon_decay
        acc_reward = 0

        while reward != 20:
            if np.random.rand() < epsilon:
                #exploration option
                action = env.action_space.sample()
            else:
                #exploitation option
                action = np.argmax(Q[state])

            # #choose current highest-valued action
            # action = np.argmax(Q[state])

            #obtain reward and next state resulting from taking action
            next_state, reward, done, info = env.step(action)

            # update Q-value for state-action pair
            ## Q[state, action] = reward + gamma * np.max(Q[next_state])
            Q[state, action] = Q[state, action] + alpha * (reward + gamma *
                                                           np.max(Q[next_state]) - Q[state, action])

            #update state
            state = next_state

            acc_reward += reward
            epochs += 1
            # time.sleep(1.0)

        # print("acc_reward {}".format( acc_reward))
        # print("epochs {}".format(epochs))

        total_epochs += epochs

    # render final dropoff state
    # env.render()
    print("Average timesteps taken: {}".format(total_epochs / episodes))
    # return epochs


# nb_try = 1
# avg = np.zeros((nb_try))
# for i in range(nb_try):
#     count = run()
#     avg[i] = count

# print(avg.mean())

run()
