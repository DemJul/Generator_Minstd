#pragma once
#include <cstdint>
#include <cmath>
#include <cstdint>
#include <vector>
#include <iostream>

// ��������� MINSTD(2^31-1, a=48271, 0, 1)
class Minstd {
public:
	// �����������
	Minstd();
	// ����� ���������� � ���������� ���������
	void At_the_start();
	// ��������� ����� � ��������� [0; 1)
	double Generate_new_number();
	// �������� ������
	void Test_Frequency();
	// �������� �����
	void Test_Series();
	// �������� ����������
	void Test_Interval(const int t);
	// �����-��������
	void Test_Poker();

	// ��-�������
	double Chi_Test(const std::vector<int> count,
		const std::vector<double> p, const int n) const;
	int Factorial(const int n) const;
	int Multiplication(const int a, const int b) const;
	// ����� ��������� ������� ����, ����� �������� 
	//��������� ��������� �� n ��������� �� ����� k ���������
	int Stirling(const int n, const int k) const;
	//����� �����������
	void Write_p(const std::vector<double> arr) const;
	//����� ������������� 
	void Write_dist(const std::vector<int> arr) const;
	//��������� �������� ��-��������
	void Hi_tabl(const int n_event);

private:
	// ��������� ���������� 
	const int m{ static_cast<int>(pow(2, 31) - 1) }, a{ 48271 }, c{ 0 },
		seed_{ 1 };
	// ������� ��������� ��������
	unsigned long long seed_current{ 0 };
	const int n{ 6000000 };
	const int d{ 10 };

};