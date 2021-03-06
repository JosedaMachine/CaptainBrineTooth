#include "MenuState.h"
#include "OptionsState.h"
#include "PlayState.h"
#include "TestZoneState.h"
#include "CreditsState.h"
#include "TutorialState.h"

#include <fstream>
MenuState::MenuState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){
	cam = a->camera;
	fadeComp = nullptr;
}

/// <summary>
/// Iniciamos el fade in, metemos el titulo, los botones y el fondo
/// </summary>
void MenuState::init() {
	//Fade. Para que se sette el Fade
	GameState::init();
	//Fade
	fadeComp = manager_->getHandler<Fader>()->getComponent<Fade>();
	if (fadeComp != nullptr) {
		fadeComp->setTimeIn(1000);
		fadeComp->setTimeOut(2000);
		fadeComp->triggerFade();
	}
	//CHECK SAVE FILe
	checkSaveFile();
	//Setear los sonidos
	if (manager_->getSoundMngr()->GeneralVolume() > manager_->getSoundMngr()->PauseVolume())
	{
		manager_->getSoundMngr()->setGeneralVolume(manager_->getSoundMngr()->GeneralVolume(), manager_->getSoundMngr()->EffectsVolume());
	}
	else {
		manager_->getSoundMngr()->setGeneralVolume(manager_->getSoundMngr()->PauseVolume(), manager_->getSoundMngr()->EffectsVolume());
	}

	manager_->getSoundMngr()->playMainMusic();

	//Background
	std::deque<std::pair<const char*, std::pair<bool, int>>> videos;
																//Filename, loop, frameRate
	std::pair<const char*, std::pair<bool, int>> video__ = { sdlutils().videos().at("waves").c_str(), {true, 25} };
	videos.push_back(video__);
	auto* video = manager_->addEntity();
	video->addComponent<VideoPlayer>(videos);
	

	// Titulo
	auto* title = manager_->addEntity();
	int w = (int)sdlutils().width() * App::camera_Zoom_Out * 0.35;
	int h = (int)w * 0.6;	//mantener aspect ratio
	int x = (int)((App::camera.w - w) * 0.5);
	int y = (int)((App::camera.h - h) * 0.05);
	title->addComponent<Image>(&sdlutils().images().at("menu_titulo"), SDL_Rect{ x, y, w, h }, "title");
#pragma region Botones
	//Atributos de los botones
	y = (int)((y + h * 0.75));
	w = (int)sdlutils().width() * 0.2 * App::camera_Zoom_Out;
	h = (int)w * 0.4;
	x = (int)((App::camera.w - w) * 0.5);

	//Boton Cargar
	if (app->IsSavedGame()) {
		auto* botonCargar = manager_->addEntity();
		w *= 0.8;
		h *= 0.8;
		x = (int)((App::camera.w - w) * 0.5);
		y = (int)((y + h * 1.2));
		//y = (int)((App::camera.h - h) * (0.48 + 0.22 * nBoton));
		botonCargar->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), w, h, 0.0f);
		botonCargar->addComponent<Button>(&sdlutils().images().at("continuar_menu"), changeToGame, app, manager_->getSoundMngr())->canFadeOut(true);
		w *= 1.25;
		h *= 1.25;
		x = (int)((App::camera.w - w) * 0.5);
	}

	//Creditos
	auto* creditos = manager_->addEntity();
	int x_ = (int)((App::camera.w - w) * 0.02);
	int y_ = (int)((App::camera.h - h) * 0.98);
	creditos->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), w * 0.7, h * 0.7, 0.0f);
	creditos->addComponent<Button>(&sdlutils().images().at("creditos_menu"), changeToCredits, app, manager_->getSoundMngr());

	// Boton de salir
	auto* salir = manager_->addEntity();
	x_ = (int)((App::camera.w - w) * 1.05);
	y_ = (int)((App::camera.h - h) * 0.98);
	salir->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), w*0.7, h*0.7, 0.0f);
	salir->addComponent<Button>(&sdlutils().images().at("salir_menu"), salirMenu, app, manager_->getSoundMngr());

	// Opciones
	auto* opciones = manager_->addEntity();
	y = (int)((y + (h * 1.2 * 2)));
	opciones->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), w, h, 0.0f);
	opciones->addComponent<Button>(&sdlutils().images().at("ajustes_menu"), changeToOptions, app, manager_->getSoundMngr());

	// Boton Iniciar
	auto* botonIniciar = manager_->addEntity();
	y = (int)((y - ((h * 1.2))));
	botonIniciar->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), w, h, 0.0f);
	botonIniciar->addComponent<Button>(&sdlutils().images().at("nueva_partida_menu"), changeToTutorial, app, manager_->getSoundMngr())->canFadeOut(true);
#pragma endregion
}

void MenuState::changeToTutorial(App* app, SoundManager* snd) {
	snd->playSoundEffect("sonido_barco", 0);
	//Borrado de archivo anterior
	app->getStateMachine()->currentState()->deleteSaveFile();
	StateMachine* sM = app->getStateMachine();
	sM->changeState(new TutorialState(app, sM->currentState()->getMngr()->getWorld(), snd));
}

void MenuState::changeToGame(App* app, SoundManager* snd) {
	snd->playSoundEffect("sonido_barco", 0);
	
	StateMachine* sM = app->getStateMachine();
	sM->changeState(new PlayState(app, sM->currentState()->getMngr()->getWorld(), snd,0,nullptr));
}

void MenuState::changeToCredits(App* app, SoundManager* snd) {
	snd->playSoundEffect("sonido_barco", 0);
	
	StateMachine* sM = app->getStateMachine();
	sM->changeState(new CreditsState(app, sM->currentState()->getMngr()->getWorld(), snd));
}

void MenuState::checkSaveFile()
{
	bool saved_game;
	ifstream readtxt;
	int pointsRead = 0;
	string file = "data.dat";
	readtxt.open("assets//user_data//" + file);
	if (!readtxt) saved_game = false;
	else {
		readtxt >> pointsRead;
		if (pointsRead < 0) saved_game = false;
		else saved_game = true;
	}
	app->setIsSavedGame(saved_game);
}

void MenuState::changeToOptions(App* app, SoundManager* snd) {
	snd->playSoundEffect("gaviota",0);

	StateMachine* sM = app->getStateMachine();
	sM->pushState(new OptionsState(app, sM->currentState()->getMngr()->getWorld(), snd));
}

void MenuState::salirMenu(App* game, SoundManager* snd){
	game->exitGame();
}