% swingDynamics.m
%
% ddp = swingDynamics(z, u, p)

function dz = swingDynamics(z, u, p)
q1  = z(1);
q2  = z(2);
dq1 = z(3);
dq2 = z(4);
[H, C, G, B] = auto_swingDynamics(q1, q2, dq1, dq2, p.g, p.l1, p.l2,...
    p.lc1, p.lc2, p.d2, p.m1, p.m2, p.I1, p.I2);

ddq = H\(B*u - C - G);

if p.constraint
    if q2+dq2*p.h< p.q2min || q2+dq2*p.h > p.q2max
        Q = [0; -H(2,2)*q2/p.h]; % constraint force
        ddq = H\(B*u + Q - C - G);
    end
end

dz = [dq1;dq2;ddq];
end