import gym
import numpy as np
import random
import tensorflow.compat.v1 as tf
import sys

env = gym.make('Taxi-v3')

tf.reset_default_graph()
tf.disable_eager_execution()
inputs = tf.placeholder(shape=[1, env.observation_space.n], dtype=tf.float32)
weights = tf.Variable(tf.random_uniform([env.observation_space.n,env.action_space.n], 0, 0.01))
q_out = tf.matmul(inputs, weights)
predict = tf.argmax(q_out,1)
# help(predict)

next_q = tf.placeholder(shape=[1,env.action_space.n],dtype=tf.float32)
loss = tf.reduce_sum(tf.square(next_q - q_out))
trainer = tf.train.GradientDescentOptimizer(learning_rate=0.1)
loss_update = trainer.minimize(loss)
init = tf.global_variables_initializer()

total_epochs = 0
total_rewards = 0

gamma = 0.7
epsilon = 0.2
epsilon_decay = .99
episodes = 50

with tf.Session() as sess:
    sess.run(init)
    for episode in range(episodes):
        state = env.reset()
        rewards_this_episode = 0
        epochs = 0
        done = False

        i_dbg = 0;
        while not done:
            action, q = sess.run([predict,q_out],
                                 feed_dict={inputs:np.identity(env.observation_space.n)[state:state + 1]})
            rand = np.random.rand(1)
            if rand < epsilon:
                action[0] = env.action_space.sample()

            # print("predict " + str(predict.eval()))
            # tf.print("predict", predict, output_stream=sys.stdout)
            # print("q_out " + str(q_out.eval()))
            # tf.print("q_out", q_out, output_stream=sys.stdout)
            # print("q " + str(q))
            # print("rand " + str(rand))
            # print("rand < epsilon " + str(rand < epsilon))
            # print("action[0] " + str(action[0]))

            next_state, reward, done, info = env.step(action[0])

            # Look at the next action probabilities
            curr_q = sess.run(q_out,
                              feed_dict = {inputs:np.identity(env.observation_space.n)[next_state:next_state + 1]})
            # Take the highest probability action
            max_next_q = np.max(curr_q)
            # Build a new probability action vector to train the network
            target_q = q
            target_q[0, action[0]] = reward + gamma * max_next_q

            # Train the network with the updated action probabilities
            # Update the action probability of action at step 'state'
            info, new_weights = sess.run([loss_update, weights],
                                         feed_dict={inputs:np.identity(env.observation_space.n)[state:state + 1], next_q:target_q})


            # print("new_weights")
            # print(new_weights)
            rewards_this_episode += reward
            state = next_state
            epochs += 1

            i_dbg += 1
            # if(3 < i_dbg):
            #     break

        epsilon = epsilon * epsilon_decay
        total_epochs += epochs
        total_rewards += rewards_this_episode
        # print ("rewards_this_episode: " + str(rewards_this_episode))

print ("Success rate: " + str(total_rewards/episodes))
