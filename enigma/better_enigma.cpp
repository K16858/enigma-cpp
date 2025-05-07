#include "better_enigma.hpp"

Rotor::Rotor(std::string code) {
    ring = R"(!"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~)";
    scrambler = code;
}

void Rotor::display_scrambler(void) {
    for (size_t i = 0; i < scrambler.size(); i++) {
        std::cout << i << ":" << ring[i] << ":" << scrambler[i] << std::endl;
    }
    std::cout << std::endl;
}

std::string Rotor::get_scrambler(void) {
    return scrambler;
}

int Rotor::forward_scrambling(int input) {
    for (size_t i = 0; i < scrambler.size(); i++) {
        if (scrambler[input] == ring[i]) {
            return i;
        }
    }
    std::cout << "Invalid number!" << std::endl;
    return -1;
}

int Rotor::back_scrambling(int input) {
    for (size_t i = 0; i < scrambler.size(); i++) {
        if (scrambler[i] == ring[input]) {
            return i;
        }
    }
    std::cout << "Invalid number!" << std::endl;
    return -1;
}

void Rotor::rotate() {
    scrambler = scrambler.substr(1) + scrambler[0];
    ring = ring.substr(1) + ring[0];
}

void Rotor::set_start_point(char point) {
    while (ring[0] != point) {
        rotate();
    }
}

Reflector::Reflector(std::string code) {
    wiring = code;
}

void Reflector::display_code(void) {
    for (size_t i = 0; i < wiring.size(); i++) {
        std::cout << wiring[i] << std::endl;
    }
    std::cout << std::endl;
}

int Reflector::reflect(int input) {
    for (size_t i = 0; i < wiring.size(); i++) {
        if (wiring[i] == wiring[input] && i != input) {
            return i;
        }
    }
    std::cout << "Invalid number!" << std::endl;
    return -1;
}

PlugBoard::PlugBoard(char node[]) {
    plug1_node1 = node[0];
    plug1_node2 = node[1];
    plug2_node1 = node[2];
    plug2_node2 = node[3];
    plug3_node1 = node[4];
    plug3_node2 = node[5];
}

void PlugBoard::display_connection(void) {
    std::cout << plug1_node1 << " <-> " << plug1_node2 << std::endl;
    std::cout << plug2_node1 << " <-> " << plug2_node2 << std::endl;
    std::cout << plug3_node1 << " <-> " << plug3_node2 << std::endl;
    std::cout << std::endl;
}

bool PlugBoard::check_connection(void) {
    std::unordered_set<char> plugs;
    for (char c : {plug1_node1, plug1_node2, plug2_node1, plug2_node2, plug3_node1, plug3_node2}) {
        if (plugs.count(c)) {
            return false;
        }
        plugs.insert(c);
    }
    return true;
}

char PlugBoard::exchange_char(char input_char) {
    if (input_char == plug1_node1) {
        return plug1_node2;
    } else if (input_char == plug1_node2) {
        return plug1_node1;
    } else if (input_char == plug2_node1) {
        return plug2_node2;
    } else if (input_char == plug2_node2) {
        return plug2_node1;
    } else if (input_char == plug3_node1) {
        return plug3_node2;
    } else if (input_char == plug3_node2) {
        return plug3_node1;
    } else {
        return input_char;
    }
}

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
