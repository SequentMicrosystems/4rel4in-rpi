import sm_4rel4in
import time
import sys


def progress(count, total):
    bar_len = 80
    filled_len = int(round(bar_len * (count + total / 2) / float(total)))
    if filled_len < 0:
        filled_len = 0
    if filled_len > bar_len:
        filled_len = bar_len
    #percents = round(100.0 * count / float(total), 1)
    bar = '=' * filled_len + '-' * (bar_len - filled_len)
    sys.stdout.write("\r%s[%s]%s %s%s" % (-1*total/2, bar, total/2, count, '    '))
    sys.stdout.flush()


rel = sm_4rel4in.SM4rel4in(0)
rel.set_encoder_cfg(1, 1)
try:
    while True:
        time.sleep(0.02)
        val = rel.get_encoder(1)
        progress(val, 200)
        # sys.stdout.write("\r%d      " % val)
        # sys.stdout.flush()
except KeyboardInterrupt:
    rel.reset_encoder(1)
    print("bye")
