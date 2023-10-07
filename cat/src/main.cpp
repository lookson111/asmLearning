#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    int w = 900;
    int h = 800;

    sf::RenderWindow window(sf::VideoMode(w, h), "Cutting an object", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Shader shader;
    if (!shader.loadFromFile("../../src/shader.frag", sf::Shader::Fragment)) {
        std::cout << "Shader load error!!" << std::endl;
    }

    sf::RenderTexture firstTexture;
    firstTexture.create(w, h);
    sf::Sprite firstTextureSprite = sf::Sprite(firstTexture.getTexture());
    sf::Sprite firstTextureSpriteFlipped = sf::Sprite(firstTexture.getTexture());
    firstTextureSpriteFlipped.setScale(1, -1);
    firstTextureSpriteFlipped.setPosition(0.0, (float)h);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(firstTextureSpriteFlipped, &shader);
        window.display();
    }

    return 0;
}