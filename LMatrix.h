#pragma once


#include <vector>


class LMatrix {
public:
	//количество строк
	int n = 0;
	//количсетво столбцов
	int m = 0;
	//массив данных матрицы
	std::vector<std::vector<double>> data;

	LMatrix();
	LMatrix(int, int);

	//инициализация
	void init(int, int);

	LMatrix operator=(LMatrix);
	LMatrix operator+(LMatrix);
	LMatrix operator-(LMatrix);
	LMatrix operator*(LMatrix&);
	LMatrix operator*(double);
	double operator() (int i, int j);

	//транспонирование 
	LMatrix T();

	//чтение матрицы
	void read();

	//вывод матрицы
	void write();

	//заполнение рандомными числами
	void Wrand();

	//применяет сигмойду ко все значениям матрицы
	void sigmoid();

	//возвращает матрицу развёрнутую в строку
	std::vector<double> getLine();

	//разврачивает строку в матрицу
	void setLine(std::vector<double>& inVector);
};