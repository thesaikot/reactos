/*
 *  FreeLoader
 *  Copyright (C) 1998-2003  Brian Palmer  <brianp@sginet.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <freeldr.h>
#include <debug.h>

VOID NTAPI HalpInitializePciStubs(VOID);
VOID NTAPI HalpInitBusHandler(VOID);

VOID BootMain(LPSTR CmdLine)
{
	CmdLineParse(CmdLine);

	MachInit(CmdLine);

	FsInit();

	DebugInit();

	DPRINTM(DPRINT_WARNING, "BootMain() called.\n");

	if (!UiInitialize(FALSE))
	{
		UiMessageBoxCritical("Unable to initialize UI.\n");
		return;
	}

	if (!MmInitializeMemoryManager())
	{
		UiMessageBoxCritical("Unable to initialize memory manager");
		return;
	}

#ifdef _M_IX86
	HalpInitializePciStubs();
	HalpInitBusHandler();
#endif
	RunLoader();
}

#ifdef _MSC_VER
long _ftol2(double f)
{
    return _ftol(f);
}
long _ftol2_sse(double f)
{
    return _ftol(f);
}
#endif

// We need to emulate these, because the original ones don't work in freeldr
int __cdecl wctomb(char *mbchar, wchar_t wchar)
{
    *mbchar = (char)wchar;
    return 1;
}

int __cdecl mbtowc (wchar_t *wchar, const char *mbchar, size_t count)
{
    *wchar = (wchar_t)*mbchar;
    return 1;
}
