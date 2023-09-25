#include <iostream>
#include <windows.h>
#include <string>
#include <cstring>
#define SIZE 100 // Размер хеш-таблицы, константа
using namespace std; // Для удобного использования cout и cin
// Без явного указания пространства имен 

string ReadString()
{
	string input;
	cin.clear();
	cin.ignore(256, '\n');
	getline(cin, input);
	return input;
}

int CheckNums(string& s)
{
	for (char const& ch : s) {
		if (isdigit(ch) == 0)
			return false;
	}
	return true;
}

class customer // Класс покупателя
{
	string id; // Строка с id
	string name; // Строка с именем
	string address; // Строка с адресом
public: // Публичная секция с методами
	customer(string id, string name, string adress);
	string GetID();
	string GetName();
	string GetAddress();
};

class HashTable // Класс хеш-таблиц
{
	customer* customers[SIZE]; // Массив указателей ячеек покупателей
	int size; // Текущий размер
public: // Публичная секция с методами
	HashTable(); // Базовый конструктор таблицы без параметров
	void Push(string id, string name, string adress); // Метод для заполнения таблицы
	void KvaziPush(); // Обработка значений перед заполнением таблицы
	int HashFunc(string id, int shift); // Хеш-функция
	int LineTrial(string id); // Функция линейного опробования
	void PrintAllCustomers(); // Фунция вывода всех покупателей
	int LineFindCustomer(string name); // Функция линейного поиска покупателя
	void KvaziLineFindCustomer(); // Функция обработкии линейного поиска
	int HashFindCustomer(string name); // Функция хешированного поиска покупателя
	void KvaziHashFindCustomer(); // Функция обработкии хешированного поиска
	int ismatch(string first, string second); // Функция для сравнения id и фамилий
	void clear(); // Удаление всей таблицы
	void dell(string id); // Удаление покупателя по id
	void predell();
};

HashTable::HashTable() // Базовый конструктов таблицы без параметров
{
	for (int i = 0; i < SIZE; i++) // Цикл стартовой иницилизации массива 
	{
		customers[i] = nullptr; // Присваивание пустой ячейки
	}
	size = 0; // Стартовый размер равен нулю
}

void HashTable::Push(string id, string name, string adress) // Функция добавления покупателя в таблицу
{
	if (size == SIZE) // Проверка таблицы на переполнение
	{
		cout << "\nТаблица переполнена\n";
		return; // Выход из функции
	}
	int index = HashFunc(id, 0); // Получаем индекс хеш-таблицы
	if (!customers[index]) // Решение проблемы коллизии
	{ // Проверка ячейки на пустоту
		customers[index] = new customer(id, name, adress); // В таблице появляется новая ячейка покупателя
		size += 1;
	}
	else // Если очевидный индекс занят
	{ // Вызов функции линейного опробования 
		index = LineTrial(id);
		customers[index] = new customer(id, name, adress); // В таблице появляется новая ячейка покупателя
		size += 1;
	}
}

void HashTable::KvaziPush() // Квази-функция для обработки информации перед добавленем
{
	string id, name, adress;
	string second_name, first_name, father_name;
	cout << "Введите id покупателя:\n>>";
	cin >> id;
	if (!CheckNums(id))
	{
		cout << "\nНекорректный id\n";
		return;
	}
	if (id.size() != 6) // Проверка на размер id
	{
		cout << "\nНекорректный id\n";
		return;
	}
	if (HashFindCustomer(id) != -1)
	{
		cout << "\nПокупатель с таким id уже присутствует в таблице\n";
		return;
	}
	cout << "Введите ФИО покупателя\n>>";
	cin >> second_name >> first_name >> father_name;
	name = second_name + " " + first_name + " " + father_name;
	cout << "Введите адрес покупателя\n>>";
	adress = ReadString();
	Push(id, name, adress);
}

