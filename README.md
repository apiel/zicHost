## Installation

```sh
sudo apt install librtaudio-dev librtmidi-dev libsndfile1-dev
```

### On RPi

Might Need to install pulse audio as well:

```sh
sudo apt-get install alsa-base pulseaudio
```

If not started audomatically, it can be started with:

```sh
pulseaudio --start
```

## Hint

Get list of audio card:

```sh
cat /proc/asound/cards
```

or 

```sh
arecord -l
```

or

```sh
aplay -l
```

