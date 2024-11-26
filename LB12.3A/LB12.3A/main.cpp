#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstring>
#include <windows.h>

using namespace std;

enum Specialty
{
    KN,
    IK,
    PZ,
    VP,
    SA
};

string strSpecialty[] = { "КН", "ІК", "ПЗ", "ВП", "СА" };

struct Student
{
    char surname[64];
    Specialty specialty;
    short unsigned course;
    short unsigned grade_physics;
    short unsigned grade_math;
    short unsigned grade_info;
};

void Create(char* file_name);
void Print(char* file_name);
void SortBin(char* file_name);
bool BinarySearch(char* file_name, const char* surname, short unsigned course, short unsigned grade_physics);
double AvgGrade(fstream& f, const int i);
void SwapRecords(fstream& f, const int i, const int j);
char* GetSurname(fstream& f, const int i);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char file_name[81];
    int menu;

    cout << "Введіть назву файла: ";
    cin.getline(file_name, sizeof(file_name));

    do
    {
        cout << "\n[1] - Створити список студентів" << endl;
        cout << "[2] - Вивести інформацію про студентів" << endl;
        cout << "[3] - Фізично впорядкувати файл" << endl;
        cout << "[4] - Знайти студента (за прізвищем, курсом, оцінкою з фізики)" << endl;
        cout << "[5] - Вийти з програми" << endl;

        cout << "Меню: ";
        cin >> menu;
        switch (menu)
        {
        case 1:
            Create(file_name);
            break;
        case 2:
            Print(file_name);
            break;
        case 3:
            SortBin(file_name);
            break;
        case 4: {
            char surname[64];
            short unsigned course, grade_physics;

            cout << "Введіть прізвище: ";
            cin >> surname;
            cout << "Введіть курс: ";
            cin >> course;
            cout << "Введіть оцінку з фізики: ";
            cin >> grade_physics;

            if (BinarySearch(file_name, surname, course, grade_physics))
                cout << "Студент знайдений." << endl;
            else
                cout << "Студент не знайдений." << endl;
            break;
        }
        case 5:
            cout << "Вихід з програми." << endl;
            break;
        default:
            cout << "Неправильний вибір. Спробуйте ще раз." << endl;
        }
    } while (menu != 5);
}

void Create(char* file_name)
{
    ofstream f(file_name, ios::binary);
    if (!f)
    {
        cerr << "Помилка відкриття файлу!" << endl;
        exit(1);
    }

    Student s;
    int specialty;
    char ch;
    int i = 0;

    do
    {
        cout << "Студент №" << i + 1 << ":" << endl;
        ++i;
        cin.sync();
        cout << "Прізвище: ";
        cin >> s.surname;
        cout << "Курс: ";
        cin >> s.course;
        cout << "Спеціальність (0-КН, 1-ІК, 2-ПЗ, 3-ВП, 4-СА): ";
        cin >> specialty;
        s.specialty = (Specialty)specialty;
        cout << "Оцінка з фізики: ";
        cin >> s.grade_physics;
        cout << "Оцінка з математики: ";
        cin >> s.grade_math;
        cout << "Оцінка з інформатики: ";
        cin >> s.grade_info;

        f.write((char*)&s, sizeof(Student));
        cout << "Продовжити введення? (Y/N): ";
        cin >> ch;
    } while (ch == 'Y' || ch == 'y');

    f.close();
}

void Print(char* file_name)
{
    ifstream f(file_name, ios::binary);
    if (!f)
    {
        cerr << "Помилка відкриття файлу!" << endl;
        exit(1);
    }

    Student s;
    int i = 0;

    cout << "===========================================================================" << endl;
    cout << "| # |  Прізвище  | Курс |  Спеціальність  | Фізика | Математика | Інформатика |" << endl;
    cout << "---------------------------------------------------------------------------" << endl;

    while (f.read((char*)&s, sizeof(Student)))
    {
        cout << "|" << setw(2) << right << i + 1
            << " |" << setw(12) << left << s.surname
            << "|" << setw(4) << right << s.course
            << " |" << setw(13) << right << strSpecialty[s.specialty]
            << " |" << setw(7) << right << s.grade_physics
            << " |" << setw(10) << right << s.grade_math
            << " |" << setw(11) << right << s.grade_info << " |" << endl;
        ++i;
    }

    cout << "===========================================================================" << endl;
    f.close();
}

void SortBin(char* file_name)
{
    fstream f(file_name, ios::binary | ios::in | ios::out);
    if (!f)
    {
        cerr << "Помилка відкриття файлу!" << endl;
        exit(1);
    }

    f.seekg(0, ios::end);
    int size = f.tellg() / sizeof(Student);
    f.seekg(0, ios::beg);

    for (int i0 = 1; i0 < size; i0++)
        for (int i1 = 0; i1 < size - i0; i1++)
        {
            double avg1 = AvgGrade(f, i1);
            double avg2 = AvgGrade(f, i1 + 1);
            char* surname1 = GetSurname(f, i1);
            char* surname2 = GetSurname(f, i1 + 1);

            if (avg1 < avg2 || (avg1 == avg2 && strcmp(surname1, surname2) > 0))
                SwapRecords(f, i1, i1 + 1);
        }

    f.close();
}

bool BinarySearch(char* file_name, const char* surname, short unsigned course, short unsigned grade_physics)
{
    fstream f(file_name, ios::binary | ios::in);
    if (!f)
    {
        cerr << "Помилка відкриття файлу!" << endl;
        return false;
    }

    f.seekg(0, ios::end);
    int size = f.tellg() / sizeof(Student);
    f.seekg(0, ios::beg);

    Student s;
    int left = 0, right = size - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        f.seekg(mid * sizeof(Student));
        f.read((char*)&s, sizeof(Student));

        if (strcmp(s.surname, surname) == 0 && s.course == course && s.grade_physics == grade_physics)
            return true;

        if (s.grade_physics < grade_physics || (s.grade_physics == grade_physics && strcmp(s.surname, surname) < 0))
            left = mid + 1;
        else
            right = mid - 1;
    }

    return false;
}

double AvgGrade(fstream& f, const int i)
{
    Student s;
    f.seekg(i * sizeof(Student));
    f.read((char*)&s, sizeof(Student));
    return (s.grade_physics + s.grade_math + s.grade_info) / 3.0;
}

char* GetSurname(fstream& f, const int i)
{
    static Student s;
    f.seekg(i * sizeof(Student));
    f.read((char*)&s, sizeof(Student));
    return s.surname;
}

void SwapRecords(fstream& f, const int i, const int j)
{
    Student s1, s2;
    f.seekg(i * sizeof(Student));
    f.read((char*)&s1, sizeof(Student));
    f.seekg(j * sizeof(Student));
    f.read((char*)&s2, sizeof(Student));

    f.seekp(i * sizeof(Student));
    f.write((char*)&s2, sizeof(Student));
    f.seekp(j * sizeof(Student));
    f.write((char*)&s1, sizeof(Student));
}
