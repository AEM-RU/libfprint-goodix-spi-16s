# Huawei MateBook D15 2022 / BoF-XX GDIX51C0 patch

This checkout is based on `berkekbgz/libfprint-goodix-spi` commit
`010a665f54089a1632b1ab7be588b316ace934e2`. The local changes are in
the three modified driver source files. Use `git show` to inspect the patch.

The modified driver source remains under `LGPL-2.1-or-later`, matching the
upstream `LICENSE` and SPDX headers. `PROVENANCE.md` records adapted source and
the separate GPL-2.0-or-later terms of the fprintd integration patch. The
Huawei Windows driver package is outside this Git checkout and is not
redistributed here.

On this BoF-XX laptop, the image-mode ACK arrives, then IRQ rises while the
listener's SPI reads are suppressed for the 80 ms sensor readout. The listener
consumes the IRQ edge during suppression and waits for another edge even though
IRQ remains high. The image read therefore times out waiting for TLS app-data
`0x17`. The patch wakes the listener when suppression ends and checks the IRQ
level. It also enters the quiet period in the ACK reader thread, closing the
gap between ACK dispatch and suppression.

Installed locally: the patched libfprint shared library is at
`/opt/goodix-spi-driver/libfprint/libfprint-2.so.2.0.0`. The previous library
is saved alongside it with suffix `.pre-ack-quiet`. Systemd loads the patched
library through `/etc/systemd/system/fprintd.service.d/20-goodix-spi-libfprint.conf`.
The SPI binding udev rule is `/etc/udev/rules.d/70-libfprint-2.rules`.

Observed on 2026-10-11: enrollment of `right-index-finger` completed; a
subsequent `fprintd-verify` returned `verify-match`. A later verification after
removing temporary diagnostic logs timed out while waiting for a finger; it did
not return a mismatch or driver error. No reboot or GDM login test was done.

To inspect the committed local patch: `git show HEAD` in this checkout. To roll back only
this capture change, restore the `.pre-ack-quiet` shared library with root
permissions and restart `fprintd`. That older build detected the device but
did not complete enrollment on this laptop.
