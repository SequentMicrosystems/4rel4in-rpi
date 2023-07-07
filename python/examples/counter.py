# Ensure counting function is enabled for all channels. Use 4rel4in -h for referance.

import sm_4rel4in

# Initialize the relay card
rel = sm_4rel4in.SM4rel4in(0)

# Define the channels to monitor (1 to 4) where signal is detected.
channel_1 = 1
channel_2 = 2

# Initialize count and sum variables for channel monitoring.
count = 0
sum_value = 0 
signal_detected = False
signal_2_detected = False

# Continuously monitor the channels for signals.
# The purpose of signal == 1 is to gather the state of a sensor/switch.
# In this case, a BALLUFF N.O. BES00PK sensor caused intermittent issues resulting in multiple "1" signals within a 3.5KHz frequency.
# To ensure accurate counting, only one "1" signal should be added per event.
# Thus, signal_# != 1 ensures that sum_value only increments by 1 and not 3500 per unit of time.
# This code can be extended to include up to 4 inputs, with 2 dynamic sums.

while True:
    signal_1 = rel.get_in(channel_1)
    signal_2 = rel.get_in(channel_2)

    if signal_1 == 1 and not signal_detected:
        count += 1
        signal_detected = True
    elif signal_1 != 1 and signal_detected:
        sum_value += count
        count = 0
        signal_detected = False

    if signal_2 == 1 and not signal_2_detected:
        sum_value -= 1
        signal_2_detected = True
    elif signal_2 != 1 and signal_2_detected:
        signal_2_detected = False

    print("Count:", count)
    print("Sum:", sum_value)
