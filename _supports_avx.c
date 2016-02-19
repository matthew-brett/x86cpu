#include <stdint.h>
#if !defined(_MSC_VER)
/*
 * We're askig whether the OS supports AVX.
 *
 * We get the answer by seeing if the OS keeps the 64 bit registers clean.
 * If we're on gcc / clang, we can depend on the xgetbv x86 instruction to
 * tell us whether this is so - see:
 * https://software.intel.com/en-us/blogs/2011/04/14/is-avx-enabled/
 *
 * On MSVC we have a problem, because the MSVC 2010 SP1 is the first version
 * of MSVC to have a compiler intrinsic for xgetbv, and MSVC does not support
 * inline ``__asm`` in 64-bit:
 * http://forums.codeguru.com/showthread.php?551499-xgetbv
 * https://msdn.microsoft.com/en-us/library/hb5z4sxd.aspx
 *
 * When we can drop support for MSVC < 2010 SP1, then we can ifdef to use the
 * ``_xgetbv`` intrinsic:
 * https://msdn.microsoft.com/en-us/library/hh977023.aspx
 */

void _xgetbv(uint32_t op, uint32_t* eax_var, uint32_t* edx_var)
{
    /* Use binary code for xgetbv */
    __asm__ __volatile__
        (".byte 0x0f, 0x01, 0xd0": "=a" (*eax_var), "=d" (*edx_var) : "c" (op) : "cc");
}

int os_supports_avx(void)
{
    uint32_t eax_val, edx_val;
    // XFEATURE_ENABLED_MASK/XCR0 register number = 0
    _xgetbv(0, &eax_val, &edx_val);
    // XFEATURE_ENABLED_MASK register is in edx:eax
    return (eax_val & 6) == 6;
}

#else
/*
 * Much of this code fragment copied from:
 * https://msdn.microsoft.com/en-us/library/windows/desktop/hh134240(v=vs.85).aspx
 * I assume that this code is public domain given its apparent intended
 * purpose as an exmaple of code usage.
 *
 * The test for AVX here is also described in:
 * https://msdn.microsoft.com/en-us/library/windows/desktop/ff919571(v=vs.85).aspx
*/
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <winerror.h>

// Windows 7 SP1 is the first version of Windows to support the AVX API.

// Since the AVX API is not declared in the Windows 7 SDK headers and 
// since we don't have the proper libs to work with, we will declare 
// the API as function pointers and get them with GetProcAddress calls 
// from kernel32.dll.  We also need to set some #defines.

#define XSTATE_AVX                          (XSTATE_GSSE)
#define XSTATE_MASK_AVX                     (XSTATE_MASK_GSSE)

typedef DWORD64 (WINAPI *PGETENABLEDXSTATEFEATURES)();
PGETENABLEDXSTATEFEATURES pfnGetEnabledXStateFeatures = NULL;

typedef BOOL (WINAPI *PINITIALIZECONTEXT)(PVOID Buffer, DWORD ContextFlags, PCONTEXT* Context, PDWORD ContextLength);
PINITIALIZECONTEXT pfnInitializeContext = NULL;

typedef BOOL (WINAPI *PGETXSTATEFEATURESMASK)(PCONTEXT Context, PDWORD64 FeatureMask);
PGETXSTATEFEATURESMASK pfnGetXStateFeaturesMask = NULL;

typedef PVOID (WINAPI *LOCATEXSTATEFEATURE)(PCONTEXT Context, DWORD FeatureId, PDWORD Length);
LOCATEXSTATEFEATURE pfnLocateXStateFeature = NULL;

typedef BOOL (WINAPI *SETXSTATEFEATURESMASK)(PCONTEXT Context, DWORD64 FeatureMask);
SETXSTATEFEATURESMASK pfnSetXStateFeaturesMask = NULL;

int _os_supports_avx(void):
{
    PVOID Buffer;
    PCONTEXT Context;
    DWORD ContextSize;
    DWORD64 FeatureMask;
    DWORD FeatureLength;
    ULONG Index;
    BOOL Success;
    PM128A Xmm;
    PM128A Ymm;

    // If this function was called before and we were not running on
    // at least Windws 7 SP1, then bail.
    if (pfnGetEnabledXStateFeatures == (PGETENABLEDXSTATEFEATURES)-1)
    {
        return -1;
    }

    // Get the addresses of the AVX XState functions.
    if (pfnGetEnabledXStateFeatures == NULL)
    {
        HMODULE hm = GetModuleHandle(_T("kernel32.dll"));
        if (hm == NULL)
        {
            pfnGetEnabledXStateFeatures = (PGETENABLEDXSTATEFEATURES)-1;
            return -2
        }

        pfnGetEnabledXStateFeatures = (PGETENABLEDXSTATEFEATURES)GetProcAddress(hm, "GetEnabledXStateFeatures");
        pfnInitializeContext = (PINITIALIZECONTEXT)GetProcAddress(hm, "InitializeContext");
        pfnGetXStateFeaturesMask = (PGETXSTATEFEATURESMASK)GetProcAddress(hm, "GetXStateFeaturesMask");
        pfnLocateXStateFeature = (LOCATEXSTATEFEATURE)GetProcAddress(hm, "LocateXStateFeature");
        pfnSetXStateFeaturesMask = (SETXSTATEFEATURESMASK)GetProcAddress(hm, "SetXStateFeaturesMask");

        if (pfnGetEnabledXStateFeatures == NULL
            || pfnInitializeContext == NULL
            || pfnGetXStateFeaturesMask == NULL
            || pfnLocateXStateFeature == NULL
            || pfnSetXStateFeaturesMask == NULL)
        {
            pfnGetEnabledXStateFeatures = (PGETENABLEDXSTATEFEATURES)-1;
            return -3
        }
    }

    FeatureMask = pfnGetEnabledXStateFeatures();
    return ((FeatureMask & XSTATE_MASK_AVX) != 0)
}


int os_supports_avx(void)
{
    return _os_supports_avx() > 0
}
#endif
