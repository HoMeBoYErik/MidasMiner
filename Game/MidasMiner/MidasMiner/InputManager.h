//
//  InputManager.h

#ifndef __InputManager__
#define __InputManager__

#include <iostream>
#include <vector>

#include "SDL.h"
#include "GameObject.h"
//#include "Vector2D.h"
#include <memory>


/*enum mouse_buttons
{
	LEFT = 0,
	MIDDLE = 1,
	RIGHT = 2
};*/



class InputManager
{
public:

	static InputManager* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new InputManager();
			return s_pInstance;
		}

		return s_pInstance;
	}
	
	void handleEvent(SDL_Event& e);

	//void reset();

	// update and clean the input handler
	void update(float timeStep);
	//void clean();

	// mouse events
	//bool getMouseButtonState(int buttonNumber) const;
	//Vector2D* getMousePosition() const;

	// mouse control
	bool userInteractionEnabled = true;


private:

	InputManager(){};
	~InputManager(){};

	InputManager(const InputManager&);
	InputManager& operator=(const InputManager&);

	int fromRow = -1;
	int fromCol = -1;
	int toRow = -1;
	int toCol = -1;

	std::shared_ptr<GameObject> selectedGameObjectFrom;
	std::shared_ptr<GameObject> selectedGameObjectTo;

	bool noGemSelected();
	void selectSwapFrom(int x, int y);
	void selectSwapTo(int x, int y);
	void clearSwapTo();
	void resetSwap();
	void clearAllSwap();

	// private functions to handle different event types

	// handle keyboard events
	//void onKeyDown();
	//void onKeyUp();

	// handle mouse events
	//void onMouseMove(SDL_Event& event);
	//void onMouseButtonDown(SDL_Event& event);
	//void onMouseButtonUp(SDL_Event& event);

	// keyboard specific
	//const Uint8* m_keystates;
	
	// mouse specific
	//std::vector<bool> m_mouseButtonStates;
	//Vector2D* m_mousePosition;


	// singleton instance reference
	static InputManager *s_pInstance;
};
typedef InputManager GInputManager;

#endif /* defined(__InputManager__) */

