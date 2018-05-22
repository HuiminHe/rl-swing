%% swingTest.m
%
% This script load a pretrained Q-table simulate the task with optimal
% strategy

function swingTest(env, agent)
zz = env.reset();    
for jj = 1:agent.testSteps
    for ii = 1:env.stateDim
        idx(ii) = tile(zz(ii), agent.lim(ii, 1), agent.lim(ii, 2), agent.bins(ii));
    end
    [v, i] = max(agent.Q(idx(1), idx(2), idx(3), idx(4), :));
    u = i*agent.U - 2*agent.U;
    [zz, rr, done] = env.step(u);
    [tt, uu] = swingEnergy(zz, env.p);
    en(jj) = tt + uu;
    if mod(jj, 5) == 1
        figure(1)
        drawSwing(env.t, zz, env.p, u);
        figure(2)
        plot(max(1, jj-1000):jj, en(max(1, jj-1000):jj));
    end
    if mod(jj, 5000) == 0 || done
        if done
            disp('task completed');
        zz = env.reset();
    end
end
end