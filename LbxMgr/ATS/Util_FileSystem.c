/*
 *
 * https://c-for-dummies.com/blog/?p=3004
 * C For Dummies Blog
 * Reading File Dates
 *
 * https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/stat-functions?view=msvc-160
 *
 */

#include "Util_FileSystem.h"

#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

int file_timestamps()
{
    struct stat filestat;

    stat("gettysburg.txt",&filestat);
    /* newline included in ctime() output */
    printf(" File access time %s",
           ctime(&filestat.st_atime)
    );
    printf(" File modify time %s",
           ctime(&filestat.st_mtime)
    );
    printf("File changed time %s",
           ctime(&filestat.st_ctime)
    );
    // printf("  File birth time %s",
    //        ctime(&filestat.st_birthtime)
    // );

    return(0);
}

// crt_stat.c
// This program uses the _stat function to
// report information about the file named crt_stat.c.

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
/* #include <w32api/corecrt.h> */ /* errno_t */
/* #include "../../Program Files (x86)/mingw-w64/i686-8.1.0-posix-dwarf-rt_v6-rev0/mingw32/i686-w64-mingw32/include/_mingw_stat64.h" */ /* _stat */
/* #include "../../Program Files (x86)/mingw-w64/i686-8.1.0-posix-dwarf-rt_v6-rev0/mingw32/i686-w64-mingw32/include/limits.h" */ /* errno_t */

int crt_stat( void )
{
    /* struct _stat buf; */
    struct stat buf;
    int result;
    char timebuf[26];
    char* filename = "crt_stat.c";
    errno_t err;

    // Get data associated with "crt_stat.c":
    /* result = _stat( filename, &buf ); */
    result = stat( filename, &buf );

    // Check if statistics are valid:
    if( result != 0 )
    {
        perror( "Problem getting information" );
        switch (errno)
        {
            case ENOENT:
                printf("File %s not found.\n", filename);
                break;
            case EINVAL:
                printf("Invalid parameter to _stat.\n");
                break;
            default:
                /* Should never be reached. */
                printf("Unexpected error in _stat.\n");
        }
    }
    else
    {
        // Output some of the statistics:
        printf( "File size     : %ld\n", buf.st_size );
        printf( "Drive         : %c:\n", buf.st_dev + 'A' );
        /*
        err = ctime_s(timebuf, 26, &buf.st_mtime);
        if (err)
        {
            printf("Invalid arguments to ctime_s.");
            exit(1);
        }
        */
        printf( "Time modified : %s", timebuf );
    }
}
