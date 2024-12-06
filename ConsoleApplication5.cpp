#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <regex>
#include <vector>
#include <algorithm>

using namespace std;

class VehiclePass
{
public:
    VehiclePass(const string& date, const string& carNumber)
        : date(date), carNumber(carNumber) {}

    void display() const
    {
        cout << date << " " << carNumber << endl;
    }

    string toString() const
    {
        return date + " " + carNumber;
    }

    string getDate() const { return date; }
    string getCarNumber() const { return carNumber; }

private:
    string date;      // Дата в формате гггг.мм.дд
    string carNumber; // Номер автомобиля формата в формате [1 буква][001-999][2 Буква][00-999]
};

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

VehiclePass parseInput(const string& input)
{
    istringstream stream(trim(input));
    string part1, part2;

    stream >> part1 >> part2;

    string date = isValidDate(part1) ? part1 : part2;
    string carNumber = isValidCarNumber(part1) ? part1 : part2;

    if (!isValidDate(date) || !isValidCarNumber(carNumber))
    {
        throw invalid_argument("Неверный формат входных данных");
    }

    return VehiclePass(date, carNumber);
}

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

bool compareByDate(const VehiclePass& a, const VehiclePass& b)
{
    return a.getDate() > b.getDate();
}

void sortedDate(vector<VehiclePass> passes)
{
    sort(passes.begin(), passes.end(), compareByDate);

    cout << endl << "Отсортированные данные по датам:" << endl;
    for (const auto& pass : passes)
    {
        pass.display();
    }
}

void findCar(vector<VehiclePass> passes, string carNumber)
{
    cout << "Результаты поиска для номера машины " << carNumber << ":" << endl;
    bool found = false;
    for (const auto& pass : passes)
    {
        if (pass.getCarNumber() == carNumber)
        {
            pass.display();
            found = true;
        }
    }

    if (!found)
    {
        cout << "Номер машины не найден." << endl;
    }
}

int main()
{
    setlocale(0, "");

    string line;
    ifstream inputFile("Pass.txt");
    vector<VehiclePass> vehiclePasses;

    if (!inputFile.is_open())
    {
        cerr << "Ошибка: Не удалось открыть файл" << endl;
        return 1;
    }

    while (getline(inputFile, line))
    {
        try
        {
            VehiclePass vehiclePass = parseInput(line);
            vehiclePass.display();
            vehiclePasses.push_back(vehiclePass);
        }
        catch (const exception& e)
        {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    inputFile.close();

    cout << endl << "Отсортировать данные? [0/1]" << endl;

    int choice1, choice2;
    cin >> choice1;

    switch (choice1)
    {
    case 1:

        sortedDate(vehiclePasses);

    default:
        break;
    }


    cout << endl << "Найти автомобиль по номеру? [0/1]" << endl;

    cin >> choice2;

    switch (choice2)
    {
    case 1: // Поиск по номеру машины
    {
        string searchCarNumber;
        cout << "Введите номер машины для поиска: ";
        cin >> searchCarNumber;

        findCar(vehiclePasses, searchCarNumber);
    }

    default:
        break;
    }

    return 0;
}
