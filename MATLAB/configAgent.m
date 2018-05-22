%% ConfigAgent.m
%
% This script configures parameters of the learning agent

agent.trainSteps = 2000000;
agent.testSteps = 20000;
agent.epiSteps = 20000;
agent.epsilon = 0.1;
agent.alpha = 0.1;
agent.gamma = 0.99;
agent.lim = [-pi/2, pi/2; -pi, pi; -pi, pi; -2*pi, 2*pi];
agent.bins = [5, 5, 9, 9];
agent.Q = rand([agent.bins, 3]) / sqrt(agent.trainSteps);
agent.actionBins = 3;
agent.U = 10; % max torque on elbow joint