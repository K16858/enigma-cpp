#ifndef BETTER_ENIGMA_HPP
#define BETTER_ENIGMA_HPP

#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>

class Rotor {
private:
    std::string ring;
    std::string scrambler;
public:
    Rotor() = default;
    Rotor(std::string code);
    void display_scrambler(void);
    std::string get_scrambler(void);
    int forward_scrambling(int input);
    int back_scrambling(int input);
    void rotate();
    void set_start_point(char point);
};

class Reflector {
private:
    std::string wiring;
public:
    Reflector(std::string code);
    void display_code(void);
    int reflect(int input);
};

class PlugBoard {
private:
    char plug1_node1;
    char plug1_node2;
    char plug2_node1;
    char plug2_node2;
    char plug3_node1;
    char plug3_node2;
public:
    PlugBoard(char node[]);
    void display_connection(void);
    bool check_connection(void);
    char exchange_char(char input_char);
};

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

#endif
