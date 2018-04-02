#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <boost/serialization/array_wrapper.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace boost::numeric::ublas;

std::wstringstream CHANGE;

int read_data(wchar_t way[], int dim[])
{
	/*
	std::wifstream f;
	f.open(way, std::ios_base::in);
	*/
	boost::filesystem::wifstream f;
	f.open(way);
	wchar_t buf[1024];
	std::vector <std::wstring>ARR(0);
	std::wstring buff, smallbuf;
	std::wstringstream BU;
	int m, k, l, n = 0;
	while (!f.eof())
	{
		f.getline(buf, 1024);
		if (wcslen(buf) > 1)
		{
			ARR.push_back(buf);
		n++;
		}
	}
	// Определим число столбцов
	m = ARR[0].length();
	k = 1;
	smallbuf = ARR[0];
	for (int i = 0; i < m; i++)
		if (int(smallbuf[i]) == int(','))
			k++;
	matrix <double> AWR(k, n);
	smallbuf = L"";
	for (int i = 2; i < n; i++)
	{
		m = ARR[i].length();
		l = 0;
		buff = ARR[i];
		for (int j = 0; j < m; j++)
			if (int(buff[j]) != int(','))
				smallbuf += buff[j];
			else
			{
				BU << smallbuf;
				BU >> AWR(l, i);
				BU.str(L"");
				BU.clear();
				smallbuf = L"";
				l++;
			}
		BU << smallbuf;
		BU >> AWR(l, i);
		BU.str(L"");
		BU.clear();
		smallbuf = L"";
	}
	dim[0] = n - 2;
	dim[1] = k;
	CHANGE.str(L"");
	CHANGE.clear();
	for (int i = 2; i < n; i++)
		for (int j = 0; j < k; j++)
			CHANGE << AWR(j, i) << std::endl;
	f.close();
	return 1;
}
int cut_arr(double _IN[], int n, double _OUT[], int l)
{
	int j, re = int(n / (l + 1));
	if ((l + 1)*re > n)
		re--;
	j = 0;
	for (int i = 0; i < l; i++)
	{
		_OUT[i] = _IN[j];
		j += re;
	}
	return 0;
}

double FUNC(double a, double b, double c, double x)
{
	return 1 - exp(-b / x) - a*(1 - exp(-c / x));
}

double root_of_equation(double a, double b, double c, double _x, double acc, int N_op)
{
	// Перейдем к значению, которое находится в пределах 3 эВ от корня, это поможет избежать некоторых ошибок
	double F = FUNC(a, b, c, _x), step = acc/2;
	double x_c = _x;
	int i = 0;
	if(F < 0)
		while ((F < 0) & (i < N_op) & (x_c > 0))
		{
			x_c -= step;
			F = FUNC(a, b, c, x_c);
			i++;
		}
	if ((F > 0) & i == 0)
		while ((F > 0) & (i < N_op) & (x_c < 1000))
		{
			x_c += step;
			F = FUNC(a, b, c, x_c);
			i++;
		}
	if ((F < 0) & (i == N_op))
	{
		i = 0;
		x_c = _x;
		while ((F < 0) & (i < N_op) & (x_c < 1000))
		{
			x_c += step;
			F = FUNC(a, b, c, x_c);
			i++;
		}
	}
	if ((F > 0) & (i == N_op))
	{
		i = 0;
		x_c = _x;
		while ((F > 0) & (i < N_op) & (x_c > 0))
		{
			x_c -= step;
			F = FUNC(a, b, c, x_c);
			i++;
		}
	}
	// Если достигнуто максимальное значение, а корня не найдено, то результата нет
	if (i == N_op)
		return -1;
	// Если х примет значение нан
	if (x_c != x_c)
		return -1;
	if (x_c < 0)
		return -1;
	return x_c;
}
