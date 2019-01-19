#include "TypingControls.h"

bool TypingControls::CheckWords(string &ActionOutput)
{
	shoot = false;
	bool _complete = false;
	bool _clear = true;
	int _newIndex = 0;

	for (std::map<string, string>::iterator it = actionWords.begin(); it != actionWords.end(); ++it)
	{
		string _action;
		string _comparison;
		
		_action.append(it->first);
		_comparison.append(it->second);

		if (_comparison.substr(0, currentWord.size()) == currentWord && currentWord.size() != 0)
		{
			_clear = false;
			if (currentWord.size() > 0)
				for (int i = 0; i < actions.size(); i++)
					if (actions[i].name == it->first)
						actions[i].Drawable->ColourAmount(sf::Color::Green, currentWord.size());

			if (_action == "SHOOT")
				shoot = true;
		}
		else
		{
			for (int i = 0; i < actions.size(); i++)
				if (actions[i].name == it->first)
					actions[i].Drawable->ColourAll(textColour);
		}

		if (_comparison == currentWord)
		{
			if (go)
			{
				_complete = true;
				go = false;
				ActionOutput = _action;
				difficultyOffset++;
				currentWord.clear();
				for (int i = 0; i < actions.size(); i++)
					if (_action == actions[i].name)
					{
						if (actions[i].name == "OPTIONS" || actions[i].name == "FULLSCREEN")
							it->second = actions[i].name;
						else
						{
							do {
								_newIndex = rand() % actions[i].words.size();
								it->second = actions[i].words[_newIndex];
							} while (actions[i].words[_newIndex].size() > difficulty);
						}
						actions[i].Drawable->SetSprites(&it->second);
					}
			}
		}
	}

	if((!_complete && go) && currentWord.size() > 0)
		currentWord.clear();

	return _complete;
}

void TypingControls::PopulateWords()
{
	int _newIndex;

	for (int i = 0; i < actions.size(); i++)
	{
		if (actions[i].name == "START" || actions[i].name == "OPTIONS" || actions[i].name == "QUIT")
		{
			actionWords[actions[i].name] = actions[i].words[0];
		}
		else
		{
			do {
				_newIndex = rand() % actions[i].words.size();
				actionWords[actions[i].name] = actions[i].words[_newIndex];
			} while (actions[i].words[_newIndex].size() > difficulty);
		}

		actions[i].Drawable->SetSprites(&actionWords[actions[i].name]);
	}
}

void TypingControls::PopulateInitialWords()
{
	int _newIndex;

	for (int i = 0; i < actions.size(); i++)
	{
		if (actions[i].name == "START" || actions[i].name == "OPTIONS" || actions[i].name == "QUIT")
		{
			actionWords[actions[i].name] = actions[i].words[0];
		}
		else
		{
			actionWords[actions[i].name] = actions[i].name;
		}

		actions[i].Drawable->SetSprites(&actionWords[actions[i].name]);
	}
}

void TypingControls::InitialiseWordPositions(float Radius)
{
	radius = Radius;
	Vector2f _origin;
	_origin = Vector2f(0, 0);

	for (int i = 0; i < actions.size(); i++)
	{
		float _degree = (360 / actions.size()) * i;

		Vector2f _position;
		_position.x = radius * cos(_degree) + _origin.x;
		_position.y = radius * sin(_degree) + _origin.y;

		wordPositions.push_back(_position);
	}
}

