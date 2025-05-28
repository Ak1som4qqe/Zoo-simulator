#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <locale>
#include <limits>
#include <ios>
#include <map>
#include <memory>
#include <io.h>
#include <fcntl.h>

using namespace std;

// Перечисление типов животных
enum AnimalType { HERBIVORE, CARNIVORE };
// Перечисление климатических зон
enum Climate { TROPICAL, TEMPERATE, ARCTIC, DESERT };
// Перечисление типов работников
enum WorkerType { VET, CLEANER, FEEDER, DIRECTOR };
// Перечисление полов животных
enum Gender { MALE, FEMALE };

// Названия климатических зон
const vector<wstring> CLIMATES = {
    L"Тропический",
    L"Умеренный",
    L"Арктический",
    L"Пустынный"
};

// Названия полов
const vector<wstring> GENDER_NAMES = {
    L"Самец",
    L"Самка"
};

// Названия типов работников
const vector<wstring> WORKER_TYPES = {
    L"Ветеринар",
    L"Уборщик",
    L"Кормильщик",
    L"Директор"
};

// Предварительное объявление класса Animal
class Animal;
using AnimalPtr = shared_ptr<Animal>;

// Класс, представляющий животное
class Animal {
private:
    wstring name;           // Имя животного
    AnimalType type;        // Тип (травоядное/хищник)
    Climate climate;        // Климатическая зона
    double price;           // Цена животного
    wstring description;    // Описание
    bool isInfected = false; // Заражено ли животное
    bool isDying = false;   // Умирает ли животное
    int age_days = 1;       // Возраст в днях
    int infectionDay = 0;   // День заражения
    double weight;          // Вес животного
    double min_weight;      // Минимальный вес для вида
    double max_weight;      // Максимальный вес для вида
    Gender gender;          // Пол животного
    AnimalPtr parent1 = nullptr; // Первый родитель
    AnimalPtr parent2 = nullptr; // Второй родитель
    wstring species;        // Вид животного
    bool isHybrid = false;  // Является ли гибридом

public:
    // Конструктор животного
    Animal(wstring n, AnimalType t, Climate c, double p, wstring desc, double min_w, double max_w, Gender g, wstring sp,
        AnimalPtr p1 = nullptr, AnimalPtr p2 = nullptr, bool hybrid = false)
        : name(n), type(t), climate(c), price(p), description(desc),
        min_weight(min_w), max_weight(max_w), gender(g), species(sp),
        parent1(p1), parent2(p2), isHybrid(hybrid) {
        // Генерация случайного веса в пределах min-max
        weight = min_weight + static_cast<double>(rand() % static_cast<int>((max_weight - min_weight) * 100)) / 100.0;
    }

    // Геттеры для всех полей
    wstring getName() const { return name; }
    AnimalType getType() const { return type; }
    Climate getClimate() const { return climate; }
    double getPrice() const { return price; }
    wstring getDescription() const { return description; }
    bool getIsInfected() const { return isInfected; }
    bool getIsDying() const { return isDying; }
    int getAgeDays() const { return age_days; }
    int getInfectionDay() const { return infectionDay; }
    double getWeight() const { return weight; }
    Gender getGender() const { return gender; }
    wstring getGenderName() const { return GENDER_NAMES[static_cast<int>(gender)]; }
    wstring getSpecies() const { return species; }
    AnimalPtr getParent1() const { return parent1; }
    AnimalPtr getParent2() const { return parent2; }
    bool getIsHybrid() const { return isHybrid; }

    // Сеттеры
    void setName(const wstring& newName) { name = newName; }
    void setInfected(bool infected, int day) { isInfected = infected; infectionDay = day; }
    void setDying(bool dying) { isDying = dying; }
    void increaseAge() { age_days++; } // Увеличение возраста на 1 день

    // Проверка, может ли животное размножаться
    bool canReproduce() const {
        return age_days >= 5 && !isInfected && !isDying;
    }

    // Проверка, может ли животное умереть от старости
    bool canDieOfOldAge(int maxAge) const {
        if (age_days > maxAge) {
            int chance = age_days - maxAge;
            return rand() % 100 < chance;
        }
        return false;
    }

    // Попытка побега (в текущей реализации всегда false)
    bool tryEscape() const {
        return false;
    }

    // Размножение двух животных
    static AnimalPtr breed(const AnimalPtr& a1, const AnimalPtr& a2) {
        // Проверка возможности размножения
        if (!a1->canReproduce() || !a2->canReproduce()) {
            wcout << L"Ошибка: одно из животных не может размножаться (слишком молодо, больно или умирает)\n";
            return nullptr;
        }

        // Проверка разных полов
        if (a1->gender == a2->gender) {
            wcout << L"Ошибка: нельзя скрещивать животных одного пола\n";
            return nullptr;
        }

        wstring hybridName;
        wstring newSpecies;
        bool hybrid = false;
        Climate newClimate;

        // Если виды одинаковые - обычное потомство
        if (a1->species == a2->species) {
            hybridName = a1->species;
            newSpecies = a1->species;
            hybrid = false;
            newClimate = a1->climate;
        }
        else {
            // Создание гибрида
            int nameVariant = rand() % 2;
            if (nameVariant == 0) {
                hybridName = a1->species.substr(0, a1->species.size() / 2 + 1) +
                    a2->species.substr(a2->species.size() / 2);
            }
            else {
                hybridName = a2->species.substr(0, a2->species.size() / 2 + 1) +
                    a1->species.substr(a1->species.size() / 2);
            }
            newSpecies = hybridName;
            hybrid = true;
            newClimate = (rand() % 2 == 0) ? a1->climate : a2->climate;
        }

        // Определение характеристик потомства
        AnimalType newType = (rand() % 2 == 0) ? a1->type : a2->type;
        Gender newGender = (rand() % 2 == 0) ? MALE : FEMALE;

        double newMinWeight = (a1->min_weight + a2->min_weight) / 2.0;
        double newMaxWeight = (a1->max_weight + a2->max_weight) / 2.0;

        wstring newDescription = hybrid ? (L"Гибрид " + a1->species + L" и " + a2->species) : a1->species;

        double newPrice = (a1->price + a2->price) / 2.0 * (hybrid ? 0.8 : 1.0);

        // Создание нового животного
        return make_shared<Animal>(hybridName, newType, newClimate, newPrice, newDescription,
            newMinWeight, newMaxWeight, newGender, newSpecies, a1, a2, hybrid);
    }

    // Оператор сравнения животных
    bool operator==(const Animal& other) const {
        return name == other.name &&
            type == other.type &&
            climate == other.climate &&
            price == other.price &&
            description == other.description &&
            isInfected == other.isInfected &&
            isDying == other.isDying &&
            age_days == other.age_days &&
            infectionDay == other.infectionDay &&
            weight == other.weight &&
            min_weight == other.min_weight &&
            max_weight == other.max_weight &&
            gender == other.gender &&
            species == other.species;
    }
};

// Структура, представляющая работника зоопарка
struct Worker {
    WorkerType type;    // Тип работника
    wstring name;       // Имя работника
    double salary;      // Зарплата

    // Конструктор работника
    Worker(WorkerType t, wstring n) : type(t), name(n) {
        // Установка зарплаты в зависимости от типа
        switch (type) {
        case VET: salary = 50.0; break;
        case CLEANER: salary = 20.0; break;
        case FEEDER: salary = 30.0; break;
        case DIRECTOR: salary = 500.0; break;
        }
    }

    // Получение названия типа работника
    wstring getTypeName() const {
        return WORKER_TYPES[static_cast<int>(type)];
    }
};

// Предварительное объявление класса Zoo
class Zoo;

