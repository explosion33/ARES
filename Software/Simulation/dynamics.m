%%% Building System Dynamics and Transfer Functions %%%


% put linear ODE describing system angular dynamics here

% horizontal:

% verticle:


% transfer functions

num = [0.4339];
den = [1 1.87 10.46];
den_mod = [1 4 12];
G_h = tf(num, den);    % horizontal transfer function
                        % asymmetric deflection --> ang. velocity
G_h_mod = tf(num, den_mod);

G_v = tf(1,[2 1 7]);    % verticle transfer function
                        % symmetric deflection --> decent rate

% might run into issues bc decent rate initial condition is not zero

figure();
step(G_h)

figure();
step(G_h_mod)