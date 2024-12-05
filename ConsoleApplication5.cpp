#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <regex>
#include <vector>

using namespace std;

class VehiclePass
{
public:

	VehiclePass() {};

	// Конструктор класса, принимающий дату и номер автомобиля
	VehiclePass(string date, string carNumber)
		: date(date), carNumber(carNumber) {}

	// Метод для отображения информации об объекте
	void display() const
	{
		cout << date << " " << carNumber << endl;
	}

	void displaynumless() const
	{
		cout << date << endl;
	}

	// Метод для получения строки с информацией об объекте
	string toString() const
	{
		return date + " " + carNumber;
	}

	int totaltime() {
		int a;
		a = stoi(date.substr(0, 4)) * 365 + stoi(date.substr(5, 2)) * 30 + stoi(date.substr(8, 2));
		return a;
	};

	string gnum() {
		return carNumber;
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

void data_sort(vector <VehiclePass> &data) {
	for (int i = 1; i < data.size(); i++) {
		if (data[i].totaltime() > data[i - 1].totaltime()) {
			swap(data[i], data[i - 1]);
			i = 0;
		}
	}
}

void find_car(vector <VehiclePass> &data, string num) {
	int f = 0;
	for (int i = 0; i < data.size(); i++) {
		if (num == data[i].gnum()) {
			f++;
		}
	}
	if (f > 1)
		cout << "Даты проезда: " << endl;
	else if (f == 1)
		cout << "Дата проезда: " << endl;
	if (f > 0) {
		for (int i = 0, j = 1; i < data.size(); i++) {
			if (num == data[i].gnum()) {
				if (f == 1)
					data[i].displaynumless();
				else if (f > 1) {
					cout << j << ". ";
					data[i].displaynumless();
				}
				j++;
			}
		}
	}
	if (f == 0)
		cout << "Такая машина не проезжала";
}

int main()
{
	setlocale(0, "");

	string line;
	ifstream inputFile("Pass.txt");
	vector <VehiclePass> data;
	bool a = 0;

	if (!inputFile.is_open())
	{
		cerr << "Ошибка: Не удалось открыть файл" << endl;
		return 1;
	}

	cout << "Провести сортировку по датам?[0/1]" << endl;
	cin >> a;


	while (getline(inputFile, line))
	{
		try
		{
			// Парсим входную строку и создаем объект VehiclePass
			VehiclePass vehiclePass = parseInput(line);
			data.push_back(vehiclePass);
			if (a == 1)
				data_sort(data);
		}
		catch (const exception& e)
		{
			// Обрабатываем возможные исключения и выводим сообщение об ошибке
			cerr << "Ошибка: " << e.what() << endl;
		}
	}

	for (int i = 0; i < data.size(); i++) {
		data[i].display();
		saveToFile(data[i], "OutputPass.txt");
	}

	string num;
	a = 0;
	cout << "Провести поиск по номеру?[0/1]" << endl;
	cin >> a;
	if (a == 1) {
		cout << "Введите номер" << endl;
		cin >> num;
		find_car(data, num);
	}

	inputFile.close();

	return 0;
}