// Класс, представляющий вольер для животных
class Pen {
private:
    int capacity;               // Вместимость вольера
    AnimalType allowedType;     // Разрешенный тип животных
    Climate climate;            // Климат вольера
    vector<AnimalPtr> animals;  // Животные в вольере
    bool isClean = true;        // Чистота вольера
    int lastInfectionDay = 0;   // Последний день заражения
    bool outbreakStarted = false; // Началась ли эпидемия
    int outbreakDay = 0;        // День начала эпидемии

public:
    // Конструктор вольера
    Pen(int cap, AnimalType type, Climate cl)
        : capacity(cap), allowedType(type), climate(cl) {}

    // Проверка, можно ли добавить животное в вольер
    bool canAdd(const AnimalPtr& a) const {
        if (a->getType() != allowedType) return false;

        if (a->getIsHybrid()) {
            if (a->getParent1()->getSpecies() == a->getParent2()->getSpecies()) {
                return a->getClimate() == climate;
            }
            else {
                return a->getClimate() == a->getParent1()->getClimate() ||
                    a->getClimate() == a->getParent2()->getClimate();
            }
        }
        return a->getClimate() == climate && animals.size() < capacity;
    }

    // Добавление животного в вольер
    void addAnimal(const AnimalPtr& a) {
        if (canAdd(a)) {
            animals.push_back(a);
        }
    }

    // Удаление животного из вольера по индексу
    void removeAnimal(int index) {
        if (index >= 0 && index < static_cast<int>(animals.size())) {
            animals.erase(animals.begin() + index);
        }
    }

    // Геттеры
    vector<AnimalPtr>& getAnimals() { return animals; }
    const vector<AnimalPtr>& getAnimals() const { return animals; }
    int getAnimalCount() const { return animals.size(); }
    AnimalType getAllowedType() const { return allowedType; }
    Climate getClimate() const { return climate; }
    int getCapacity() const { return capacity; }
    bool getIsClean() const { return isClean; }
    void setClean(bool clean) { isClean = clean; }

    // Получение описания вольера
    wstring getDescription() const {
        return L"Вольер для " +
            (allowedType == HERBIVORE ? wstring(L"травоядных") : wstring(L"хищников")) +
            L" (" + CLIMATES[static_cast<int>(climate)] + L")";
    }

    // Подсчет зараженных животных в вольере
    int getInfectedCount() const {
        int count = 0;
        for (const auto& a : animals) {
            if (a->getIsInfected() && !a->getIsDying()) count++;
        }
        return count;
    }

    // Заражение случайного животного
    void infectRandomAnimal(int currentDay) {
        if (getInfectedCount() == 0 &&
            lastInfectionDay != currentDay &&
            rand() % 100 < 35) {
            vector<AnimalPtr> healthy;
            for (auto& a : animals) {
                if (!a->getIsInfected() && !a->getIsDying()) healthy.push_back(a);
            }
            if (!healthy.empty()) {
                auto selected = healthy[rand() % healthy.size()];
                selected->setInfected(true, currentDay);
                lastInfectionDay = currentDay;
                wcout << L"⚠ В вольере " << getDescription() << L" заражено: " << selected->getName() << endl;
            }
        }
    }

    // Распространение болезни в вольере
    void spreadDisease(int currentDay) {
        vector<AnimalPtr> infected;
        for (auto& a : animals) {
            if (a->getIsInfected() && !a->getIsDying() &&
                a->getInfectionDay() <= currentDay - 1) {
                infected.push_back(a);
            }
        }

        for (auto& a : infected) {
            vector<AnimalPtr> healthy;
            for (auto& b : animals) {
                if (!b->getIsInfected() && !b->getIsDying() && b->getInfectionDay() == 0)
                    healthy.push_back(b);
            }

            for (int i = 0; i < 2 && !healthy.empty(); i++) {
                int idx = rand() % healthy.size();
                auto newInfected = healthy[idx];
                newInfected->setInfected(true, currentDay);
                wcout << L"⚠ В вольере " << getDescription() << L" заражено: " << newInfected->getName() << endl;
                healthy.erase(healthy.begin() + idx);
            }
        }
    }

    // Обработка вспышки болезни
    void handleOutbreak(int currentDay) {
        int total = animals.size();
        int infected = getInfectedCount();

        if (!outbreakStarted && infected > total / 2) {
            outbreakStarted = true;
            outbreakDay = currentDay;
            wcout << L"⚠ В вольере " << getDescription() << L" началась вспышка болезни!\n";
        }
    }

    // Обработка умирающих животных
    void handleDying(int maxAge) {
        if (!outbreakStarted) return;

        vector<AnimalPtr> survivors;
        for (auto& a : animals) {
            bool shouldDie = false;
            wstring deathReason;

            if (a->getIsInfected() && outbreakStarted) {
                shouldDie = true;
                deathReason = L"тиаравирус";
            }
            else if (a->canDieOfOldAge(maxAge)) {
                shouldDie = true;
                deathReason = L"старость";
            }

            if (shouldDie) {
                wcout << L"⚠ В вольере " << getDescription() << L" погибло: " << a->getName() << L" (" << deathReason << L")" << endl;
            }
            else {
                survivors.push_back(a);
            }
        }
        animals = survivors;

        if (outbreakStarted && getInfectedCount() == 0) {
            outbreakStarted = false;
        }
    }

    // Увеличение возраста животных в вольере
    void handleAging() {
        for (auto& a : animals) {
            a->increaseAge();
        }
    }

    // Обновление состояния чистоты вольера
    void updateCleanliness() {
        if (!animals.empty() && rand() % 3 == 0) {
            isClean = false;
            wcout << L"Вольер " << getDescription() << L" стал грязным." << endl;
        }
    }

    // Попытка ручного скрещивания животных
    bool tryManualBreeding(AnimalPtr a1, AnimalPtr a2, Zoo& zoo);
};

// Класс, представляющий рынок животных
class AnimalMarket {
private:
    vector<AnimalPtr> animals;  // Доступные животные
    int lastUpdateDay = -1;     // Последний день обновления
    const int MAX_ANIMALS = 10; // Максимальное количество животных
    const int UPDATE_COST = 200; // Стоимость обновления

public:
    // Генерация новых животных на рынке
    void generateAnimals(int currentDay) {
        animals.clear();

        vector<wstring> species = { L"Лев", L"Тигр", L"Жираф", L"Слон", L"Зебра", L"Волк", L"Гепард", L"Овцебык" };
        vector<AnimalType> types = { HERBIVORE, CARNIVORE };
        vector<Climate> climates = { TROPICAL, TEMPERATE, ARCTIC, DESERT };
        vector<Gender> genders = { MALE, FEMALE };

        // Данные о видах животных (цена, мин и макс вес)
        map<wstring, tuple<double, double, double>> speciesData = {
            {L"Лев", {1000.0, 180.0, 250.0}},
            {L"Тигр", {950.0, 160.0, 230.0}},
            {L"Жираф", {700.0, 800.0, 1200.0}},
            {L"Слон", {800.0, 5000.0, 6000.0}},
            {L"Зебра", {600.0, 250.0, 400.0}},
            {L"Волк", {700.0, 40.0, 80.0}},
            {L"Гепард", {850.0, 35.0, 65.0}},
            {L"Овцебык", {750.0, 200.0, 400.0}}
        };

        for (int i = 0; i < MAX_ANIMALS; ++i) {
            wstring speciesName = species[rand() % species.size()];
            auto data = speciesData[speciesName];

            AnimalType type;
            Climate climate;

            // Определение типа и климата в зависимости от вида
            if (speciesName == L"Лев" || speciesName == L"Тигр" || speciesName == L"Волк" || speciesName == L"Гепард") {
                type = CARNIVORE;
            }
            else {
                type = HERBIVORE;
            }

            if (speciesName == L"Лев" || speciesName == L"Жираф") {
                climate = TROPICAL;
            }
            else if (speciesName == L"Волк" || speciesName == L"Овцебык") {
                climate = ARCTIC;
            }
            else if (speciesName == L"Зебра" || speciesName == L"Гепард") {
                climate = DESERT;
            }
            else {
                climate = TEMPERATE;
            }

            Gender gender = genders[rand() % genders.size()];
            wstring desc = (climate == TROPICAL ? L"Тропический" :
                climate == ARCTIC ? L"Арктический" :
                climate == DESERT ? L"Пустынный" : L"Умеренный");

            // Создание нового животного
            animals.push_back(make_shared<Animal>(
                speciesName, type, climate, get<0>(data), desc,
                get<1>(data), get<2>(data), gender, speciesName
            ));
        }

        lastUpdateDay = currentDay;
    }

