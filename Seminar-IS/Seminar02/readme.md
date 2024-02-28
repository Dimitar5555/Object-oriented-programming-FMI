# Семинар 02
*Резюме: Въвеждаме понятието enum. Въвеждаме понятието поток. Разглеждаме форматиран и неформатиран вход. Запознаваме се с част от интерфейса на потоците и разглеждаме как да ги използваме за да четем и пишем във файлове.*

## Enum
Структурите, в някакъв смисъл, ни помагат да създаваме наши типове данни. Доста често практиката изисква нашите типове данни да се класифицират по някакъв начин. Неформално казано, ако създаваме тип, който описва студент, можем да се интересуваме от това в кой курс е. Или ако създаваме тип, който описва лаптоп, може да се интересуваме какъв модел е. Общото между тези два примера е, че имаме краен брой характеристики, които описват нашия тип. 

```cpp
struct Student {
    char name[16];
    unsigned age;
    unsigned course; // Values of 1, 2, 3, 4
};
```

Това би бил наивния начин да пазим информацията в кой курс е студента. Този начин работи, но не е особено удобен. Гледайки само типа `unsigned` не можем да разберем какви биха били стойностите които може да приема. 
Езикът С++ ни дава начин да създадем собствен тип, който има за цел да служи като описаната класификация.

```cpp
enum class Course {
    First,
    Second, 
    Third,
    Fourth
};
```

Пренаписвайки структурата би изглеждала така
```cpp
struct Student {
    char name[16];
    unsigned age;
    Course course;
};
```

Първо, какво е `Course`??? Той е тип, който има краен брой стойности, описани от нас. Можем да го подаваме като част от структурата (както подаваме примитивните типове). Как да му дадем стойност?

```cpp
int main()
{
    Student myStudent;
    myStudent.course = Course::Second;
}
```
Трябва да запомните, че стойностите на enum class се достъпват през `EnumClassName::Value`. Това е за добро - така кодът става в пъти по-четим и ясен.

След като веднъж сме дали стойност можем да използваме условни конструкции за да вземаме решения базирани на стойността:
```cpp
int main()
{
    Student myStudent;
    myStudent.course = Course::Second;

    if(myStudent.course == Course::First)
    {
        std::cout << ":(";
    }

    // We can switch too!
    switch (myStudent.course)
	{
	case Course::First:

		break;
	case Course::Second:

		break;
	case Course::Third:

		break;
	case Course::Fourth:

		break;
	default:
		break;
	}
}
```

## Потоци
Поток в С++ е последователност от информация (символи) която влиза или излиза от нашата програма.
Използват се за осъществяване на комуникация между програмата и някое входно изходно устройство.
Пример за обекти използващи потоци са **std::cin** и **std::cout**. Те осъществяват комуникацията между програмата и стандартния вход/изход.

## Форматиран и неформатиран вход и изход
Споменахме, че потоците са последователност от байтове. Но доста често виждаме нещо подобно:
```cpp
int x = 0;
std::cin >> x;
```

Въвеждайки 42 в конзолата това реално са два символа 4 и 2. Ако въведем "42 43 aaa 44" x ще стане 42 а останалите символи ще останат в потока. Това е защото използваме оператора >>. Този оператор се използва за **форматиран вход** - вход, който форматира данните в очакван формат.

Това е част от удобствата на езика - не сме длъжни да обикаляме потока байт по байт и да създаваме числото. Форматирания вход обаче (както знаем от курса по УП) пропуска някои символи.

```cpp
#include <iostream>

int main()
{
    int x = 0;
    int x1 = 1;
    std::cin >> x >> x1;
    std::cout << x << " " << x1;
}
```

Сега, при вход:
```
1                                       3
```
променливите х и х1 ще имат стойности 1 и 3. Можем да използваме функциите `.getline(char*, size_t)` и `get()` за да вземем неформатиран вход от потока.

```cpp
#include <iostream>

int main()
{
    char buff[25];
    std::cin.getline(buff, 25);
    std::cout << buff;

    char ch = std::cin.get(); // Полезна функция - взема един символ от стандартния вход. Връща го като резултат.
}
```


