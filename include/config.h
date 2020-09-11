/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H

// Define cross-platform macros,taken from uhd(USRP Hardware Driver) library.
#if defined(_MSC_VER)
    #define RD_EXPORT       __declspec(dllexport)
    #define RD_IMPORT       __declspec(dllimport)
    #define RD_INLINE       __forceinline
    #define RD_DEPRECATED   __declspec(deprecated)
    #define RD_C_MSC
#elif defined(__MINGW32__) || defined(__MINGW64__)
    #define RD_EXPORT       __declspec(dllexport)
    #define RD_IMPORT       __declspec(dllimport)
    #define RD_INLINE       inline
    #define RD_DEPRECATED   __declspec(deprecated)
    #define RD_C_GNUC
#elif defined(__GNUC__) && __GNUC__ >= 4
    #define RD_EXPORT       __attribute__((visibility("default")))
    #define RD_IMPORT       __attribute__((visibility("default")))
    #define RD_INLINE       inline __attribute__((always_inline))
    #define RD_DEPRECATED   __attribute__((deprecated))
    #define RD_C_GNUC
#elif defined(__clang__)
    #define RD_EXPORT       __attribute__((visibility("default")))
    #define RD_IMPORT       __attribute__((visibility("default")))
    #define RD_INLINE       inline __attribute__((always_inline))
    #define RD_DEPRECATED   __attribute__((deprecated))
    #define RD_C_CLANG
#else
    #define RD_EXPORT
    #define RD_IMPORT
    #define RD_INLINE       inline
    #define RD_DEPRECATED
    #define RD_C_UNKNOWN
#endif

#ifdef RD_DLL_EXPORTS
    #define RD_API  RD_EXPORT
#else
    #define RD_API  RD_IMPORT
#endif

#ifdef RD_EXCEPTION
    #define RD_THROW throw
#else
    #define RD_THROW //
#endif

// Platform defines
/*
 * Supported platform :
 * 1.Linux
 * 2.Windows
 * 3.MacOS
 * 4.FreeBSD
 */
#if (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GLIBC__)) && !defined(_CRAYC) && !defined(__FREEBSD_kernel__) && !defined(__GNU__)
    #define RD_PLATFORM_LINUX
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #define RD_PLATFORM_WIN_X86
#elif defined(_WIN64) || defined(__WIN64__) || defined(WIN64)
    #define RD_PLATFORM_WIN_X86_64
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
    #define RD_PLATFORM_MACOS
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__FreeBSD_kernel__)
    #define RD_PLATFORM_BSD
#endif

#if defined(RD_PLATFORM_WIN_X86) || defined(RD_PLATFORM_WIN_X86_64)
    #define RD_PLATFORM_WIN32
#endif

#ifdef RD_C_MSC
    #if _MSC_VER >= 1913
    /*
     * Both pthread.h & time.h of Windows define struct timespec.
     * Use Windows time.h here.
     */
    #define _TIMESPEC_DEFINED 1
    #endif
#endif

#endif // INCLUDE_CONFIG_H
