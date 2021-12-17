/// <summary>
/// author Pete Lowe May 2019
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupFontAndText();
	void setupSprite();
	void animateHelo();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	sf::Texture m_heloTexture; // texture for helo sprite sheet
	sf::Sprite m_helicopter; // sprite for helo
	bool m_exitGame; // control exiting game

	int m_currentFrame = 0; // helo frame to draw
	float m_currentFrameCounter = 0.0f; // frame value float
	float m_frameIncrement =0.24; // add to counter each gsmre frame 60fps


};

#endif // !GAME_HPP

