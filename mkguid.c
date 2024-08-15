// start: early 2021
// 04.11.21 HS cleanup 2 Linux
// 15.08.24 HS added -l -t shared on github

#include "tools.h"
#include "VERSION.h"
//#include <windows.h>

char szTemp[]  = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
char szHex[]   = "0123456789ABCDEF-";

int Help(void)
{
    printf ("%s\n", m_PRG_INFO);
    printf("Generate UID for Windows or UID for ext4\n");
    printf("-l     Generate Lowercase\n");
    printf("-t     uid as timeinfo ; like 20240815-1300-.... for 08/15/24 1:oo pm\n");
    return 0;
}

signed int main(int argc, char *argv[])
{
	if (InitTools(argc , argv, "%v%t%m", I_MAJOR, I_MINOR, I_BUILD, I_BETA, LOG_STDERR)) return -1;
    //printf("%s\n",m_PRG_INFO);

    char GUID[40];
    int t = 0;
    int  timefirst=0;

    int nLen = strlen (szTemp);
    srand((unsigned)time(NULL));

    if (aChkARG("l")) strlwr(szHex);                                            // simple lowercase the source
    if (aChkARG("t")) timefirst=1;                                              // queried later
    if (aChkARG("?")||aChkARGlong("help")) return Help();                       // return 0 if help displayed

//    lprintf ("%s : %i", szHex, timefirst); return 0;
                                                                                // this generate the number
    for (t=0; t<nLen+1; t++)
    {
        int r = rand () % 16;
        char c = ' ';
        switch (szTemp[t])
        {
            case 'x' : { c = szHex [r]; } break;                                // x-digit
            case 'y' : { c = szHex [r & (0x03 | 0x08)]; } break;                // y-digit little be triked
            case '-' : { c = '-'; } break;                                      // yes
            case '4' : { c = '4'; } break;                                      // 4 = 4
            // default : clean your eyes
        }
        GUID[t] = ( t < nLen ) ? c : 0x00;                                      // the 0x00 terminate the string
    }
    if (timefirst)
    {
        char szDate[] = "YYYYMMDD-hhmm";
        time_t T = unixtime();
        struct tm *ti = localtime(&T);
        strftimeR( szDate, sizeof(szDate), "%Y%m%d-%H%M", ti );                 // catch the result for now...

        for (t=0;;t++)                                                          // copy string to guid
        {
            if (szDate[t]==0) break;
            GUID[t]=szDate[t];
        }
    }
    printf ("%s\n", GUID);                                                      // result !

#ifdef OS_WINDOWS
    LPWSTR cwdBuffer;                                                           // for windows copy the result into buffer
    cwdBuffer = malloc ( (sizeof(GUID)+1) * sizeof(WCHAR));
    mbstowcs(cwdBuffer, GUID, strlen(GUID)+1);//Plus null

    DWORD len = wcslen(cwdBuffer);
    HGLOBAL hdst;
    LPWSTR dst;

    // Allocate string
    hdst = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (len + 1) * sizeof(WCHAR));
    dst = (LPWSTR)GlobalLock(hdst);
    memcpy(dst, cwdBuffer, len * sizeof(WCHAR));
    dst[len] = 0;
    GlobalUnlock(hdst);

    // Set clipboard data
    if (!OpenClipboard(NULL)){free(cwdBuffer); return GetLastError();}
    EmptyClipboard();
    if (!SetClipboardData(CF_UNICODETEXT, hdst)) {free(cwdBuffer); return GetLastError();}
    CloseClipboard();
    free(cwdBuffer);
#endif
    return 0;
}
