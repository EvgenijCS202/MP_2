#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <string>
#include <map>

std::string info = "info.txt";
std::string resultFile = "res.txt";
std::string timesFile = "times.txt";

std::ofstream fout(resultFile);

int sizesNum = 7;

int sizes[7] = { 100, 1000, 2000, 5000, 10000, 25000, 100000 };

struct Child {
    int year;
    int squad;
    std::string name;
    std::string date;
    int shift;

    Child(
    int year,
    int squad,
    std::string name,
    std::string date,
    int shift
    )
    {
        this->year = year;
        this->squad = squad;
        this->name = name;
        this->date = date;
        this->shift = shift;
    }

    friend bool operator== (const Child& a, const Child& b) {
        return (a.year == b.year && a.squad == b.squad && a.name == b.name && a.shift == b.shift)?true:false;
    }

    friend bool operator< (const Child& a, const Child& b) {
        return (a.year < b.year || ((a.year == b.year) && (a.squad < b.squad)) ||
                ((a.year == b.year) && (a.squad == b.squad) && (a.shift < b.shift))||
                ((a.year == b.year) && (a.squad == b.squad) && (a.shift == b.shift) && (a.name < b.name)))?true:false;
    }

    friend bool operator<= (const Child& a, const Child& b) {
        return (a < b || a == b)?true:false;
    }

    friend bool operator> (const Child& a, const Child& b) {
        return (!(a < b) && !(a == b))?true:false;
    }

    friend bool operator>= (const Child& a, const Child& b) {
        return (!(a < b))?true:false;
    }


    friend std::ostream& operator<<(std::ostream& os, const Child& a) {
        os << a.year << ' ' << a.squad << ' ' << a.name << ' ' << a.date << ' ' << a.shift << '\n';
        return os;
    }
};

std::string randomString(const int len) {
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);
    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return tmp_s;
}

std::string randomDate(int minY, int maxY)
{
    return std::to_string(rand() % 28 + 1)+'.'+std::to_string(rand() % 12+1)+'.'+std::to_string(minY + rand() % (maxY - minY));
}

void createInfo() {
    srand((unsigned)time(NULL));
    std::ofstream fout(info);

    for (int i = 0; i < sizesNum; ++i) {
        //Вывод числа записей
        fout << sizes[i] << "\n";
        for (int j = 0; j < sizes[i]; ++j) {
            //Вывод полей по порядку
            fout << 2016 - rand() % 40 << "\n";   // Год
            fout << rand() % 13 + 1 << "\n";   // Отряд
            fout << randomString(10)  << "\n";   // ФИО
            fout << randomDate(1990, 2016) << "\n"; // Дата рождения
            fout << rand() % 5 + 1 << "\n"; // Смена
        }
    }
}

std::vector<std::vector<Child>> readInfo() {
    std::ifstream fin(info);

    std::vector<std::vector<Child>> result;

    int dim;
    int year;
    int squad;
    std::string name;
    std::string date;
    int shift;

    for (int i = 0; i < sizesNum; ++i) {
        //Ввод числа записей
        fin >> dim;
        std::vector<Child> v;
        for (int j = 0; j < dim; ++j) {
            //Ввод полей по порядку
            fin >> year >> squad >> name >> date >> shift;
            Child temp(year, squad, name, date, shift);
            v.push_back(temp);
        }
        result.push_back(v);
    }

    return result;
}

std::vector<Child> selectSort(std::vector<Child> vec) {
    std::vector<Child> a = vec;
    int min = 0; // индекс минимального значения
    for (int i = 0; i < a.size(); i++)
    {
        min = i; // изначально min - номер текущей ячейки
        for (int j = i + 1; j < a.size(); j++)
            if(a[j] < a[min])
                min = j;
        // после нахождения минимального элемента, поменяем его местами с текущим
        if(min != i)
            std::swap(a[i], a[min]);
    }
    return a;
}

std::vector<Child> insertSort(std::vector<Child> vec) {
    std::vector<Child> a = vec;
    int i, j;
    for (i = 0; i < a.size(); i++)
    {
        Child x = a[i];
        // Поиск места для вставки
        for (j=i-1; j>=0 && a[j] > x; j--)
            a[j+1] = a[j]; // двигаем неподходящие элементы
        a[j+1] = x; // Помещаем на нужное место
    }
    return a;
}

std::vector<Child> shakerSort(std::vector<Child> vec) {
    std::vector<Child> arr = vec;

    int control = arr.size() - 1;
    int left = 0;
    int right = arr.size() - 1;
    do {
        for (int i = left; i < right; i++) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                control = i;
            }
        }
        right = control;
        for (int i = right; i > left; i--) {
            if (arr[i] < arr[i - 1]) {
                std::swap(arr[i], arr[i - 1]);
                control = i;
            }
        }
        left = control;
    } while (left < right);

    return arr;
}

void writeResult(std::string title, std::vector<std::vector<Child>>& res) {
    std::ofstream fout(resultFile, std::ios::app);

    fout << title << "\n";

    for (int i = 0; i < sizesNum; ++i) {
        //Вывод числа записей
        fout << "Размер: " << res[i].size() << "\n";
        for (int j = 0; j < res[i].size(); ++j) {
            //Вывод объекта
            fout << res[i][j] << "\n";
        }
    }
}

