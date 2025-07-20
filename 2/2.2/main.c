#include <stdio.h>
#include <stdarg.h>

#define EPSILON 1e-15
#define MAX_ARGS 5

int sum(double* res, int argc, ...)
{
	*res = 0;
	va_list args;
	va_start(args, argc);
	for (size_t i = 0; i < argc; i++)
		*res += va_arg(args, double);
	va_end(args);
	return 0;
}

int sub(double* res, int argc, ...)
{
	va_list args;
	va_start(args, argc);
	*res = va_arg(args, double);
	for (size_t i = 1; i < argc; i++)
		*res -= va_arg(args, double);
	va_end(args);
	return 0;
}

int mult(double* res, int argc, ...)
{
	*res = 1;
	va_list args;
	va_start(args, argc);
	for (size_t i = 0; i < argc; i++)
		*res *= va_arg(args, double);
	va_end(args);
	return 0;
}

int div(double* res, int argc, ...)
{
	va_list args;
	va_start(args, argc);
	*res = va_arg(args, double);
	double d;
	for (size_t i = 1; i < argc; i++)
	{
		d = va_arg(args, double);
		if ((d > 0 && d <= EPSILON) || (d < 0 && d >= -EPSILON))
			return -1;
		*res /= d;
	}
	va_end(args);

	return 0;
}


int main() {
	int choice;
	int argNum;

	while (1) {

		printf("Choose an operation:\n");
		printf("1. Sum;\n");
		printf("2. Substract;\n");
		printf("3. Multiply;\n");
		printf("4. Divide.\nYour choice: ");
		scanf("%d", &choice);

		if (choice < 1 || choice > 4)
		{
			printf("Incorrect choice! Try again.\n");
			continue;
		}

		printf("Enter number of arguments: ");
		while (!scanf("%d", &argNum) || argNum < 1 || argNum > MAX_ARGS)
		{
			printf("Incorrent number, try again: ");
		}
		
		double nums[MAX_ARGS];
		for (int i = 0; i < argNum; i++) {
			printf("Enter number %d: ", i + 1);
			scanf("%lf", &nums[i]);
		}

		int (*op) (double*, int, ...);

		switch (choice)
		{
		case 2:
		{
			op = sub;
		} break;
		case 3:
		{
			op = mult;
		} break;
		case 4:
		{
			op = div;
		} break;
		default:
		{
			op = sum;
		} break;
		}

		double res;
		int good;

		switch (argNum)
		{
		case 1: {
			good = op(&res, argNum, nums[0]);
		} break;
		case 2: {
			good = op(&res, argNum, nums[0], nums[1]);
		} break;
		case 3: {
			good = op(&res, argNum, nums[0], nums[1], nums[2]);
		} break;
		case 4: {
			good = op(&res, argNum, nums[0], nums[1], nums[2], nums[3]);
		} break;
		case 5: {
			good = op(&res, argNum, nums[0], nums[1], nums[2], nums[3], nums[4]);
		} break;
		default: {
			good = op(&res, argNum, nums[0], nums[1]);
		}
		}

		if (good != -1)
			printf("Result: %lf\n", res);
		else
			printf("Error!\n");
	}
}