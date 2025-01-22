"""Example simulation using a pre-built paraglider model."""

import pfh.glidersim as gsim
import scipy.interpolate
import scipy.io
import matplotlib.pyplot as plt
import numpy as np


def lines_to_as(delta_1, delta_2):
    delta_a = delta_1 - delta_2
    delta_s = np.average([delta_1, delta_2])
    return delta_a, delta_s 


T = 20  # Run sim for T seconds

# Component models: use a pre-built wing and add a suitable harness
wing = gsim.extras.wings.niviuk_hook3(size=23, verbose=False)
harness = gsim.paraglider_harness.Spherical(
    mass=30,
    z_riser=0.5,
    S=0.7,
    CD=1,
    kappa_w=0.1,
)


# The system dynamics model provides the physical accelerations
paraglider = gsim.paraglider.ParagliderSystemDynamics6a(wing, harness)

# Build a control input sequence for right brake using linear interpolation:
# the input is 0 until t=3, increases to 1 over t=3..5, holds at 1 until t=22,
# decreases to zero over t=22..23, and holds at 0 indefinitely.

# ### STEP ###
# t = np.array([0, 0.99, 1, T])   # times (s)
# br = np.array([0, 0, 1, 1]) # fraction (1 = 100%)
# bl = np.array([0, 0, 0, 0])
# ### ^^^ ###

### RAMP ###
t = np.array([0, 1, 5, 8, T])   # times (s)
br = np.array([0, 0, 1, 1, 0]) # fraction (1 = 100%)
bl = np.array([0, 0, 0, 0, 0])
### ^^^ ###


delta_bl = scipy.interpolate.interp1d(
    t,      # times
    bl,     # fraction (1 = 100%)
    fill_value=0,
    bounds_error=False,
)

delta_br = scipy.interpolate.interp1d(
    t,      # times
    br,     # fraction (1 = 100%)
    fill_value=0,
    bounds_error=False,
)

# Inputs to the state dynamics are either constants or functions of time `t`
sim_parameters = {
    "delta_a": 0.0,
    "delta_bl": delta_bl, # TRY SYMMETRIC BREAIKING
    "delta_br": delta_br,
    "delta_w": 0.0,
    "rho_air": 1.225,
    "v_W2e": (0, 0, 0),  # Uniform global wind velocity
}

# The state dynamics model provides the state derivatives
model = gsim.simulator.ParagliderStateDynamics6a(paraglider, **sim_parameters)

# Setup and run the simulation
state0 = model.starting_equilibrium()  # Start the simulation at equilibrium

dt = 0.1  # Record the state every 0.5 seconds

times, states = gsim.simulator.simulate(model, state0, dt=dt, T=T)

omega_z = np.array( states['omega_b2e'][:,2] )
v_z = np.array( states['v_RM2e'][:,2] )


# print(v_z)

# initialize deflection arrays
delta_1 = delta_br(times)
delta_2 = delta_bl(times)
delta_s = np.zeros(len(times))
delta_a = np.zeros(len(times))
# calculate asymetric and symetric deflection from the deflections on lines 1 and 2
# asymetric deflection is the difference between delta_1 and delta_2
# symetric deflection is the average between delta_1 and delta_2
for i in range(len(times)):
    delta_a[i], delta_s[i] = lines_to_as(delta_1[i], delta_2[i])


## plot useful stuff
fig, ax = plt.subplots(4, 2, sharex=True, figsize = [12,8])
ax[0,0].plot(times, delta_1*100, 'b-')
ax[0,0].set_ylabel('right break deflection (%)')
ax[1,0].plot(times, omega_z*(360/(2*np.pi)), 'r-') # turn rad/s into deg/s
ax[1,0].set_ylabel('angular velocity (deg/s)')
ax[2,0].plot(times, delta_a, 'm-')
ax[2,0].set_ylabel('asymetric deflection')
ax[3,0].plot(times, delta_s, 'c-')
ax[3,0].set_ylabel('symetric deflection')
ax[-1,0].set_xlabel('time (s)')
plt.tight_layout()
for a in ax:
    for x in a:
        x.grid()

# 3D plot: position over time
points = gsim.extras.simulation.sample_paraglider_positions(model, states, times)
gsim.extras.plots.plot_3d_simulation_path(**points)
# print(type(points), points)

print(f"times: {times.shape} {times}")
print(f"asymetric deflection: {delta_a.shape} {delta_a}")
print(f"angular velocity: {omega_z.shape} {omega_z}")

matdict = dict(times=times, delta_a=delta_a, omega_z=omega_z)
scipy.io.savemat('simdata.mat', matdict, oned_as='column')

plt.show()