clear all;
close all;
clc;

% Step 1: Combined Probability Distributions
% Data from multiple datasets (converted to ft/s)
mph_to_fts = 1.46667; % Conversion factor from mph to ft/s

% normal distribution for wind speeds, based on far out wind site data from last june
mean_speed = 11.6 * mph_to_fts; % mean wind speed in far out site, june 2024
max_speed = 20 * mph_to_fts; % max mean wind speed permitted to launch
std_speed_ground = 11 * mph_to_fts; % estimated standard deviation of wind speed from far out wind site data from june
std_speed_altitude = 3; % estimated standard deviation of wind speed at high altitudes based on wind altitude data, without gust

% normal distribution for wind speeds based on far out wind site data from last june
mean_direction = deg2rad(240);   % Mean direction in radians, based on far out wind site data from last june
std_direction = deg2rad(10);     % Standard deviation in radians, estimated far out wind site data from last june

% Parameters for simulation
initial_altitude = 10000; % ft, starting altitude based on progress report 
final_altitude = 0; % ft, stop simulation at ground
descent_rate = 11.28; % ft/s, vertical descent rate based on progress report
glide_ratio_avg = 1; % parachute's forward glide ratio, past flight data from same parachute showed average glide ratio of 1:1
time_step = 1; % seconds per simulation step
num_simulations = 10000; % number of Monte Carlo runs
i = 1; % initialize simulation step

% Gust parameters
gust_probability = 0.1; % Probability of gust occurrence at each step
gust_std = 5; % standard deviation of wind gust speed (ft/s)

% direction change parameters
direction_change_probability = 0.05; % Probability of wind direction change at each step

% initialize results
landing_positions = zeros(num_simulations, 3); % store x, y, z positions
trajectory = zeros(5000,3);
wind_theta = zeros(5000,1);
altitude_all = zeros(5000,1);
wind_speed_dist = zeros(5000,1);

% Monte Carlo Simulation
while i < num_simulations
    
    % normal distribution of glide ratios
    glide_velocity = abs(descent_rate * (glide_ratio_avg * randn));
    
    % initialize positions
    altitude = initial_altitude;
    position_x = 500 * randn; 
    position_y = 500 * randn; 
    
    % initial wind conditions from normal distributions
    wind_speed_ground = rand * max_speed; % random sampling for ground, cannot exceed max speed 
    wind_speed_current = wind_speed_ground + std_speed_altitude * rand; % wind speed at the top is a random standard deviation from the ground speed
    wind_direction = mean_direction + std_direction * randn; % Normal sampling for wind directiaon at the top
    wind_u = wind_speed_ground * cos(wind_direction); % x-component of wind 
    wind_v = wind_speed_ground * sin(wind_direction); % y-component of wind 
    
    % Random initial parachute orientation
    parachute_orientation = deg2rad(360 * rand); % Random parachute orientation in radians
    glide_u = glide_velocity * cos(parachute_orientation); % x-component of glide
    glide_v = glide_velocity * sin(parachute_orientation); % y-component of glide
    
    time_incr = 1;

    % Simulate descent in time steps
    while altitude > final_altitude
       % Calculate wind speed and direction based on altitude, wind_speed_current is the approximate average of the current altitude
    
       % Apply random wind gusts
        if rand < gust_probability 
            wind_speed_current = abs(wind_speed_current + gust_std * randn); % add gust effects on speed
        end
    
        if rand < direction_change_probability
            wind_direction = wind_direction + deg2rad(360) * rand; % add gust effects on direction
            wind_direction = mod(wind_direction, 2*pi); 
        end
    
        wind_speed = wind_speed_current + sign(rand - 0.5) * std_speed_altitude * rand; 
        % wind speed at the current step is a random distribution above or below wind_speed_current
    
    % Direction change model a
        wind_direction = wind_direction + randn * std_direction; % use normal distribution for wind direction
        wind_direction = mod(wind_direction, 2*pi); % Ensure direction is in [0, 2π]
    
        % Update wind components
        wind_u = wind_speed * cos(wind_direction); 
        wind_v = wind_speed * sin(wind_direction);
        
        % Adjust horizontal velocities by combining glide and wind effects
        effective_u = glide_u + wind_u; % Total x-direction velocity
        effective_v = glide_v + wind_v; % Total y-direction velocity
        
        % Calculate horizontal drift for this time step
        drift_x = effective_u * time_step; % Drift in x-direction
        drift_y = effective_v * time_step; % Drift in y-direction
        
        % Update position
        position_x = position_x + drift_x;
        position_y = position_y + drift_y;
        
        % Update altitude
        altitude = altitude - (descent_rate * time_step); % Reduce altitude by descent rate
        
        % store data, plots generated for these parameters will be the last simulation
        trajectory(time_incr, 1) = position_x; % trajectory(time_incr, 1)  + position_x;
        trajectory(time_incr, 2) = position_y; % trajectory(time_incr, 2)  + position_y;
        trajectory(time_incr, 3) = altitude; % trajectory(time_incr, 3)  + altitude
        wind_speed_dist(time_incr) = wind_speed; 
        wind_theta(time_incr) = wind_direction; 
        altitude_all(time_incr) = altitude;
    
        % increment the time
        time_incr = time_incr + 1;
    end

    % data is only stored if the ground wind speed is below the max allowed speed
    ground = wind_speed(end);
    if ground < max_speed
        i = i + 1;
    end

    % Store final position
    landing_positions(i, :) = [position_x, position_y, 0]; % Z = 0 at ground level
