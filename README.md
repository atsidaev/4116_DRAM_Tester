# Тестер микросхем DRAM 4116

![Фото платы](https://github.com/atsidaev/4116_dram_tester/blob/master/doc/board.jpg)

For English description scroll below.

Минималистичный тестер динамической памяти 4116 для быстрой проверка чипов без сложной сборки. Без реле, защитных резисторов и кнопок управления. Сборка навесным монтажом, питание +12 В и −5 В — от недорогих DC-DC модулей. Разводка позволяет подключать выводы 2–6 панельки напрямую к отверстиям платы Arduino.

Поддерживаются два варианта отображения: LCD-экран или вывод в последовательный порт. При отсутствии экрана можно ориентироваться по встроенным светодиодам: постоянное свечение — чип исправен, серия из пяти коротких вспышек — ошибка.

Проект изначально основан на [4116TesterMod](https://github.com/Wierzbowsky/4116TesterMod) от Wierzbowsky; но код в процессе разработки весь был переписан. Также упомяну [dramtester](https://github.com/zeus074/dramtester), хотя наткнулся на него уже после того, как закончил свой — в нём используются те же экран и один из DC-DC модулей. Оба они ведут свою родословную от [этого проекта](https://forum.defence-force.org/viewtopic.php?p=15035).

## Схема

![Схема](https://github.com/atsidaev/4116_dram_tester/blob/master/doc/schematics.png)

## Комплектующие

| Имя | Название | Примерная цена | Рандомная ссылка на Aliexpress |
|-----|----------|----------------|--------------------------------|
| A1 | LGT8F328P TYPE-C Arduino Nano3 Clone | $1.5 |  https://aliexpress.ru/item/1005009016874090.html |
| U1 | DIP-16 socket | $0.1 | |
| U2 | MT3608 DC-DC Adjustable Boost Module 2A | $0.4 | https://aliexpress.ru/item/1005010413039458.html |
| U3 | DC-DC Negative Voltage Power Supply Module -5V | $1 | https://aliexpress.ru/item/32902764735.html | 
| J1 | OLED module 0.91" 128X32 | $1 | https://aliexpress.ru/item/1005004572487969.html |

Сокет U1 можно взять ZIF-16. Ардуина должна подойти любая на AtMega328. U2 стоит настраиваемый только потому, что у меня был лишь такой под рукой. 
Проще взять на фиксированное напряжение 12V, типа такого: https://aliexpress.ru/item/1005007581823204.html.

## Прошивка

Используйте Arduino или PlatformIO для сборки прошивки и загрузки её в плату Arduino. 
Основной скетч-файл это firmware/firmware.ino.

При сборке через platformio все зависимости подтянутся сами, при сборке через Arduino IDE нужно 
добавить библиотеку [Adafruit SSD1306](https://docs.arduino.cc/libraries/adafruit-ssd1306/).
У неё есть две свои зависимости - Adafruit_GFX_Library и Adafruit_BusIO.

Важно! Если тоже будете использовать плату на LGT8F328, то потребуется установить пакет "lgt8fx boards" и выбрать в меню Clock Source "External 16 MHz".

## Использование

Отключите тестер от USB. Вставьте микросхему в панельку. Подключите USB. После старта контроллера запустится тестирование.

## Тесты

Во время работы выводятся двухсимвольное обозначение теста и результат: **OK** при успехе, **X** при ошибке.

| Код | Описание |
|-----|----------|
| IO  | Проверка линий DI/DO на замыкание между собой |
| RC  | Проверка линий адреса на замыкание между собой |
| 10, 00, 01, 11 | Запись/чтение. Цифры — паттерн для чётных (первая) и нечётных (вторая) адресов |

---

# 4116 DRAM Tester

Minimalist tester for 4116 dynamic RAM. No relays, protection resistors, or control buttons. Built with point-to-point wiring; +12 V and −5 V are supplied by inexpensive DC-DC modules. Pinout allows connecting socket pins 2–6 directly to Arduino board holes.

Two output modes: LCD display or serial console. Without a display, status can be read from the board LEDs: steady on — chip OK, five short blinks — fault.

Originally based on [4116TesterMod](https://github.com/Wierzbowsky/4116TesterMod) by Wierzbowsky; the code was fully rewritten during development. See also [dramtester](https://github.com/zeus074/dramtester), which uses the same display and one of the DC-DC modules. Both these testers are evolutoin of this [project](https://forum.defence-force.org/viewtopic.php?p=15035).

Aimed at quick chip verification without elaborate assembly or enclosure.

## Schematic

See above

## Bill of materials

| Ref | Name | Approx. price | Random Aliexpress link |
|-----|------|---------------|------------------------|
| A1 | LGT8F328P TYPE-C Arduino Nano3 Clone | $1.5 |  https://aliexpress.ru/item/1005009016874090.html |
| U1 | DIP-16 socket | $0.1 | |
| U2 | MT3608 DC-DC Adjustable Boost Module 2A | $0.4 | https://aliexpress.ru/item/1005010413039458.html |
| U3 | DC-DC Negative Voltage Power Supply Module -5V | $1 | https://aliexpress.ru/item/32902764735.html | 
| J1 | OLED module 0.91" 128X32 | $1 | https://aliexpress.ru/item/1005004572487969.html |

Socket U1 can be a ZIF-16 type. Any Arduino based on ATmega328 should work. U2 is adjustable only because that is what I had on hand.
It is simpler to use a fixed 12 V module like this one: https://aliexpress.ru/item/1005007581823204.html.

## Flashing

Use Arduino or PlatformIO to build and flash firmware. Main sketch file is firmware/firmware.ino.

When building with PlatformIO all dependencies will be pulled in automatically. When using Arduino IDE you need to install the [Adafruit SSD1306](https://docs.arduino.cc/libraries/adafruit-ssd1306/) library.
It has two dependencies of its own: Adafruit_GFX_Library and Adafruit_BusIO.

If you are using an LGT8F328-based board, install the "lgt8fx boards" package and set "Clock Source" to "External 16 MHz" in the board menu.

## Usage

Disconnect USB. Put 4116 in the socket. Connect USB. Testing will start immediately.

## Tests

Each test is shown by a two-letter code and result: **OK** on success, **X** on failure.

| Code | Description |
|------|-------------|
| IO   | DI/DO lines short test |
| RC   | Address lines short test |
| 10, 00, 01, 11 | Write/read. Digits are the pattern for even (first) and odd (second) addresses |
