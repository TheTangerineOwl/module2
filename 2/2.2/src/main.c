#include <stdio.h>
#include "calc.h"

int main()
{
	int choice;
	int argNum;

	while (1)
	{
		printf("Выберите операцию:\n");
		printf("1. +;\n");
		printf("2. -;\n");
		printf("3. *;\n");
		printf("4. /;\n");
		printf("5. Выход.\nВыбор: ");
		scanf("%d", &choice);

		if (choice < 1 || choice > 5)
		{
			printf("Некорректный ввод! Попробуйте еще раз.\n");
			continue;
		}
		else if (choice == 5)
			break;

		double *(*op)(int, ...);

		switch (choice)
		{
		case 2:
		{
			op = sub;
			puts("Выбрано вычитание.");
		}
		break;
		case 3:
		{
			op = mult;
			puts("Выбрано умножение.");
		}
		break;
		case 4:
		{
			op = divide;
			puts("Выбрано деление.");
		}
		break;
		default:
		{
			op = sum;
			puts("Выбрано сложение.");
		}
		break;
		}

		printf("Введите количество аргументов: ");
		while (!scanf("%d", &argNum) || argNum < 1 || argNum > MAX_ARGS)
		{
			printf("Некорректное число, попробуйте еще раз: ");
		}

		double nums[MAX_ARGS];
		for (int i = 0; i < argNum; i++)
		{
			printf("Введите число номер %d: ", i + 1);
			scanf("%lf", &nums[i]);
		}

		double *res;

		switch (argNum)
		{
		case 1:
		{
			res = op(argNum, nums[0]);
		}
		break;
		case 3:
		{
			res = op(argNum, nums[0], nums[1], nums[2]);
		}
		break;
		case 4:
		{
			res = op(argNum, nums[0], nums[1], nums[2], nums[3]);
		}
		break;
		case 5:
		{
			res = op(argNum, nums[0], nums[1], nums[2], nums[3], nums[4]);
		}
		break;
		default:
		{
			res = op(argNum, nums[0], nums[1]);
		}
		}

		if (res)
			printf("Результат: %lf\n", *res);
		else
			printf("Ошибка!\n");
	}
}