end

% cut down vectors to the right size
trajectory = trajectory(1:(time_incr-1), :);
wind_theta = wind_theta(1:(time_incr-1), :);
altitude_all = altitude_all(1:(time_incr-1), :);
wind_speed_dist = wind_speed_dist(1:(time_incr-1), :);

% convert to miles
x = trajectory(:, 1) ./ 5280;  
y = trajectory(:, 2) ./ 5280;  
z = trajectory(:, 3) ./ 5280;  

% 3d trajectory plot for final simulation 
figure;                
plot3(x, y, z, 'LineWidth', 2); 
grid minor       
xlabel('X (mi)')   
ylabel('Y (mi)')           
zlabel('Z (mi)')          
title('3D Trajectory with no ARES')

% wind direction data for final simulation 
figure;
scatter(altitude_all, wind_theta, 8, 'filled') 
grid minor
xlabel('altitude (ft)')
ylabel('wind direction (radians)')
title('wind direction by altitude')

% wind speed for final simulation 
figure;
plot(altitude_all, wind_speed_dist)
grid minor
xlabel('altitude (ft)')
ylabel('wind speed (ft/s)')
title('wind speed by altitude')

% 2D scatter plot of landing positions
figure;
scatter(landing_positions(:,1)./5280, landing_positions(:,2)./5280, 15, 'filled');
title('2D Landing Scatter Plot with Wind Effects');
xlabel('X Distance (mi)');
ylabel('Y Distance (mi)');
xlim([-12 12]);
ylim([-12 12]);
grid on;

% Display statistics
mean_landing_position = mean(landing_positions(:, 1:2), 1);
fprintf('Mean Landing Position (X, Y): (%.2f, %.2f) ft\n', mean_landing_position(1), mean_landing_position(2));

% Distribution of distance away from launch pad
landing_distance = sqrt(landing_positions(:,1).^2 + landing_positions(:,2).^2);
landing_distance = landing_distance./5280; % ft to miles
mean_dist = mean(landing_distance);
std_dist = std(landing_distance);
range_dist = linspace(min(landing_distance), max(landing_distance), 1000);
pdf_dist = normpdf(range_dist, mean_dist, std_dist);

% distribution of landing distance without active ares control
figure;
histogram(landing_distance, 'Normalization', 'pdf');
hold on;
plot(range_dist, pdf_dist, 'LineWidth', 2);
hold off;
grid minor
title('Distribution of Landing Distance with No ARES Control');
xlabel('Distance (mi)')
ylabel('Probability Density')

prob_outside = mean(landing_distance > 12);
fprintf('Probability of landing over %f miles: %.4f\n', 12, prob_outside);
% end


