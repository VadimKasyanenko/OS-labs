#include <stdlib.h>
#include <iostream>

void main(int argc, char* argv[]) {
	if (argc > 1) {	
		int n = atoi(argv[1]);
		int k = atoi(argv[2]);
		std::cout << "n = " << n << ", k = " << k << "\nArray: ";
		for (int i = 3; i < argc; ++i) {
			std::cout << argv[i] << " ";
		}
		unsigned int result = 1;
		for (int i = 3; i <= 3 + k; ++i) {
			if (atof(argv[i]) != 0)
			{
				result *= atof(argv[i]);
			}
		}
		std::cout << "\nProduct of elements from Array[0] to Array[" << k << "] = " << result << std::endl;
	}
	else {
		std::cout << "Error!" << std::endl;
	}
	system("pause");
}