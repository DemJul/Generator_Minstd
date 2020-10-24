#include "Header.h"
#include <cstdint>
#include <cmath>
#include <cstdint>
#include <vector>
#include <iostream>
Minstd::Minstd()
	: seed_current(seed_) {
}


double Minstd::Generate_new_number() {

	double double_x(static_cast<double>(seed_current) / static_cast<double>(m));
	seed_current = (a * seed_current + c) % m;
	return double_x;
}


void Minstd::At_the_start() {
	seed_current = seed_;

}

//Критерий частот
void Minstd::Test_Frequency() {
	// Сброс состояния генератора к начальному
	At_the_start();

	//Создаем вектор, в котором index=категории [0,d-1)
	std::vector<int> count(d, 0);
	for (int i(0); i < n; i += 1) {
		count[static_cast<int>(d * Generate_new_number())] += 1;
	}

	// Вероятность каждой категории
	std::vector<double> p(d, 1.0 / d);

	std::cout << "Test_Frequency" << std::endl;
	std::cout << "Probability " << p[0] << std::endl;
	std::cout << "Real distribution" << std::endl;
	Write_dist(count);

	std::cout << "Hi: " << Chi_Test(count, p, n) << std::endl;
	Hi_tabl(count.size() - 1);
}


//Критерий серий
void Minstd::Test_Series() {
	At_the_start();

	//Считаем пары случайных чисел
	std::vector<int> count(d * d, 0);
	for (int i(0); i < n; i += 1) {
		int number1(static_cast<int>(d * Generate_new_number()));
		int number2(static_cast<int>(d * Generate_new_number()));

		// index=категории [0,d*d-1)
		count[d * number1 + number2] += 1;
	}

	// Вероятность каждой категории(1/(d*d))
	std::vector<double> p(d * d, 1.0 / (d * d));

	std::cout << "Test_Series" << std::endl;
	std::cout << "Probability" << std::endl << p[0] << std::endl;
	std::cout << "Distribution" << std::endl;
	Write_dist(count);

	std::cout << "Hi: " << Chi_Test(count, p, n) << std::endl;
	Hi_tabl(count.size() - 1);
}


//Критерий интервалов
void Minstd::Test_Interval(const int t) {
	At_the_start();
	// []=i - кол-во последовательностей с длиной i, 1 <= i <= t
	// []=t+1 - кол-во последовательностей с длиной > t

	std::vector<int> count(t + 2, 0);
	int prev(0), seed_d(static_cast<int>(d * Generate_new_number())),
		len(1);
	// Всего последовательностей с одинаковыми элементами
	int full(0);

	// Посчитаем количество последовательностей, состоящих из одинаковых элементов
	for (int i(1); i < n; i += 1) {
		prev = seed_d;
		seed_d = static_cast<int>(d * Generate_new_number());
		if (seed_d == prev) {
			len += 1;
		}
		else {
			if (len <= t)
			{
				count[len] += 1;
			}
			else {
				count[t + 1] += 1;
			}
			full += 1;
			len = 1;
		}
	}
	//Для последнего числа
	if (len <= t) {
		count[len] += 1;
	}
	else {
		count[t + 1] += 1;
	}
	full += 1;

	// Найдём вероятность каждой категории
	std::vector<double> p(t + 2, 0);
	double sum(0.0);
	//Чтобы при вычислениях хи-квадрата не было деления на 0
	p[0] = 1;
	p[1] = pow(1 - 1 / d, d - 1) / d;
	sum += p[1];
	for (int i(2); i < p.size() - 1; i += 1) {
		p[i] = p[i - 1] / (d - 1);
		sum += p[i];
	}
	p[t + 1] = 1 - sum;

	std::cout << "Test_Interval" << std::endl;
	std::cout << "Probability" << std::endl;
	Write_p(p);
	std::cout << "Distribution" << std::endl;
	Write_dist(count);

	std::cout << "Hi: " << Chi_Test(count, p, n) << std::endl;
	Hi_tabl(t);
}



