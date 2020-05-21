#include "jlcolordef.h"
#include "jlrand.h"
#include "jlutil.h"

namespace jlsvr
{
    namespace jlbase
    {
        void RedPrint(const char *pData)
        {
            fprintf(stdout, "\033[%d;%dm%s\033[%dm", COLOR_FOREGROUND_RED, COLOR_BACKGROUND_BLACK, pData, COLOR_CONTROL_RESET);
        }

        void MagentaPrint(const char *pData)
        {
            fprintf(stdout, "\033[%d;%dm%s\033[%dm", COLOR_FOREGROUND_MAGENTA, COLOR_BACKGROUND_BLACK, pData, COLOR_CONTROL_RESET);
        }

        void YellowPrint(const char *pData)
        {
            fprintf(stdout, "\033[%d;%dm%s\033[%dm", COLOR_FOREGROUND_YELLOW, COLOR_BACKGROUND_BLACK, pData, COLOR_CONTROL_RESET);
        }

        void GreenPrint(const char *pData)
        {
            fprintf(stdout, "\033[%d;%dm%s\033[%dm", COLOR_FOREGROUND_GREEN, COLOR_BACKGROUND_BLACK, pData, COLOR_CONTROL_RESET);
        }

        void BluePrint(const char *pData)
        {
            fprintf(stdout, "\033[%d;%dm%s\033[%dm", COLOR_FOREGROUND_BLUE, COLOR_BACKGROUND_BLACK, pData, COLOR_CONTROL_RESET);
        }

        void CyanPrint(const char *pData)
        {
            fprintf(stdout, "\033[%d;%dm%s\033[%dm", COLOR_FOREGROUND_CYAN, COLOR_BACKGROUND_BLACK, pData, COLOR_CONTROL_RESET);
        }

        void BlackPrint(const char *pData)
        {
            fprintf(stdout, "\033[%d;%dm%s\033[%dm", COLOR_FOREGROUND_BLACK, COLOR_BACKGROUND_WHITE, pData, COLOR_CONTROL_RESET);
        }

        void WhitePrint(const char *pData)
        {
            fprintf(stdout, "\033[%d;%dm%s\033[%dm", COLOR_FOREGROUND_WHITE, COLOR_BACKGROUND_BLACK, pData, COLOR_CONTROL_RESET);
        }

        void RandColorPrint(const char *pData)
        {
            static int colorCtrl[] = {COLOR_CONTROL_HIGHLIGHT, COLOR_CONTROL_DIM, COLOR_CONTROL_UNDERLINE, COLOR_CONTROL_FLICKER, COLOR_CONTROL_REVERSE};
            int iCtrl = colorCtrl[CJlRand::Instance()->RandInt(0, JLARRAYLEN(colorCtrl) - 1)];
            int iFore = CJlRand::Instance()->RandInt(COLOR_FOREGROUND_BLACK, COLOR_FOREGROUND_WHITE);
            int iBlack = CJlRand::Instance()->RandInt(COLOR_BACKGROUND_BLACK, COLOR_BACKGROUND_WHITE);
            while (iBlack == iFore)
            {
                iBlack = CJlRand::Instance()->RandInt(COLOR_BACKGROUND_BLACK, COLOR_BACKGROUND_WHITE);
            }

            fprintf(stdout, "\033[%d;%d;%dm%s\033[%dm", iCtrl, iFore, iBlack, pData, COLOR_CONTROL_RESET);
        }

        void CustomColorPrint(const char *pData, ColorControl iCtrl, ColorForeground iFore, ColorBackground iBak)
        {
               fprintf(stdout, "\033[%d;%d;%dm%s\033[%dm", iCtrl, iFore, iBak, pData, COLOR_CONTROL_RESET);
        }
    }; // namespace jlbase
} // namespace jlsvr