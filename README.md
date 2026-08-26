# Hyggshi OS

Hyggshi OS is a custom Linux distribution built on **Linux Mint 22.3 “Zena”** (Ubuntu Noble base).

## Build model

The repository is intentionally Mint-only. Legacy builders for Debian, Ubuntu, Alpine, Arch and Fedora have been removed.

- `config/` — Calamares and Hyggshi branding configuration
- `packages/hyggshi/` — Hyggshi applications, themes and bundled packages
- `scripts/base/` — Linux Mint rootfs bootstrap
- `scripts/target/` — scripts executed inside the target root filesystem
- `scripts/components/` — optional source/build helpers such as NexWM and Welcome
- `scripts/host/` — host-side build and ISO packaging scripts
- `build/` — local/generated build output (ignored by Git)
- `.github/workflows/` — reproducible GitHub Actions ISO builder

## Base OS

Current base: **Linux Mint 22.3 “Zena”**, Cinnamon/Xfce/MATE/KDE/LXQt/GNOME selectable at build time, while the base repository remains Linux Mint-only.

The Linux Mint release list currently identifies 22.3 as the newest supported Mint release, with support through April 2029. See the official release list: https://www.linuxmint.com/download_all.php

## GitHub Actions

Open **Actions → Build Hyggshi OS (Linux Mint) → Run workflow**. Customize desktop, branding URLs, feature flags, optional packages and NexWM build settings, then download the generated ISO from the workflow artifacts.

## Local build

```bash
export HYGGSHI_VERSION_ID=1.0
export DE=cinnamon
bash scripts/host/local-build.sh
```

The ISO and intermediate files are written under `build/`.
