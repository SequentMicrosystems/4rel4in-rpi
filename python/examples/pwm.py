import sm_4rel4in
import time

rel = sm_4rel4in.SM4rel4in(0)
print("PWM fill factor on channel 4 is " + str(rel.get_pwm_fill(4)) + " %")
print("PWM frequency on channel 4 is " + str(rel.get_frequency(4)) + " Hz")
