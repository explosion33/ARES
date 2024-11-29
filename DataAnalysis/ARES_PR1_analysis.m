%%% ARES PR Launch Data Analysis %%%


%% get data
data = readtable("data/Pocket_Rocket_ARES_Data.csv");

% remove NaNs from data
gpsData = rmmissing( data(:, {'GPSLat', 'GPSlong', 'GPSAlt'}));
gpsLat = gpsData{:, 'GPSLat'};
gpsLong = gpsData{:, 'GPSlong'};
gpsAlt = gpsData{:, 'GPSAlt'};

t = data{:, 'SystemMillis'}(:,1)/1000; % convert miliseconds to seconds

% check to make sure data is the same length
if size(gpsLat) == size(gpsLong)
    disp(['size: ', string(size(gpsLat))])
    disp('wahoo')
else
    disp('wAAAaaaaa')
end


%% plot 2D GPS trajectory
figure;

hold on;
plot(gpsLong, gpsLat, 'k.-', 'MarkerSize', 20)
plot(gpsLong(1), gpsLat(1), 'g.-', 'MarkerSize', 20)
plot(gpsLong(end), gpsLat(end), 'r.-', 'MarkerSize', 20)
hold off;

% label 2D plot
title('ARES 2D Flight Trajectory')
xlabel('longitude');
ylabel('lattitude');
grid on
savefig('figs/ARES_2d_traj')


%% geoplot
figure;
geoplot(gpsLat, gpsLong, 'k.-', gpsLat(1), gpsLong(1), 'g.', MarkerSize=15)
title('ARES 2D Flight Trajectory (GPS)')
savefig('figs/ARES_2d_geoplot_traj')


%% plot 3D GPS trajectory
figure;

% plot3(long, lat, alt, 'b.-') % lat long axes
% xlabel('longitude');
% ylabel('lattitude');
% zlabel('altitude');

% use haversine to change lat long into meters fromm start position
N = length(gpsLat);
coordsConstLong = NaN(N, 2);
coordsConstLat = NaN(N, 2);
coordsStart = NaN(N, 2);
for i=1:N
    coordsConstLat(i,:) = [gpsLat(1) gpsLong(i)];
    coordsConstLong(i,:) = [gpsLat(i) gpsLong(1)];
    coordsStart(i,:) = [gpsLat(1) gpsLong(1)];
end

x_long = NaN(N, 1);
y_lat = NaN(N, 1);
for i=1:N
    x_long(i) = haversine(coordsStart(i,:), coordsConstLat(i,:));
    y_lat(i) = haversine(coordsStart(i,:), coordsConstLong(i,:));
end

plot3(x_long, y_lat, gpsAlt, 'b.-') 
xlabel('x distance (m)'); % longitudinal 
ylabel('y distance (m)'); % lattitudinal
zlabel('altitude (m)'); 
axis equal

title('ARES 3D Flight Trajectory (GPS)')
grid on

savefig('figs/ARES_3d_traj')


%% get 3D trajectory w/ accelerometer data (IN PROGRESS)
accel = data{:, {'BNOAccelX', 'BNOAccelY', 'BNOAccelZ'}};
% accel(:,1) = accel(:,1) - data{:, {'BNO_Grav_X'}}; % idk what to do with the gravity

% initialize position and velocity = [0, 0, 0]
N = length(accel);
pos = NaN(N, 3);
pos(1, :) = [0 0 0];
vel = NaN(N, 3);
vel(1, :) = [0 0 0];

% use projectile motion equations
for i=1:N-1
    dt = t(i+1)-t(i);
    vel(i+1,:) = vel(i,:) + accel(i,:)*dt;
    pos(i+1,:) = pos(i,:) + vel(i,:)*dt + accel(i,:)*dt^2;
end

figure;
plot(t, pos(:,3))


%% analyze vertical velocity w/ BME data
bmeAlt = data{:, 'BMEApproximateAlt'}(:,1);

figure('Position',[10 10 600 800]);

% plot altitude over time
subplot(2,1,1)
plot(t/60, bmeAlt, 'r-');
xlabel('time (min)')
ylabel('approx. altitude (m)')
title('ARES Altitude vs. Time')
xlim([0 16])
grid on

% calculate descent velocity (v = dz/dt)
N = length(bmeAlt);
v_z = NaN(N, 1);
nEuler = 1;
for i = 1:N-nEuler
    v_z(i) = ( bmeAlt(i+nEuler)-bmeAlt(i) ) / ( (t(i+nEuler)-t(i)) );
    % avg descent rate over the next 100 samples 
end
v_z(end) = 0;
disp(v_z(:,1)) 

% implement moving average filter
dt = 500; % ms
sampleAvg = 100; % use avg across 50 samples (25 sec)
coeff = ones(1, sampleAvg)/sampleAvg; 
v_mvngAvg = filter(coeff, 1, v_z);
fDelay = (length(coeff)-1)/2; % filter delay

% plot descent velocity over time
subplot(2,1,2)
hold on;
% plot(t/60, v_z, LineWidth=1)
plot((t-fDelay)/60,v_mvngAvg, 'b-', LineWidth=1)

grid on

% reverse y-axis direction and label plot
hAx=gca;
hAx.YDir='reverse';

% label plot
xlabel('time (min)')
ylabel('approx. descent rate (m/s)')
title('ARES Descent Rate vs. Time')

savefig('figs/ARES_descent_rate') 