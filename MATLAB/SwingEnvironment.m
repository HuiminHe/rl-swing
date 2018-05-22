% swingEnvironment.m
%
% This script define a swing environment updating the state precisely
% using Runge-Kutta method

classdef SwingEnvironment<handle
    properties
        p; % parameters
        z; % state
        t; % time
        theta0; % initial pivot angle
        maxSteps; % maxSteps before done
        stateDim; % state dimensions
        actionDim; % action dimension
    end
    methods
        function obj = SwingEnvironment(p, theta0, maxSteps)
            obj.p = p;
            obj.stateDim = 4;
            obj.actionDim = 1;
            if nargin < 3
                maxSteps = 5000;
            end
            if nargin < 2
                theta0 = 20;
            end
            obj.maxSteps = maxSteps;
            obj.theta0 = theta0;
            obj.reset();
        end
        function zz = reset(obj)
            obj.z = [(2*deg2rad(obj.theta0)*(rand()<0.5) - deg2rad(obj.theta0)) * rand(); 0; 0; 0];
            zz = obj.z;
            obj.t = 0;
        end
        function [z, r, done] = step(obj, u)
            % step current state using 4th-order Runge-Kutta
            k1 = obj.p.h * swingDynamics(obj.z, u, obj.p);
            k2 = obj.p.h * swingDynamics(obj.z + k1/2, u, obj.p);
            k3 = obj.p.h * swingDynamics(obj.z + k2/2, u, obj.p);
            k4 = obj.p.h * swingDynamics(obj.z + k3/2, u, obj.p);
            z = obj.z + (k1 + 2*k2 + 2*k3 + k4)/6;
            t = obj.t + obj.p.h;
            [r, done] = obj.cost();
            if done
                obj.reset();
            else
                obj.z = z;
                obj.t = t;
            end    
        end
        function [r, done] = cost(obj)
            [pc1, pc2, p1, p2, p3] = swingKinematics(obj.z, obj.p);
            if pc1(2) < 0
                r = -1;
                done = false;
            else
                r = 0;
                done = true;
            end
        end
    end
end