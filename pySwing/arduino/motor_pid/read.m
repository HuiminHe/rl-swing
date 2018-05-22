clear
clc

a = arduino('COM0', 'Uno', 'Libraries', 'rotaryEncoder');
encoder = rotaryEncoder(a, 'D18', 'D19');

while true
    [count, time] = readCount(encoder);
end