int HashTable::HashFunc(string id, int shift) // Хеш-функция 
{ // Однозначно возвращает индекс покупателя в хеш-таблице 
  // shift - сдвиг, решение проблемы колизии
  // сдвиг будет увеличиваться на единицу до обнаружения новой свободной ячейки
	int hash = (stoi(id) + shift) % SIZE; // Преобразование string в int
	// Хеш - остаток от деления шестизначного id на размер таблицы
	return hash;
}

int HashTable::LineTrial(string id) // Функция линейного опробования
{ // Возвращает следующую свободную ячейку
	int start = HashFunc(id, 0); // Старт линейного опробования
	int shift = 0; // Функция начинает сдвиг с нуля
	for (int i = start; i < SIZE; i++) { // Обход таблицы вправо
		shift += 1; // Увеличиваем сдвиг на единицу
		int index = HashFunc(id, shift); // Вызов хеш-функции со сдвигом
		if (!customers[index]) // Если ячейка покупателя в таблице пуста
		{
			return index; // Вернуть свободный индекс хеш-таблицы
		}
	}
	for (int i = 0; i < start; i++) { // Обход таблицы вправо
		shift += 1; // Увеличиваем сдвиг на единицу
		int index = HashFunc(id, shift);// Вызов хеш-функции со сдвигом
		if (!customers[index]) // Если ячейка покупателя в таблице пуста
		{
			return index; // Вернуть свободный индекс хеш-таблицы
		}
	}
}

void HashTable::PrintAllCustomers()
{
	if (size == 0) {
		cout << "Таблица с покупателями пуста\n";
		return;
	}
	cout << "Список покупателей:\n";
	for (int i = 0; i < SIZE; i++) 
	{
		if (customers[i] != nullptr)
		{
			cout << "\nID покупателя: ";
			cout << customers[i]->GetID();
			cout << "\nФИО покупателя: ";
			cout << customers[i]->GetName();
			cout << "\nАдресс покупателя: ";
			cout << customers[i]->GetAddress();
			cout << "\n";
		}
	}
}

int HashTable::LineFindCustomer(string name) // Функция линейного поиска покупателя
{
	for (int i = 0; i < SIZE; i++) // Линейный обход таблицы
	{
		if (!customers[i])
		{
			continue;
		}
		string full_name = customers[i]->GetName(); // Получение полного имени покупателя
		// Вычленение фамилии через метод строки
		string second_name = full_name.substr(0 /*Начальный индекс*/, full_name.find(" ") /*Количество символов через индекс первого пробела*/);
		if (ismatch(second_name, name))
		{
			return i; // Возвращает номер ячейки с совпадающей фамилией
		}
	}
	return -1;
}

void HashTable::KvaziLineFindCustomer()
{
	if (size == 0) { // 
		cout << "Таблица с покупателями пуста\n";
		return;
	}
	string name;
	cout << "Введите фамилию покупателя\n>>";
	cin >> name;
	if (LineFindCustomer(name) == -1)
	{
		cout << "Покупатель с такой фамилией в таблице отсутствует\n";
		return;
	}
	int index;
	index = LineFindCustomer(name);
	cout << "ID покупателя: ";
	cout << customers[index]->GetID();
	cout << "\nФИО покупателя: ";
	cout << customers[index]->GetName();
	cout << "\nАдресс покупателя: ";
	cout << customers[index]->GetAddress();
	cout << "\n";
}

int HashTable::HashFindCustomer(string id) // Функция хешированного поиска покупателя
{
	int start = HashFunc(id, 0); // Получаем стартовый очевидный id
	int shift = 0; // Смещение индекса хеш-таблцицы вправо 
	for (int i = start; i < SIZE; i++)
	{
		shift += 1; // Увеличение смещение на единицу вправо
		int index = HashFunc(id, shift);
		if (!customers[index]) // Если ячейка таблицы с покупателем пустая
		{
			continue; // Следующая итерация цикла for(i)
		}
		if (ismatch(customers[index]->GetID(), id)) // Если id совпали
		{
			return index; // Возвращение индекса покупателя
		}
	}
	for (int i = 0; i < start; i++)
	{
		shift += 1; // Увеличение смещение на единицу вправо
		int index = HashFunc(id, shift);
		if (!customers[index]) // Если ячейка таблицы с покупателем пустая
		{
			continue; // Следующая итерация цикла for(i)
		}
		if (ismatch(customers[index]->GetID(), id)) // Если id совпали
		{
			return index; // Возвращение индекса покупателя
		}
	}
	return -1;
}

