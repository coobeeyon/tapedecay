# Tape Decay

An audio plugin that simulates cassette tape decay. Quiet details are eaten first; louder parts survive. Available as [LADSPA](https://www.ladspa.org/) and [JUCE](https://juce.com/) (VST3, LV2, AU, Standalone).

Each sample is moved toward zero by a fixed amount, then rescaled — like a worn-out tape that loses its quietest information while keeping the overall level intact.

## Parameter

| Name | Range | Default | Description |
|------|-------|---------|-------------|
| **Decay** | 0.0 – 0.99 | 0.1 | Amount of quiet detail to eat per sample |

## JUCE (VST3 / LV2 / AU / Standalone)

### Build

```
cd juce
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Requires CMake 3.22+ and a C++17 compiler. JUCE is fetched automatically. On Linux, install these first:

```
sudo apt-get install libasound2-dev libfontconfig1-dev \
  libfreetype-dev libx11-dev libxcomposite-dev libxcursor-dev \
  libxext-dev libxfixes-dev libxinerama-dev libxrandr-dev \
  libxrender-dev libgl1-mesa-dev
```

Plugins are output to `juce/build/TapeDecay_artefacts/Release/`. AU is built on macOS only.

### Pre-built binaries

CI builds for Linux, Windows, and macOS are available as artifacts on the [Actions](../../actions) tab.

### DAW usage

Copy the `.vst3` or `.component` (AU) bundle to your DAW's plugin directory. The Decay parameter is automatable.

## LADSPA

### Build

```
make
```

Requires only a C compiler. No external dependencies beyond the LADSPA header (included).

### Install

```
make install
```

This copies `tapedecay.so` to `~/.ladspa/` and the PipeWire filter-chain config to `~/.config/pipewire/filter-chain.conf.d/`.

To load immediately (restarts PipeWire):

```
make load
```

### PipeWire

The included `tapedecay.conf` sets up a filter-chain sink. After `make load`, select "Tape Decay" as your audio output.

### Uninstall

```
make uninstall
```

Or to uninstall and restart PipeWire:

```
make unload
```

## License

[MIT](LICENSE)
