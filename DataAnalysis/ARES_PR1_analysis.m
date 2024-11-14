%%% ARES PR Launch Data Analysis %%%

%% get data
data = readtable("data/Pocket_Rocket_ARES_Data.csv");

% remove NaNs from data
gpsData = rmmissing( data(:, {'GPSLat', 'GPSlong', 'GPSAlt'}));
lat = gpsData{:, 'GPSLat'};
long = gpsData{:, 'GPSlong'};
alt = gpsData{:, 'GPSAlt'};

% check to make sure data is the same length
if size(gpsLat) == size(gpsLong)
    disp(['size: ', string(size(gpsLat))])
    disp('wahoo')
else
    disp('waaaaaaaa')
end

%% plot 2D trajectory
figure;

hold on;
plot(long, lat, 'k.-', 'MarkerSize', 20)
plot(long(1), lat(1), 'g.-', 'MarkerSize', 20)
plot(long(end), lat(end), 'r.-', 'MarkerSize', 20)
hold off;

% label 2D plot
xlabel('longitude');
ylabel('lattitude');
grid on
savefig('figs/2d_traj')

%% geoplot
figure;
geoplot(lat, long, 'k.-', lat(1), long(1), 'g.', MarkerSize=15)
savefig('figs/2d_geoplot_traj')

%% plot 3D trajectory
figure;

plot3(long, lat, alt)

xlabel('longitude');
ylabel('lattitude');
zlabel('altitude');

savefig('figs/3d_traj')

%% analyze vertical velocity w/ BME data
t = data{:, 'SystemMillis'}(:,1);
alt = data{:, 'BMEApproximateAlt'}(:,1);

% calculate descent velocity (v = dz/dt)
N = length(alt);
v_z = NaN(N, 1);
for i = 1:N-100
    v_z(i) = ( alt(i+100)-alt(i) ) / ( (t(i+100)-t(i))/1000 );
    % avg descent rate over the next 100 samples 
end
v_z(end) = 0;
disp(v_z(:,1)) 

% % implement moving average filter
% dt = 500; % ms
% sampleAvg = 50; % use avg across 50 samples (25 sec)
% coeff = ones(1, sampleAvg)/sampleAvg; 
% v_mvngAvg = filter(coeff, 1, v_z);
% fDelay = (length(coeff)-1)/2; % filter delay

% plot descent velocity over time
figure();
hold on;
plot(t/60000,v_z, LineWidth=1.5)
% plot((t-fDelay)/60000,v_mvngAvg, LineWidth=2)

grid on

% reverse y-axis direction and label plot
hAx=gca;
hAx.YDir='reverse';

xlabel('time (min)')
ylabel('approximate descent rate (m/s)')

savefig('figs/descent_rate')