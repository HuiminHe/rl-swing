%% MAIN_swingSimulate.m
%
% this script simulate the swing dynamics with no control signal

clear;close all;clc

configSwing;

% Initial state;
q1 = deg2rad(50);
q2 = deg2rad(40);
dq1 = 0;
dq2 = 0;
z0 = [q1;q2;dq1;dq2];

tSpan = [0, 30];
u = 0;
dynFun = @(t, z) ( swingDynamics(z, u, p)); 

% Run simulation
sol = ode45(dynFun, tSpan, z0);
t = linspace(tSpan(1), tSpan(2), 10000);
z = deval(sol, t);
u = zeros(size(t));

A.plotFunc = @(t,z)( drawSwing(t,z,p,u) );
A.speed = 1.0;
A.figNum = 101;
A.verbose = true;

animate(t, z, A);

for i = 1:length(z)
    [t, u] = swingEnergy(z(:,i), p);
    en(i) = t + u;
end
figure()
plot(en);

