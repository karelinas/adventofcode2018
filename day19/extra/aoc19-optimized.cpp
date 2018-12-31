#include <iostream>
#include <cmath>

int main()
{
	int target = 10551296;
	int sum = 0;
	int end = sqrt(target);
	for (int x = 1; x <= end; x++) {
		if (target % x == 0) {
			sum += x;
			if ((x*x) != target)
				sum += (target / x);
		}
	}
	std::cout << sum << '\n';
}

