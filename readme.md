# Инструкция по компиляции приложения (создание деревьев по заданным данным и их вывод)
Скомпилируйте приложение, находясь в директории *src*:
`g++ main.cpp -o main`
# Инструкция по запуску приложения
Запустите сгенерированный исполняемый файл:
`./main`

# Инструкция по настройке и компиляции приложения (исследование времени выполнения операций в деревьях)
В файле *utils/benchmark.cpp* укажите
+ имена файлов, в которые будет записана информация о результатах исследования (предварительно создайте эти файлы)
	> const std::string avl_insert_file = "";
	> const std::string avl_delete_file = "";
	> const std::string rb_insert_file = "";
	> const std::string rb_delete_file = "";
+ количество ключей, которые будут вставлены и удалены из деревьев
	> const int nodes_count = 100000;

Скомпилируйте приложение, находясь в директории *src*:
`g++ -03 utils/benchmark.cpp -o benchmark`
# Инструкция по запуску приложения
Запустите сгенерированный исполняемый файл:
`./benchmark`
