#include <iostream>
#include <string>
#include <unordered_set>

class Rotor{
    private:
        std::string ring;
        std::string scrambler;
    public:
        Rotor(std::string code)
        {
            ring = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            scrambler = code;
        }

        void display_scrambler(void)
        {
            for(size_t i=0;i<scrambler.size();i++){
                std::cout << i << ":" << ring[i] << ":" << scrambler[i] << std::endl;
            }
            std::cout << std::endl;
        }

        std::string get_scrambler(void)
        {
            return scrambler;
        }

        int forward_scrambling(int input)
        {
            for(size_t i=0;i<scrambler.size();i++){
                if(scrambler[input]==ring[i]){
                    return i;
                }
            }

            return -1;
        }

        int back_scrambling(int input)
        {
            for(size_t i=0;i<scrambler.size();i++){
                if(scrambler[i]==ring[input]){
                    return i;
                }
            }

            return -1;
        }

        void rotate()
        {
            scrambler = scrambler.substr(1) + scrambler[0];
            ring = ring.substr(1) + ring[0];
        }

        void set_start_point(char point)
        {
            while(ring[0] != point)
            {
                rotate();
            }
        }
};

class Reflector
{
    private:
        std::string wiring;
    public:
        Reflector(std::string code)
        {
            wiring = code;
        }

        void display_code(void)
        {
            for(size_t i=0;i<wiring.size();i++){
                std::cout << wiring[i] << std::endl;
            }
            std::cout << std::endl;            
        }

        int reflect(int input)
        {
            for(size_t i=0;i<wiring.size();i++){
                if(wiring[i]==wiring[input] && i!=input){
                    return i;
                }
            }

            return -1; 
        }
};

class PlugBoard
{
    private:
        char plug1_node1;
        char plug1_node2;

        char plug2_node1;
        char plug2_node2;

        char plug3_node1;
        char plug3_node2;
    public:
        PlugBoard(char node[])
        {
            plug1_node1 = node[0];
            plug1_node2 = node[1];
            plug2_node1 = node[2];
            plug2_node2 = node[3];
            plug3_node1 = node[4];
            plug3_node2 = node[5];
        }

        void display_connection(void)
        {
            std::cout << plug1_node1 << " <-> " << plug1_node2 << std::endl;
            std::cout << plug2_node1 << " <-> " << plug2_node2 << std::endl;
            std::cout << plug3_node1 << " <-> " << plug3_node2 << std::endl;
            std::cout << std::endl;
        }

        bool check_connection(void)
        {
            //　集合を扱う
            std::unordered_set<char> plugs;
            for(char c : {plug1_node1, plug1_node2, plug2_node1, plug2_node2, plug3_node1, plug3_node2}){
                if(plugs.count(c)){
                    return false;
                }
                plugs.insert(c);
            }

            return true;
        }

        char exchange_char(char input_char)
        {
            if(input_char == plug1_node1){
                return plug1_node2;
            }
            else if(input_char == plug1_node2){
                return plug1_node1;
            }
            else if(input_char == plug2_node1){
                return plug2_node2;
            }
            else if(input_char == plug2_node2){
                return plug2_node1;
            }
            else if(input_char == plug3_node1){
                return plug3_node2;
            }
            else if(input_char == plug3_node2){
                return plug3_node1;
            }
            else{
                return input_char;
            }
        }
};

class Enigma
{
    private:
        Rotor first_rotor;
        Rotor second_rotor;
        Rotor third_rotor;
        Reflector reflector;
        PlugBoard plugboard;

        int first_counter;
        int second_counter;

        int first_rotor_output;
        int second_rotor_output;
        int third_rotor_output;
        int Reflector_output;
    public:
        Enigma(std::string& code1, std::string& code2, std::string& code3, std::string& wiring, char node[])
            : first_rotor(code1), second_rotor(code2), third_rotor(code3), reflector(wiring), plugboard(node)
        {
            first_counter = 0;
            second_counter = 0;
        }
        // 参照型メンバ変数は初期化リストを使わないと初期化できない

        void display_rotors()
        {
            std::cout << "First Rotor:" << std::endl;
            first_rotor.display_scrambler();
            std::cout << "Second Rotor:" << std::endl;
            second_rotor.display_scrambler();
            std::cout << "Third Rotor:" << std::endl;
            third_rotor.display_scrambler();
        }

        void display_setting()
        {
            display_rotors();
            reflector.display_code();
            plugboard.display_connection();
        }

        char translate_char(char plain_character, bool show_result = false)
        {
            char exchange_character = plugboard.exchange_char(plain_character);
            int input = exchange_character - 'A';
            char encrypted_character;

            first_rotor.rotate();
            first_counter ++;
            if(first_counter >= first_rotor.get_scrambler().length()){
                first_counter = 0;
                second_rotor.rotate();
                second_counter ++;
            }
            if(second_counter >= second_rotor.get_scrambler().length()){
                second_counter = 0;
                third_rotor.rotate();
            }

            first_rotor_output = first_rotor.forward_scrambling(input);
            second_rotor_output = second_rotor.forward_scrambling(first_rotor_output);
            third_rotor_output = third_rotor.forward_scrambling(second_rotor_output);

            Reflector_output = reflector.reflect(third_rotor_output);

            third_rotor_output = third_rotor.back_scrambling(Reflector_output);
            second_rotor_output = second_rotor.back_scrambling(third_rotor_output);
            first_rotor_output = first_rotor.back_scrambling(second_rotor_output);

            encrypted_character = first_rotor_output + 'A';

            encrypted_character = plugboard.exchange_char(encrypted_character);

            return encrypted_character;
        }

        std::string translate_text(std::string plain_text, bool show_result = false)
        {
            std::string encrypted_text;
            for(char c : plain_text){
                encrypted_text += translate_char(c, show_result);
            }

            return encrypted_text;
        }

        void set_rotor(void)
        {
            char point;
            std::cout << "First rotor start point: ";
            std::cin >> point;
            first_rotor.set_start_point(point);
            std::cout << "Second rotor start point: ";
            std::cin >> point;
            second_rotor.set_start_point(point);
            std::cout << "Third rotor start point: ";
            std::cin >> point;
            third_rotor.set_start_point(point);            
        }
};

int main(void){
    std::string r1 = "QWERTYUIOPASDFGHJKLZXCVBNM";
    std::string r2 = "MNBVCXZLKJHGFDSAPOIUYTREWQ";
    std::string r3 = "PLOKMIJNUHBYGVTFCRDXESZWAQ";
    std::string w1 = "QWERTYUIOPPOIUYTREWQASDDSA";
    char node[6] = {'A','B', 'F', 'C', 'V', 'M'};

    Enigma enigma(r1, r2, r3, w1, node);
    
    std::string text = "";
    while(1)
    {
        std::cout << "Input: ";
        std::cin >> text;
        if(text == "!exit"){
            break;
        }
        else if(text == "!set"){
            enigma.set_rotor();
        }
        else{
            std::cout << "Output: " << enigma.translate_text(text) << std::endl;
        }
    }
    return 0;
}