    // Проверка возможности обновления рынка
    bool canUpdate(int currentDay) const {
        return currentDay > lastUpdateDay;
    }

    // Обновление рынка животных
    bool update(int currentDay, double& money) {
        if (canUpdate(currentDay)) {
            if (money >= UPDATE_COST) {
                money -= UPDATE_COST;
                generateAnimals(currentDay);
                return true;
            }
        }
        return false;
    }

    // Геттеры
    const vector<AnimalPtr>& getAnimals() const { return animals; }

    // Покупка животного с рынка
    AnimalPtr buyAnimal(int index, double& money) {
        if (index >= 0 && index < animals.size()) {
            if (money >= animals[index]->getPrice()) {
                money -= animals[index]->getPrice();
                AnimalPtr bought = animals[index];
                animals.erase(animals.begin() + index);
                return bought;
            }
        }
        return nullptr;
    }
};

// Класс, представляющий зоопарк
class Zoo {
private:
    wstring name;               // Название зоопарка
    double money;               // Деньги
    int food;                   // Количество еды
    int popularity;             // Популярность зоопарка
    vector<Pen> pens;           // Вольеры
    vector<Worker> workers;     // Работники
    int day;                    // Текущий день
    AnimalMarket market;        // Рынок животных
    const int MAX_AGE = 30;     // Максимальный возраст животных
    const int MAX_DAYS = 50;    // Максимальное количество дней игры
    int animalsBoughtToday = 0; // Количество купленных сегодня животных
    double debt = 0.0;          // Долг
    double dailyDebtPayment = 0.0; // Ежедневный платеж по долгу
    int debtDaysLeft = 0;       // Оставшееся количество дней по кредиту
    vector<wstring> events;     // События дня
    int lastCelebCount = 0;     // Количество знаменитостей вчера
    int lastPhotographerCount = 0; // Количество фотографов вчера

public:
    // Конструктор зоопарка
    Zoo(wstring zooName, wstring directorName)
        : name(zooName), money(10000.0), food(0), popularity(50), day(0) {
        workers.push_back(Worker(DIRECTOR, directorName));
        market.generateAnimals(0);
    }

    // Геттеры
    wstring getName() const { return name; }
    int getDay() const { return day; }
    double getMoney() const { return money; }
    int getFood() const { return food; }
    int getPopularity() const { return popularity; }
    vector<Pen>& getPens() { return pens; }
    vector<Worker>& getWorkers() { return workers; }
    AnimalMarket& getMarket() { return market; }
    double getDebt() const { return debt; }
    int getDebtDaysLeft() const { return debtDaysLeft; }
    int getMaxDays() const { return MAX_DAYS; }
    int getMaxAge() const { return MAX_AGE; }

    // Сеттеры
    void setMoney(double m) { money = m; }
    void setFood(int f) { food = f; }
    void setPopularity(int p) { popularity = p; }

    // Добавление события в журнал
    void addEvent(const wstring& event) {
        events.push_back(event);
    }

    // Отображение событий дня
    void showEvents() {
        if (!events.empty()) {
            wcout << L"\nСобытия дня:\n";
            for (const auto& e : events) {
                wcout << L"- " << e << endl;
            }
            events.clear();
        }
    }

    // Взятие кредита
    void takeLoan(double amount, int days) {
        money += amount;
        debt += amount * 1.2;  // 20% interest
        dailyDebtPayment = debt / days;
        debtDaysLeft = days;
        addEvent(L"Взят кредит на $" + to_wstring(static_cast<int>(amount)) + L" на " + to_wstring(days) + L" дней");
    }

    // Обработка платежей по долгу
    void processDebt() {
        if (debtDaysLeft > 0) {
            double payment = min(dailyDebtPayment, debt);
            if (money >= payment) {
                money -= payment;
                debt -= payment;
                debtDaysLeft--;
                addEvent(L"Платеж по кредиту: $" + to_wstring(static_cast<int>(payment)));
            }
            else {
                addEvent(L"Просрочка платежа по кредиту!");
                popularity -= 10;
            }
        }
    }

    // Подсчет общего количества животных
    int totalAnimals() const {
        int total = 0;
        for (const Pen& pen : pens) total += pen.getAnimalCount();
        return total;
    }

    // Расчет необходимого количества ветеринаров
    int calculateNeededVets() const {
        return (totalAnimals() + 19) / 20;
    }

    // Расчет необходимого количества уборщиков
    int calculateNeededCleaners() const {
        return pens.size();
    }

    // Расчет необходимого количества кормильщиков
    int calculateNeededFeeders() const {
        return (pens.size() + 1) / 2;
    }

    // Проверка наличия директора
    bool hasDirector() const {
        for (const auto& w : workers) {
            if (w.type == DIRECTOR) return true;
        }
        return false;
    }

    // Увольнение работника
    void removeWorker(int index) {
        if (index >= 0 && index < workers.size()) {
            bool wasDirector = workers[index].type == DIRECTOR;
            wstring workerName = workers[index].name;
            workers.erase(workers.begin() + index);
            addEvent(L"Уволен работник: " + workerName);

            if (wasDirector) {
                wcout << L"Вас изгнали из братства, геге\n";
                exit(0);
            }
        }
    }

    // Уничтожение вольера
    void destroyPen(int index) {
        if (index >= 0 && index < pens.size()) {
            if (pens[index].getAnimalCount() > 0) {
                wcout << L"Нельзя уничтожить вольер с животными!\n";
                return;
            }

            pens.erase(pens.begin() + index);
            addEvent(L"Уничтожен вольер");
            wcout << L"Вольер успешно уничтожен!\n";
        }
    }

    // Лечение всех животных
    void treatAllAnimals() {
        int treatedCount = 0;
        double totalCost = 0;

        for (auto& pen : pens) {
            for (auto& animal : pen.getAnimals()) {
                if (animal->getIsInfected()) {
                    totalCost += 100;
                }
            }
        }

        if (totalCost > money) {
            wcout << L"Недостаточно средств для лечения всех животных!\n";
            return;
        }

        for (auto& pen : pens) {
            for (auto& animal : pen.getAnimals()) {
                if (animal->getIsInfected()) {
                    animal->setInfected(false, 0);
                    animal->setDying(false);
                    treatedCount++;
                }
            }
        }

        money -= totalCost;
        addEvent(L"Вылечено " + to_wstring(treatedCount) + L" животных за $" + to_wstring(static_cast<int>(totalCost)));
        wcout << L"Вылечено " << treatedCount << L" животных за $" << static_cast<int>(totalCost) << endl;
    }

    // Автоматическое лечение животных ветеринарами
    void autoTreatAnimals() {
        int vets = 0;
        for (const auto& w : workers) {
            if (w.type == VET) vets++;
        }

        if (vets == 0) return;

        int animalsPerVet = totalAnimals() / vets;
        int treated = 0;

        for (auto& pen : pens) {
            for (auto& animal : pen.getAnimals()) {
                if (animal->getIsInfected() && treated < animalsPerVet * vets) {
                    animal->setInfected(false, 0);
                    animal->setDying(false);
                    treated++;
                }
            }
        }

        if (treated > 0) {
            addEvent(L"Ветеринары вылечили " + to_wstring(treated) + L" животных");
        }
    }

