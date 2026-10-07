# image_processor

Консольное приложение для применения графических фильтров к изображениям в формате BMP
(24 бита, без сжатия, `BITMAPINFOHEADER`). Написано на C++20 без сторонних библиотек для работы с изображениями.

Учебный проект курса по C++ (ВШЭ).

## Сборка

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Юнит-тесты собираются по умолчанию; отключить их можно флагом `-DIMAGE_PROCESSOR_BUILD_TESTS=OFF`.

## Использование

```
image_processor <input.bmp> <output.bmp> [-filter [args...]] ...
```

Фильтры применяются в порядке перечисления. Без фильтров изображение сохраняется без изменений.

```bash
./build/image_processor input.bmp output.bmp -crop 800 600 -gs -blur 0.5
```

| Фильтр | Аргументы | Описание |
|---|---|---|
| `-crop` | `width height` | Обрезка: остаётся верхняя левая часть заданного размера |
| `-gs` | — | Оттенки серого: `0.299 R + 0.587 G + 0.114 B` |
| `-neg` | — | Негатив |
| `-sharp` | — | Повышение резкости (матрица 3×3) |
| `-edge` | `threshold` | Выделение границ: grayscale + матрица, затем порог |
| `-blur` | `sigma` | Гауссово размытие |

Некорректные файлы, неизвестные фильтры и неверные аргументы обрабатываются через исключения
с понятным сообщением об ошибке, без падения программы.

## Структура

```
Main/              точка входа, класс Image, фабрика фильтров
BMP/               чтение и запись BMP
Abstract_Filters/  базовые классы Filter и MatrixFilter
Final_Filters/     конкретные фильтры
Exceptions/        собственные исключения
tests/             юнит-тесты (Catch2)
test_script/       сравнение результатов с эталонными изображениями
third_party/catch/ Catch2 v2 (single header)
```

## Тесты

```bash
ctest --test-dir build --output-on-failure
```

`filter_tests` прогоняет `test_script/test_image_processor.py`, ему нужен Pillow (`pip install pillow`).
Скрипт можно запустить и вручную:

```bash
python3 test_script/test_image_processor.py build/image_processor
```
