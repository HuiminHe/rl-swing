%% swingKinematics.m
%
% function [pc1, pc2, p1, p2, p3] = swingKinematics(z, p)

%% Code
function [pc1, pc2, p1, p2, p3] = swingKinematics(z, p)
q1  = z(1);
q2  = z(2);
[pc1, pc2, p1, p2, p3] = auto_swingKinematics(q1, q2, p.l1, p.l2, p.lc1, p.lc2, p.d2);
end