    // Переименование работника
    void renameWorker() {
        if (workers.empty()) {
            wcout << L"Нет работников для переименования\n";
            return;
        }

        wcout << L"Выберите работника:\n";
        for (size_t i = 0; i < workers.size(); ++i) {
            wcout << (i + 1) << L". " << workers[i].name << L" ("
                << workers[i].getTypeName() << L")\n";
        }

        int workerIndex;
        while (true) {
            wcout << L"Работник (1-" << workers.size() << L", 0 для отмены): ";
            if (wcin >> workerIndex && (workerIndex == 0 || (workerIndex >= 1 && workerIndex <= static_cast<int>(workers.size())))) break;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            wcout << L"Ошибка: введите 0 или номер работника\n";
        }
        wcin.ignore();

        if (workerIndex == 0) return;

        wstring newName;
        wcout << L"Введите новое имя: ";
        getline(wcin, newName);
        workers[workerIndex - 1].name = newName;
        wcout << L"Имя изменено!\n";
        addEvent(L"Работник переименован: " + newName);
    }

    // Наем работника
    void hireWorker() {
        wcout << L"Выберите тип работника:\n";
        for (size_t i = 0; i < WORKER_TYPES.size(); ++i) {
            wcout << (i + 1) << L". " << WORKER_TYPES[i] << endl;
        }

        int typeChoice;
        while (true) {
            wcout << L"Тип (1-" << WORKER_TYPES.size() << L", 0 для отмены): ";
            if (wcin >> typeChoice && (typeChoice == 0 || (typeChoice >= 1 && typeChoice <= static_cast<int>(WORKER_TYPES.size())))) break;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            wcout << L"Ошибка: введите 0 или число от 1 до " << WORKER_TYPES.size() << endl;
        }
        wcin.ignore();

        if (typeChoice == 0) return;

        WorkerType type = static_cast<WorkerType>(typeChoice - 1);

        if (type == DIRECTOR) {
            for (const auto& worker : workers) {
                if (worker.type == DIRECTOR) {
                    wcout << L"В зоопарке уже есть директор!\n";
                    return;
                }
            }
        }

        wstring name;
        while (true) {
            wcout << L"Введите имя работника: ";
            getline(wcin, name);
            if (!name.empty()) break;
            wcout << L"Имя не может быть пустым!\n";
        }

        workers.emplace_back(type, name);
        wcout << WORKER_TYPES[typeChoice - 1] << L" " << name << L" нанят!\n";
        addEvent(L"Нанят новый работник: " + name + L" (" + WORKER_TYPES[typeChoice - 1] + L")");
    }

    // Случайные события дня
    void randomEvents() {
        lastCelebCount = rand() % 3;
        lastPhotographerCount = rand() % 6;

        int bonus = lastCelebCount * 10 + lastPhotographerCount * 5;

        if (lastCelebCount > 0 || lastPhotographerCount > 0) {
            wstring eventMsg = L"Посетители дня: ";
            if (lastCelebCount > 0) {
                eventMsg += to_wstring(lastCelebCount) + L" знаменитост" +
                    (lastCelebCount == 1 ? L"ь" : (lastCelebCount < 5 ? L"и" : L"ей")) + L" (+" +
                    to_wstring(lastCelebCount * 10) + L")";
                if (lastPhotographerCount > 0) eventMsg += L", ";
            }
            if (lastPhotographerCount > 0) {
                eventMsg += to_wstring(lastPhotographerCount) + L" фотограф" +
                    (lastPhotographerCount == 1 ? L"" : (lastPhotographerCount < 5 ? L"а" : L"ов")) + L" (+" +
                    to_wstring(lastPhotographerCount * 5) + L")";
            }
            addEvent(eventMsg);
        }

        popularity += bonus;
    }

    // Отображение всех вольеров
    void showAllPens() const {
        wcout << L"\nВольеры:\n";
        for (size_t i = 0; i < pens.size(); ++i) {
            const Pen& pen = pens[i];
            wcout << i + 1 << L". " << pen.getDescription() << endl;
            wcout << L"   Животных: " << pen.getAnimalCount() << L"/" << pen.getCapacity() << endl;
            wcout << L"   Чистота: " << (pen.getIsClean() ? L"Чистый" : L"Грязный") << endl;
            wcout << L"   Содержимое:";
            if (pen.getAnimalCount() == 0) {
                wcout << L" пусто";
            }
            else {
                wcout << endl;
                for (const auto& a : pen.getAnimals()) {
                    wcout << L"   - " << a->getName() << L" (" << a->getSpecies();
                    if (a->getIsHybrid()) {
                        if (a->getParent1()->getSpecies() == a->getParent2()->getSpecies()) {
                            wcout << L" [гибрид]";
                        }
                        else {
                            wcout << L" [гибрид " << a->getParent1()->getSpecies() << L" и " << a->getParent2()->getSpecies() << L"]";
                        }
                    }
                    wcout << L", " << a->getGenderName() << L", " << a->getDescription() << L"), возраст: " << a->getAgeDays() << L" дн., вес: " << a->getWeight() << L" кг";
                    if (a->getIsInfected()) wcout << L" (заражено)";
                    if (a->getIsDying()) wcout << L" (погибает)";
                    if (a->getParent1() && a->getParent2()) {
                        wcout << L" (родители: " << a->getParent1()->getName() << L" и " << a->getParent2()->getName() << L")";
                    }
                    wcout << endl;
                }
            }
            wcout << endl;
        }
    }

    // Переход на следующий день
    void nextDay() {
        if (day >= MAX_DAYS) {
            wcout << L"Поздравляем! Вы успешно завершили игру за " << MAX_DAYS << L" дней!\n";
            exit(0);
        }

        if (!hasDirector()) {
            wcout << L"Вас изгнали из братства, геге\n";
            exit(0);
        }

        animalsBoughtToday = 0;
        processDebt();

        // Увеличение возраста животных
        for (Pen& pen : pens) {
            pen.handleAging();
        }

        // Обновление чистоты вольеров
        for (Pen& pen : pens) {
            pen.updateCleanliness();
        }

        // Обработка болезней
        for (Pen& pen : pens) {
            pen.infectRandomAnimal(day);
            pen.spreadDisease(day);
            pen.handleOutbreak(day);
        }

        // Автоматическое лечение
        autoTreatAnimals();

        // Обработка умирающих животных
        for (Pen& pen : pens) {
            pen.handleDying(MAX_AGE);
        }

        // Кормление животных
        int neededFood = totalAnimals();
        bool wasFed = food >= neededFood;
        if (wasFed) {
            food -= neededFood;
        }
        else {
            addEvent(L"Не хватило еды для всех животных!");
            for (Pen& pen : pens) {
                vector<AnimalPtr> survivors;
                int currentFood = food;
                for (const auto& a : pen.getAnimals()) {
                    if (currentFood > 0) {
                        currentFood--;
                        survivors.push_back(a);
                    }
                    else if (rand() % 2 == 0) {
                        survivors.push_back(a);
                    }
                }
                int deadCount = pen.getAnimalCount() - survivors.size();
                if (deadCount > 0) {
                    addEvent(L"В вольере " + pen.getDescription() + L" умерло " + to_wstring(deadCount) + L" животных из-за голода!");
                }
                pen.getAnimals() = survivors;
            }
            food = 0;
        }

        // Уборка вольеров
        for (auto& worker : workers) {
            if (worker.type == CLEANER) {
                for (Pen& pen : pens) {
                    if (!pen.getIsClean()) {
                        pen.setClean(true);
                        break;
                    }
                }
            }
        }

        // Расчет популярности
        int dirtyPens = 0;
        int totalInfected = 0;
        for (const Pen& pen : pens) {
            if (!pen.getIsClean()) dirtyPens++;
            totalInfected += pen.getInfectedCount();
        }
        popularity = max(popularity - dirtyPens - totalInfected + (rand() % 21 - 10), 0);

        // Выплата зарплат
        double totalSalary = 0;
        for (const Worker& w : workers) totalSalary += w.salary;
        money -= totalSalary;
        addEvent(L"Зарплата работникам: $" + to_wstring(static_cast<int>(totalSalary)));

        // Доход от посетителей
        if (totalAnimals() > 0) {
            int visitors = static_cast<int>(2 * popularity);
            money += visitors * max(totalAnimals(), 1);
            addEvent(L"Доход от посетителей: $" + to_wstring(visitors * max(totalAnimals(), 1)));
        }

        // Проверка на банкротство
        if (money < 0) {
            wcout << L"Проигрыш: деньги закончились!" << endl;
            exit(EXIT_FAILURE);
        }

        // Обновление рынка животных
        if (market.canUpdate(day)) {
            market.generateAnimals(day);
        }

        // Случайные события
        randomEvents();
        showEvents();

        day++;
    }

