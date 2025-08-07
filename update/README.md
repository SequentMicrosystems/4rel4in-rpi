# update

This is the [Smart Relays with Universal Inputs 8-Layer Stackable HAT for Raspberry Pi](https://sequentmicrosystems.com/products/smart-relays-with-universal-inputs-for-raspberry-pi) firmware update tool.

## Usage

```bash
git clone https://github.com/SequentMicrosystems/4rel4in-rpi.git
cd 4rel4in-rpi/update/
./update 0
```

If you cloned the repository already, skip the first step. 
The command will download the newest firmware version from our server and write it to the board.
The stack level of the board must be provided as a parameter. 
During firmware update, we strongly recommend disconnecting all outputs from the board, since they can change state unpredictably. Make sure no one else uses the I2C port during the update process. 
