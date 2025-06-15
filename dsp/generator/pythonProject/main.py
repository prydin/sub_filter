from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

sample_rate = 44100
main_taps = 100
interp_taps = 50
f_c = 50
decimation_factor = 10
order = 4

for f in range(50, 250):
    sos = signal.butter(order, f, fs=sample_rate, output="sos")
    for i in range(int(order / 2)):
        print(f"{sos[i][0]:.12e}, {sos[i][1]:.12e}, {sos[i][2]:.12e}, {sos[i][4]:.12e}, {sos[i][5]:.12e},")