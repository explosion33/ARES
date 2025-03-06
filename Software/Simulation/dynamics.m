%%% Building System Dynamics and Transfer Functions %%%
clear all; close all; clc;

% put linear ODE describing system angular dynamics here

% xDot = 

% from horizontal TF
n1 = 0.4339;
n2 = 1.933;
z = -n2/n1; % zero of TF
b = 1.87;
c = 10.46;

G_h_num = [n1 n2];
G_h_den = [1 b c];

% find system properties
K = n2/c; % Gain
w_n = sqrt(c); % natural frequency
zeta = b/(2*w_n); % damping ratio


G_h = tf(G_h_num, G_h_den)      % horizontal transfer function
                                % asymmetric deflection --> ang. velocity

%% Manually Manipulating TF to Slow the Response
K_mod = K*0.7; % reduce gain by 30% to model slower max w_z
w_n_mod = w_n*0.8; % reduce natural frequency by 20% to reduce rise time
zeta_mod = zeta*1.4; % increace damping ratio by 40%


b_mod = 2*zeta_mod*w_n_mod; % solve for new damping term
c_mod = (w_n_mod)^2;        % solve for new 0th order denomenator term
n2_mod = K_mod*c_mod;       % solve for new 0th order numerator term
n1_mod = -n2_mod/z;         % solve for new 1st order numerator term
                            % (keeping the same zero)

% build modified transfer function
num_mod = [n1_mod n2_mod];
den_mod = [1 b_mod c_mod];
G_h_mod = tf(num_mod, den_mod)

den_mod_tune = [1 b_mod c_mod 0]; % multiply by integrator TF (1/s)
G_h_mod_tune = tf(num_mod, den_mod_tune);

%% Verticle TF
G_v = tf(1,[2 1 7]);    % verticle transfer function
                        % symmetric deflection --> decent rate

% might run into issues bc decent rate initial condition is not zerof
% --> need to model gravity/weight as an input to the system

%% Plot Step Responses and Show Step Info
figure();
step(G_h)
title('Original TF')
S1 = stepinfo(G_h)


figure();
step(G_h_mod, 7)
title('Modified TF')
S_mod = stepinfo(G_h_mod)
