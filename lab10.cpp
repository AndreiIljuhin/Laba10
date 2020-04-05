#include <iostream>
#include <ctime>
#include <fstream>
#include <chrono>
#define width N*3+2

using namespace std;

void stream_cleaning() {
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
}

struct Node {
	int info;
	Node* left = nullptr;
	Node* right = nullptr;
};

Node* node_search(Node* current, int value) {
	if (current == nullptr)
		return nullptr;
	if (value > current->info) {
		if (current->right != nullptr)
			current = node_search(current->right, value);
	}
	else if (value < current->info) {
		if (current->left != nullptr)
			current = node_search(current->left, value);
	}
	return current;
}

Node* node_search(Node* current, Node*& previous, int value) {//поиск вместе с предыдущим
	if (current == nullptr)
		return nullptr;
	if (value > current->info) {
		if (current->right != nullptr) {
			previous = current;
			current = node_search(current->right, previous, value);
		}
	}
	else if (value < current->info) {
		if (current->left != nullptr) {
			previous = current;
			current = node_search(current->left, previous, value);
		}
	}
	return current;
}

void tree_direct_walk(Node* root) {
	cout << root->info << " ";
	if (root->left != nullptr)
		tree_direct_walk(root->left);
	if (root->right != nullptr)
		tree_direct_walk(root->right);
}

void tree_reverse_walk(Node* root) {
	if (root->left != nullptr)
		tree_reverse_walk(root->left);
	if (root->right != nullptr)
		tree_reverse_walk(root->right);
	cout << root->info << " ";
}

void tree_symmetrical_walk(Node* root) {
	if (root->left != nullptr)
		tree_symmetrical_walk(root->left);
	cout << root->info << " ";
	if (root->right != nullptr)
		tree_symmetrical_walk(root->right);
}

bool add_to_tree(Node* root, int value) {
	Node* current = node_search(root, value);//место для вставки
	if (current == nullptr)
		return 0;
	if (value == current->info)
		return 0;
	Node* new_g;
	if (value > current->info) {
		new_g = new Node;
		current->right = new_g;
		new_g->info = value;
	}
	if (value < current->info) {
		new_g = new Node;
		current->left = new_g;
		new_g->info = value;
	}
	return 1;
}

bool remove_from_tree(Node*& root, int value) {
	if (root == nullptr)
		return 0;
	Node* previous = nullptr;
	Node* current = node_search(root, previous, value);//ближайший узел
	if (value != current->info)
		return 0;

	if (current->left != nullptr and current->right != nullptr) {//если есть правый и левый, то цепляю правый на правую сторону
		Node* place = current->left;							 //последнего элемента в правой цепочке левого и переписываю предыдущий
		while (place->right != nullptr)
			place = place->right;
		if (previous != nullptr) {
			if (previous->info < current->info)
				previous->right = current->left;
			else
				previous->left = current->left;
		}
		else
			root = current->left;
		place->right = current->right;
		delete current;
	}
	else if (current->left != nullptr){	//иначе просто удаляю узел и ставлю на его место следующий
		if (previous != nullptr) {
			if (previous->info < current->info)
				previous->right = current->left;
			else
				previous->left = current->left;
		}
		else
			root = current->left;
		delete current;
	}
	else if (current->right != nullptr){
		if (previous != nullptr) {
			if (previous->info < current->info)
				previous->right = current->right;
			else
				previous->left = current->right;
		}
		else
			root = current->right;
		delete current;
	}
	else {
		if (previous != nullptr) {
			if (previous->info < current->info)
				previous->right = nullptr;
			else
				previous->left = nullptr;
		}
		else
			root = nullptr;
		delete current;
	}

	return 1;
}

void tree_randomly(int N, Node*& root, int range) {
	root = new Node;
	root->info = rand() % range - range / 2;
	for (int i = 1; i < N; i++)
		if (!add_to_tree(root, rand() % range - range / 2))
			i--;
}

bool tree_with_keyboard(int& N, Node*& root) {
	cout << "Вводите целочисленные элементы бинарного дерева, для конца записи введите любой символ или букву\n";
	int value;
	while (1) {
		if (cin >> value) {
			if (root == nullptr) {
				root = new Node;
				root->info = value;
				N++;
			}
			else if (add_to_tree(root, value))
				N++;
			stream_cleaning();
		}
		else {
			stream_cleaning();
			if (N == 0)
				return 0;
			return 1;
		}
	}
}