void HashTable::KvaziHashFindCustomer()
{
	if (size == 0) { // 
		cout << "Таблица с покупателями пуста\n";
		return;
	}
	string id;
	cout << "Введите id покупателя\n>>";
	cin >> id;
	if (!CheckNums(id))
	{
		cout << "\nНекорректный id\n";
		return;
	}
	if (HashFindCustomer(id) == -1)
	{
		cout << "Покупатель с таким id в таблице отсутствует\n";
		return;
	}
	int index;
	index = HashFindCustomer(id);
	cout << "ID покупателя: ";
	cout << customers[index]->GetID();
	cout << "\nФИО покупателя: ";
	cout << customers[index]->GetName();
	cout << "\nАдресс покупателя: ";
	cout << customers[index]->GetAddress();
	cout << "\n";
}

int HashTable::ismatch(string first, string second) { // Функция проверки совпадения данных
	if (first == second)
	{
		return 1;
	}
	return 0;
}

void HashTable::clear()
{
	for (int i = 0; i < SIZE; i++) { // Проход по всей хеш-таблице
		if (!customers[i]) // Если ячейка незаполнена ее удаление не требуется
		{
			continue; // Переход на следующую итерацию цикла for(i)
		}
		delete customers[i]; // Удаление покупателя
		customers[i] = nullptr; // Ячейка хеш-таблицы с покупателем пустая
		;
	}
	size = 0;
}

void HashTable::dell(string id)
{
	int index = HashFindCustomer(id); // Получение индекса покупателя с нужным id
	delete customers[index]; // Удаление покупателя
	customers[index] = nullptr; // Ячейка хеш-таблицы с покупателем пустая
	size -= 1;
}

void HashTable::predell()
{
	string id;
	cout << "\nВведите id покупателя, которого нужно удалить из таблицы:\n>>";
	cin >> id;
	if (!CheckNums(id))
	{
		cout << "\nНекорректный id\n";
		return;
	}
	int index = HashFindCustomer(id); // Получение индекса покупателя с нужным id
	if (index == -1) { // В случая если покупатель не найден
		cout << "Покупатель с таким id не найден\n";
		return;
	}
	dell(id); 
}

customer::customer(string id, string name, string adress) // Базовый конструктор покупателя с параметрами
{
	// this - указатель на текущий объект (класса)
	this->id = id;
	this->name = name;
	this->address = adress;
	// Конструктор при создании покупателя (В момент заполнения таблицы)
}

string customer::GetID()
{ // Метод получения id
	return id; // Возвращает id
}

string customer::GetName()
{ // Метод получения имени
	return name; // Возвращает имя
}

string customer::GetAddress()
{ // Метод получения адреса
	return address; // Возвращает имя
}

void PrintMenu()
{
	cout << "\nМеню\n"
		"1 - Добавить запись покупателя\n"
		"2 - Удалить запись покупателя\n"
		"3 - Вывести все записи покупателей в таблице\n"
		"4 - Хешированный поиск покупателя по id\n"
		"5 - Линейный поиск покупателя через фамилию\n"
		"6 - Удалить хеш-таблицу\n"
		"0 - Выход из программы\n";
}

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251); // Установка русской расскладки
	HashTable Table;
	// Проверка id на шестеру
	int console = -1;
	string name;
	while (console != 0)
	{
		PrintMenu();
		cout << ">>";
		cin >> console;
		switch (console)
		{
		case 1:
			Table.KvaziPush();
			break;
		case 2:
			Table.predell();
			break;
		case 3:
			Table.PrintAllCustomers();
			break;
		case 4:
			Table.KvaziHashFindCustomer();
			break;
		case 5:
			Table.KvaziLineFindCustomer();
			break;
		case 6:
			Table.clear();
			break;
		}
	}
	return 0;
}
