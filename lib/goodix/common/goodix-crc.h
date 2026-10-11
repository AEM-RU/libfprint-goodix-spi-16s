// SPDX-License-Identifier: GPL-3.0-only
/*
 * Goodix checksum primitives shared by independently verified device stacks.
 *
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

/* CRC-32/MPEG-2: poly 0x04c11db7, init 0xffffffff, non-reflected, xorout 0. */
guint32 goodix_crc32_mpeg2 (const guint8 *data,
                            gsize         length);

G_END_DECLS
