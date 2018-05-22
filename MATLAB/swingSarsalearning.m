%% swingagent.Sarsalearning.m
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
R = 0;
zz_max = zeros(4,1);

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
    for kk = 1:length(zz)
        if abs(zz(kk)) > zz_max(kk)
            zz_max(kk) = abs(zz(kk));
        end
    end
    R = R + rr;
    for ii = 1:env.stateDim
        idxx(ii) = tile(zz(ii), agent.lim(ii, 1), agent.lim(ii, 2), agent.bins(ii));
    end
    [Q_max, ii] = max(agent.Q(idxx(1), idxx(2), idxx(3), idxx(4), :));
    if rand() < agent.epsilon
        ii = randi(agent.actionBins);
    end
    % update Q-table
    agent.Q(idx(1), idx(2), idx(3), idx(4), i) = agent.Q(idx(1), idx(2), idx(3), idx(4), i) + agent.alpha * (rr + agent.gamma*agent.Q(idxx(1), idxx(2), idxx(3), idxx(4), ii) -  agent.Q(idx(1), idx(2), idx(3), idx(4), i));
    
    if mod(jj, agent.epiSteps) == 0 || done
        fprintf('Episode %i finished in %i steps \n', episode, jj - lastSteps);
        zz = env.reset();
        steps(episode) = jj - lastSteps;
        rewards(episode) = R;
        R = 0;
        episode = episode + 1;
        if episode > 50
            agent.alpha = agent.alpha / (episode^0.3);
        end
        lastSteps = jj;
    end
end

fprintf('training finished.\n');

%% Energy
en = zeros(1, agent.testSteps);
[tt, uu] = swingEnergy(zz, p);
en0 = tt + uu;

for i = 1:length(steps)
    cumMean(i) = mean(steps(1:i));
end

figure()
plot(steps);
hold on;
plot(cumMean);

figure()
plot(rewards);

save('Q_table.mat', 'env', 'agent');