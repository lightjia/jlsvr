#include "jlcolordef.h"
#include "jlrand.h"

namespace jlsvr
{
    namespace jlbase
    {
        void RedPrint(const char *pData)
        {
            fprintf(stdout, "\033[%dm%s\033[%dm", COLOR_FOREGROUND_RED, pData, COLOR_CONTROL_RESET);
        }

        void MagentaPrint(const char *pData)
        {
            fprintf(stdout, "\033[%dm%s\033[%dm", COLOR_FOREGROUND_MAGENTA, pData, COLOR_CONTROL_RESET);
        }

        void YellowPrint(const char *pData)
        {
            fprintf(stdout, "\033[%dm%s\033[%dm", COLOR_FOREGROUND_YELLOW, pData, COLOR_CONTROL_RESET);
        }

        void GreenPrint(const char *pData)
        {
            fprintf(stdout, "\033[%dm%s\033[%dm", COLOR_FOREGROUND_GREEN, pData, COLOR_CONTROL_RESET);
        }

        void BluePrint(const char *pData)
        {
            fprintf(stdout, "\033[%dm%s\033[%dm", COLOR_FOREGROUND_BLUE, pData, COLOR_CONTROL_RESET);
        }

        void CyanPrint(const char *pData)
        {
            fprintf(stdout, "\033[%dm%s\033[%dm", COLOR_FOREGROUND_CYAN, pData, COLOR_CONTROL_RESET);
        }

        void RandColorPrint(const char *pData)
        {
            static int colorCtrl[] = {COLOR_CONTROL_HIGHLIGHT, COLOR_CONTROL_DIM, COLOR_CONTROL_UNDERLINE, COLOR_CONTROL_FLICKER, COLOR_CONTROL_REVERSE};
            int iCtrl = colorCtrl[CJlRand::Instance()->RandInt(0, 4)];
            int iFore = CJlRand::Instance()->RandInt(COLOR_FOREGROUND_BLACK, COLOR_FOREGROUND_WHITE);
            int iBlack = CJlRand::Instance()->RandInt(COLOR_BACKGROUND_BLACK, COLOR_BACKGROUND_WHITE);
            fprintf(stdout, "\033[%d;%d;%dm%s\033[%dm", iCtrl, iFore, iBlack, pData, COLOR_CONTROL_RESET);
        }
    }; // namespace jlbase
} // namespace jlsvr