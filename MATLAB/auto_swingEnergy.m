function [T,U] = auto_swingEnergy(q1,q2,dq1,dq2,g,l1,l2,lc1,lc2,d2,m1,m2,I1,I2)
%AUTO_SWINGENERGY
%    [T,U] = AUTO_SWINGENERGY(Q1,Q2,DQ1,DQ2,G,L1,L2,LC1,LC2,D2,M1,M2,I1,I2)

%    This function was generated by the Symbolic Math Toolbox version 8.0.
%    01-Mar-2018 23:04:15

t2 = q1+q2;
t3 = cos(t2);
t5 = cos(q1);
t6 = sin(q1);
t12 = lc2.*t3;
t13 = l1.*t5;
t14 = d2.*t6;
t15 = t12+t13+t14;
t4 = dq1.*t15+dq2.*lc2.*t3;
t7 = sin(t2);
t8 = dq1.*(-d2.*t5+l1.*t6+lc2.*t7)+dq2.*lc2.*t7;
t9 = dq1+dq2;
t10 = dq1.^2;
t11 = lc1.^2;
T = I1.*t10.*(1.0./2.0)+m1.*(t5.^2.*t10.*t11+t6.^2.*t10.*t11).*(1.0./2.0)+I2.*t9.^2.*(1.0./2.0)+m2.*(t4.^2+t8.^2).*(1.0./2.0);
if nargout > 1
    U = -g.*m2.*t15-g.*lc1.*m1.*t5;
end