void TypingControls::Update(Vector2f ScreenPosition, Vector2f ScreenSize)
{
	distanceString = "DISTANCE: " + to_string(distanceCounter);

	for (int i = 0; i < actions.size(); i++)
	{
		if (actions[i].name == "JUMP")
		{
			float _degree = (270.0f * ut::PI) / 180;

			Vector2f _position;
			_position.x = radius * cos(_degree);
			_position.y = radius * sin(_degree);

			actions[i].Drawable->SetPosition(_position);
		}

		if (actions[i].name == "SHOOT")
		{
			float _degree = (325.0f * ut::PI) / 180;

			Vector2f _position;
			_position.x = radius * cos(_degree);
			_position.y = radius * sin(_degree);

			actions[i].Drawable->SetPosition(_position);
		}

		if (actions[i].name == "ATTACK")
		{
			float _degree = (0.0f * ut::PI) / 180;

			Vector2f _position;
			_position.x = radius * cos(_degree);
			_position.y = radius * sin(_degree);

			actions[i].Drawable->SetPosition(_position);
		}

		if (actions[i].name == "ROLL")
		{
			float _degree = (45.0f * ut::PI) / 180;

			Vector2f _position;
			_position.x = radius * cos(_degree);
			_position.y = radius * sin(_degree);

			actions[i].Drawable->SetPosition(_position);
		}

		if (actions[i].name == "OK")
		{
			Vector2f _position = Vector2f(ScreenPosition.x - actions[i].Drawable->GetLength().x / 2, ScreenPosition.y + 50);
			actions[i].Drawable->SetOrigin(Vector2f(actions[i].Drawable->GetLength().x / 2, actions[i].Drawable->GetSize().y / 2));
			actions[i].Drawable->SetPosition(_position);
		}

		if (menu)
		{
			if(!options)
			{
				if (actions[i].name == "START")
				{
					Vector2f _position;
					_position.x = (ScreenPosition.x - (ScreenSize.x / 2)) + (ScreenSize.x / 20);
					_position.y = (ScreenPosition.y - (ScreenSize.y / 2)) + (ScreenSize.y / 20);

					actions[i].Drawable->SetPosition(_position);
				}

				if (actions[i].name == "OPTIONS")
				{
					Vector2f _position;
					_position.x = (ScreenPosition.x - (ScreenSize.x / 2)) + (ScreenSize.x / 20);
					_position.y = (ScreenPosition.y - (ScreenSize.y / 2)) + (ScreenSize.y / 20 * 2);

					actions[i].Drawable->SetPosition(_position);
				}

				if (actions[i].name == "QUIT")
				{
					Vector2f _position;
					_position.x = (ScreenPosition.x - (ScreenSize.x / 2)) + (ScreenSize.x / 20);
					_position.y = (ScreenPosition.y - (ScreenSize.y / 2)) + (ScreenSize.y / 20 * 3);

					actions[i].Drawable->SetPosition(_position);
				}
			}
			else
			{
				if (actions[i].name == "FULLSCREEN")
				{
					Vector2f _position;
					_position.x = (ScreenPosition.x - (ScreenSize.x / 2)) + (ScreenSize.x / 20);
					_position.y = (ScreenPosition.y - (ScreenSize.y / 2)) + (ScreenSize.y / 20);

					actions[i].Drawable->SetPosition(_position);
				}
				if (actions[i].name == "BACK")
				{
					Vector2f _position;
					_position.x = (ScreenPosition.x - (ScreenSize.x / 2)) + (ScreenSize.x / 20);
					_position.y = (ScreenPosition.y - (ScreenSize.y / 2)) + (ScreenSize.y / 20 * 2);

					actions[i].Drawable->SetPosition(_position);
				}
			}
		}
	}

	distanceText->SetSprites(&distanceString);
	distanceText->SetPosition(Vector2f((ScreenPosition.x - (ScreenSize.x / 2)) + ScreenSize.x / 20, ScreenPosition.y - ScreenSize.y / 2.25));
	distanceText->SetOrigin(Vector2f(distanceText->GetSize().x / 2, distanceText->GetSize().y / 2));

	drawable->SetSprites(&currentWord);
	drawable->SetPosition(Vector2f(ScreenPosition.x - (panelSprite.getTexture()->getSize().x / 2) + 12, ScreenPosition.y + ScreenSize.y / 2.25));
	drawable->SetOrigin(Vector2f(drawable->GetSize().x / 2, drawable->GetSize().y / 2));

	panelSprite.setPosition(Vector2f(ScreenPosition.x, ScreenPosition.y + ScreenSize.y / 2.25));
}

