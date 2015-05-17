/*
 * Real mode code
 *
 * Copyright (C) 2015, Julian Stecklina <js@alien8.de>
 *
 * This file is part of Morbo.
 *
 * Morbo is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Morbo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details.
 */

#pragma once

void enable_real_mode(uint16_t selector, uint16_t offset) __attribute__((regparm(3),noreturn));

/* EOF */
