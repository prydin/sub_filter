""""{
        biquad_num_t x = ((biquad_num_t) *data) / 32768.0f;
        biquad_num_t y = 0.0f;
        for(uint32_t i = 0; i < num_stages; i++)
        {
            biquad_num_t *my_coeff = coeff + i * 6;
            // Print my_coeff for debugging
            // Serial.printf("Stage %d Coefficients: b0=%f, b1=%f, b2=%f, a0=%f, a1=%f, a2=%f\n", i, my_coeff[0], my_coeff[1], my_coeff[2], my_coeff[3], my_coeff[4], my_coeff[5]);
            biquad_num_t w = x - state[i * 2] * my_coeff[4] - state[i * 2 + 1] * my_coeff[5];
            y = w * my_coeff[0] + state[i * 2] * my_coeff[1] + state[i * 2 + 1] * my_coeff[2];
            state[i * 2 + 1] = state[i * 2];
            state[i * 2] = w;
            //   v Serial.printf("Stage %d: x=%f, y=%f, w=%f, state[%d]=%f, state[%d]=%f\n", i, x, y, w, i * 2, state[i * 2], i * 2 + 1, state[i * 2 + 1]);
            x = y;
        }
        *data = (int16_t) (y * 32768.0f);
    }
    """
import math

import matplotlib.pyplot as plt

#coeff = [1.039336563849e-09, 2.078673127698e-09, 1.039336563849e-09, 1.000000000000e+00, -1.979030470548e+00, 9.791590313361e-01, 1.000000000000e+00, 2.000000000000e+00, 1.000000000000e+00, 1.000000000000e+00, -1.991185019122e+00, 9.913143694883e-01 ]
coeff = [3.221897216955e-05, 6.443794433911e-05, 3.221897216955e-05, 1.000000000000e+00, -1.983881041661e+00, 9.840099175495e-01]
order = int(len(coeff) / 2)
state = [0] * order

def filter(x):
    for i in range(int(order / 2)):
        si = i * 2
        ci = i * 6
        my_coeff = coeff[ci:ci+6]
        w = x - state[si] * my_coeff[4] - state[si + 1] * my_coeff[5]
        y = w * coeff[0] + state[si] * my_coeff[0] + state[si + 1] * my_coeff[2]
        state[si + 1] = state[si]
        state[si] = w
        x = y
    return y

ys = []
for i in range(10000):
    x = math.sin(i / 50) #1 if i == 0 else 0
    y = filter(x)
    ys.append(y)
    # print(state)
    print(y)

plt.plot(ys)
plt.show()