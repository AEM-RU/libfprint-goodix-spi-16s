# Huawei MateBook D15 2022 / BoF-XX image capture fix

[Detailed investigation and fix (Chinese)](https://xingwangzhe.fun/posts/matebook-d15-goodix-gdix51c0-fingerprint/)

[中文](D15-README.md) · [Project home](README_EN.md)

This branch starts from [berkekbgz/libfprint-goodix-spi](https://github.com/berkekbgz/libfprint-goodix-spi) commit `010a665f54089a1632b1ab7be588b316ace934e2`. **Goodix** makes the fingerprint sensor and Huawei makes the laptop. The fingerprint device is `ACPI\GDIX51C0` on SPI. Three files under `drivers/gdix51c0/` were changed; inspect them with `git log -p -- drivers/gdix51c0/`.

On this BoF-XX, IRQ rises during the approximately 80 ms quiet SPI period after the image-mode ACK. The listener consumes that edge but does not recheck the IRQ level, which stays high, so the TLS image-data `0x17` wait times out. The fix wakes the listener when suppression ends and enters the quiet period in the ACK reader thread.

**Observed locally on 2026-10-11:** right index finger enrollment completed and one `fprintd-verify` returned `verify-match`. After temporary diagnostics were removed, the service still listed the enrolled finger; a later verification timed out while waiting for a touch. Reboot and GDM login have not been tested.

The installed library is `/opt/goodix-spi-driver/libfprint/libfprint-2.so.2.0.0`; the previous version is saved beside it with a `.pre-ack-quiet` suffix. The systemd drop-in is `/etc/systemd/system/fprintd.service.d/20-goodix-spi-libfprint.conf`, and the SPI binding rule is `/etc/udev/rules.d/70-libfprint-2.rules`. To roll back only this capture change, restore that backup with root permissions and restart `fprintd`. The earlier library detected the device but could not finish enrollment on this laptop.

This copy was converted to **GPL-3.0-only** under LGPL-2.1 section 3. Original copyright and source provenance are recorded in [PROVENANCE.md](PROVENANCE.md). No Huawei Windows driver, fingerprint image, template, key, or calibration state is tracked in Git.
