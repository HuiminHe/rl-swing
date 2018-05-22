%% swingEnergy.m
%
% [T, U] = swingEnergy(z, p)
function [T, U] = swingEnergy(z, p)
q1  = z(1);
q2  = z(2);
dq1 = z(3);
dq2 = z(4);
[T, U] = auto_swingEnergy(q1, q2, dq1, dq2, p.g, p.l1, p.l2, p.lc1,...
    p.lc2, p.d2, p.m1, p.m2, p.I1, p.I2);
end