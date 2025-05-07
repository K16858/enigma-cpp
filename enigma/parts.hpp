#ifndef PARTS_HPP
#define PARTS_HPP

#include <string>
#include <iostream>
#include <unordered_set>

// Rotorクラス：暗号化のための回転子
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

// Reflectorクラス：信号を反射する
class Reflector {
private:
    std::string wiring;
public:
    Reflector(std::string code);
    void display_code(void);
    int reflect(int input);
};

// PlugBoardクラス：文字の交換を行う
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

#endif // PARTS_HPP
