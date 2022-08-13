#include <iostream> //библиотека для вывода текста в консоль
#include <string> // библиотека для работы со строками
#include <fstream> // библиотека для работы с файлом
#include <vector> // библиотека для инициализации массива типа vector
#include <unistd.h> // библиотека для задержки перед перезагрузкой программы
#include <algorithm> // библиотека для нахождения индекста элемента и выполнения действий по данному идексу 

#define LINE20 "--------------------" // макросы для ввода и вывода конечного автомата  
#define INPUT  "<< INPUT >> "
#define OUTPUT "<< OUTPUT >> "              

using namespace std; // стандартное пространство имен для упрощения написания типов данных

/*________________ Разработать конечный автомат, управляющий работой системы резервирования номеров в мини гостинице.__________ */

int main(){ // инициализация функции main
    printf("\e[8;%d;%d;t", 30, 120); // вывод консоли с размерами 30х120
    setlocale(LC_CTYPE, "rus"); // поддрежка Русского алфавита
    string header[5] = {"Тип номера", "Стоимость(руб./день)", "Состояние", "Фамилия постояльца", "Номер"}; // строковый массив для вывода шапки таблицы

    enum command_code {BOOKING, DELETE, ADD, EXIT, EVICTION, HELP, REBOOT, INPUT_COMMAND}; // перечисление доступных команд

    vector <string> TYPE; // массив, который хранит типы гостиничных номеров
    vector <int> PRICE; // массив, который хранит цены на жилье 
    vector <string> STATUS; // массив, который хранит состояния номеров
    vector <string> SURNAME; // массив, который хранит фамилии постояльцев
    vector <int> NUMBER; // массив, который хранит порядковые номера доступных апартаментов 
    
    int command; // переменная для присвоения одной из команд 
    int number_room; // переменная для присвоения введенного номера апартаментов 
    int invested_money; // переменная для присвоения введенной цены за номер

    string line; // переменная для хранения введенной команды 
    string buffer; // переменная для хранения элементов таблицы из файла
    int counter_table = 0; // переменная для хранения счетчика, что бы двигаться и записывать элементы в массивы
    system("clear"); // очистка консоли перед началом работы с программой
    for (const auto & el: header){ // вывод шапки массива для удобного ориентирования 
        cout << left << el << "\t\t";
    }
    cout << endl;

    ifstream file("table"); // открытие файла в режиме чтения 

    while (getline (file, line)) { // чтение файла построчно 
        for (int i = 0; i < line.length(); i++) { // чтение строки посимвольно 
            if(line[i] != '\t'){  // добавление в буфер символов строки, если символ не является табуляцией
                buffer += line[i];   
            } else if (counter_table == 0) { // проверка счетчика
                TYPE.push_back(buffer);  // запись в массив нужного слова    
                cout << buffer << "\t\t\t"; // вывод слова
                buffer = ""; counter_table = 1; // обнуление буфера и прибавление единицы к счетчику
            } else if (counter_table == 1) {
                PRICE.push_back(atoi(buffer.c_str()));
                cout << buffer << "\t\t\t\t";
                buffer = ""; counter_table = 2;
            } else if (counter_table == 2) {
                STATUS.push_back(buffer);
                cout << buffer << "\t\t";
                buffer = ""; counter_table = 3;
            }else if (counter_table == 3) {
                SURNAME.push_back(buffer);
                cout << buffer << "\t\t";
                buffer = ""; counter_table = 4;
		    }else if (counter_table == 4) {
                NUMBER.push_back(atoi(buffer.c_str()));
                cout << buffer << endl;
                buffer = ""; counter_table = 0; // обнуление счетчика для новой итерации цикла
	        }
        }
    }
    cout << '\n';
    file.close(); // закрытие файла

    cout << "START:" << endl; // вывод макросов
    cout << LINE20 << endl;

    command = INPUT_COMMAND; // присвоение начального состояния 
    while(true){ // начало цикла
        if (command == INPUT_COMMAND){ // проверка, находится ли программа в начальном состоянии
            cout << OUTPUT << "Для ознакомелнения с командами введите HELP" << endl;
            cout << INPUT; cin >> line; // ввод пользователем команды
        }
        else if (command == REBOOT){ // проверка, присвоено ли команде состояние "перезагрузка"
            cout << OUTPUT << "Обновление данных. Подождите..." << endl; 
            usleep(3000000); // задержка обновления данных для реалистичности 
            system ( "clear" );  // очистка консоли 
            line=""; cin.clear(); cin.ignore(256,'\n'); // очистка пользовательского ввода

            for (const auto & el: header){ // вывод шапки массива для удобного ориентирования 
                cout << left << el << "\t\t";
            }
            cout << endl;
            for (int i = 0; i < TYPE.size(); i++){ // вывод содержимого массивов
                cout << left << TYPE[i] << "\t\t\t";
                cout << left << PRICE[i] << "\t\t\t\t";
                cout << left << STATUS[i] << "\t\t";
                cout << left << SURNAME[i] << "\t\t";
                cout << right << NUMBER[i] << "\t" << endl;
            }
            cout << endl;

            cout << "START:" << endl; // вывод макроса
            cout << LINE20 << endl;
            command = INPUT_COMMAND; // присвоение команде начального состояния         
        }
        if(line == "HELP") { // проверка введена ли команда "помощь"
            command = HELP; 
            cout << OUTPUT << endl // вывод справки по пользованию командой
                           << "Список команд: BOOKING, DELETE, ADD, EXIT, EVICTION, HELP" << endl
                           << "BOOKING - бронирование номера" << endl
                           << "DELETE - удаление доступного номера" << endl
                           << "ADD - добавление доступного номера" << endl
                           << "EVICTION - выселение жильца из бронированного номера" << endl
                           << "HELP - вызов справки о доступных командах" << endl
                           << "EXIT - выход из программы" << endl;
            command = INPUT_COMMAND; // присвоение команде начального состояния
        } 
        else if (line == "BOOKING"){ // проверка введена ли команда "бронирование"
            command = BOOKING;
            bool flag = true; // создание переменной флаг, хранящей правду или ложь 
            while(true){ // начало цикла
                if (flag == false){ // проверка является ли флаг ложью
                    cout << OUTPUT << "Введенные данные были неверны, убедитесь в правильности ввода данных и повторите попытку." << endl; // вывод подсказки пользователю 
                    cin.clear(); cin.ignore(32767, '\n');
                    flag = true; // измение флана на ПРАВДА для дальнейшей работы
                }
                cout << OUTPUT << "Введите номер, который вы хотите занять." << endl;
                cout << INPUT; cin >> number_room; // ввод пользователем номера апартаментов
                if (cin.fail()) { flag = false; continue;} // если данные введены неверно переход к началу цикла
                auto result = find(begin(NUMBER), end(NUMBER), number_room); // возвращение итератора, находящегося в диапазоне от начала до конца массива NUMBER

                if (result == end(NUMBER)){ // если такого итератора нет, переход к следующей итерации 
                    cout << OUTPUT << "Такого номера в гостинице нет. Повторите попытку." << endl;
                    cin.clear(); //очистка ошибочного ввода пользователя
                    continue;
                }
                if (STATUS[result - NUMBER.begin()] == "Занят   "){ // если данных номер уже кем-то занят, переход к следующей итерации
                    cout << OUTPUT << "Данный номер занят. Пожалуйста, выберите другой номер." << endl;
                    continue;
                }
                cout << OUTPUT << "Для бронирования номера внесите " << PRICE[result - NUMBER.begin()] << " руб." << endl; // вывод нужной цены для оплаты пользователем 
                cout << INPUT; cin >> invested_money; // ввод оплаты
                if (cin.fail()){ flag = false; continue;} // если данные введены неверно переход к началу цикла
                if (invested_money >= PRICE[result - NUMBER.begin()]){ // проверка равна ли оплата нужной цене
                    cout << OUTPUT << "Оплата прошла. Вот ваша сдача: " << invested_money - PRICE[result - NUMBER.begin()] << " руб." << endl; // вывод сдачи пользователю
                    cout << OUTPUT << "Введите свою фамилию для закрепления номера."<< endl; 
                    cout << INPUT; cin >> line; // ввод фамилии для резервирования номера
                    for (int i = 0; i < (sizeof(header[3]) - line.length()); i++){ // для удобного выравнивания таблицы добавляются нужные пробелы к введенной фамилии
                        line += " ";
                    }
                    SURNAME[result - NUMBER.begin()] = line; // присвоение фамилии вместо черточек
                    cout << OUTPUT << "Вы успешно забронировали " << NUMBER[result - NUMBER.begin()] << " номер." << endl;
                    STATUS[result - NUMBER.begin()] = "Занят   "; // изменение статуса номера
                    break; // выход из цикла
                }
                else{ // если оплата оказалась меньше нужной цены переход к следующей итерации
                    cout << OUTPUT << "Введенной вами суммы недостаточно. Попробуйте забронировать номер дешевле." << endl;
                    continue; 
                }
            }
            command = REBOOT; // присовение команде состояния "перезагрузка"
        }
        else if (line == "EVICTION"){ // проверка введена ли команда "выселение"
            command = EVICTION;
            while(true){ //начало цикла
                cout << OUTPUT << "Введите номер, из которого хотите выселится." << endl; 
                cout << INPUT; cin >> number_room; // ввод нужного номера апартаментов
                if (cin.fail()) { // если данные введены неверно, переход к следующей итерации
                    cout << OUTPUT << "Введенные данные были неверны, убедитесь в правильности ввода данных и повторите попытку." << endl;
                    cin.clear();
                    continue;
                }
                auto result = find(begin(NUMBER), end(NUMBER), number_room); // возвращение итератора, находящегося в диапазоне от начала до конца массива NUMBER
                if (result == end(NUMBER)){ // если такого итератора нет, переход к следующей итерации 
                    cout << OUTPUT << "Такого номера в гостинице нет. Повторите попытку." << endl;
                    cin.clear(); 
                    continue;
                }
                if (STATUS[result - NUMBER.begin()] == "Свободен" ){ // если номер находится в состоянии "свободен" переход к следующей итерации
                    cout << OUTPUT << "Данный номер не занят, выберите другой." << endl;
                    continue;
                }
                SURNAME[result - NUMBER.begin()] = "------------------"; // удаление фамилии постояльца
                STATUS[result - NUMBER.begin()] = "Свободен"; // стастус номера теперь "свободен"
                cout << OUTPUT << "Жилец успешно выселен." << endl;
                break; // выход из цикла
            }
            command = REBOOT; // присовение команде состояния "перезагрузка"

        }
        else if (line == "DELETE"){ // проверка введена ли команда "удаление"
            command = DELETE; 
            while(true){ // начало цикла
                cout << OUTPUT << "Введите номер гостиницы, которую хотите удалить." << endl;
                cout << INPUT; cin >> number_room; // ввод нужного номера апартаментов
                if (cin.fail()) { // если данные введены неверно, переход к следующей итерации
                    cout << OUTPUT << "Введенные данные были неверны, убедитесь в правильности ввода данных и повторите попытку." << endl;
                    cin.clear();
                    continue;
                }
                auto result = std::find(NUMBER.begin(), NUMBER.end(), number_room); // возвращение итератора, находящегося в диапазоне от начала до конца массива NUMBER
                if (result == end(NUMBER)){
                    cout << OUTPUT << "Такого номера в гостинице нет. Повторите попытку." << endl;
                    cin.clear(); 
                    continue;
                }
                int index = std::distance(NUMBER.begin(), result); // запись колличества элементов от начала цикла до нужного итератора в переменную index
                TYPE.erase(TYPE.begin() + index); // удаление элементов из каждого массива равного индексу
                PRICE.erase(PRICE.begin() + index);
                STATUS.erase(STATUS.begin() + index);
                SURNAME.erase(SURNAME.begin() + index);
                NUMBER.erase(NUMBER.begin() + index);
                cout << OUTPUT << "Данный номер успешно удален." << endl;
                break; // выход из цикла      
            }
            command = REBOOT; // присовение команде состояния "перезагрузка"
        }
        else if (line == "ADD"){ // проверка введена ли команда "добавление"
            command = ADD;
            bool flag = true; // создание переменной флаг, хранящей правду или ложь
            while(true){
                string type_num; // создание переменной типа номера 
                invested_money = 0;
                number_room = 0;
                if (flag == false){ // проверка является ли флаг ложью
                    cout << OUTPUT << "Введенные данные были неверны, убедитесь в правильности ввода данных и повторите попытку." << endl;
                    cin.clear(); cin.ignore(32767, '\n');
                    flag = true;
                }
                cout << OUTPUT << "Введите тип нового гостиничного номера." << endl;
                cout << INPUT; cin >> type_num; // ввод пользователем типа нового номера
                if (cin.fail()) {flag = false; continue;} // если данные введены неверно переход к началу цикла

            
                cout << OUTPUT << "Введите цену за жилье (руб / день)." << endl;
                cout << INPUT; cin >> invested_money; // ввод цены для нового номера
                if (cin.fail()) {flag = false; continue;} // если данные введены неверно переход к началу цикла

                cout << OUTPUT << "Введите новый гостиничный номер." << endl;
                cout << INPUT; cin >> number_room; // ввод нового порядкового номера апартаментов
                if (cin.fail()) {flag = false; continue;} // если данные введены неверно переход к началу цикла

                TYPE.push_back(type_num); // добавление в массивы новых элементов
                PRICE.push_back(invested_money);
                STATUS.push_back("Свободен");
                SURNAME.push_back("------------------");
                NUMBER.push_back(number_room);

                cout << OUTPUT << "Новый гостиничный номер добавлен." << endl;
                break; // выход из цикла
            }
            command = REBOOT; // присовение команде состояния "перезагрузка"
        }
        else if(line == "EXIT") { // проверка введена ли команда "выход"
            command = EXIT;
            ofstream file("table"); // открытие файла в режиме записи 
            for (int i = 0; i < TYPE.size(); i++){ // запись массивов данных в файл
                file << left << TYPE[i] << "\t";
                file << left << PRICE[i] << "\t";
                file << left << STATUS[i] << "\t";
                file << left << SURNAME[i] << "\t";
                file << right << NUMBER[i] << "\t" << endl;
            }
            file.close(); // закрытие файла 
            break; //выход из цикла
        }
    }
    cout << LINE20 << endl; // вывод макроса 
    cout << "STOP:" << endl;
    return 0;
}
