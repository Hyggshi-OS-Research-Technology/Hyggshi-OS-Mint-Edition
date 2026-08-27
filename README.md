# Hyggshi OS

Hyggshi OS is a custom Linux distribution built directly on **Ubuntu 24.04 “Noble Numbat”**.

## Build model

The repository is intentionally Ubuntu-only. Legacy builders for Debian, Linux Mint, Alpine, Arch and Fedora have been removed.

- `config/` — Calamares and Hyggshi branding configuration
- `packages/hyggshi/` — Hyggshi applications, themes and bundled packages
- `scripts/base/` — Ubuntu rootfs bootstrap
- `scripts/target/` — scripts executed inside the target root filesystem
- `scripts/components/` — optional source/build helpers such as NexWM and Welcome
- `scripts/host/` — host-side build and ISO packaging scripts
- `build/` — local/generated build output (ignored by Git)
- `.github/workflows/` — reproducible GitHub Actions ISO builder

## Base OS

Current base: **Ubuntu 24.04 “Noble Numbat”**, Cinnamon/Xfce/MATE/KDE/LXQt/GNOME selectable at build time, while the base repository remains Ubuntu-only.

Ubuntu 24.04 LTS is supported through April 2029. See the official release notes: https://wiki.ubuntu.com/NobleNumbat/ReleaseNotes

## GitHub Actions

Open **Actions → Build Hyggshi OS (Ubuntu) → Run workflow**. Customize desktop, branding URLs, feature flags, optional packages and NexWM build settings, then download the generated ISO from the workflow artifacts.

## Local build

```bash
export HYGGSHI_VERSION_ID=1.0
export DE=cinnamon
bash scripts/host/local-build.sh
```

The ISO and intermediate files are written under `build/`.

## QEMU testing (important)

Calamares can only install to a real target disk. If QEMU is started with only
`-cdrom` and no HDD/SSD is attached, the installer Welcome page can report
`There are no partitions to install on.` Calamares' own documentation notes
that new virtual media can be partitioned from the Partitions page; the test
machine therefore needs an attached writable virtual disk. [Calamares partitioning documentation]

Use the included runner so every test VM gets a 32 GiB QCOW2 disk automatically:

```bash
chmod +x scripts/host/run-qemu.sh
scripts/host/run-qemu.sh ./hyggshi-os-local.iso
```

You can override the disk, RAM, and CPU count:

```bash
QEMU_DISK_SIZE=48G QEMU_RAM=6G QEMU_SMP=6 \
  scripts/host/run-qemu.sh ./hyggshi-os-local.iso
```

The Calamares Welcome storage check is also no longer a hard blocker. It can
warn about storage, but the installer is allowed to continue to the Partitions
page so a fresh/blank QEMU disk can have its partition table created there.
This follows Calamares' documented separation between the Welcome checks and
the actual partitioning UI. [Calamares welcome/partition documentation]

## Hyggshi Welcome

Hyggshi Welcome is built from `packages/hyggshi/hyggshi-welcome/` and installed
into the ISO by both GitHub Actions and `scripts/host/local-build.sh` by default.
The installed app lives at `/usr/bin/hyggshi-welcome` and uses the system-wide
`/etc/xdg/autostart/hyggshi-welcome.desktop` entry. The first-run marker is stored
per user, so the wizard can finish setup without affecting other accounts.

## GitHub Actions

Builds are Ubuntu-only. Run **Build Hyggshi OS (Ubuntu)** from Actions. The workflow defaults to Cinnamon, Hyggshi Auto Theme, and Hyggshi Welcome.

Hyggshi Welcome source is committed at `packages/hyggshi/hyggshi-welcome/` and is built inside the target chroot, so the CI job verifies both `/usr/bin/hyggshi-welcome` and the system-wide autostart entry.