    // Проверка возможности покупки животного
    bool canBuyAnimal() const {
        return animalsBoughtToday < (day >= 10 ? 1 : INT_MAX);
    }

    // Увеличение счетчика купленных сегодня животных
    void incrementAnimalsBought() {
        animalsBoughtToday++;
    }

    // Ручное скрещивание животных
    bool manualBreeding() {
        if (pens.empty()) {
            wcout << L"Нет вольеров с животными\n";
            return false;
        }

        wcout << L"Выберите вольер для первого животного:\n";
        for (size_t i = 0; i < pens.size(); ++i) {
            wcout << (i + 1) << L". " << pens[i].getDescription() << L" ("
                << pens[i].getAnimalCount() << L" животных)\n";
        }

        int pen1Index;
        while (true) {
            wcout << L"Вольер (1-" << pens.size() << L", 0 для отмены): ";
            if (wcin >> pen1Index && (pen1Index == 0 || (pen1Index >= 1 && pen1Index <= static_cast<int>(pens.size())))) break;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            wcout << L"Ошибка: введите 0 или число от 1 до " << pens.size() << endl;
        }
        wcin.ignore();

        if (pen1Index == 0) return false;

        Pen& pen1 = pens[pen1Index - 1];
        if (pen1.getAnimalCount() == 0) {
            wcout << L"Вольер пуст\n";
            return false;
        }

        wcout << L"Выберите первое животное:\n";
        for (size_t i = 0; i < pen1.getAnimals().size(); ++i) {
            wcout << (i + 1) << L". " << pen1.getAnimals()[i]->getName() << L" ("
                << pen1.getAnimals()[i]->getSpecies() << L", "
                << pen1.getAnimals()[i]->getGenderName() << L")\n";
        }

        int animal1Index;
        while (true) {
            wcout << L"Животное (1-" << pen1.getAnimals().size() << L", 0 для отмены): ";
            if (wcin >> animal1Index && (animal1Index == 0 || (animal1Index >= 1 && animal1Index <= static_cast<int>(pen1.getAnimals().size())))) break;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            wcout << L"Ошибка: введите 0 или число от 1 до " << pen1.getAnimals().size() << endl;
        }
        wcin.ignore();

        if (animal1Index == 0) return false;

        AnimalPtr animal1 = pen1.getAnimals()[animal1Index - 1];

        wcout << L"Выберите вольер для второго животного:\n";
        for (size_t i = 0; i < pens.size(); ++i) {
            wcout << (i + 1) << L". " << pens[i].getDescription() << L" ("
                << pens[i].getAnimalCount() << L" животных)\n";
        }

        int pen2Index;
        while (true) {
            wcout << L"Вольер (1-" << pens.size() << L", 0 для отмены): ";
            if (wcin >> pen2Index && (pen2Index == 0 || (pen2Index >= 1 && pen2Index <= static_cast<int>(pens.size())))) break;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            wcout << L"Ошибка: введите 0 или число от 1 до " << pens.size() << endl;
        }
        wcin.ignore();

        if (pen2Index == 0) return false;

        Pen& pen2 = pens[pen2Index - 1];
        if (pen2.getAnimalCount() == 0) {
            wcout << L"Вольер пуст\n";
            return false;
        }

        wcout << L"Выберите второе животное:\n";
        for (size_t i = 0; i < pen2.getAnimals().size(); ++i) {
            wcout << (i + 1) << L". " << pen2.getAnimals()[i]->getName() << L" ("
                << pen2.getAnimals()[i]->getSpecies() << L", "
                << pen2.getAnimals()[i]->getGenderName() << L")\n";
        }

        int animal2Index;
        while (true) {
            wcout << L"Животное (1-" << pen2.getAnimals().size() << L", 0 для отмены): ";
            if (wcin >> animal2Index && (animal2Index == 0 || (animal2Index >= 1 && animal2Index <= static_cast<int>(pen2.getAnimals().size())))) break;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            wcout << L"Ошибка: введите 0 или число от 1 до " << pen2.getAnimals().size() << endl;
        }
        wcin.ignore();

        if (animal2Index == 0) return false;

        AnimalPtr animal2 = pen2.getAnimals()[animal2Index - 1];

        return pen1.tryManualBreeding(animal1, animal2, *this);
    }

    // Лечение конкретного животного
    bool treatAnimal() {
        if (pens.empty()) {
            wcout << L"Нет животных для лечения\n";
            return false;
        }

        wcout << L"Выберите вольер:\n";
        for (size_t i = 0; i < pens.size(); ++i) {
            wcout << (i + 1) << L". " << pens[i].getDescription() << endl;
        }

        int penIndex;
        while (true) {
            wcout << L"Вольер (1-" << pens.size() << L", 0 для отмены): ";
            if (wcin >> penIndex && (penIndex == 0 || (penIndex >= 1 && penIndex <= static_cast<int>(pens.size())))) break;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            wcout << L"Ошибка: введите 0 или корректный номер\n";
        }
        wcin.ignore();

        if (penIndex == 0) return false;

        Pen& selectedPen = pens[penIndex - 1];
        vector<AnimalPtr>& animals = selectedPen.getAnimals();

        if (animals.empty()) {
            wcout << L"Вольер пуст\n";
            return false;
        }

        wcout << L"Выберите животное:\n";
        for (size_t i = 0; i < animals.size(); ++i) {
            wcout << (i + 1) << L". " << animals[i]->getName() << L" ("
                << animals[i]->getSpecies() << L", " << animals[i]->getGenderName() << L", " << animals[i]->getDescription() << L")"
                << L", возраст: " << animals[i]->getAgeDays() << L" дн., вес: " << animals[i]->getWeight() << L" кг";
            if (animals[i]->getIsInfected()) wcout << L" (заражено)";
            if (animals[i]->getIsDying()) wcout << L" (погибает)";
            wcout << endl;
        }

        int animalIndex;
        while (true) {
            wcout << L"Животное (1-" << animals.size() << L", 0 для отмены): ";
            if (wcin >> animalIndex && (animalIndex == 0 || (animalIndex >= 1 && animalIndex <= static_cast<int>(animals.size())))) break;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            wcout << L"Ошибка: введите 0 или номер животного\n";
        }
        wcin.ignore();

        if (animalIndex == 0) return false;

        AnimalPtr selectedAnimal = animals[animalIndex - 1];

        if (selectedAnimal->getIsInfected()) {
            if (money >= 100) {
                selectedAnimal->setInfected(false, 0);
                selectedAnimal->setDying(false);
                money -= 100;
                wcout << L"Животное вылечено!\n";
                addEvent(L"Вылечено животное: " + selectedAnimal->getName());
                return true;
            }
            else {
                wcout << L"Недостаточно средств\n";
                return false;
            }
        }
        else {
            wcout << L"Животное не заражено\n";
            return false;
        }
    }
};

