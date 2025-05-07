#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

class Rotor
{
    private:
        std::string ring;
        std::string scrambler;
    public:
        Rotor() = default;

        Rotor(std::string code)
        {
            ring = R"(!"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~)";
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

            std::cout << "Invalid number!" << std::endl;
            return -1;
        }

        int back_scrambling(int input)
        {
            for(size_t i=0;i<scrambler.size();i++){
                if(scrambler[i]==ring[input]){
                    return i;
                }
            }

            std::cout << "Invalid number!" << std::endl;
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

            std::cout << "Invalid number!" << std::endl;
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
        static const int MAX_ROTORS = 20;
        int num_rotors;
        Rotor rotors[MAX_ROTORS];
        Reflector reflector;
        PlugBoard plugboard;

        int counters[MAX_ROTORS];
        int rotor_outputs[MAX_ROTORS];

    public:
        Enigma(std::vector<std::string>& codes ,std::string& wiring, char node[])
            : reflector(wiring), plugboard(node), num_rotors(codes.size())
        {
            for(int i=0;i<num_rotors;i++){
                rotors[i] = Rotor(codes[i]);
                counters[i] = 0;
            }
        }

        void display_rotors()
        {
            for(int i=0; i<num_rotors; i++){
                std::cout << "Rotor " << i+1 << std::endl;
                rotors[i].display_scrambler();
            }
        }

        void display_setting()
        {
            reflector.display_code();
            plugboard.display_connection();
        }

        char translate_char(char plain_character, bool display_process = false)
        {
            char exchange_character = plugboard.exchange_char(plain_character);
            if(display_process){
                std::cout << plain_character << " -> " << exchange_character;
            }
            int input = exchange_character - '!';
            char encrypted_character;

            rotors[0].rotate();
            counters[0] ++;
            for(int i=0;i<num_rotors-1;i++){
                if(counters[i] >= rotors[i].get_scrambler().length()){
                    counters[i] = 0;
                    rotors[i+1].rotate();
                    counters[i+1] ++;
                }
            }

            rotor_outputs[0] = rotors[0].forward_scrambling(input);
            if(display_process){
                std::cout << " -> " << rotor_outputs[0];
            }
            for(int i=1;i<num_rotors;i++){
                rotor_outputs[i] = rotors[i].forward_scrambling(rotor_outputs[i-1]);
                if(display_process){
                    std::cout << " -> " << rotor_outputs[i];
                }
            }

            int reflector_output = reflector.reflect(rotor_outputs[num_rotors-1]);
            rotor_outputs[num_rotors-1] = rotors[num_rotors-1].back_scrambling(reflector_output);
            if(display_process){
                std::cout << " -> " << rotor_outputs[num_rotors-1];
            }
            for(int i=num_rotors-2;i>-1;i--){
                rotor_outputs[i] = rotors[i].back_scrambling(rotor_outputs[i+1]);
                if(display_process){
                    std::cout << " -> " << rotor_outputs[i];
                }
            }

            encrypted_character = rotor_outputs[0] + '!';

            if(display_process){
                std::cout << " -> " << encrypted_character << std::endl;
            }

            encrypted_character = plugboard.exchange_char(encrypted_character);

            return encrypted_character;
        }

        std::string translate_text(std::string plain_text, bool display_process = false)
        {
            std::string encrypted_text;
            for(char c : plain_text){
                encrypted_text += translate_char(c, display_process);
            }

            return encrypted_text;
        }

        void set_rotor(void)
        {
            char point;
            for(int i=0;i<num_rotors;i++){
                std::cout << "Rotor " << i + 1 << ": ";
                std::cin >> point;
                rotors[i].set_start_point(point);
                counters[i] = 0;
            }
        }
};

int main(void){
    std::string r1 = R"(?_ed%RQ6U/pxS9#qBNhOX>F;-&{EI1!.JZCtbMD4u*az:k73f|L($j"0+mHgryKn,sPwv8T5'^i<[}WlAV2\@~G])=o`cY)";
    std::string r2 = R"(erqi:tP"|d+l<paWD$!bn0`@-[);>9ySf\Cg~]8zkN%?F3=R/}6uv7HwUjsx{.&IL_YhEoMc2'#4^(5BVOJT*mA1XKQZG,)";
    std::string r3 = R"(2>)hG'|Ha]rk<z/x{[#0^OXuMnZLdJU_RYQ9wB7cVvNmE61~`,*&?=4P8%yCD;}ioKp"j:q3-ft+el$@!sTAFgIW(Sb5.\)";
    std::string w1 = R"(QWERTYUIOPASDFGHJKLZXCVBNMPLOKMIJNUHBYGVTFCRDXESZWAQqwertyuioppoiuytrewq12345678900987654321==)";

    std::vector<std::string> rotors = {r1, r2, r3};
    char node[6] = {'A','B', 'F', 'C', 'V', 'M'};

    Enigma enigma(rotors, w1, node);
    
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
        else if(text == "!show"){
            enigma.display_rotors();
        }
        else{
            std::cout << "Output: " << enigma.translate_text(text) << std::endl;
        }
    }
    return 0;
}