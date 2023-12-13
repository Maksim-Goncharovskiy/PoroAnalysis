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

### Используемые библиотеки:
- OpenCV
- nlohmann-json
