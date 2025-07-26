#include <stdio.h>
#include "calc.h"

Operations operations[] = {
	{"+", sum},
	{"-", sub},
	{"*", mult},
	{"/", divide}};

int main()
{
	int choice, argNum;
	double *res;
	int operationsCount = sizeof(operations) / sizeof(operations[0]);

	while (1)
	{

		printf("Выберите операцию:\n");
		for (int j = 0; j < operationsCount; j++)
			printf("%d. %s;\n", j + 1, operations[j].str);
		printf("%d. Выход. ", operationsCount + 1);
		printf("Выбор: ");
		scanf("%d", &choice);

		if (choice == operationsCount + 1)
			return 0;
		else if (choice < 1 || choice > operationsCount + 1)
		{
			printf("Некорректный выбор! Попробуйте еще раз.\n");
			continue;
		}

		double *(*op)(int, ...) = operations[choice - 1].op;

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

		res ? printf("Результат: %lf\n", *res) : printf("Ошибка!\n");
	}
}