void TypingControls::Update(Vector2f ScreenPosition, Vector2f ScreenSize, Vector2f PlayerPosition, Vector2f PlayerVelocity, int Difficulty)
{	
	difficulty = Difficulty + (difficultyOffset / 2);

	distanceCounter = (PlayerPosition.x - initialDistance)/10;
	distanceString = "DISTANCE: " + to_string(distanceCounter);

	for (int i = 0; i < actions.size(); i++)
	{
		if (actions[i].name == "JUMP")
		{
			float _degree = (270.0f * ut::PI) / 180;

			Vector2f _position;
			_position.x = radius * cos(_degree) + PlayerPosition.x;
			_position.y = radius * sin(_degree) + PlayerPosition.y;

			actions[i].Drawable->SetPosition(_position);
		}

		if (actions[i].name == "SHOOT")
		{
			float _degree = (325.0f * ut::PI) / 180;

			Vector2f _position;
			_position.x = radius * cos(_degree) + PlayerPosition.x;
			_position.y = radius * sin(_degree) + PlayerPosition.y;

			actions[i].Drawable->SetPosition(_position);
		}

		if (actions[i].name == "ATTACK")
		{
			float _degree = (0.0f * ut::PI) / 180;

			Vector2f _position;
			_position.x = radius * cos(_degree) + PlayerPosition.x;
			_position.y = radius * sin(_degree) + PlayerPosition.y;

			actions[i].Drawable->SetPosition(_position);
		}

		if (actions[i].name == "ROLL")
		{
			float _degree = (45.0f * ut::PI) / 180;

			Vector2f _position;
			_position.x = radius * cos(_degree) + PlayerPosition.x;
			_position.y = radius * sin(_degree) + PlayerPosition.y;

			actions[i].Drawable->SetPosition(_position);
		}

		if (actions[i].name == "OK")
		{			
			Vector2f _position = Vector2f(ScreenPosition.x - actions[i].Drawable->GetLength().x / 2, ScreenPosition.y + 50);
			actions[i].Drawable->SetOrigin(Vector2f(actions[i].Drawable->GetLength().x / 2, actions[i].Drawable->GetSize().y / 2));
			actions[i].Drawable->SetPosition(_position);
		}

		if (menu)
		{
			if (!options)
			{
				if (actions[i].name == "START")
				{
					Vector2f _position;
					_position.x = (ScreenPosition.x - (ScreenSize.x / 2)) + (ScreenSize.x / 20);
					_position.y = (ScreenPosition.y - (ScreenSize.y / 2)) + (ScreenSize.y / 20);

					actions[i].Drawable->SetPosition(_position);
				}

				if (actions[i].name == "OPTIONS")
				{
					Vector2f _position;
					_position.x = (ScreenPosition.x - (ScreenSize.x / 2)) + (ScreenSize.x / 20);
					_position.y = (ScreenPosition.y - (ScreenSize.y / 2)) + (ScreenSize.y / 20 * 2);

					actions[i].Drawable->SetPosition(_position);
				}

				if (actions[i].name == "QUIT")
				{
					Vector2f _position;
					_position.x = (ScreenPosition.x - (ScreenSize.x / 2)) + (ScreenSize.x / 20);
					_position.y = (ScreenPosition.y - (ScreenSize.y / 2)) + (ScreenSize.y / 20 * 3);

					actions[i].Drawable->SetPosition(_position);
				}
			}
			else
			{
				if (actions[i].name == "FULLSCREEN")
				{
					Vector2f _position;
					_position.x = (ScreenPosition.x - (ScreenSize.x / 2)) + (ScreenSize.x / 20);
					_position.y = (ScreenPosition.y - (ScreenSize.y / 2)) + (ScreenSize.y / 20);

					actions[i].Drawable->SetPosition(_position);
				}
				if (actions[i].name == "BACK")
				{
					Vector2f _position;
					_position.x = (ScreenPosition.x - (ScreenSize.x / 2)) + (ScreenSize.x / 20);
					_position.y = (ScreenPosition.y - (ScreenSize.y / 2)) + (ScreenSize.y / 20 * 2);

					actions[i].Drawable->SetPosition(_position);
				}
			}
		}
	}

	distanceText->SetSprites(&distanceString);
	distanceText->SetPosition(Vector2f((ScreenPosition.x - (ScreenSize.x / 2)) + ScreenSize.x / 20, ScreenPosition.y - ScreenSize.y / 2.25));
	distanceText->SetOrigin(Vector2f(distanceText->GetSize().x / 2, distanceText->GetSize().y / 2));

	drawable->SetSprites(&currentWord);
	drawable->SetPosition(Vector2f(ScreenPosition.x - (panelSprite.getTexture()->getSize().x / 2) + 12, ScreenPosition.y + ScreenSize.y / 2.25));
	drawable->SetOrigin(Vector2f(drawable->GetSize().x / 2, drawable->GetSize().y / 2));

	panelSprite.setPosition(Vector2f(ScreenPosition.x , ScreenPosition.y + ScreenSize.y / 2.25));
}