```cpp
#include<iostream>

int main() {
    int x = 0;
    char data[8];
    
    std::cout << "Enter x: ";
    std::cin >> x;
    std::cout << std::endl << "Enter data: ";
    std::cin.getline(data, 8, '\n'); // взема първите 8 символа или докато или докато не види нов ред.

    std::cout << x << std::endl << data;
}
```
Нека да стартираме тази програма и да проверим какво точно ще се случи:

![](media/example-io.bmp)

Виждаме, че нашата програма не работи точно по начинът по който очаквахме, но защо? Цялата информация (в случая последователност от символи) се движи точно като поток от нашата конзола към нашата програма.

Как изглежда потокът в случая:
```
8 \n
```
Този нов ред се появява понеже сме натиснали бутонът Enter. Реално, това което се случва е, че x става 8 и заради символът за нов ред не ни се предоставя възможност да въведем data. Логичното нещо, което можем да направим сега е да се отървем от символа за нов ред. Това можем да направим с `std::cin.get()`, който ще вземе следващия символ в потока.

```cpp
#include<iostream>

int main() {
    int x = 0;
    char data[8];
    
    std::cout << "Enter x: ";
    std::cin >> x;
    std::cin.get();

    std::cout << std::endl << "Enter data: ";
    std::cin.getline(data, 8, '\n');

    std::cout << x << std::endl << data;
}
```

## Работа с файлове.
Споменахме, че чрез потоците извършваме входно/изходна комуникация между програмата ни и някакво входно/изходно устройство. Стандартния вход и изход бяха добри примери за такива устройства. Все пак обаче, те не са единствени. Езикът С++ ни позволява гъвкаво да работим с файлове. Всеки файл е последователност от байтове. Можем да използваме потоци за да четем и пишем в тях.
Разбира се, вече не използваме `std::cin` и `std::cout`, но потоците, които ще използваме, споделят доста общи неща с тях.

Библиотеката която използваме за работа с файлови потоци е `<fstream>`.
Обектите, които използваме, за да работим с файлове са: 

* <b> std::ifstream - поток за вход от файл </b>
* <b> std::ofstream - поток за изход към файл </b>

Ще забележите, че интерфейсът на тези потоци е доста подобен на този на std::cin и std::cout. Примерно, нека се опитаме да запишем нещо във файл:
```cpp
#include<fstream>

int main() {
    // Отваряме поток за писане във файл. Ако във файла има информация тя се затрива.
    std::ofstream outFile("test-file.txt");

    // Разбира се, всичко се случва, трябва да проверим дали файлът е отворен успешно
    if(!outFile.is_open()) 
        return 1;
    
    // Пишем по същия начин както при cout
    outFile << "Hello files";

    // Можем също така да добавим един символ в края.
    outFile.put("!");

    // Затваряме файла
    outFile.close();
}
```

Интерфейсът за четене от файл също наподобява този на cin. Все пак имаме добре работеща логика за четене/писане от конзола. Защо да не я преизползваме и за файлове?

```cpp
#include<fstream>

int main() {
    std::ifstream inFile("test-file.txt");

    if(!inFile.is_open())
        return 1;

    char buff[64];

    inFile.getline(buff, 64, '\n');

    std::cout << "First line of our file is: << buff;

    char ch = inFile.get(); // Вземи следващия символ от файла.

    inFile.close();
}
```

## Нужно за задачите
Когато се опитаме да четем от поток след като сме прочели всичко от него стигаме до края. За да проверим дали сме стигнали до края или не можем да извикаме функцията `eof()` на потока за четене. Примери можете да видите в решенията.

## Задача първа
Напишете програма, която отпечатва собствения си код.

## Задача втора
Да се напише програма, която приема път към файл и създава негово копие.

-- Задачата може да стане по редове или символ по символ.

## Задача трета
Да се напише програма, която прочита файл и записва в нов файл думите му разделени със запетайка. Думите във файла са с максимална дължина от 64 символа.

Примерно съдържание на файла:
This      is         example.


Примерен изход:
This,is,example.