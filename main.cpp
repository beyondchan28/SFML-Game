#include "Game.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Vec2.h"

#include <memory>
#include <vector>


int main()
{
    std::string key1 = "fuck";
    std::shared_ptr<int> val1 (new int(33));
    std::vector<std::shared_ptr<int>> vec1 {val1};

    std::string key2 = "duck";
    std::shared_ptr<int> val2 (new int(22));
    std::vector<std::shared_ptr<int>> vec2 {val2};

    std::shared_ptr<int> val3 (new int(666));

    std::map<std::string, std::vector<std::shared_ptr<int>>> tMap {{key1, vec1}, {key2, vec2}};

    std::string test = "fuck";
    std::cout << "heck ? "  << test << "\n";

    for (auto & [key, val] : tMap)
    {
        if(key == "duck")
        {
            tMap[key].push_back(val3);
            //std::cout << *tMap[key].back() << "\n";
            //findVal.push_back(new int(5));
        }

        std::cout << key << " " << val.size() << "\n";
    }



    Game game;
    game.run();


    return 0;

}
