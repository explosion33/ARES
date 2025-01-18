%%% Building System Dynamics and Transfer Functions %%%


% put linear ODE describing system angular dynamics here

% horizontal:

% verticle:


% transfer functions

G_h_num = [1];
G_h_den = [3 2 1];

G_h = tf(1,[3 2 1]);    % horizontal transfer function
                        % asymmetric deflection --> ang. velocity

G_v = tf(1,[2 1 7]);    % verticle transfer function
                        % symmetric deflection --> decent rate

% might run into issues bc decent rate initial condition is not zero