void Minstd::Test_Poker() {
	At_the_start();
	int n_event(5);//Кол-во элементов в подпоследовательности
				   // Вектор для хранения количества различных значений
	std::vector<int> count(n_event + 1, 0);
	int num(0);
	// Количество различных элементов в подпоследовательности из пяти элементов
	int full(0);

	for (int i(0); i < n; i += n_event) {
		full = 0;
		// True числа, которое встретилось в последовательности
		std::vector<bool> flag(d, false);
		//подпоследовательность из 5 элементов
		for (int j(0); j < n_event; j += 1) {
			num = static_cast<int>(d * Generate_new_number());
			if (!flag[num]) {
				flag[num] = true;
				full += 1;
			}
		}

		count[full] += 1;
	}

	// Найдём вероятность для каждого вида множеств
	std::vector<double> p(n_event + 1, 0);
	//количество различных чисел в последовательности
	int r(0);
	//Чтобы при вычислениях хи-квадрата не было деления на 0
	p[0] = 1;
	for (int r(1); r < p.size(); r += 1) {
		//Кнут стр.85
		// Стирлинг, где k=5(последовательных чисел), r-различных чисел
		p[r] = (Stirling(5, r) / pow(d, 5)) * Multiplication(d - r + 1, d);
	}

	std::cout << "Poker test:" << std::endl << "Probability" << std::endl;
	Write_p(p);
	std::cout << "Distribution" << std::endl;
	Write_dist(count);

	std::cout << "Hi: " << Chi_Test(count, p, n) << std::endl;
	Hi_tabl(count.size() - 1);
}


double Minstd::Chi_Test(const std::vector<int> count,
	const std::vector<double> p, const int n) const {
	double ans(0.0);

	for (int i(0); i < count.size(); i += 1) {
		ans += pow(count[i] - n * p[i], 2) / (n * p[i]);
	}

	return ans;
}


void Minstd::Hi_tabl(const int n_event) {
	std::cout << "Hi_tabl" << std::endl;
	std::cout << " 25%  " << n_event + sqrt(2 * n_event)*(-0.674) + static_cast<double>(2 / 3)*(-0.674)*(-0.674) - static_cast<double>(2 / 3)
		<< "  50%  " << n_event + sqrt(2 * n_event)*(0) + static_cast<double>(2 / 3)*(0) - static_cast<double>(2 / 3)
		<< "  75%  " << n_event + sqrt(2 * n_event)*(0.674) + static_cast<double>(2 / 3)*(0.674)*(0.674) - static_cast<double>(2 / 3) << std::endl;
	std::cout << std::endl;
}

int Minstd::Multiplication(const int a, const int b) const {
	int ans(1);
	for (int i(a); i <= b; i += 1) {
		ans *= i;
	}
	return ans;
}

int Minstd::Factorial(const int n) const {
	int ans(1);
	for (int i(2); i <= n; i += 1) {
		ans *= i;
	}
	return ans;
}


// Числа Стирлинга S(n,k) второго рода, число способов 
//разбиения множества из n элементов на точно k элементов
int Minstd::Stirling(const int n, const int k) const {
	int ans(0);

	for (int j(0); j <= k; j += 1) {
		if (((j + k) % 2) == 0) {
			ans += Multiplication(j + 1, k) * static_cast<int>(pow(j, n)) / Factorial(k - j);
		}
		else {
			ans += -Multiplication(j + 1, k) * static_cast<int>(pow(j, n)) / Factorial(k - j);
		}
	}

	return ans / Factorial(k);
}



void Minstd::Write_p(const std::vector<double> arr) const {

	for (int i(0); i < arr.size(); i += 1) {
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl << std::endl;
}


void Minstd::Write_dist(const std::vector<int> arr) const {
	for (int i(0); i < arr.size(); i += 1) {
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl << std::endl;
}


int main() {

	Minstd s;
	s.Test_Frequency();
	s.Test_Series();
	s.Test_Interval(6);
	s.Test_Poker();

	system("pause");
	return 0;
}