bool tree_from_file(int& N, Node*& root) {
	cout << "Введите элементы бинарного дерева в файл через пробел\n";
	fstream file("file.txt", ios_base::out);
	file.close();
	system("file.txt");
	chrono::time_point<chrono::high_resolution_clock> start, end;
	double elapsed_seconds;
	start = chrono::high_resolution_clock::now();
	file.open("file.txt", ios_base::in);
	char ch;
	int value;
	while (1) {
		if (file >> value) {
			if (root == nullptr) {
				root = new Node;
				root->info = value;
				N++;
			}
			else if (add_to_tree(root, value))
				N++;
		}
		else if (file >> ch);
		else {
			if (N == 0)
				return 0;
			end = chrono::high_resolution_clock::now();
			elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
			cout << "Дерево создано за " << 0.000000001 * elapsed_seconds << " секунды.\n";
			return 1;
		}
	}
	file.close();
}

void delete_tree(Node* root) {
	if (root->left != nullptr)
		delete_tree(root->left);
	if (root->right != nullptr)
		delete_tree(root->right);
	delete root;
}

char* int_to_char(int i) {
	char* str = new char[15];
	bool minus = 0;
	if (i < 0) {
		i = abs(i);
		minus = 1;
	}
	else if (i == 0) {
		str[0] = (unsigned char)(48);
		str[1] = '\0';
		return str;
	}
	str[0] = '\0';
	while (i > 0) {
		for (int i = 14; i > 0; i--)//сдвиг вправо
			str[i] = str[i - 1];
		str[0] = (unsigned char)(48 + i % 10);
		i = i / 10;
	}
	if (minus) {
		for (int i = 14; i > 0; i--)
			str[i] = str[i - 1];
		str[0] = '-';
	}
	return str;
}

template<class T>
struct List {
	T info;
	List* next = nullptr;
};

void right_shift_matrix(int** matrix, int i, int j, int N) {//сдвиг на 1 вправо // j становится пустой
	for (int q = i; q >= 0; q--) {
		for (int k = width - 1; k > j; k--)
			matrix[q][k] = matrix[q][k - 1];
		if (matrix[q][j] != -999998)//если не горизонтальная линия
			matrix[q][j] = -999999;
	}
}

