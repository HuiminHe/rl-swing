%% swingagent.Qlearning.m
%
% This script learn swing up using agent.Q-learning

clear all; close all; clc
% rand('seed',1); randn('seed',1); format short; format compact; 

configSwing;
configAgent;

env = SwingEnvironment(p, 10);

zz = env.reset();

episode = 1;
lastSteps = 0;
for jj = 1: agent.trainSteps
    for ii = 1:env.stateDim
        idx(ii) = tile(zz(ii), agent.lim(ii, 1), agent.lim(ii, 2), agent.bins(ii));
    end
    [v, i] = max(agent.Q(idx(1), idx(2), idx(3), idx(4), :));
    
    % e-greedy
    if rand() < agent.epsilon
        i = randi(agent.actionBins);
    end
    u = i*agent.U - 2*agent.U;
    
    [zz, rr, done] = env.step(u);    
    for ii = 1:env.stateDim
        idxx(ii) = tile(zz(ii), agent.lim(ii, 1), agent.lim(ii, 2), agent.bins(ii));
    end
    Q_max = max(agent.Q(idxx(1), idxx(2), idxx(3), idxx(4), :));
    
    % update Q-table
    agent.Q(idx(1), idx(2), idx(3), idx(4), i) = agent.Q(idx(1), idx(2), idx(3), idx(4), i) + agent.alpha * (rr + agent.gamma*Q_max -  agent.Q(idx(1), idx(2), idx(3), idx(4), i));
    
    if mod(jj, agent.epiSteps) == 0 || done
        fprintf('Episode %i finished in %i steps \n', episode, jj - lastSteps);
        zz = env.reset();
        steps(episode) = jj - lastSteps;
        episode = episode + 1;
        lastSteps = jj;
    end
end

disp('training finished.')
zz = env.reset();
episode = 1;
en = zeros(1, agent.testSteps);

[tt, uu] = swingEnergy(zz, p);
en0 = tt + uu;

for i = 1:length(steps)
    cumMean(i) = mean(steps(1:i))
end


figure()
plot(steps);
hold on;
plot(cumMean);
save('Q_table.mat', 'p', 'agent');