"""Example simulation using a pre-built paraglider model."""

import pfh.glidersim as gsim
import scipy.interpolate
import matplotlib.pyplot as plt
import numpy as np

# Component models: use a pre-built wing and add a suitable harness
wing = gsim.extras.wings.niviuk_hook3(size=23, verbose=False)
harness = gsim.paraglider_harness.Spherical(
    mass=20,
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
t = np.array([0, 3, 10])   # times
br = np.array([0, 1, 1]) # fraction (1 = 100%)

delta_br = scipy.interpolate.interp1d(
    t,      # times
    br,     # fraction (1 = 100%)
    fill_value=0,
    bounds_error=False,
)

# Inputs to the state dynamics are either constants or functions of time `t`
sim_parameters = {
    "delta_a": 0.0,
    "delta_bl": 0.0,
    "delta_br": delta_br,
    "delta_w": 0.0,
    "rho_air": 1.225,
    "v_W2e": (0, 0, 0),  # Uniform global wind velocity
}

# The state dynamics model provides the state derivatives
model = gsim.simulator.ParagliderStateDynamics6a(paraglider, **sim_parameters)

# Setup and run the simulation
state0 = model.starting_equilibrium()  # Start the simulation at equilibrium
dt = 0.5  # Record the state every 0.5 seconds
T = 10  # Run for 10 seconds

times, states = gsim.simulator.simulate(model, state0, dt=dt, T=T)

omega_z = np.array( states['omega_b2e'][:,2] )
print(omega_z)

fig, ax = plt.subplots(2, sharex=True)
ax[0].plot(t, br*100, 'b-')
ax[0].set_ylabel('right break deflection (%)')
ax[1].plot(times, omega_z*(360/(2*np.pi)), 'r-') # turn rad/s into deg/s
ax[1].set_ylabel('angular velocity (deg/s)')
ax[1].set_xlabel('time (s)')
ax[0].grid()
ax[1].grid()
plt.show()

# 3D plot: position over time
points = gsim.extras.simulation.sample_paraglider_positions(model, states, times)
# print(type(points), points)

# gsim.extras.plots.plot_3d_simulation_path(**points)