// SPDX-License-Identifier: GPL-3.0-only
/*
 * Checksum shared by Goodix MCU configuration blobs.
 *
 * Copyright (C) 2024 goodix-fp-linux-dev contributors
 * Copyright (C) 2026 Berke Kabagöz <berkekbgz@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License.
 */

#pragma once

#include <glib.h>

G_BEGIN_DECLS

gboolean goodix_mcu_config_checksum_valid (const guint8 *config,
                                           gsize         length);

gboolean goodix_mcu_config_update_checksum (guint8 *config,
                                            gsize   length);

G_END_DECLS
