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

int GetIntIndexDigit(int index, unsigned int key) {
	unsigned int key_length = 1;

	while (key / pow(10, key_length) >= 1) {
		key_length++;
	};

	unsigned int key_divider = pow(10, key_length - index);

	return (1 >= key_divider) ? 0 : floor((key % key_divider) / (key_divider / 10));
}


void encriptMessage() {
	std::cout << "Please, enter input message" << std::endl;

	std::string user_input_msg;
	std::getline(std::cin, user_input_msg);

	/* Convert input variable pointer into decimal */
	std::stringstream ss;
	ss << &user_input_msg;
	std::string hex_key = ss.str().replace(0, 2, "").substr(0, kMaxKeyLength);
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
			errno = 1;
			return;
		}
		else {
			/* Add general code padding */
			char_asci_val += char_value_padding;

			/* Add index and index+1 padding squared */
			for (int indexValue : {index, index + 1}) {
				unsigned int inted_to_get = AdujstIntToBound(indexValue, 0, kMaxKeyLength - 1);

				char_asci_val += pow(
						GetIntIndexDigit(inted_to_get, decimal_key)
					, 2);
			}


			/* Make sure new char is in bounds*/
			output_message += char(AdujstIntToBound(char_asci_val, 32, 126));

		}
	}


	std::cout << "Output message:" << std::endl << output_message << double_endl;
	std::cout << "Decript key:" << std::endl << decimal_key << std::endl;
}

void decriptMessage() {
	std::cout << "Please, enter input message" << std::endl;

	std::string user_input_msg;
	std::getline(std::cin, user_input_msg);

	std::cout << "Please, enter decription code" << std::endl;

	int message_key;
	std::cin >> message_key;

	/* Generate random number between [0-kMaxCharCodePadding-1] */
	/* Range is decreased by 1 becouse 0 and maxValue would gave the same resoult */
	int char_value_padding = message_key % (kMaxCharCodePadding - 1);
	std::string output_message;

	for (int index = 0; index < user_input_msg.length(); index++) {
		int char_asci_val = int(user_input_msg[index]);

		/* Input validation */
		if (char_asci_val > 126 || char_asci_val < 32) {
			std::cout << "Bad input" << std::endl;
			errno = 1;
			return;
		}
		else {
			/* Add index and index+1 padding squared */
			for (int indexValue : {index, index + 1}) {
				unsigned int inted_to_get = AdujstIntToBound(indexValue, 0, kMaxKeyLength - 1);

				char_asci_val -= pow(
					GetIntIndexDigit(inted_to_get, message_key)
					, 2);
			}

			/* Add general code padding */
			char_asci_val -= char_value_padding;


			/* Make sure new char is in bounds*/
			output_message += char(AdujstIntToBound(char_asci_val, 32, 126));

		}
	}


	std::cout << "Output message:" << std::endl << output_message << double_endl;
};


int main() {
	std::cout << "Welcome!" << std::endl << "Please select which operation do you wanna perform:" << double_endl;
	std::cout << "[1] -> Encodeing" << std::endl;
	std::cout << "[2] -> Decodeing" << std::endl;

	int operation_type;
	std::cin >> operation_type;

	std::cin.ignore();

	if (operation_type == 1) {
		encriptMessage();
	}
	else {
		decriptMessage();
	}

	return errno;
}