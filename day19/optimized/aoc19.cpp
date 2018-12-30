#include <iostream>

int main()
{
	int target = 10551296;
	int sum = 0;
	for (int x = 1; x <= target; x++) {
		if (target % x == 0)
			sum += x;
	}
	std::cout << sum << '\n';
}

