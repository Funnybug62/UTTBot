#include "UTT.h"

int main() {
    UTT game(900, 900, 40.0f);
    game.Run();

    std::ifstream artFile("GameOver.txt");
    std::string line;
    while (std::getline(artFile, line)) { std::cout << line << std::endl; }
    artFile.close();
    
    return 0;
}
