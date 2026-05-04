#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int SIZE = 20;

enum FlightType {
    CHARTER,
    TRANSIT,
    STIKOVKA
};

struct Time {
    int hours;
    int minutes;
};

struct Flight {
    char punct[30];
    Time TimeOtp;
    Time TimePrib;
    Time TimeFlight;
    double SumTicket;
    FlightType FType;
};

// Функция для преобразования типа рейса в строку
string flightTypeString(FlightType type) {
    switch (type) {
    case CHARTER: return "Чартер";
    case TRANSIT: return "Транзит";
    case STIKOVKA: return "Стыковочный";
    default: return "Неизвестно";
    }
}

// Функция для преобразования строки в тип рейса
FlightType stringToFlightType(const string& str) {
    if (str == "Чартер") return CHARTER;
    if (str == "Транзит") return TRANSIT;
    if (str == "Стыковочный") return STIKOVKA;
    return CHARTER;
}

// Функция для вывода времени
void printTime(Time t) {
    cout << (t.hours < 10 ? "0" : "") << t.hours << ":"
        << (t.minutes < 10 ? "0" : "") << t.minutes;
}

// Функция вывода информации об одном рейсе
void showInfo(const Flight& flight) {
    cout << "Пункт назначения: " << flight.punct << endl;
    cout << "Время отправления: ";
    printTime(flight.TimeOtp);
    cout << endl;
    cout << "Время прибытия: ";
    printTime(flight.TimePrib);
    cout << endl;
    cout << "Время полета: ";
    printTime(flight.TimeFlight);
    cout << " (" << flight.TimeFlight.hours << " ч "
        << flight.TimeFlight.minutes << " мин)" << endl;
    cout << "Стоимость билета: " << flight.SumTicket << " руб." << endl;
    cout << "Тип рейса: " << flightTypeString(flight.FType) << endl;
}

// Функция для проверки, оканчивается ли пункт назначения на "инск"
bool endsWithInsk(const char* str) {
    int len = strlen(str);
    if (len < 4) return false;
    return (strcmp(str + len - 4, "инск") == 0);
}

// Функция для копирования подходящих рейсов (пункт на "инск" и стыковочный)
int copyFilteredFlights(const Flight source[], Flight destination[], int size) {
    int j = 0;
    for (int i = 0; i < size; i++) {
        if (endsWithInsk(source[i].punct) && source[i].FType == STIKOVKA) {
            destination[j] = source[i];
            j++;
        }
    }
    return j;
}

// Сортировка по времени полета
int timeToMinutes(Time t) {
    return t.hours * 60 + t.minutes;
}

void bubbleSortByDuration(Flight flights[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (timeToMinutes(flights[j].TimeFlight) >
                timeToMinutes(flights[j + 1].TimeFlight)) {
                Flight temp = flights[j];
                flights[j] = flights[j + 1];
                flights[j + 1] = temp;
            }
        }
    }
}

// Функция вывода данных по конкретному рейсу (по индексу)
void displaySpecificFlight(const Flight flights[], int count, int index) {
    if (index >= 0 && index < count) {
        cout << "\n=== Данные по конкретному рейсу (индекс " << index << ") ===" << endl;
        showInfo(flights[index]);
    }
    else {
        cout << "Неверный индекс рейса!" << endl;
    }
}

// Функция вывода 2 чартерных рейсов, вылетающих утром (до 12:00)
void displayTwoMorningCharters(const Flight flights[], int count) {
    cout << "\n=== 2 чартерных рейса, вылетающих утром ===" << endl;
    int found = 0;
    for (int i = 0; i < count && found < 2; i++) {
        if (flights[i].FType == CHARTER && flights[i].TimeOtp.hours < 12) {
            cout << "\nЧартерный рейс #" << found + 1 << ":" << endl;
            showInfo(flights[i]);
            found++;
        }
    }
    if (found == 0) {
        cout << "Утренние чартерные рейсы не найдены." << endl;
    }
}

// Функция изменения рейса (изменяем стоимость и время полета)
void modifyFlight(Flight& flight, double newPrice, Time newTimeFlight) {
    cout << "\n=== Изменение рейса ===" << endl;
    cout << "Пункт назначения: " << flight.punct << endl;
    cout << "Старая стоимость: " << flight.SumTicket << " руб." << endl;
    cout << "Новая стоимость: " << newPrice << " руб." << endl;
    cout << "Старое время полета: ";
    printTime(flight.TimeFlight);
    cout << endl;
    cout << "Новое время полета: ";
    printTime(newTimeFlight);
    cout << endl;

    flight.SumTicket = newPrice;
    flight.TimeFlight = newTimeFlight;

    cout << "Данные рейса успешно обновлены!" << endl;
}

