#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <regex>

using namespace std;

class VehiclePass
{
public:
    // Конструктор класса, принимающий дату и номер автомобиля
    VehiclePass(const string& date, const string& carNumber)
        : date(date), carNumber(carNumber) {}

    // Метод для отображения информации об объекте
    void display() const
    {
        cout << date << " " << carNumber << endl;
    }

    // Метод для получения строки с информацией об объекте
    string toString() const
    {
        return date + " " + carNumber;
    }

private:
    string date;      // Дата в формате гггг.мм.дд
    string carNumber; // Номер автомобиля формата в формате [1 буква][001-999][2 Буква][00-999]
};

// Функция для удаления лишних пробелов в начале и конце строки
string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

bool isValidCarNumber(const string& carNumber)
{
    regex pattern("^[A-Z][0-9]{3}[A-Z]{2}[0-9]{1,3}$");
    return regex_match(carNumber, pattern);
}

bool isValidDate(const string& date)
{
    regex pattern(R"(^\d{4}\.(0[1-9]|1[0-2])\.(0[1-9]|[12][0-9]|3[01])$)");
    return regex_match(date, pattern);
}

// Функция для разбора входной строки и создания объекта VehiclePass
VehiclePass parseInput(const string& input)
{
    istringstream stream(trim(input));
    string part1, part2;

    stream >> part1 >> part2;

    // Проверяем, какая часть является датой, а какая номером автомобиля
    string date = isValidDate(part1) ? part1 : part2;
    string carNumber = isValidCarNumber(part1) ? part1 : part2;

    if (!isValidDate(date) || !isValidCarNumber(carNumber))
    {
        throw invalid_argument("Неверный формат входных данных");
    }

    return VehiclePass(date, carNumber);

    //string date = isValidDate(part1) ? part1 : (isValidDate(part2) ? part2 : part3);
    //string carNumber = isValidCarNumber(part1) ? part1 : (isValidCarNumber(part2) ? part2 : part3);
}

// Функция для записи данных в файл
void saveToFile(const VehiclePass& vehiclePass, const string& filename)
{
    ofstream outputFile(filename, ios::app);
    if (!outputFile.is_open())
    {
        cerr << "Ошибка: Не удалось открыть файл для записи" << endl;
        return;
    }
    outputFile << vehiclePass.toString() << endl;
    outputFile.close();
}

int main()
{
    setlocale(0, "");

    string line;
    ifstream inputFile("Pass.txt");

    if (!inputFile.is_open())
    {
        cerr << "Ошибка: Не удалось открыть файл" << endl;
        return 1;
    }

    while (getline(inputFile, line))
    {
        try
        {
            // Парсим входную строку и создаем объект VehiclePass
            VehiclePass vehiclePass = parseInput(line);
            vehiclePass.display();
            saveToFile(vehiclePass, "OutputPass.txt");
        }
        catch (const exception& e)
        {
            // Обрабатываем возможные исключения и выводим сообщение об ошибке
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    inputFile.close();

    return 0;
}