void tree_output(ofstream& file, const Node* root, int N, bool in_file) {
	if (root != nullptr) {
		if (in_file)
			file << "Бинарное дерево из " << N << " элемент(а)ов\n";//строка для однозначного определения кодировки текстовым редактором

		List<const Node*>* head, * current, * new_t;
		head = new List<const Node*>;
		head->info = root;
		int** matrix;//здесь будет находиться дерево графа
		int* entry_points = new int[N / 2];//точки, хранящие положение узлов в матрице для следующего уровня дерева
		int i = 0, j = 0, entry_tail = 1;
		matrix = new int*[N * 2];
		for (int i = 0; i < N * 2; i++) {
			matrix[i] = new int[width];
			for (int j = 0; j < width; j++)
				matrix[i][j] = -999999;//-999999 означает символ ' '
		}
		entry_points[0] = 1;//изначальное положение по x корня дерева

		while (head != nullptr) {
			j = 0;
			current = head;
			while (current != nullptr) {//запись информации в матрицу
				matrix[i][entry_points[j]] = current->info->info;
				if (current->info->left != nullptr){//добавление левой ветки для узла
					for (int q = 0; q < 3; q++) {//сдвиг всех вышестоящих элементов
						right_shift_matrix(matrix, i + 1, entry_points[j], N);
						for (int q = j + 1; q < entry_tail; q++)//увеличение всех точек справа
							entry_points[q]++;
						matrix[i + 1][entry_points[j]] = -999998;//-999998 означает символ '-'
					}
					entry_tail++;
					for (int i = entry_tail; i > j; i--)	  //entry_points[j] хранит точку узла следующего уровня, поэтому сдвиг вправо
						entry_points[i] = entry_points[i - 1];//для ее дублирования,
					j++;									  //инкремент j возвращает к текущей точке,
					entry_points[j] += 3;					  //увеличение текущей точки узла на 3(так как матрица сдвинулась на 3 позиции вправо)
				}

				if (current->info->right != nullptr) {//добавление правой ветки узла
					for (int q = 0; q < 3; q++) {
						right_shift_matrix(matrix, i + 1, entry_points[j] + 1, N);
						for (int q = j + 1; q < entry_tail; q++)
							entry_points[q]++;
						matrix[i + 1][entry_points[j] + 1] = -999998;
					}
					entry_points[j] += 3;//меняю старый узел на новый и перехожу к следующему
					j++;
				}
				else {//если нет правого узла, удаляю точку
					for (int i = j; i < entry_tail; i++)//сдвиг влево переходит к следующей точке
						entry_points[i] = entry_points[i + 1];
					entry_tail--;
				}
				current = current->next;
			}
			current = head;
			while (current != nullptr) {//переход к следующей линии элементов на одинаковом расстоянии от вершины
				bool b = 0;
				if (current->info->right != nullptr) {//добавление правого элемента списка на место перед текущим
					new_t = new List<const Node*>;
					new_t->next = current->next;
					current->next = new_t;
					new_t->info = current->info->right;
					b = 1;
				}
				if (current->info->left != nullptr) {//замена элемента списка
					current->info = current->info->left;
					current = current->next;
				}
				else{//удаление элемента списка
					if (current == head) {
						head = current->next;
						delete current;
						current = head;
					}
					else {
						new_t = current;
						current = head;
						while (current->next != new_t)
							current = current->next;
						current->next = new_t->next;
						delete new_t;
						current = current->next;
					}
				}
				if (b)//пропуск добавленного правого элемента
					current = current->next;
			}
			i+= 2;
		}
		for (int q = 0; q < i - 1; q++) {//вывод дерева в файл
			if (q % 2 == 0) {//формирование стрелок к узлам
				for (j = 0; j < width; j++) {
					if (matrix[q][j] > -999998) {
						if (in_file)
							file << "|";
						else
							cout << "|";
						j++;
						while (matrix[q][j] > -999998 and j < width) {
							j++;
							if (in_file)
								file << " ";
							else
								cout << " ";
						}
						j--;
					}
					else if (in_file)
						file << " ";
					else
						cout << " ";
				}
				if (in_file)
					file << endl;
				else
					cout << endl;
				if (in_file) {
					for (j = 0; j < width; j++) {
						if (matrix[q][j] > -999998) {
							file << (unsigned char)149;//в консоле нет этого символа
							j++;
							while (matrix[q][j] > -999998 and j < width) {
								j++;
								file << " ";
							}
							j--;
						}
						else
							file << " ";
					}
					file << endl;
				}
			}
			bool b = 0;//b означает первый символ правой или левой ветки, так как иногда после сдвигов матрицы символы '-' выходят левее узлов
			for (j = 0; j < width; j++) {
				if (!b and matrix[q][j] == -999998 and (matrix[q + 1][j] != -999999 or matrix[q - 1][j - 1] != -999999))
					b = 1;
				if (b and matrix[q][j] == -999999)
					b = 0;
				if (matrix[q][j] == -999999 or matrix[q][j] == -999998 and !b) {
					matrix[q][j] = -999999;
					if (in_file)
						file << " ";
					else
						cout << " ";
				}
				else if (matrix[q][j] == -999998) {//нахожу левую точку левой ветки и правую правой
					if (matrix[q + 1][j] != -999999 and matrix[q][j - 1] == -999999)
						if (in_file)
							file << ",";
						else
							cout << ",";
					else if (matrix[q + 1][j] != -999999)
						if (in_file)
							file << ".";
						else
							cout << ".";
					else if (in_file)
						file << "-";
					else
						cout << "-";
				}
				else {
					if (in_file)
						file << matrix[q][j];
					else
						cout << matrix[q][j];
					char* str = int_to_char(matrix[q][j]);
					j += strlen(str) - 1;//пропускаю n пробелов, n равно длине строки числа - 1
					delete[] str;
				}
			}
			if (in_file)
				file << endl;
			else
				cout << endl;
		}

		for (i = 0; i < N * 2; i++)
			delete[] matrix[i];
		delete[] matrix;
	}
}
//"Создайте еще одно бинарное дерево размерности N, заполненное"
//"случайными числами (от – 99 до 99). Соедините два дерево в одно"
//"Сравните скорость выполнение объединения при аналогичных"
//"обстоятельствах с линейной структурой (динамический"
//"массив/двусвязный список)"

void combination_of_trees(Node* root1, Node* root2, int& N_node2) {
	if (add_to_tree(root2, root1->info))
		N_node2++;
	if (root1->left != nullptr)
		combination_of_trees(root1->left, root2, N_node2);
	if (root1->right != nullptr)
		combination_of_trees(root1->right, root2, N_node2);
}

