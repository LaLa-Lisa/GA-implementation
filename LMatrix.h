#pragma once


#include <vector>


class LMatrix {
public:
	//���������� �����
	int n = 0;
	//���������� ��������
	int m = 0;
	//������ ������ �������
	std::vector<std::vector<double>> data;

	LMatrix();
	LMatrix(int, int);

	//�������������
	void init(int, int);

	LMatrix operator=(LMatrix);
	LMatrix operator+(LMatrix);
	LMatrix operator-(LMatrix);
	LMatrix operator*(LMatrix&);
	LMatrix operator*(double);
	double operator() (int i, int j);

	//���������������� 
	LMatrix T();

	//������ �������
	void read();

	//����� �������
	void write();

	//���������� ���������� �������
	void Wrand();

	//��������� �������� �� ��� ��������� �������
	void sigmoid();

	//���������� ������� ���������� � ������
	std::vector<double> getLine();

	//������������ ������ � �������
	void setLine(std::vector<double>& inVector);
};