// Реализация метода ручного скрещивания для класса Pen
bool Pen::tryManualBreeding(AnimalPtr a1, AnimalPtr a2, Zoo& zoo) {
    if (animals.size() >= capacity) {
        wcout << L"Нет места в этом вольере для потомства\n";
        return false;
    }

    auto offspring = Animal::breed(a1, a2);
    if (!offspring) {
        return false;
    }

    vector<Pen*> suitablePens;
    for (auto& pen : zoo.getPens()) {
        if (pen.canAdd(offspring)) {
            suitablePens.push_back(&pen);
        }
    }

    if (suitablePens.empty()) {
        wcout << L"Нет подходящих вольеров для потомства\n";
        return false;
    }

    wcout << L"\nВыберите вольер для потомства:\n";
    for (size_t i = 0; i < suitablePens.size(); ++i) {
        wcout << (i + 1) << L". " << suitablePens[i]->getDescription() << L" ("
            << suitablePens[i]->getAnimalCount() << L"/" << suitablePens[i]->getCapacity() << L")\n";
    }

    int penChoice;
    while (true) {
        wcout << L"Вольер (1-" << suitablePens.size() << L"): ";
        if (wcin >> penChoice) {
            if (penChoice >= 1 && penChoice <= static_cast<int>(suitablePens.size())) break;
        }
        wcin.clear();
        wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
        wcout << L"Ошибка: введите число от 1 до " << suitablePens.size() << endl;
    }
    wcin.ignore();

    suitablePens[penChoice - 1]->addAnimal(offspring);
    wstring eventMsg = offspring->getIsHybrid() ?
        L"Родился новый гибрид: " + offspring->getName() + L" (от " + a1->getName() + L" и " + a2->getName() + L")" :
        L"Родилось новое животное: " + offspring->getName() + L" (от " + a1->getName() + L" и " + a2->getName() + L")";

    wcout << L"🎉 " << eventMsg << endl;
    wcout << L"Помещено в вольер: " << suitablePens[penChoice - 1]->getDescription() << endl;
    zoo.addEvent(eventMsg);

    return true;
}

