#!/usr/bin/env bash
# Bootstrap the Hyggshi OS root filesystem from plain Ubuntu (no Linux Mint
# repositories, no Mint keyring/packages).
set -Eeuo pipefail
[ "${DEBUG_MODE:-false}" = "true" ] && set -x

: "${UBUNTU_CODENAME:=noble}"
: "${BASE_CODENAME:=${UBUNTU_CODENAME}}"
: "${UBUNTU_MIRROR:=http://archive.ubuntu.com/ubuntu}"
: "${SECURITY_MIRROR:=http://security.ubuntu.com/ubuntu}"
: "${CHROOT_DIR:=build/chroot}"

mkdir -p "$CHROOT_DIR"

echo "===== Hyggshi OS base: Ubuntu ${BASE_CODENAME} ====="

sudo debootstrap --arch=amd64 --variant=minbase \
  "$BASE_CODENAME" "$CHROOT_DIR" "$UBUNTU_MIRROR/"

sudo mkdir -p "$CHROOT_DIR/dev/pts" "$CHROOT_DIR/run" "$CHROOT_DIR/sys" "$CHROOT_DIR/proc"
sudo mount --bind /dev "$CHROOT_DIR/dev"
sudo mount --bind /dev/pts "$CHROOT_DIR/dev/pts" || true
sudo mount --bind /run "$CHROOT_DIR/run"
sudo mount -t proc proc "$CHROOT_DIR/proc"
sudo mount -t sysfs sysfs "$CHROOT_DIR/sys"

sudo cp -L /etc/resolv.conf "$CHROOT_DIR/etc/resolv.conf"

sudo tee "$CHROOT_DIR/etc/apt/sources.list" >/dev/null <<EOF
deb ${UBUNTU_MIRROR} ${BASE_CODENAME} main restricted universe multiverse
deb ${UBUNTU_MIRROR} ${BASE_CODENAME}-updates main restricted universe multiverse
deb ${SECURITY_MIRROR} ${BASE_CODENAME}-security main restricted universe multiverse
EOF

sudo chroot "$CHROOT_DIR" env DEBIAN_FRONTEND=noninteractive apt-get update
sudo chroot "$CHROOT_DIR" env DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends ca-certificates curl gnupg dirmngr

{
  echo "BASE_CODENAME=${BASE_CODENAME}"
  echo "BASE_DISTRO=ubuntu"
  echo "UBUNTU_CODENAME=${UBUNTU_CODENAME}"
  echo "DISTRO_LABEL=Ubuntu ${BASE_CODENAME}"
} >> "${GITHUB_ENV:-build/build.env}"

echo "===== Ubuntu rootfs ready ====="
