#include <iostream>
#include <sstream>

unsigned int const kMaxCharCodePadding = 95;
unsigned int const kMaxKeyLength = 8;

struct double_endl_impl {} double_endl;

std::ostream& operator<<(std::ostream& out, double_endl_impl)
{
	return out << "\n\n" << std::flush;
}


int AdujstIntToBound(int int_to_adjust, int min, int max) {
	bool is_too_large = int_to_adjust > max;

	while (is_too_large || int_to_adjust < min) {
		int_to_adjust -= (max - min + 1) * (is_too_large ? 1 : -1);
		is_too_large = int_to_adjust > max;
	}

	return int_to_adjust;
}

int GetIntIndexDigit(int index, unsigned int integer) {
	unsigned int key_length = 1;

	while (integer / pow(10, key_length) >= 1) {
		key_length++;
	};

	unsigned int key_divider = pow(10, key_length - index);

	return (1 >= key_divider) ? 0 : floor((integer % key_divider) / (key_divider / 10));
}

int main() {
	std::cout << "Welcome!" << std::endl << "Please select which operation do you wanna perform:" << double_endl;
	std::cout << "[1] -> Encodeing" << std::endl;
	std::cout << "[2] -> Decodeing" << std::endl;

	int operation_type {};
	std::cin >> operation_type;
	bool is_encription = operation_type == 1;

	std::cout << std::endl << "Please, enter input message" << std::endl;
	std::cin.ignore();

	std::string user_input_msg;
	std::getline(std::cin, user_input_msg);

	unsigned int message_key {};
	if (is_encription) {
		std::stringstream ss;
		ss << &user_input_msg;
		std::string hex_key = ss.str().replace(0, 2, "").substr(0, kMaxKeyLength);
		message_key = std::stoll(hex_key, nullptr, 16);
	}
	else {
		std::cout << std::endl << "Please, enter decription code" << std::endl;
		std::cin >> message_key;
	}

	int char_value_padding = message_key % (kMaxCharCodePadding - 1);
	std::string output_message;

	for (int index = 0; index < user_input_msg.length(); index++) {
		int char_asci_val = int(user_input_msg[index]);

		/* Input validation */
		if (char_asci_val > 126 || char_asci_val < 32) {
			std::cout << "Bad input" << std::endl;
			return 1;
		}
		else {
			int key_padding_value {};

			/* Add general code padding */
			key_padding_value += char_value_padding;

			/* Add index and index+1 padding squared */
			for (int indexValue : {index, index + 1}) {
				unsigned int inted_to_get = AdujstIntToBound(indexValue, 0, kMaxKeyLength - 1);

				key_padding_value += pow(
					GetIntIndexDigit(inted_to_get, message_key)
					, 2);
			}

			/* ASCI modification */
			char_asci_val += key_padding_value * (is_encription ? 1 : -1);

			/* Make sure new char is in bounds*/
			output_message += char(AdujstIntToBound(char_asci_val, 32, 126));
		}
	}

	std::cout << std::endl << "Message:" << std::endl << output_message << double_endl;

	if (is_encription) {
		std::cout << "Decript key:" << std::endl << message_key << std::endl;
	}

	return 0;
}