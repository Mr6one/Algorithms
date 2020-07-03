**Самая удаленная вершина**

Дано невзвешенное дерево. Расстоянием между двумя вершинами будем называть количество ребер в пути, соединяющем эти две вершины. Для каждой вершины определите расстояние до самой удаленной от нее вершины.

Формат ввода
В первой строке записано количество вершин n ≤ 10000. Затем следует n - 1 строка, описывающая ребра дерева.
Каждое ребро - это два различных целых числа - индексы вершин в диапазоне [0, n-1].
Индекс корня – 0. В каждом ребре родительской вершиной является та, чей номер меньше.

Формат вывода
Выход должен содержать n строк. В i-ой строке выводится расстояние от i-ой вершины до самой удаленной от нее.