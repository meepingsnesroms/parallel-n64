/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus-rsp-hle - alist.c                                         *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2012 Bobby Smiles                                       *
 *   Copyright (C) 2009 Richard Goedeken                                   *
 *   Copyright (C) 2002 Hacktarux                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "hle.h"
#include "alist_internal.h"

// FIXME: this decomposition into 3 ABI is not accurate,
// there are a least 9 or 10 different ABI, each with one or a few revisions
// for a total of almost 16 differents audio ucode.
//
// ABI2 in fact is a mix of at least 7 differents ABI which are mostly compatible
// but not totally, that's why there is a isZeldaABI/isMKABI workaround.
//
extern const acmd_callback_t ABI1[0x10];
extern const acmd_callback_t ABI2[0x20];
extern const acmd_callback_t ABI3[0x10];

/* local functions */
static void alist_process(const acmd_callback_t abi[], unsigned int abi_size)
{
    uint32_t inst1, inst2;
    unsigned int acmd;

    const uint32_t *alist = dram_u32(*dmem_u32(TASK_DATA_PTR));
    const uint32_t * const alist_end = alist + (*dmem_u32(TASK_DATA_SIZE) >> 2);

    while (alist != alist_end)
    {
        inst1 = *(alist++);
        inst2 = *(alist++);

        acmd = inst1 >> 24;

        if (acmd < abi_size)
        {
            (*abi[acmd])(inst1, inst2);
        }
#ifndef NDEBUG
        else
        {
            RSP_DEBUG_MESSAGE(M64MSG_WARNING, "Invalid ABI command %u", acmd);
        }
#endif
    }
}

/* global functions */
void alist_process_ABI1(void)
{
    alist_process(ABI1, 0x10);
}

void alist_process_ABI2(void)
{
    alist_process(ABI2, 0x20);
}

void alist_process_ABI3(void)
{
    alist_process(ABI3, 0x10);
}


