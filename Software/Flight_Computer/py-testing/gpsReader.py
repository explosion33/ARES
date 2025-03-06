import numpy as np
lat = 1000.5388 # ddmm.mmmm

lat_deg = np.floor(lat/100)
lat_min = lat%100
print(lat_deg, lat_min)

lat_rad = (lat_deg + lat_min/60) * np.pi/180
print(lat_rad)