// Функция для фильтрации чартерных рейсов дороже N рублей
int filterCharterByPrice(const Flight source[], Flight destination[], int count, double N) {
    int j = 0;
    for (int i = 0; i < count; i++) {
        if (source[i].FType == CHARTER && source[i].SumTicket > N) {
            destination[j] = source[i];
            j++;
        }
    }
    return j;
}

// Оберточная функция для вывода отфильтрованных данных
void displayFilteredFlights(const Flight flights[], int count, const char* title) {
    cout << "\n=== " << title << " ===" << endl;
    if (count == 0) {
        cout << "Нет рейсов, соответствующих критериям." << endl;
        return;
    }
    for (int i = 0; i < count; i++) {
        cout << "\n--- Рейс #" << i + 1 << " ---" << endl;
        showInfo(flights[i]);
    }
}

// Функция для вывода всех рейсов
void printAllFlights(const Flight flights[], int size) {
    cout << "=== Исходный список всех рейсов ===" << endl;
    for (int i = 0; i < size; i++) {
        cout << "\n------ Рейс #" << i + 1 << " ------" << endl;
        showInfo(flights[i]);
    }
}

// Функция для считывания данных о стоимости билетов из текстового файла
void readTicketsFromFile(Flight flights[], int size, const char* filename) {
    ifstream fin;
    fin.open(filename);

    if (!fin.is_open()) {
        cout << "Ошибка: не удалось открыть файл " << filename << " для чтения!" << endl;
        return;
    }

    char punct[30];
    double ticketPrice;
    int count = 0;

    cout << "\n=== Чтение данных из текстового файла ===" << endl;

    while (fin >> punct >> ticketPrice && count < size) {
        for (int i = 0; i < size; i++) {
            if (strcmp(flights[i].punct, punct) == 0) {
                cout << "Найден рейс: " << punct << " - старая стоимость: "
                    << flights[i].SumTicket << " руб., новая стоимость: "
                    << ticketPrice << " руб." << endl;
                flights[i].SumTicket = ticketPrice;
                break;
            }
        }
        count++;
    }

    fin.close();
    cout << "Обработано " << count << " записей из файла." << endl;
}

// Функция для записи структуры в бинарный файл
void writeBinaryFile(const Flight flights[], int count, const char* filename) {
    ofstream fout(filename, ios::binary | ios::out);

    if (!fout.is_open()) {
        cout << "Ошибка: не удалось открыть файл " << filename << " для записи!" << endl;
        return;
    }

    fout.write((char*)flights, sizeof(Flight) * count);
    fout.close();

    cout << "Записано " << count << " рейсов в бинарный файл " << filename << endl;
}

// Функция для чтения структуры из бинарного файла
int readBinaryFile(Flight flights[], int maxSize, const char* filename) {
    ifstream fin(filename, ios::binary | ios::in);

    if (!fin.is_open()) {
        cout << "Ошибка: не удалось открыть файл " << filename << " для чтения!" << endl;
        return 0;
    }

    // Определяем размер файла
    fin.seekg(0, ios::end);
    int fileSize = fin.tellg();
    fin.seekg(0, ios::beg);

    int count = fileSize / sizeof(Flight);
    if (count > maxSize) {
        count = maxSize;
    }

    fin.read((char*)flights, sizeof(Flight) * count);
    fin.close();

    cout << "Прочитано " << count << " рейсов из бинарного файла " << filename << endl;
    return count;
}

// Функция для вывода данных из бинарного файла
void displayBinaryFileContent(const char* filename) {
    Flight flights[SIZE];
    int count = readBinaryFile(flights, SIZE, filename);

    if (count > 0) {
        cout << "\n=== Содержимое бинарного файла ===" << endl;
        for (int i = 0; i < count; i++) {
            cout << "\n--- Запись #" << i + 1 << " ---" << endl;
            showInfo(flights[i]);
        }
    }
}

