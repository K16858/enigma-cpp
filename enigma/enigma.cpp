#include "enigma.hpp"

Enigma::Enigma(std::vector<std::string>& codes, std::string& wiring, char node[])
    : reflector(wiring), plugboard(node), num_rotors(codes.size()) {
    for (int i = 0; i < num_rotors; i++) {
        rotors[i] = Rotor(codes[i]);
        counters[i] = 0;
    }
}

void Enigma::display_rotors() {
    for (int i = 0; i < num_rotors; i++) {
        std::cout << "Rotor " << i + 1 << std::endl;
        rotors[i].display_scrambler();
    }
}

void Enigma::display_setting() {
    reflector.display_code();
    plugboard.display_connection();
}

char Enigma::translate_char(char plain_character, bool display_process) {
    char exchange_character = plugboard.exchange_char(plain_character);
    if (display_process) {
        std::cout << plain_character << " -> " << exchange_character;
    }
    int input = exchange_character - '!';
    char encrypted_character;

    rotors[0].rotate();
    counters[0]++;
    for (int i = 0; i < num_rotors - 1; i++) {
        if (counters[i] >= rotors[i].get_scrambler().length()) {
            counters[i] = 0;
            rotors[i + 1].rotate();
            counters[i + 1]++;
        }
    }

    rotor_outputs[0] = rotors[0].forward_scrambling(input);
    if (display_process) {
        std::cout << " -> " << rotor_outputs[0];
    }
    for (int i = 1; i < num_rotors; i++) {
        rotor_outputs[i] = rotors[i].forward_scrambling(rotor_outputs[i - 1]);
        if (display_process) {
            std::cout << " -> " << rotor_outputs[i];
        }
    }

    int reflector_output = reflector.reflect(rotor_outputs[num_rotors - 1]);
    rotor_outputs[num_rotors - 1] = rotors[num_rotors - 1].back_scrambling(reflector_output);
    if (display_process) {
        std::cout << " -> " << rotor_outputs[num_rotors - 1];
    }
    for (int i = num_rotors - 2; i > -1; i--) {
        rotor_outputs[i] = rotors[i].back_scrambling(rotor_outputs[i + 1]);
        if (display_process) {
            std::cout << " -> " << rotor_outputs[i];
        }
    }

    encrypted_character = rotor_outputs[0] + '!';

    if (display_process) {
        std::cout << " -> " << encrypted_character << std::endl;
    }

    encrypted_character = plugboard.exchange_char(encrypted_character);

    return encrypted_character;
}

std::string Enigma::translate_text(std::string plain_text, bool display_process) {
    std::string encrypted_text;
    for (char c : plain_text) {
        encrypted_text += translate_char(c, display_process);
    }

    return encrypted_text;
}

void Enigma::set_rotor(void) {
    char point;
    for (int i = 0; i < num_rotors; i++) {
        std::cout << "Rotor " << i + 1 << ": ";
        std::cin >> point;
        rotors[i].set_start_point(point);
        counters[i] = 0;
    }
}
