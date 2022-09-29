/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

#ifndef YFEL_MACROS_H
#define YFEL_MACROS_H

#undef  YFEL_BEGIN_C_DECLS
#undef  YFEL_END_C_DECLS

#ifdef __cplusplus
#  define YFEL_BEGIN_C_DECLS extern "C" {
#  define YFEL_END_C_DECLS }
#else  // !__cplusplus
#  define YFEL_BEGIN_C_DECLS
#  define YFEL_END_C_DECLS
#endif  // __cplusplus

#endif // YFEL_MACROS_H