void one_array_bubbleSort(int* arr, int N)
{
	int temp;
	for (int i = 0; i < N - 1; i++)
		for (int j = 0; j < N - i - 1; j++)
			if (arr[j] > arr[j + 1]) {
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
}

void part_5(int& N_node, Node* root) {
	Node* root2 = nullptr;
	int N_node2 = 0;
	chrono::time_point<chrono::high_resolution_clock> start, end;
	cout << "Количество элементов нового бинарного дерева:\n";
	while (!(N_node2 > 0 and N_node2 < 190)) {
		cin >> N_node2;
		stream_cleaning();
		if (!(N_node2 > 0 and N_node2 < 190))
			cout << "Размер дерева должен быть больше 0 и меньше 190.\n";
	}
	tree_randomly(N_node2, root2, 200);
	cout << "Элементы в порядке возрастания:\n";
	tree_symmetrical_walk(root2);
	cout << endl;
	ofstream in_file("tree2.txt");
	in_file << "Новое ";
	tree_output(in_file, root2, N_node2, 1);
	in_file.close();
	system("tree2.txt");

	int N_mass1 = N_node;
	int N_mass2 = N_node2;
	int* mass1 = (int*)malloc(N_mass1 * sizeof(int));
	int* mass2 = (int*)malloc(N_mass2 * sizeof(int));
	for (int i = 0; i < N_mass1; i++)
		mass1[i] = rand() % 2000 - 1000;
	one_array_bubbleSort(mass1, N_mass1);
	for (int i = 0; i < N_mass2; i++)
		mass2[i] = rand() % 200 - 100;
	one_array_bubbleSort(mass2, N_mass2);

	start = chrono::high_resolution_clock::now();
	for (int k = 0; k < N_mass1; k++)
		for (int i = 0; i < N_mass2; i++) {//удаление одинаковых элементов
			if (mass1[k] == mass2[i]) {
				for (int j = i; j < N_mass2 - 1; j++)
					mass2[j] = mass2[j + 1];
				N_mass2--;
				break;
			}
		}
	mass2 = (int*)realloc(mass2, (N_mass1 + N_mass2) * sizeof(int));//выделение новой памяти
	int* mass1_ptr = mass1;
	for (int i = N_mass2; i < N_mass1 + N_mass2; i++) {//добавление элементов
		mass2[i] = *mass1_ptr;
		mass1_ptr++;
	}
	one_array_bubbleSort(mass2, N_mass1 + N_mass2);//сортировка
	end = chrono::high_resolution_clock::now();

	double elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
	cout << "Объединение и сортировка соразмерных динаммических массивов за " << 0.000000001 * elapsed_seconds << " секунды.\n";
	free(mass1);
	free(mass2);

	start = chrono::high_resolution_clock::now();
	combination_of_trees(root, root2, N_node2);
	end = chrono::high_resolution_clock::now();
	double elapsed_seconds2 = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
	in_file.open("combinedtree.txt");
	in_file << "Объединенное ";
	tree_output(in_file, root2, N_node2, 1);
	in_file.close();
	system("combinedtree.txt");
	cout << "Объединение бинарных деревьев за " << 0.000000001 * elapsed_seconds2 << " секунды.\n";
	if (elapsed_seconds > elapsed_seconds2)
		cout << "Объединение бинарных деревьев быстрее объединения массивов\n";
	else
		cout << "Объединение массивов быстрее из-за небольшого размера деревьев,\n"
		<< "попробуйте создать бинарные деревья большего размера\n";
}

int main()
{
	setlocale(LC_ALL, "ru");
	srand(time(0));
	chrono::time_point<chrono::high_resolution_clock> start, end;
	double elapsed_seconds;
	ifstream out_file;
	ofstream in_file;
	int choice, N_node = 0;
	Node* root = nullptr;

	while (1) {

		cout << "1 - Формирование бинарного дерева из N элементов\n";
		cout << "2 - Вывод бинарного дерева\n";
		cout << "3 - Вставка, удаление или поиск узла бинарного дерева\n";
		cout << "4 - Обход бинарного дерева\n";
		cout << "5 - Задание по варианту\n";
		cin >> choice;
		stream_cleaning();

		if (choice == 1) {
			if (root != nullptr) {
				delete_tree(root);
				root = nullptr;
			}
			N_node = 0;
			cout << " 1 - N с клавиатуры, элементы рандомно\n";
			cout << " 2 - элементы с клавиатуры\n";
			cout << " 3 - элементы с файла\n ";
			cin >> choice;
			stream_cleaning();

			switch (choice) {

			case 1:
				cout << "Введите количество элементов бинарного дерева: ";
				cin >> N_node;
				if (N_node <= 0 or N_node > 310) {
					cout << "Размер дерева должен быть больше 0 и меньше 310.\n";
					break;
				}
				stream_cleaning();
				start = chrono::high_resolution_clock::now();
				tree_randomly(N_node, root, 2000);
				end = chrono::high_resolution_clock::now();
				elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				cout << "Дерево создано за " << 0.000000001 * elapsed_seconds << " секунды.\n" << "Элементы в порядке возрастания:\n";
				tree_symmetrical_walk(root);
				cout << endl;
				break;

			case 2:
				if (!tree_with_keyboard(N_node, root))
					cout << "Не введено ни одного элемента дерева.\n";
				else {
					cout << "Дерево создано, элементы в порядке возрастания:\n";
					tree_symmetrical_walk(root);
					cout << endl;
					cout << "Количество элементов: " << N_node << endl;
				}
				break;

			case 3:
				if(!tree_from_file(N_node, root))
					cout << "Не введено ни одного элемента дерева.\n";
				else {
					cout << "Элементы в порядке возрастания:\n";
					tree_symmetrical_walk(root);
					cout << endl;
					cout << "Количество элементов: " << N_node << endl;
				}
				break;
			}
			choice = 0;
		}

		if (choice == 2 and root != nullptr) {
			cout << " 1 - вывод дерева в файл\n";
			cout << " 2 - вывод в консоль(не рекомендуется для деревьев размера больше 25)\n ";
			cin >> choice;
			stream_cleaning();
			if (choice == 1) {
				in_file.open("tree.txt");
				tree_output(in_file, root, N_node, 1);
				in_file.close();
				system("tree.txt");
			}
			else {
				tree_output(in_file, root, N_node, 0);
				system("Pause");
			}
			choice = 0;
		}
		else if (choice == 2)
			cout << "Сначала создайте бинарное дерево.\n";

		if (choice == 3 and root != nullptr) {
			cout << " 1 - добавление узла\n";
			cout << " 2 - удаление узла\n";
			cout << " 3 - поиск узла \n";
			cin >> choice;
			stream_cleaning();
			if (choice == 1) {
				cout << "Добавить узел со значением: ";
				int value = 0;
				cin >> value;
				stream_cleaning();
				start = chrono::high_resolution_clock::now();
				if (add_to_tree(root, value)) {
					end = chrono::high_resolution_clock::now();
					elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
					cout << "Узел добавлен за " << 0.000000001 * elapsed_seconds << " секунды.\n";
					N_node++;
				}
				else
					cout << "Узел с таким значением уже существует.\n";
			}
			else if (choice == 2){
				cout << "Удалить узел со значением: ";
				int value = 0;
				cin >> value;
				stream_cleaning();
				start = chrono::high_resolution_clock::now();
				if (remove_from_tree(root, value)) {
					end = chrono::high_resolution_clock::now();
					elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
					cout << "Узел удален за " << 0.000000001 * elapsed_seconds << " секунды.\n";
					N_node--;
				}
				else
					cout << "Узел с значением " << value << " не найден\n";
			}
			else if (choice == 3){
				cout << "Найти узел со значением: ";
				int value = 0;
				cin >> value;
				stream_cleaning();
				start = chrono::high_resolution_clock::now();
				Node* search = node_search(root, value);
				end = chrono::high_resolution_clock::now();
				elapsed_seconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
				if (search->info != value)
					cout << "Узел с таким значением не найден.\n";
				else
					cout << "Узел найден.\n";
				cout << "Поиск " << 0.000000001 * elapsed_seconds << " секунды.\n";
			}
			choice = 0;
		}
		else if (choice == 3)
			cout << "Сначала создайте бинарное дерево.\n";

		if (choice == 4 and root != nullptr) {
			cout << "Прямой обход дерева\n";
			tree_direct_walk(root);
			cout << "\nОбратный обход дерева\n";
			tree_reverse_walk(root);
			cout << "\nСимметричный обход дерева\n";
			tree_symmetrical_walk(root);
			cout << endl;
		}
		else if (choice == 4)
			cout << "Сначала создайте бинарное дерево.\n";

		if (choice == 5 and root != nullptr) {
			cout << "\nСоздайте еще одно бинарное дерево размерности N, заполненное \n"
				"случайными числами (от – 99 до 99). Соедините два дерево в одно\n"
				"Сравните скорость выполнение объединения при аналогичных\n"
				"обстоятельствах с линейной структурой (динамический\n"
				"массив/двусвязный список)\n";
			part_5(N_node, root);
		}
		else if (choice == 5)
			cout << "Сначала создайте бинарное дерево.\n";

		cout << endl;
	}
}
