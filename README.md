# Tape Decay

A [LADSPA](https://www.ladspa.org/) audio plugin that simulates cassette tape decay. Quiet details are eaten first; louder parts survive.

Each sample is moved toward zero by a fixed amount, then rescaled — like a worn-out tape that loses its quietest information while keeping the overall level intact.

## Build

```
make
```

Requires only a C compiler. No external dependencies beyond the LADSPA header (included).

## Install

```
make install
```

This copies `tapedecay.so` to `~/.ladspa/` and the PipeWire filter-chain config to `~/.config/pipewire/filter-chain.conf.d/`.

To load immediately (restarts PipeWire):

```
make load
```

## Usage

### PipeWire

The included `tapedecay.conf` sets up a filter-chain sink. After `make load`, select "Tape Decay" as your audio output.

### Any LADSPA host

The plugin exposes a single control port:

| Port | Type | Range | Description |
|------|------|-------|-------------|
| **Decay** | Control | 0.0 – 0.99 | Amount of quiet detail to eat per sample |

Audio ports: stereo in (`Input L`, `Input R`) and stereo out (`Output L`, `Output R`).

## Uninstall

```
make uninstall
```

Or to uninstall and restart PipeWire:

```
make unload
```

## License

[MIT](LICENSE)
