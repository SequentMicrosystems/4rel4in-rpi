import sm_4rel4in
import time

rel = sm_4rel4in.SM4rel4in(0)

for i in range(4):
    rel.set_relay(i + 1, 1)
    time.sleep(.05)
    print("Relays: " + str(rel.get_all_relays()))

time.sleep(1)
for i in range(4):
    rel.set_relay(i + 1, 0)
    time.sleep(.05)
    print("Relays: " + str(rel.get_all_relays()))

for i in range(4):
    print("In channel " + str(i + 1) + " state " + str(rel.get_in(i + 1)))

print(" All input channels value: " + str(rel.get_all_in()))

for i in range(4):
    print("In ac channel " + str(i + 1) + " state " + str(rel.get_ac_in(i + 1)))

print(" All ac input channels value: " + str(rel.get_all_ac_in()))

for i in range(4):
    print("Config counting on  channel " + str(i + 1) + " state is " + str(rel.get_count_cfg(i + 1)))

print("Enable counting on all channels ")
for i in range(4):
    rel.set_count_cfg(i + 1, 1)

for i in range(4):
    print("Channel " + str(i + 1) + " count = " + str(rel.get_count(i + 1)))

for i in range(4):
    print("Reset count on channel " + str(i + 1))
    rel.reset_count(i + 1)
    time.sleep(0.05)

for i in range(4):
    print("Channel " + str(i + 1) + " count = " + str(rel.get_count(i + 1)))

for i in range(4):
    rel.set_led_cfg(i + 1, sm_4rel4in.LED_MANUAL)  # All LEDS to manual

for i in range(4):
    rel.set_led(i + 1, 1)
    time.sleep(.15)
    print("Leds: " + str(rel.get_all_leds()))

time.sleep(1)
for i in range(4):
    rel.set_led(i + 1, 0)
    time.sleep(.15)
    print("Leds: " + str(rel.get_all_leds()))

for i in range(4):
    rel.set_led_cfg(i + 1, sm_4rel4in.LED_AUTO)  # All LEDS to auto
