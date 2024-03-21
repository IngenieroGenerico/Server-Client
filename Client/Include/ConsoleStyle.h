#pragma once
///******************************************************/
///*				Windows Library.					*/
///******************************************************/
#include <windows.h>

/**
 * @brief Used for defined a colors.
 */
enum ConsoleColor : WORD{
    RED = 4u,
    YELLOW = 14u,
    WHITE = 15u
};

/**
 * @brief Functions used for set an specific word color into the console.
 * @param ConsleColor: textColor to print into the console.
 */
static void SetConsoleColor(ConsoleColor textColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, textColor);
}
