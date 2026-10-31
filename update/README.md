# update

This is the [Smart Relays with Universal Inputs 8-Layer Stackable HAT for Raspberry Pi](https://sequentmicrosystems.com/products/smart-relays-with-universal-inputs-for-raspberry-pi) firmware update tool.

## Usage

```bash
git clone https://github.com/SequentMicrosystems/4rel4in-rpi.git
cd 4rel4in-rpi/update/
./update 0
```

## Usage for 64-bit OS

```bash
git clone https://github.com/SequentMicrosystems/4rel4in-rpi.git
cd 4rel4in-rpi/update/
./update64 0
```

If you cloned the repository already, skip the first step. 
The command downloads the latest firmware from our server and writes it to the board.
Provide the board stack level as a parameter. 
During the firmware update, we strongly recommend disconnecting all outputs from the board, since they can change state unpredictably. Make sure no one else uses the I2C port during the update process. 
