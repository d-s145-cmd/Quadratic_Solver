# Quadratic_Solver

Решает квадратное уравнение **ax² + bx + c = 0** с коэффициентами произвольной точности. Длинная арифметика реализована с нуля на строках — сторонние библиотеки не используются.

## 📋 Содержание

- [Возможности](#возможности)
- [Сборка](#сборка)
- [Использование](#использование)
- [Формат входных данных](#формат-входных-данных)
- [Формат выходных данных](#формат-выходных-данных)
- [Примеры](#примеры)
- [Структура проекта](#структура-проекта)
- [Алгоритм](#алгоритм)
- [Тесты](#тесты)
- [Лицензия](#лицензия)

## Возможности

- ✅ Решение квадратных и линейных уравнений
- ✅ Поддержка чисел с произвольной точностью (до 10⁵ десятичных знаков)
- ✅ Поддержка научной нотации (`1.5e10`, `1e-3`)
- ✅ Точные вычисления без потери precision
- ✅ Валидация входных данных
- ✅ Кроссплатформенность (Windows / Linux / macOS)

## Сборка

### Linux / macOS

```bash
g++ -std=c++17 -O2 -o QuadraticSolver \
    main.cpp QuadSolver.cpp BigDecimal.cpp UnsignedArith.cpp
###Windows (MinGW)
cmd
Copy
g++ -std=c++17 -O2 -o QuadraticSolver.exe ^
    main.cpp QuadSolver.cpp BigDecimal.cpp UnsignedArith.cpp
###Windows (Visual Studio)
cmd
Copy
cl /std:c++17 /O2 /Fe:QuadraticSolver.exe ^
    main.cpp QuadSolver.cpp BigDecimal.cpp UnsignedArith.cpp
Или запустите:
cmd
Copy
build.bat
##Использование
Интерактивный режим
bash
Copy
./QuadraticSolver
Введите три коэффициента через пробел:
Text
Copy
Enter a b c (or 'exit' to quit): 1 -3 2
Для выхода введите exit.
Перенаправление ввода
bash
Copy
./QuadraticSolver < tests.txt
Формат входных данных
Три коэффициента a b c через пробел:
Table
Тип	Примеры
Целые	42, -3, 0
Дробные	3.14, -0.001, 1.5
Экспонента	2e3, 1e-3, -1.5e10
Большие числа	1000000000000000000000
Ноль	0, -0
Формат выходных данных
Table
Статус	Описание
OK	Есть конечное количество решений
INF	Решений бесконечно много
WRONG	Неверный ввод коэффициента
NO SOLUTION	Решений не существует
При OK на следующих строках выводится:
Количество корней n
n корней по одному на строку (от меньшего к большему)
Примеры
Два корня
Text
Copy
$ ./QuadraticSolver
Enter a b c: 1 -3 2

[INPUT] a=1 b=-3 c=2
[OUTPUT]
OK
2
1
2
Один корень
Text
Copy
Enter a b c: 1 2 1

[INPUT] a=1 b=2 c=1
[OUTPUT]
OK
1
-1
Линейное уравнение
Text
Copy
Enter a b c: 0 2 -6

[INPUT] a=0 b=2 c=-6
[OUTPUT]
OK
1
3
Бесконечно много решений
Text
Copy
Enter a b c: 0 0 0

[INPUT] a=0 b=0 c=0
[OUTPUT]
INF
Нет решений
Text
Copy
Enter a b c: 0 0 5

[INPUT] a=0 b=0 c=5
[OUTPUT]
NO SOLUTION
Неверный ввод
Text
Copy
Enter a b c: abc 1 2

[INPUT] a=abc b=1 c=2
[OUTPUT]
WRONG
Структура проекта
Text
Copy
QuadraticSolver/
├── include/
│   ├── BigDecimal.h
│   ├── QuadSolver.h
│   └── UnsignedArith.h
├── src/
│   ├── main.cpp
│   ├── QuadSolver.cpp
│   ├── BigDecimal.cpp
│   └── UnsignedArith.cpp
├── test/
│   ├── tests.txt
│   └── expected_output.txt
└── README.md
##Алгоритм
Парсинг: коэффициенты преобразуются в BigDecimal — структуру (digits, exponent10, sign)
Дискриминант: D = b² − 4ac вычисляется точно через длинную арифметику
Корни:
D < 0 → NO SOLUTION
D = 0 → один корень x = −b / 2a
D > 0 → два корня x = (−b ± √D) / 2a
Квадратный корень: метод Ньютона с точностью до 100 знаков
Тесты
Файл tests.txt содержит тестовые данные:
Text
Copy
1 -3 2
1 5 6
1 -1 -2
1 2 1
1 0 0
4 -4 1
1 0 1
1 1 1
0 2 -6
0 4 -1
0 1 5
0 0 0
0 0 5
abc 1 2
1000000000000000000000 -2000000000000000000000 1000000000000000000000
1.5 -4.5 3
-1 -2 -1
2e3 -7e3 6e3
1e-3 -2e-3 1e-3
-0 5 -0
##Запуск тестов:
bash
Copy
./QuadraticSolver < tests.txt
