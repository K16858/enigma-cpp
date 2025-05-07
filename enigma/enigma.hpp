#ifndef ENIGMA_HPP
#define ENIGMA_HPP

#include "parts.hpp"
#include <string>
#include <vector>

class Enigma {
private:
    static const int MAX_ROTORS = 20;
    int num_rotors;
    Rotor rotors[MAX_ROTORS];
    Reflector reflector;
    PlugBoard plugboard;
    int counters[MAX_ROTORS];
    int rotor_outputs[MAX_ROTORS];
public:
    Enigma(std::vector<std::string>& codes, std::string& wiring, char node[]);
    void display_rotors();
    void display_setting();
    char translate_char(char plain_character, bool display_process = false);
    std::string translate_text(std::string plain_text, bool display_process = false);
    void set_rotor(void);
};

#endif // ENIGMA_HPP
