# Huawei MateBook D15 2022 / BoF-XX 采图修复

## 中文

本分支基于 [berkekbgz/libfprint-goodix-spi](https://github.com/berkekbgz/libfprint-goodix-spi) 的 `010a665f54089a1632b1ab7be588b316ace934e2`。硬件厂家是 **Goodix（汇顶科技）**，笔记本整机厂家是华为；指纹设备是 SPI 总线上的 `ACPI\GDIX51C0`。三个修改文件位于 `drivers/gdix51c0/`，可用 `git log -p -- drivers/gdix51c0/` 查看补丁。

这台 BoF-XX 在图像模式 ACK 后，传感器于约 80 ms 的 SPI 静默期内拉高中断脚。监听线程消耗该边沿，却不再检查保持高电平的 IRQ，导致等待 TLS 图像数据 `0x17` 超时。修复在静默结束后主动唤醒监听线程，并在接收 ACK 的线程内进入静默期。

**2026-10-11 本机结果：**右手食指录入完成，一次 `fprintd-verify` 返回 `verify-match`。去掉临时诊断日志后，服务能读取已录入的手指；再次验证因未触摸传感器而超时。尚未进行重启或 GDM 登录测试。

本机安装的共享库位于 `/opt/goodix-spi-driver/libfprint/libfprint-2.so.2.0.0`；修改前的库备份为同目录下带 `.pre-ack-quiet` 后缀的文件。systemd 配置位于 `/etc/systemd/system/fprintd.service.d/20-goodix-spi-libfprint.conf`，SPI 绑定规则位于 `/etc/udev/rules.d/70-libfprint-2.rules`。若只回退本次采图修改，使用管理员权限恢复备份库并重启 `fprintd`；旧版能识别设备，但在本机无法完成录入。

本副本按 LGPL-2.1 第 3 条转换为 **GPL-3.0-only**，原作者版权与来源见 [PROVENANCE.md](PROVENANCE.md)。华为 Windows 驱动包、指纹图像、模板、密钥和校准数据均未收入 Git 仓库。

## English

This branch starts from [berkekbgz/libfprint-goodix-spi](https://github.com/berkekbgz/libfprint-goodix-spi) commit `010a665f54089a1632b1ab7be588b316ace934e2`. **Goodix** makes the fingerprint sensor and Huawei makes the laptop. The fingerprint device is `ACPI\GDIX51C0` on SPI. Three files under `drivers/gdix51c0/` were changed; inspect them with `git log -p -- drivers/gdix51c0/`.

On this BoF-XX, IRQ rises during the approximately 80 ms quiet SPI period after the image-mode ACK. The listener consumes that edge but does not recheck the IRQ level, which stays high, so the TLS image-data `0x17` wait times out. The fix wakes the listener when suppression ends and enters the quiet period in the ACK reader thread.

**Observed locally on 2026-10-11:** right index finger enrollment completed and one `fprintd-verify` returned `verify-match`. After temporary diagnostics were removed, the service still listed the enrolled finger; a later verification timed out while waiting for a touch. Reboot and GDM login have not been tested.

The installed library is `/opt/goodix-spi-driver/libfprint/libfprint-2.so.2.0.0`; the previous version is saved beside it with a `.pre-ack-quiet` suffix. The systemd drop-in is `/etc/systemd/system/fprintd.service.d/20-goodix-spi-libfprint.conf`, and the SPI binding rule is `/etc/udev/rules.d/70-libfprint-2.rules`. To roll back only this capture change, restore that backup with root permissions and restart `fprintd`. The earlier library detected the device but could not finish enrollment on this laptop.

This copy was converted to **GPL-3.0-only** under LGPL-2.1 section 3. Original copyright and source provenance are recorded in [PROVENANCE.md](PROVENANCE.md). No Huawei Windows driver, fingerprint image, template, key, or calibration state is tracked in Git.