void TypingControls::ProcessInput(Event &Event)
{
	if (Event.type == Event::KeyPressed)
	{
		if (Event.key.code == Keyboard::A)
			currentWord.append("A");
		if (Event.key.code == Keyboard::B)
			currentWord.append("B");
		if (Event.key.code == Keyboard::C)
			currentWord.append("C");
		if (Event.key.code == Keyboard::D)
			currentWord.append("D");
		if (Event.key.code == Keyboard::E)
			currentWord.append("E");
		if (Event.key.code == Keyboard::F)
			currentWord.append("F");
		if (Event.key.code == Keyboard::G)
			currentWord.append("G");
		if (Event.key.code == Keyboard::H)
			currentWord.append("H");
		if (Event.key.code == Keyboard::I)
			currentWord.append("I");
		if (Event.key.code == Keyboard::J)
			currentWord.append("J");
		if (Event.key.code == Keyboard::K)
			currentWord.append("K");
		if (Event.key.code == Keyboard::L)
			currentWord.append("L");
		if (Event.key.code == Keyboard::M)
			currentWord.append("M");
		if (Event.key.code == Keyboard::N)
			currentWord.append("N");
		if (Event.key.code == Keyboard::O)
			currentWord.append("O");
		if (Event.key.code == Keyboard::P)
			currentWord.append("P");
		if (Event.key.code == Keyboard::Q)
			currentWord.append("Q");
		if (Event.key.code == Keyboard::R)
			currentWord.append("R");
		if (Event.key.code == Keyboard::S)
			currentWord.append("S");
		if (Event.key.code == Keyboard::T)
			currentWord.append("T");
		if (Event.key.code == Keyboard::U)
			currentWord.append("U");
		if (Event.key.code == Keyboard::V)
			currentWord.append("V");
		if (Event.key.code == Keyboard::W)
			currentWord.append("W");
		if (Event.key.code == Keyboard::X)
			currentWord.append("X");
		if (Event.key.code == Keyboard::Y)
			currentWord.append("Y");
		if (Event.key.code == Keyboard::Z)
			currentWord.append("Z");
		if (Event.key.code == Keyboard::Return || Event.key.code == Keyboard::Space)
			go = true;
		else
			go = false;
		if (Event.key.code == Keyboard::BackSpace)
			if (currentWord.size() > 0)
				currentWord.pop_back();
		if (Event.key.code == Keyboard::Delete)
			currentWord.clear();
	}
}

void TypingControls::Draw(RenderWindow &Window, bool DrawWords)
{
	if(DrawWords)
	{
		if (drawSurrounds)
		{
			for (int i = 0; i < actions.size(); i++)
				if (actions[i].name != "START" && actions[i].name != "OPTIONS" && actions[i].name != "QUIT" && actions[i].name != "OK")
					actions[i].Drawable->Draw(Window);
		}
	}
	else
	{
		if (menu)
		{
			if (!options)
			{
				for (int i = 0; i < actions.size(); i++)
					if (actions[i].name == "START" || actions[i].name == "OPTIONS" || actions[i].name == "QUIT")
						actions[i].Drawable->Draw(Window);
			}
			else
			{
				for (int i = 0; i < actions.size(); i++)
					if (actions[i].name == "BACK" || actions[i].name == "FULLSCREEN")
						actions[i].Drawable->Draw(Window);
			}
		}
	}

	if(!menu && DrawWords)
		distanceText->Draw(Window);

	Window.draw(panelSprite);
	drawable->Draw(Window);
}

void TypingControls::DrawOK(RenderWindow & Window)
{
	for (int i = 0; i < actions.size(); i++)
		if (actions[i].name == "OK")
			actions[i].Drawable->Draw(Window);
}

