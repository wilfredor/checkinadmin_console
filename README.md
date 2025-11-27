# checkinadmin_console

CLI tool to register equipment check-ins/outs. Supports Spanish/English at runtime, configurable data file, CSV export, and a simple summary.

## Build
- macOS/Linux (auto compiler): `make` (release), `make debug`
- Force compiler: `make mac` (clang), `make linux` (gcc)
- Windows (cross from Linux): `make win` (needs mingw-w64)
- Drop binaries into `bin/` (ignored by git): `make bin`

## Run
- Spanish (default): `./checkin`
- English: `./checkin --lang=en`
- Custom data file: `./checkin --file=/path/to/pcs.dat`
- Menu options: check-in, check-out, search, list, export CSV, summary/exit.

## Features
- Input validation and duplicate handling (prompt to overwrite).
- List all records and export to CSV.
- Summary of total/in/out; shows current data file on start.
- Normalizes serials (trim + uppercase) for consistent lookup.

## CI / artifacts
- GitHub Actions workflow (`.github/workflows/build.yml`) builds on macOS/Linux and cross-compiles Windows on Ubuntu, uploading artifacts per platform.
- Tag `v*` creates a Release and attaches binaries automatically.
- Downloads: https://github.com/wilfredor/checkinadmin_console/releases
