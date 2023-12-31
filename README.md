# Проект: PoroAnalysis - модуль анализа пористости материала в рамках проекта PoroMarker.
## Задача: подсчёт необходимых в рамках проекта характеристик пористого материала по маскам изображений(срезов объекта).
### Части проекта:
1. Библиотека ObjData, содержащая весь функционал для подсчета характеристик пористого объекта.
2. Модульный тест для детальной проверки реализованных алгоритмов.
3. Консольное тестовое приложение для демонстрации работы библиотеки.

---
## Библиотека ObjData
Содержит класс ObjData, методы которого представляют собой функционал для подсчета характеристик пористого материала
```c++
  double get_object_volume();
  double get_absolute_porosity(); // получения абсолютного объёма пор
  double get_material_volume();
  double get_porosity(); // получение относительного объема пор(содержание пор)

/**
	@brief Функция получения распределения пор вдоль выбранной оси.
	@param axis Направление, вдоль которого распределение рассматривается. Может принимать только 3 значения: [0, 0, 1], [0, 1, 0], [1, 0, 0].
	@return Массив из объемов на каждом срезе вдоль оси
	*/
  std::vector<double> get_porosity_distribution(float axis[3]);

/**
	@brief Функция получения основных характеристик пористости в части тела(в вырезанном кубе)
	@param coordinates Массив чисел - координаты, которыми куб внутри тела вырезается(x1, x2, y1, y2, z1, z2). Целые числа.
	@return Пара чисел, первое - абсолютный объем пор, второе - содержание пор в выбранном куске.
	*/
  std::pair<double, double> get_partial_porosity(int coordinates[6]);
```

---
## Тестовое приложение
Консольное тестовое приложение принимает в качестве параметра командной строки путь до json-файла с информацией для теста. Примеры тестовых файлов лежат в папке tests(в корне проекта). Если аргумент не передать, тестовое приложение запустится для тестового файла по умолчанию. \
Пример тестового файла:
```json
{
  "paths": [
    "..\\..\\..\\..\\data\\test1\\test6.png",
    "..\\..\\..\\..\\data\\test1\\test7.png",
    "..\\..\\..\\..\\data\\test1\\test8.png",
    "..\\..\\..\\..\\data\\test1\\test9.png",
    "..\\..\\..\\..\\data\\test1\\test10.png"
  ],
  "pixel_square": 0.01,
  "slice_thickness": 0.1,
  "sub_volume": [1, 755, 1, 950, 1, 5]
}
```
### Примерный результат работы тестового приложения.
![example](ex.png)
### Используемые библиотеки:
- OpenCV
- nlohmann-json