int main() {
    setlocale(LC_ALL, "ru");

    Flight flights[SIZE] = {
        {"Минск", {8, 30}, {10, 45}, {2, 15}, 5000, STIKOVKA},
        {"Екатеринбург", {14, 0}, {16, 30}, {2, 30}, 7500, TRANSIT},
        {"Курганинск", {9, 15}, {11, 0}, {1, 45}, 4200, STIKOVKA},
        {"Санкт-Петербург", {6, 0}, {8, 15}, {2, 15}, 6800, CHARTER},
        {"Новинск", {12, 0}, {14, 20}, {2, 20}, 5500, STIKOVKA},
        {"Сочи", {15, 30}, {18, 0}, {2, 30}, 8200, TRANSIT},
        {"Бобруйнск", {10, 0}, {11, 45}, {1, 45}, 3800, STIKOVKA},
        {"Москва", {7, 0}, {9, 30}, {2, 30}, 6500, CHARTER},
        {"Слуцк", {13, 0}, {14, 30}, {1, 30}, 3000, TRANSIT},
        {"Луцк", {16, 0}, {17, 45}, {1, 45}, 3500, CHARTER},
        {"Красноярск", {11, 0}, {14, 0}, {2, 0}, 9000, TRANSIT},
        {"Оршанск", {8, 45}, {10, 15}, {1, 30}, 3200, STIKOVKA},
        {"Киев", {17, 0}, {19, 0}, {2, 0}, 4800, CHARTER},
        {"Витебск", {9, 30}, {11, 0}, {1, 30}, 2900, TRANSIT},
        {"Полоцк", {14, 30}, {16, 15}, {1, 45}, 3600, STIKOVKA},
        {"Томск", {6, 30}, {10, 0}, {3, 30}, 11000, CHARTER},
        {"Иркутск", {12, 30}, {17, 0}, {4, 30}, 12500, TRANSIT},
        {"Гомель", {15, 0}, {16, 30}, {1, 30}, 3100, STIKOVKA},
        {"Могилевск", {10, 30}, {12, 0}, {1, 30}, 3400, STIKOVKA},
        {"Новосибирск", {8, 0}, {11, 30}, {3, 30}, 10500, TRANSIT}
    };

    // Вывод всех рейсов
    printAllFlights(flights, SIZE);

    // ========== ЗАДАНИЕ 1: Чтение из текстового файла ==========
    // Создаем текстовый файл с данными
    ofstream testFile("tickets.txt");
    testFile << "Минск 5200\n";
    testFile << "Москва 6800\n";
    testFile << "Киев 5000\n";
    testFile << "Томск 11500\n";
    testFile << "Сочи 8500\n";
    testFile.close();

    // Читаем данные из файла
    readTicketsFromFile(flights, SIZE, "tickets.txt");

    // Выводим обновленные данные
    cout << "\n=== Данные после обновления стоимости билетов ===" << endl;
    printAllFlights(flights, SIZE);

    // ========== ЗАДАНИЕ 2: Запись и чтение бинарного файла ==========
    // Создаем массив для тестовой записи
    Flight testFlights[SIZE];
    int testCount = 5;

    // Копируем первые 5 рейсов для теста
    for (int i = 0; i < testCount; i++) {
        testFlights[i] = flights[i];
    }

    // Записываем в бинарный файл
    writeBinaryFile(testFlights, testCount, "flights.bin");

    // Читаем из бинарного файла
    Flight readFlights[SIZE];
    int readCount = readBinaryFile(readFlights, SIZE, "flights.bin");

    // Выводим прочитанные данные
    cout << "\n=== Данные, прочитанные из бинарного файла ===" << endl;
    for (int i = 0; i < readCount; i++) {
        cout << "\n------ Рейс #" << i + 1 << " ------" << endl;
        showInfo(readFlights[i]);
    }

    // Демонстрация работы с бинарным файлом через отдельную функцию
    displayBinaryFileContent("flights.bin");

    // ========== Остальные функции из задания 9 ==========
    Flight filteredFlights[SIZE];
    int filteredCount = copyFilteredFlights(flights, filteredFlights, SIZE);

    displayFilteredFlights(filteredFlights, filteredCount,
        "Рейсы с пунктом назначения на 'инск' и стыковочные");

    bubbleSortByDuration(filteredFlights, filteredCount);

    displayFilteredFlights(filteredFlights, filteredCount,
        "Отсортированные рейсы по времени полета (по возрастанию)");

    if (filteredCount > 0) {
        displaySpecificFlight(filteredFlights, filteredCount, 1);
    }

    displayTwoMorningCharters(flights, SIZE);

    cout << "\n=== ДО изменения ===" << endl;
    showInfo(flights[0]);
    Time newTime = { 2, 30 };
    modifyFlight(flights[0], 5500.0, newTime);
    cout << "\n=== ПОСЛЕ изменения ===" << endl;
    showInfo(flights[0]);

    const double N = 6000.0;
    Flight charterExpensive[SIZE];
    int charterCount = filterCharterByPrice(flights, charterExpensive, SIZE, N);

    displayFilteredFlights(charterExpensive, charterCount,
        "Чартерные рейсы стоимостью более 6000 рублей");

    return 0;
}