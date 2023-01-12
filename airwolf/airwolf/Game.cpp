/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setupAudio();
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (sf::Event::MouseButtonPressed == newEvent.type)
		{
			procressMouse(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

void Game::procressMouse(sf::Event t_event)
{
	float lenght = 0.0f;
	sf::Vector2f displacement(0.0f, 0.0f);
	if (sf::Mouse::Middle == t_event.mouseButton.button)
	{
		displacement.x = static_cast<float>(t_event.mouseButton.x) - m_location.x;
		displacement.y = static_cast<float>(t_event.mouseButton.y) - m_location.y;
		m_target.x = t_event.mouseButton.x;
		m_target.y = t_event.mouseButton.y;
		lenght = std::sqrtf((displacement.x * displacement.x) + (displacement.y * displacement.y));
		displacement = displacement / lenght;
		displacement = displacement * m_speed;
		m_velocity = displacement;
		m_frameIncrement = 0.5;
		m_sound.setPitch(1.0f);
		if (static_cast<float>(t_event.mouseButton.x) > m_location.x)
		{
			m_facing = Direction::Right;
			m_helicopter.setScale(1.0, 1.0);			
		}
		else
		{
			m_facing = Direction::Left;
			m_helicopter.setScale(-1.0, 1.0);
			
		}

	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	animateHelo();
	move();
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	m_window.draw(m_welcomeMessage);
	m_window.draw(m_helicopter);
	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("Use middle button");
	
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(50U);
	
	m_welcomeMessage.setFillColor(sf::Color::Black);
	

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_heloTexture.loadFromFile("ASSETS\\IMAGES\\helicopter.png"))
	{
		std::cout << "problem with png file for helo" << std::endl;
	}
	m_helicopter.setTexture(m_heloTexture);
	m_helicopter.setTextureRect(sf::IntRect(0, 128, 180, 64));
	m_helicopter.setPosition(m_location);
	m_helicopter.setOrigin(90.0f, 32.0f);
}

void Game::animateHelo()
{
	int newFrameNumber = 0; // new current frame
	m_currentFrameCounter += m_frameIncrement;
	newFrameNumber = static_cast<int>(m_currentFrameCounter);
	newFrameNumber = newFrameNumber % 4; // frames 0,1,2,3
	if (newFrameNumber != m_currentFrame) // change the frame
	{
		m_currentFrame = newFrameNumber;
			//sf::IntRect(0, 0, 180, 64) // frame 0
			//sf::IntRect(0, 64, 180, 64) // 1
			//sf::IntRect(0, 128, 180, 64) // 2
			//sf::IntRect(0, 196, 180, 64) // 3

			m_helicopter.setTextureRect(sf::IntRect(0, 64 * m_currentFrame, 180, 64));
	}
}

void Game::move()
{
	if (m_facing != Direction::None)
	{
		m_location += m_velocity;
		m_helicopter.setPosition(m_location);
		if (m_facing == Direction::Right && m_location.x > m_target.x)
		{
			m_facing = Direction::None;
			m_frameIncrement = 0.25;
			m_sound.setPitch(0.5f);			
		}
		if (m_facing == Direction::Left && m_location.x < m_target.x)
		{
			m_facing = Direction::None;
			m_frameIncrement = 0.25;
			m_sound.setPitch(0.5f);
		}
	}
}

void Game::setupAudio()
{
	if (!m_buffer.loadFromFile("ASSETS\\AUDIO\\helicopter.wav"))
	{
		std::cout << "problem with audio";
	}
	m_sound.setBuffer(m_buffer);
	m_sound.setLoop(true);
	m_sound.setPitch(0.5f);
	m_sound.play();
}
