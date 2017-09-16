#include "GAME.h"

GAME::GAME(HWND &window)
{
	presentscene = MENUSCENE;
	teamcount = TWO;
	presentteamturn = ONE;

	this->menugpu = new MENUGPU(window);

	this->menugpu->CreateRenderTargetView();
	this->menugpu->DefineViewPort();
	this->menugpu->DefineAndCreateDepthStencil();

	this->gamegpu = new GAMEGPU(window);

	this->gamegpu->CreateRenderTargetView();
	this->gamegpu->DefineViewPort();
	this->gamegpu->DefineAndCreateDepthStencil();
}
GAME::~GAME()
{
	menugpu->ShutDown();
	gamegpu->ShutDown();

	delete menugpu;
	delete gamegpu;
}

void GAME::Run(XTime &T)
{
	switch (presentscene)//render the proper scene with this switch 
	{
	case MENUSCENE:
	{
		menugpu->CameraUpdate(T);
		menugpu->DrawToScreen();
		SceneTransition();
		break;//MENUSCENE break
	}
	case GAMESCENE:
	{
		switch (presentteamturn)//while in the game scene, team moves piece then ends its own turn begining the next teams turn
		{
		case ONE:
		{
			gamegpu->PlayerInput(gamegpu->teamone, ONE);
			gamegpu->CameraUpdate(T);
			gamegpu->DrawToScreen();
			SceneTransition();
			TurnTransition();

			break;//ONE break
		}
		case TWO:
		{
			gamegpu->PlayerInput(gamegpu->teamtwo, TWO);
			gamegpu->CameraUpdate(T);
			gamegpu->DrawToScreen();
			SceneTransition();
			TurnTransition();

			break;//TWO break
		}
		}
		break;//GAMESCENE break
	}
	case LOADMENU:
	{

	}
	case LOADGAME:
	{

	}
	}

}

void GAME::SceneTransition()
{
	if (gamegpu->returntomenu)//bool triggered to change to menu scene
	{
		presentscene = MENUSCENE;
		gamegpu->returntomenu = false;//??to consider: returning to menu could shutdown game scene and what is the need to set this to false because when game is reinitialized bool value is default false?? 
	}
	if (menugpu->loadgame)//transition to loading the game scene
	{
		presentscene = GAMESCENE;
		menugpu->loadgame = false;
	}
}

void GAME::TurnTransition()
{
	if (gamegpu->turnended)
	{
		presentteamturn++;
		if (presentteamturn > teamcount)//this logic can be re-written to hardcoded to set present team turn to next team rather than using presentteamturn++
			presentteamturn = 1;
		gamegpu->turnended = false;
	}
}