void TypingControls::DrawPanel(RenderWindow & Window)
{
	Window.draw(panelSprite);
	drawable->Draw(Window);
}

bool TypingControls::LoadWords(string Filepath)
{
	bool _return = true;
	string _tempString;
	ifstream _input(Filepath);
	int _stringSize = 0;

	if (!_input)
		_return = false;
	else
	{
		getline(_input, _tempString);
		Action* newAction = new Action(_tempString, texture);

		while (getline(_input, _tempString))
		{
			newAction->words.push_back(_tempString);
			_tempString.size() > _stringSize ? _stringSize = _tempString.size() : _stringSize;
			_tempString.clear();
		}

		newAction->Drawable->SetLength(_stringSize);

		actions.push_back(*newAction);
	}

	return _return;
}

TypingControls::TypingControls()
{
	menu = true;
	texture.loadFromFile("Images/TestText.png");
	textPanel.loadFromFile("Images/TextPanel2.png");
	panelSprite.setTexture(textPanel);
	panelSprite.setOrigin(textPanel.getSize().x / 2, textPanel.getSize().y / 2);

	currentWordTexture.loadFromFile("Images/NewText.png");
	drawable = new DrawText(currentWordTexture, 8);
	distanceText = new DrawText(texture, 8);
	
	drawable->SetLength(20);
	distanceText->SetLength(20);

	string _tempString;
	int _stringSize = 0;

	for (int i = 0; i < 6; i++)
	{
		switch (i)
		{
		case 0:
			_tempString = "START";
			break;
		case 1:
			_tempString = "OPTIONS";
			break;
		case 2:
			_tempString = "QUIT";
			break;
		case 3:
			_tempString = "BACK";
			break;
		case 4:
			_tempString = "FULLSCREEN";
			break;
		case 5:
			_tempString = "OK";
			break;
		default:
			break;
		}

		Action* newAction = new Action(_tempString, texture);
		newAction->words.push_back(_tempString);
		_tempString.size() > _stringSize ? _stringSize = _tempString.size() : _stringSize;
		_tempString.clear();
		newAction->Drawable->SetLength(_stringSize);
		actions.push_back(*newAction);
	}

	LoadWords("TextFiles/Attack.txt");
	LoadWords("TextFiles/Roll.txt");
	LoadWords("TextFiles/Jump.txt");
	LoadWords("TextFiles/Shoot.txt");

	PopulateInitialWords();

	InitialiseWordPositions(RADIUS);
}

TypingControls::TypingControls(int Difficulty, bool Menu)
{
	difficulty = Difficulty;
	menu = Menu;
	texture.loadFromFile("Images/TestText.png");
	textPanel.loadFromFile("Images/TextPanel2.png");
	panelSprite.setTexture(textPanel);
	panelSprite.setOrigin(textPanel.getSize().x / 2, textPanel.getSize().y / 2);

	currentWordTexture.loadFromFile("Images/NewText.png");
	drawable = new DrawText(currentWordTexture, 8);
	distanceText = new DrawText(texture, 8);

	drawable->SetLength(20);
	distanceText->SetLength(20);

	string _tempString;
	int _stringSize = 0;

	for (int i = 0; i < 6; i++)
	{
		switch (i)
		{
		case 0:
			_tempString = "START";
			break;
		case 1:
			_tempString = "OPTIONS";
			break;
		case 2:
			_tempString = "QUIT";
			break;
		case 3:
			_tempString = "BACK";
			break;
		case 4:
			_tempString = "FULLSCREEN";
			break;
		case 5:
			_tempString = "OK";
			break;
		default:
			break;
		}

		Action* newAction = new Action(_tempString, texture);
		newAction->words.push_back(_tempString);
		_tempString.size() > _stringSize ? _stringSize = _tempString.size() : _stringSize;
		_tempString.clear();
		newAction->Drawable->SetLength(_stringSize);
		actions.push_back(*newAction);
	}

	LoadWords("TextFiles/Attack.txt");
	LoadWords("TextFiles/Roll.txt");
	LoadWords("TextFiles/Jump.txt");
	LoadWords("TextFiles/Shoot.txt");

	PopulateInitialWords();

	InitialiseWordPositions(RADIUS);
}


TypingControls::~TypingControls()
{
}
