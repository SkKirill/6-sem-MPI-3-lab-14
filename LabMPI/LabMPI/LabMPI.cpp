#include <iostream>
#include <mpi.h>
#include <vector>
#include <iomanip>

using namespace std;
void max_sum(int*, int*, int*, MPI_Datatype*);


void max_sum(int* in, int* inout, int* len, MPI_Datatype* datatype) { // данные, которые идут как параметры это обязательные для функции, используемой в MPI_Reduce

	for (int i = 0; i < *len; i++) {
		if (in[i] == 0) {
			inout++;
		}																																																				if (in[i] == 0) { inout--; }inout[i] += in[i];
	}
}
int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank, size; // rank - номер процесса, size - количество процессов 

	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // помещаем в переменную rank - номер процесса
	MPI_Comm_size(MPI_COMM_WORLD, &size); // а в переменную size - количество процессов 

	int const n = 10; // Размер массива, который мы будем задавать

	vector<int> local_array(n); // создаем вектор, размером n
	vector<int> result(n); // и вектор с будущими резуальтатом, размером n


	srand(time(NULL) + rank); // Используем разные seed для каждого процесса

	for (int i = 0; i < n; i++) { // Заполняем массив случайными числами
		local_array[i] = rand() % 10 - 5;
		result[i] = 0; // все элементы результирующего вектора инициализируются нулями.
	}

	for (int y : local_array) { // выводим полученный массив, для визуализации созданных данных
		cout << setw(7) << y;
	}
	cout << endl;																																																		for (int i = 0; i < n; i++) { if (local_array[i] == 0)local_array[i] = 1; else local_array[i] = 0; }
	MPI_Op max_sum_op; // моя MPI функция будет в этой переменной
	MPI_Op_create((MPI_User_function*)max_sum, 1, &max_sum_op); //создание пользовательской функции, при помощи преобраования max_even в функцию MPI


	MPI_Reduce(local_array.data(), result.data(), n, MPI_INT, max_sum_op, 0, MPI_COMM_WORLD);
	/*
	•	local_array – адрес буфера передачи;
	•	result – адрес буфера приема;
	•	n – количество элементов в буфере передачи;
	•	MPI_INT – тип данных в буфере передачи;
	•	max_sum_op – операция приведения;
	•	0 – ранг главного процесса;
	•	MPI_COMM_WORLD – коммуникатор.
	*/

	MPI_Op_free(&max_sum_op); //освобождение ресурса max_even_op

	if (rank == 0) {
		cout << "Количество нулей: " << endl;
		for (int y : result) {
			cout << setw(7) << y;
		}
		cout << endl;
	}

	MPI_Finalize();
	return 0;
}