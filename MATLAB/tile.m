%% tile.m
%
% function t = tile(x, x_min, x_max, n)
% This function saturate signal x in the interval of (x_min, x_max),
% divides it into n discrete sections and return the index

function t = tile(x, x_min, x_max, n)
if x <= x_min
    x = x_min + 1e-6;
end
if x >= x_max
   x = x_max - 1e-6; 
end
t = floor((x - x_min) / (x_max- x_min) * n) + 1;