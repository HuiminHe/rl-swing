%% derive_swing.m
% 
% This script derive the equations of motion for a swing model.

clear all;clc;
syms q1 q2 dq1 dq2 ddq1 ddq2 'real'     %  state variables
syms u 'real'                           %  actuation
syms m1 m2 g I1 I2 l1 l2 d2 lc1 lc2 'real' %  physical parameters

% d2 = 0;

%% Unit vectors
i = sym([1; 0]);
j = sym([0; 1]);

e1 = cos(q1)*(-j) + sin(q1)*( i);
e2 = cos(q1+q2)*(-j) + sin(q1+q2)*( i);
e3 = sin(q1)*(-j) + cos(q1)*(-i);

%% State
q  = [q1;q2];
dq = [dq1;dq2];
z = [q1;q2;dq1;dq2];
dz =[dq1;dq2;ddq1;ddq2];

%% Kinematics%% Cross2d
cross2d = @(a, b)(a(1)*b(2) - a(2)*b(1));

p1 = l1 * e1;
p2 = l1 * e1 + d2 * e3 + l2 * e2;
p3 = l1 * e1 + d2 * e3; 

pc1 = lc1 * e1;
pc2 = l1 * e1 + d2 * e3 + lc2 * e2;

dpc1 = jacobian(pc1, z)*dz;
dpc2 = jacobian(pc2, z)*dz;

% Energy
T1 = 0.5*I1*dq1^2 + 0.5*m1*(dpc1'*dpc1);
T2 = 0.5*I2*(dq1 + dq2)^2 + 0.5*m2*(dpc2'*dpc2);
T = T1 + T2;
U = m1*g*pc1(2) + m2*g*pc2(2);

L = T - U;

% H*ddq + C(q, dq) + G = b*u
tmp = jacobian(T, dq);
H = simplify(jacobian(tmp, dq));
C = simplify(jacobian(tmp, q) * dq - transpose(jacobian(T, q)));
G = simplify(transpose(jacobian(U, q)));
B = [0; 1];

%% Generate an optimized matlab function for dynamics
matlabFunction(H, C, G, B,...
    'file', 'auto_swingDynamics.m',...
    'vars', {q1, q2, dq1, dq2, g, l1, l2, lc1, lc2, d2, m1, m2, I1, I2},...
    'outputs', {'H', 'C', 'G', 'B'});

matlabFunction(pc1, pc2, p1, p2, p3,...
    'file', 'auto_swingKinematics.m',...
    'vars', {q1, q2, l1, l2, lc1, lc2, d2},...
    'outputs', {'pc1', 'pc2', 'p1', 'p2', 'p3'});

matlabFunction(T, U,...
    'file', 'auto_swingEnergy.m',...
    'vars', {q1, q2, dq1, dq2, g, l1, l2, lc1, lc2, d2, m1, m2, I1, I2},...
    'outputs', {'T', 'U'});