// Главная функция
int wmain(int argc, wchar_t* argv[]) {
    // Настройка консоли для работы с Unicode
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    srand(time(0));

    // Ввод названия зоопарка
    wstring zooName;
    wcout << L"Введите название зоопарка: ";
    getline(wcin, zooName);

    // Ввод имени директора
    wstring directorName;
    while (true) {
        wcout << L"Введите имя директора: ";
        getline(wcin, directorName);
        if (!directorName.empty()) break;
        wcout << L"Имя не может быть пустым!\n";
    }

    // Создание зоопарка
    Zoo myZoo(zooName, directorName);

    // Главный игровой цикл
    while (true) {
        system("cls");
        // Отображение информации о зоопарке
        wcout << L"=== " << myZoo.getName() << L" ===\n";
        wcout << L"Директор: " << directorName << endl;
        wcout << L"День: " << myZoo.getDay() << L" из " << myZoo.getMaxDays() << endl;
        wcout << L"Деньги: $" << static_cast<int>(myZoo.getMoney()) << endl;
        wcout << L"Еда: " << myZoo.getFood() << endl;
        wcout << L"Популярность: " << myZoo.getPopularity() << endl;
        wcout << L"Животных: " << myZoo.totalAnimals() << endl;

        if (myZoo.getDebt() > 0) {
            wcout << L"Кредит: $" << static_cast<int>(myZoo.getDebt()) << L" (осталось дней: " << myZoo.getDebtDaysLeft() << L")" << endl;
        }

        // Отображение информации о работниках
        wcout << L"Работники:\n";
        int vets = 0, cleaners = 0, feeders = 0, directors = 0;
        for (const auto& w : myZoo.getWorkers()) {
            if (w.type == VET) vets++;
            else if (w.type == CLEANER) cleaners++;
            else if (w.type == FEEDER) feeders++;
            else if (w.type == DIRECTOR) directors++;
        }
        wcout << L"   Ветеринары: " << vets << L" (рекомендуется: " << myZoo.calculateNeededVets() << L") ($50/день)\n";
        wcout << L"   Уборщики: " << cleaners << L" (рекомендуется: " << myZoo.calculateNeededCleaners() << L") ($20/день)\n";
        wcout << L"   Кормильщики: " << feeders << L" (рекомендуется: " << myZoo.calculateNeededFeeders() << L") ($30/день)\n";
        wcout << L"   Директор: " << directors << L" ($500/день)\n";

        // Меню действий
        wcout << L"\nДействия:\n";
        wcout << L"1. Управление животными\n"
            << L"2. Закупки\n"
            << L"3. Постройки\n"
            << L"4. Следующий день\n"
            << L"5. Рынок животных\n"
            << L"6. Скрещивание\n"
            << L"7. Кредит\n"
            << L"8. Лечение\n"
            << L"9. Управление работниками\n"
            << L"0. Выход\n";

        int choice;
        while (true) {
            wcout << L"Выберите действие (0-9): ";
            if (wcin >> choice && 0 <= choice && choice <= 9) break;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
            wcout << L"Ошибка: введите число от 0 до 9\n";
        }
        wcin.ignore();

        if (choice == 0) break;

        // Обработка выбранного действия
        switch (choice) {
        case 1: { // Управление животными
            wcout << L"1. Купить животное с рынка\n"
                << L"2. Продать животное\n"
                << L"3. Просмотр животных\n"
                << L"4. Переименовать животное\n"
                << L"0. Отмена\n";
            int sub;
            while (true) {
                wcout << L"Выберите действие (0-4): ";
                if (wcin >> sub && sub >= 0 && sub <= 4) break;
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                wcout << L"Ошибка: введите число от 0 до 4\n";
            }
            wcin.ignore();

            if (sub == 0) break;

            if (sub == 1) { // Купить животное
                if (!myZoo.canBuyAnimal()) {
                    wcout << L"После 10 дня можно покупать только 1 животное в день\n";
                    break;
                }

                if (myZoo.getPens().empty()) {
                    wcout << L"Нет вольеров для животных\n";
                    break;
                }

                wcout << L"\nДоступные животные на рынке:\n";
                const auto& marketAnimals = myZoo.getMarket().getAnimals();
                for (size_t i = 0; i < marketAnimals.size(); ++i) {
                    wcout << (i + 1) << L". " << marketAnimals[i]->getName() << L" ("
                        << marketAnimals[i]->getSpecies() << L", "
                        << marketAnimals[i]->getGenderName() << L", "
                        << marketAnimals[i]->getDescription() << L", $"
                        << static_cast<int>(marketAnimals[i]->getPrice()) << L")\n";
                }

                int animalChoice;
                while (true) {
                    wcout << L"Животное (1-" << marketAnimals.size() << L", 0 для отмены): ";
                    if (wcin >> animalChoice && (animalChoice == 0 || (animalChoice >= 1 &&
                        animalChoice <= static_cast<int>(marketAnimals.size())))) break;
                    wcin.clear();
                    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                    wcout << L"Ошибка: введите 0 или номер животного\n";
                }
                wcin.ignore();

                if (animalChoice == 0) break;

                Pen* suitablePen = nullptr;
                for (Pen& pen : myZoo.getPens()) {
                    if (pen.canAdd(marketAnimals[animalChoice - 1])) {
                        suitablePen = &pen;
                        break;
                    }
                }

                if (!suitablePen) {
                    wcout << L"Нет подходящего вольера\n";
                    break;
                }

                double money = myZoo.getMoney();
                auto bought = myZoo.getMarket().buyAnimal(animalChoice - 1, money);
                myZoo.setMoney(money);
                if (bought) {
                    suitablePen->addAnimal(bought);
                    myZoo.incrementAnimalsBought();
                    wcout << L"Животное куплено!\n";
                    myZoo.addEvent(L"Куплено новое животное: " + bought->getName());
                }
                else {
                    wcout << L"Не удалось купить животное\n";
                }
            }
            else if (sub == 2) { // Продать животное
                if (myZoo.getPens().empty()) {
                    wcout << L"Нет животных для продажи\n";
                    break;
                }

                wcout << L"Выберите вольер:\n";
                for (size_t i = 0; i < myZoo.getPens().size(); ++i) {
                    wcout << (i + 1) << L". " << myZoo.getPens()[i].getDescription() << L" ("
                        << myZoo.getPens()[i].getAnimalCount() << L")\n";
                }
                int penIndex;
                while (true) {
                    wcout << L"Вольер (1-" << myZoo.getPens().size() << L", 0 для отмены): ";
                    if (wcin >> penIndex && (penIndex == 0 || (penIndex >= 1 &&
                        penIndex <= static_cast<int>(myZoo.getPens().size())))) break;
                }
                wcin.ignore();

                if (penIndex == 0) break;

                Pen& selectedPen = myZoo.getPens()[penIndex - 1];
                vector<AnimalPtr>& animals = selectedPen.getAnimals();

                if (animals.empty()) {
                    wcout << L"Вольер пуст\n";
                    continue;
                }

                wcout << L"Выберите животное:\n";
                for (size_t i = 0; i < animals.size(); ++i) {
                    wcout << (i + 1) << L". " << animals[i]->getName() << L" ("
                        << animals[i]->getSpecies() << L", " << animals[i]->getGenderName() << L", " << animals[i]->getDescription() << L")"
                        << L", возраст: " << animals[i]->getAgeDays() << L" дн., вес: " << animals[i]->getWeight() << L" кг";
                    wcout << endl;
                }
                int animalIndex;
                while (true) {
                    wcout << L"Животное (1-" << animals.size() << L", 0 для отмены): ";
                    if (wcin >> animalIndex && (animalIndex == 0 || (animalIndex >= 1 &&
                        animalIndex <= static_cast<int>(animals.size())))) break;
                    wcin.clear();
                    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                    wcout << L"Ошибка: введите 0 или номер животного\n";
                }
                wcin.ignore();

                if (animalIndex == 0) break;

                AnimalPtr soldAnimal = animals[animalIndex - 1];
                animals.erase(animals.begin() + animalIndex - 1);
                myZoo.setMoney(myZoo.getMoney() + soldAnimal->getPrice());
                wcout << L"Продано за $" << static_cast<int>(soldAnimal->getPrice()) << endl;
                myZoo.addEvent(L"Продано животное: " + soldAnimal->getName());
            }
            else if (sub == 3) { // Просмотр
                myZoo.showAllPens();
            }
            else if (sub == 4) { // Переименовать
                if (myZoo.getPens().empty()) {
                    wcout << L"Нет животных для переименования\n";
                    break;
                }

                wcout << L"Выберите вольер:\n";
                for (size_t i = 0; i < myZoo.getPens().size(); ++i) {
                    wcout << (i + 1) << L". " << myZoo.getPens()[i].getDescription() << L" ("
                        << myZoo.getPens()[i].getAnimalCount() << L")\n";
                }
                int penIndex;
                while (true) {
                    wcout << L"Вольер (1-" << myZoo.getPens().size() << L", 0 для отмены): ";
                    if (wcin >> penIndex && (penIndex == 0 || (penIndex >= 1 &&
                        penIndex <= static_cast<int>(myZoo.getPens().size())))) break;
                    wcin.clear();
                    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                    wcout << L"Ошибка: введите 0 или корректный номер\n";
                }
                wcin.ignore();

                if (penIndex == 0) break;

                Pen& selectedPen = myZoo.getPens()[penIndex - 1];
                vector<AnimalPtr>& animals = selectedPen.getAnimals();

                if (animals.empty()) {
                    wcout << L"Вольер пуст\n";
                    continue;
                }

                wcout << L"Выберите животное:\n";
                for (size_t i = 0; i < animals.size(); ++i) {
                    wcout << (i + 1) << L". " << animals[i]->getName() << L" ("
                        << animals[i]->getSpecies() << L", " << animals[i]->getGenderName() << L", " << animals[i]->getDescription() << L")"
                        << L", возраст: " << animals[i]->getAgeDays() << L" дн., вес: " << animals[i]->getWeight() << L" кг";
                    wcout << endl;
                }
                int animalIndex;
                while (true) {
                    wcout << L"Животное (1-" << animals.size() << L", 0 для отмены): ";
                    if (wcin >> animalIndex && (animalIndex == 0 || (animalIndex >= 1 &&
                        animalIndex <= static_cast<int>(animals.size())))) break;
                    wcin.clear();
                    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                    wcout << L"Ошибка: введите 0 или номер животного\n";
                }
                wcin.ignore();

                if (animalIndex == 0) break;

                wstring newName;
                wcout << L"Введите новое имя: ";
                getline(wcin, newName);
                animals[animalIndex - 1]->setName(newName);
                wcout << L"Имя изменено!\n";
                myZoo.addEvent(L"Переименовано животное: " + newName);
            }
            break;
        }
        case 2: { // Закупки
            wcout << L"1. Купить еду\n"
                << L"2. Реклама\n"
                << L"0. Отмена\n";
            int sub;
            while (true) {
                wcout << L"Действие (0-2): ";
                if (wcin >> sub && sub >= 0 && sub <= 2) break;
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                wcout << L"Ошибка: введите 0, 1 или 2\n";
            }
            wcin.ignore();

            if (sub == 0) break;

            if (sub == 1) { // Купить еду
                int amount;
                while (true) {
                    wcout << L"Сколько еды купить? (1 еда = $1, 0 для отмены): ";
                    if (wcin >> amount && (amount == 0 || amount > 0)) break;
                    wcin.clear();
                    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                    wcout << L"Ошибка: введите 0 или положительное число\n";
                }
                wcin.ignore();

                if (amount == 0) break;

                if (myZoo.getMoney() >= amount) {
                    myZoo.setFood(myZoo.getFood() + amount);
                    myZoo.setMoney(myZoo.getMoney() - amount);
                    myZoo.addEvent(L"Куплено " + to_wstring(amount) + L" еды");
                }
                else {
                    wcout << L"Недостаточно средств\n";
                }
            }
            else if (sub == 2) { // Реклама
                int cost;
                while (true) {
                    wcout << L"Сколько потратить? (1$ = +1 популярность, 0 для отмены): ";
                    if (wcin >> cost && (cost == 0 || cost > 0)) break;
                    wcin.clear();
                    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                    wcout << L"Ошибка: введите 0 или положительное число\n";
                }
                wcin.ignore();

                if (cost == 0) break;

                if (cost <= myZoo.getMoney()) {
                    myZoo.setMoney(myZoo.getMoney() - cost);
                    myZoo.setPopularity(myZoo.getPopularity() + cost);
                    myZoo.addEvent(L"Потрачено $" + to_wstring(cost) + L" на рекламу");
                }
                else {
                    wcout << L"Недостаточно средств\n";
                }
            }
            break;
        }
        case 3: { // Постройки
            wcout << L"1. Построить вольер\n"
                << L"2. Просмотр вольеров\n"
                << L"3. Уничтожить вольер\n"
                << L"0. Отмена\n";
            int sub;
            while (true) {
                wcout << L"Действие (0-3): ";
                if (wcin >> sub && sub >= 0 && sub <= 3) break;
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                wcout << L"Ошибка: введите 0, 1, 2 или 3\n";
            }
            wcin.ignore();

            if (sub == 0) break;

            if (sub == 1) { // Построить вольер
                wcout << L"1. Травоядные\n"
                    << L"2. Хищники\n"
                    << L"0. Отмена\nТип: ";
                int type;
                while (true) {
                    if (wcin >> type && (type == 0 || type == 1 || type == 2)) break;
                    wcin.clear();
                    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                    wcout << L"Ошибка: введите 0, 1 или 2\n";
                }
                wcin.ignore();

                if (type == 0) break;

                AnimalType at = (type == 1) ? HERBIVORE : CARNIVORE;

                wcout << L"\nКлимат:\n";
                for (size_t i = 0; i < CLIMATES.size(); ++i) {
                    wcout << (i + 1) << L". " << CLIMATES[i] << endl;
                }
                int climateChoice;
                while (true) {
                    wcout << L"Климат (1-" << CLIMATES.size() << L", 0 для отмены): ";
                    if (wcin >> climateChoice && (climateChoice == 0 || (climateChoice >= 1 && climateChoice <= static_cast<int>(CLIMATES.size())))) break;
                    wcin.clear();
                    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                    wcout << L"Ошибка: введите 0 или число от 1 до " << CLIMATES.size() << endl;
                }
                wcin.ignore();

                if (climateChoice == 0) break;

                Climate cl = static_cast<Climate>(climateChoice - 1);

                int cap;
                while (true) {
                    wcout << L"Вместимость (1-100, 0 для отмены): ";
                    if (wcin >> cap && (cap == 0 || (cap >= 1 && cap <= 100))) break;
                    wcin.clear();
                    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                    wcout << L"Ошибка: введите 0 или число от 1 до 100\n";
                }
                wcin.ignore();

                if (cap == 0) break;

                int cost = cap * 10;
                if (myZoo.getMoney() >= cost) {
                    myZoo.setMoney(myZoo.getMoney() - cost);
                    myZoo.getPens().emplace_back(cap, at, cl);
                    wcout << L"Вольер построен за $" << cost << L"!\n";
                    myZoo.addEvent(L"Построен новый вольер: " +
                        (at == HERBIVORE ? wstring(L"Травоядные") : wstring(L"Хищники")) +
                        L" (" + CLIMATES[climateChoice - 1] + L")");
                }
                else {
                    wcout << L"Недостаточно средств\n";
                }
            }
            else if (sub == 2) { // Просмотр вольеров
                myZoo.showAllPens();
            }
            else if (sub == 3) { // Уничтожить вольер
                if (myZoo.getPens().empty()) {
                    wcout << L"Нет вольеров для уничтожения\n";
                    break;
                }

                wcout << L"Выберите вольер для уничтожения:\n";
                for (size_t i = 0; i < myZoo.getPens().size(); ++i) {
                    wcout << (i + 1) << L". " << myZoo.getPens()[i].getDescription() << L" ("
                        << myZoo.getPens()[i].getAnimalCount() << L" животных)\n";
                }

                int penIndex;
                while (true) {
                    wcout << L"Вольер (1-" << myZoo.getPens().size() << L", 0 для отмены): ";
                    if (wcin >> penIndex) {
                        if (penIndex == 0 || (penIndex >= 1 && penIndex <= static_cast<int>(myZoo.getPens().size()))) break;
                    }
                    wcin.clear();
                    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                    wcout << L"Ошибка: введите 0 или номер вольера\n";
                }
                wcin.ignore();

                if (penIndex == 0) break;

                myZoo.destroyPen(penIndex - 1);
            }
            break;
        }
        case 4: // Следующий день
            myZoo.nextDay();
            break;
        case 5: { // Рынок животных
            wcout << L"1. Просмотр рынка животных\n"
                << L"2. Обновить рынок животных ($200)\n"
                << L"0. Отмена\n";
            int sub;
            while (true) {
                wcout << L"Действие (0-2): ";
                if (wcin >> sub && sub >= 0 && sub <= 2) break;
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                wcout << L"Ошибка: введите 0, 1 или 2\n";
            }
            wcin.ignore();

            if (sub == 0) break;

            if (sub == 1) { // Просмотр рынка животных
                wcout << L"\nДоступные животные на рынке:\n";
                const auto& marketAnimals = myZoo.getMarket().getAnimals();
                for (size_t i = 0; i < marketAnimals.size(); ++i) {
                    wcout << (i + 1) << L". " << marketAnimals[i]->getName() << L" ("
                        << marketAnimals[i]->getSpecies() << L", "
                        << marketAnimals[i]->getGenderName() << L", "
                        << marketAnimals[i]->getDescription() << L", $"
                        << static_cast<int>(marketAnimals[i]->getPrice()) << L")\n";
                }
            }
            else if (sub == 2) { // Обновить рынок животных
                double money = myZoo.getMoney();
                if (myZoo.getMarket().update(myZoo.getDay(), money)) {
                    myZoo.setMoney(money);
                    wcout << L"Рынок животных обновлен!\n";
                    myZoo.addEvent(L"Рынок животных обновлен");
                }
                else {
                    wcout << L"Не удалось обновить рынок (недостаточно средств или еще не время)\n";
                }
            }
            break;
        }
        case 6: { // Скрещивание
            myZoo.manualBreeding();
            break;
        }
        case 7: { // Кредит
            if (myZoo.getDebt() > 0) {
                wcout << L"У вас уже есть непогашенный кредит\n";
                break;
            }

            int amount;
            int days;
            wcout << L"Сумма кредита: ";
            wcin >> amount;
            wcout << L"Срок (дни): ";
            wcin >> days;
            if (amount > 0 && days > 0) {
                myZoo.takeLoan(amount, days);
            }
            break;
        }
        case 8: { // Лечение
            wcout << L"1. Лечить животное ($100)\n"
                << L"2. Лечить всех животных\n"
                << L"0. Отмена\n";
            int sub;
            while (true) {
                wcout << L"Действие (0-2): ";
                if (wcin >> sub && sub >= 0 && sub <= 2) break;
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                wcout << L"Ошибка: введите 0, 1 или 2\n";
            }
            wcin.ignore();

            if (sub == 0) break;

            if (sub == 1) { // Лечить животное
                myZoo.treatAnimal();
            }
            else if (sub == 2) { // Лечить всех животных
                myZoo.treatAllAnimals();
            }
            break;
        }
        case 9: { // Управление работниками
            wcout << L"1. Нанять работника\n"
                << L"2. Переименовать работника\n"
                << L"3. Уволить работника\n"
                << L"0. Отмена\n";
            int sub;
            while (true) {
                wcout << L"Выберите действие (0-3): ";
                if (wcin >> sub && sub >= 0 && sub <= 3) break;
                wcin.clear();
                wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                wcout << L"Ошибка: введите число от 0 до 3\n";
            }
            wcin.ignore();

            if (sub == 0) break;

            if (sub == 1) { // Нанять работника
                myZoo.hireWorker();
            }
            else if (sub == 2) { // Переименовать работника
                myZoo.renameWorker();
            }
            else if (sub == 3) { // Уволить работника
                if (myZoo.getWorkers().empty()) {
                    wcout << L"Нет работников для увольнения\n";
                    break;
                }

                wcout << L"Выберите работника для увольнения:\n";
                for (size_t i = 0; i < myZoo.getWorkers().size(); ++i) {
                    wcout << (i + 1) << L". " << myZoo.getWorkers()[i].name << L" ("
                        << myZoo.getWorkers()[i].getTypeName() << L")\n";
                }

                int workerIndex;
                while (true) {
                    wcout << L"Работник (1-" << myZoo.getWorkers().size() << L", 0 для отмены): ";
                    if (wcin >> workerIndex) {
                        if (workerIndex == 0 || (workerIndex >= 1 && workerIndex <= static_cast<int>(myZoo.getWorkers().size()))) break;
                    }
                    wcin.clear();
                    wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
                    wcout << L"Ошибка: введите 0 или номер работника\n";
                }
                wcin.ignore();

                if (workerIndex == 0) break;

                myZoo.removeWorker(workerIndex - 1);
                wcout << L"Работник уволен!\n";
            }
            break;
        }
        }

        system("pause");
    }

    return 0;
}
