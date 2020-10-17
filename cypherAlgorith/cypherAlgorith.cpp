#include <iostream>
#include <sstream>

unsigned int const kMaxCharCodePadding = 95;

/// <summary>
/// Parse Int to range. Input is changed by 'change_value' until its reaches bounds
/// </summary>
int AdujstIntToBound(int int_to_adjust, int min, int max, int change_value) {
	bool is_too_large = int_to_adjust > max;
	while (is_too_large || int_to_adjust < min) {
		int_to_adjust -= change_value * (is_too_large ? 1 : -1);
		is_too_large = int_to_adjust > max;
	}

	return int_to_adjust;
}

int GetIntIndexDigit(int index, unsigned int key) {
	unsigned int key_length = 1;

	while (key / pow(10, key_length) >= 1) {
		key_length++;
	};

	unsigned int key_divider = pow(10, key_length - index);

	return floor((key % key_divider) / (key_divider / 10));
}


/// <returns>
/// 0->ok;
/// 1->bad user input;
/// </returns>
int main() {
	/* User greetings */
	std::cout << "Hello" << std::endl;
	std::cout << "Please, enter input message" << std::endl;

	/* Get user input */
	std::string user_input_msg;
	std::getline(std::cin, user_input_msg);
	/*std::cin >> user_input_msg;*/

	/* Convert input variable pointer into decimal */
	std::stringstream ss;
	ss << &user_input_msg;
	std::string hex_key = ss.str().replace(0, 2, "").substr(0, 8);
	unsigned int decimal_key = std::stoll(hex_key, nullptr, 16);

	/* Generate random number between [0-kMaxCharCodePadding-1] */
	/* Range is decreased by 1 becouse 0 and maxValue would gave the same resoult */
	int char_value_padding = decimal_key % (kMaxCharCodePadding - 1);
	std::string output_message;

	for (int index = 0; index < user_input_msg.length(); index++) {
		int char_asci_val = int(user_input_msg[index]);

		/* Input validation */
		if (char_asci_val > 126 || char_asci_val < 32) {
			std::cout << "Bad input" << std::endl;
			return 1;
		}
		else {
			/* Add general code padding */
			char_asci_val += char_value_padding;

			/* Add index and index+1 padding squared */
			for (int indexValue : {index, index + 1}) {
				char_asci_val += pow(
					GetIntIndexDigit(
						AdujstIntToBound(indexValue, 0, hex_key.length() - 1, hex_key.length()), decimal_key
					), 2);
			}


			/* Make sure new char is in bounds*/
			output_message += char(AdujstIntToBound(char_asci_val, 32, 126, kMaxCharCodePadding));

		}
	}


	std::cout << "Output message:" << std::endl << output_message << std::endl << std::endl;
	std::cout << "Decript key:" << std::endl << decimal_key << std::endl;
	return 0;
}