void writeTime(std::string title, std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) {
    fout << title << std::endl;

    fout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " [микросекунд]\n\n";
}

std::vector<int> linearSearch(std::vector<Child>& v, std::string key) {
    std::vector<int> res;
    for (int i = 0; i < v.size(); ++i)
        if (v[i].name == key)
            res.push_back(i);
    return res;
}

int getLowerBound(std::vector<Child>& v, std::string key) {
    int mid = 0, left = 0, right = v.size();
    while (1)
    {
        mid = (left + right) / 2;

        if (mid < 0 || mid >= v.size())
            return -1;

        if (key <= v[mid].name)
            right = mid - 1;
        else if (key > v[mid].name && mid + 1 < v.size() && key == v[mid + 1].name)
            return mid;
        else if (key > v[mid].name)
            left = mid + 1;

        if (left > right)
            return -1;
    }
}

int getUpperBound(std::vector<Child>& v, std::string key) {
    int mid = 0, left = 0, right = v.size();
    while (1)
    {
        mid = (left + right) / 2;

        if (mid < 0 || mid >= v.size())
            return -1;

        if (key < v[mid].name && mid - 1 >= 0 && key == v[mid - 1].name)
            return mid;
        else if (key < v[mid].name)
            right = mid - 1;
        else if (key >= v[mid].name)
            left = mid + 1;

        if (left > right)
            return -1;
    }
}

std::vector<Child> binarySearch(std::vector<Child>& v, std::string key) {
    int left = getLowerBound(v, key);
    int right = getUpperBound(v, key);

    if (left == -1 || right == -1)
        return std::vector<Child>();

    auto first = v.cbegin() + left + 1;
    auto last = v.cbegin() + right;

    std::vector<Child> res(first, last);

    return res;
}

bool operator<(Child a, Child b) { return (a.name < b.name); }

int main()
{
    setlocale(LC_ALL, "Russian");

    std::vector<std::vector<Child>> initialArray = readInfo();

    std::vector<std::string> selectedKeys = { "1", "1", "1", "1", "1", "1", "1", "1"};

    std::chrono::steady_clock::time_point start, end;
    Child foundValue(1,1,"1","1",1);

    fout << "Прямой поиск:\n\n";

    start = std::chrono::steady_clock::now();

    for (int i = 0; i < sizesNum; ++i) {
        std::vector<int> temp = linearSearch(initialArray[i], selectedKeys[i]);
        end = std::chrono::steady_clock::now();
        fout << "Искался ключ: " << selectedKeys[i] << std::endl;
        if (!temp.empty())
            for (int j = 0; j < temp.size(); ++j)
                fout << "Найден элемент: " << initialArray[i][temp[j]];
        writeTime("На прямой поиск с " + std::to_string(initialArray[i].size()) + " значениями ушло: ", start, end);
        start = end;
    }

    fout << "Сортировка:" << std::endl << std::endl;

    start = std::chrono::steady_clock::now();

    for (int i = 0; i < sizesNum; ++i) {
        initialArray[i] = selectSort(initialArray[i]);
        binarySearch(initialArray[i], selectedKeys[i]);
        end = std::chrono::steady_clock::now();
        writeTime("На сортировку и двоичный поиск " + std::to_string(initialArray[i].size()) + " значениий ушло: ", start, end);
        start = end;
    }

    fout << "Бинарный поиск в отсортированном массиве:" << std::endl;

    start = std::chrono::steady_clock::now();

    for (int i = 0; i < sizesNum; ++i) {
        std::vector<Child> temp = binarySearch(initialArray[i], selectedKeys[i]);
        end = std::chrono::steady_clock::now();
        fout << "Искался ключ: " << selectedKeys[i] << "\n";
        if (!temp.empty())
        for (int j = 0; j < temp.size(); ++j)
            fout << "Найден элемент: " << temp[j];
        writeTime("На бинарный поиск с " + std::to_string(initialArray[i].size()) + " значениями ушло: ", start, end);
        start = end;
    }

    std::vector<std::multimap<std::string, Child>> m;

    for (int i = 0; i < sizesNum; ++i) {
        std::multimap<std::string, Child> temp;
        for (int j = 0; j < initialArray[i].size(); ++j)
            temp.insert({ initialArray[i][j].name, initialArray[i][j]});
        m.push_back(temp);
    }

    initialArray.clear();

    fout << "Поиск в ассоциативном массиве:\n\n";

    start = std::chrono::steady_clock::now();

    for (int i = 0; i < sizesNum; ++i) {
        auto bounds = m[i].equal_range(selectedKeys[i]);
        end = std::chrono::steady_clock::now();
        fout << "Искался ключ: " << selectedKeys[i] << "\n";
        if (bounds.first != m[i].end())
        for (auto it = bounds.first; it != bounds.second; ++it)
            fout << "Найден элемент: " << it->second;
        writeTime("На поиск в ассоциативном массиве с " + std::to_string(m[i].size()) + " значениями ушло: ", start, end);
        start = end;
    }

    return 0;
}
