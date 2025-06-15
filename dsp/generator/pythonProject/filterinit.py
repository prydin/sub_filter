import math

coef = [0] * 5
frequency = 100
q = 0.707
w0 = frequency * (2.0 * 3.141592654 / 44100)
sinW0 = math.sin(w0)
alpha = sinW0 / q * 2.0
cosW0 = math.cos(w0)
scale = 1.0 / (1.0 + alpha)
coef[0] = ((1.0 - cosW0) / 2.0) * scale
coef[1] = (1.0 - cosW0) * scale
coef[2] = coef[0]
coef[3] = (-2.0 * cosW0) * scale
coef[4] = (1.0 - alpha) * scale

print(coef)