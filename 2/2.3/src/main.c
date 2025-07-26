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

struct {
    const char *str;
    int (*op) (double*, int, ...);
} operations[] = {
    {"Sum", sum},
    {"Subtract", sub},
    {"Multiply", mult},
    {"Divide", div}
};

int main() {
	int choice, argNum;
    double res, good;
    int operationsCount = sizeof(operations) / sizeof(operations[0]);

	while (1) {

		printf("Choose an operation:\n");
		for (size_t j = 0; j < operationsCount; j++)
            printf("%ld. %s;\n", j + 1, operations[j].str);
        printf("%d. Exit.\n", operationsCount + 1);
		printf("Your choice: ");
		scanf("%d", &choice);

		if (choice == operationsCount + 1)
            return 0;
        else if (choice < 1 || choice > operationsCount + 1)
		{
			printf("Incorrect choice! Try again.\n");
			continue;
		}

		int (*op) (double*, int, ...) = operations[choice - 1].op;

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

        good != -1 ? printf("Result: %lf\n", res) : printf("Error!\n");
	}
}