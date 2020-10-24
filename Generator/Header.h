#pragma once
#include <cstdint>
#include <cmath>
#include <cstdint>
#include <vector>
#include <iostream>

// Генератор MINSTD(2^31-1, a=48271, 0, 1)
class Minstd {
public:
	// Конструктор
	Minstd();
	// Сброс генератора к начальному состоянию
	void At_the_start();
	// Генерация чисел в интервале [0; 1)
	double Generate_new_number();
	// Критерий частот
	void Test_Frequency();
	// Критерий серий
	void Test_Series();
	// Критерий интервалов
	void Test_Interval(const int t);
	// Покер-критерий
	void Test_Poker();

	// Хи-квадрат
	double Chi_Test(const std::vector<int> count,
		const std::vector<double> p, const int n) const;
	int Factorial(const int n) const;
	int Multiplication(const int a, const int b) const;
	// Число Стирлинга второго рода, число способов 
	//разбиения множества из n элементов на точно k элементов
	int Stirling(const int n, const int k) const;
	//Вывод вероятности
	void Write_p(const std::vector<double> arr) const;
	//Вывод распределения 
	void Write_dist(const std::vector<int> arr) const;
	//Табличное значение хи-квадрата
	void Hi_tabl(const int n_event);

private:
	// Параметры генератора 
	const int m{ static_cast<int>(pow(2, 31) - 1) }, a{ 48271 }, c{ 0 },
		seed_{ 1 };
	// Текущее случайное значение
	unsigned long long seed_current{ 0 };
	const int n{ 6000000 };
	const int d{ 10 };

};