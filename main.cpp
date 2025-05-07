#include "enigma/better_enigma.hpp"
#include <iostream>
#include <string>

int main() {
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
