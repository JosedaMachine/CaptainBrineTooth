#include "../states/OptionsState.h"
#include "../states/MenuState.h"
#include "../states/PlayState.h"

OptionsState::OptionsState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){
	cam = a->camera;
}

/// <summary>
/// Metemos los botones y los sliders
/// </summary>
void OptionsState::init(){
	auto* fondo = manager_->addEntity();

	SDL_Rect posImage;
	posImage.x = 0;
	posImage.y = 0;
	posImage.w = cam.w;
	posImage.h = cam.h;

	fondo->addComponent<Image>(&sdlutils().images().at("bg_options"), posImage, "fondoOpciones");

	// Back Button
	int w = (int)sdlutils().width() * 0.25 * App::camera_Zoom_Out * 0.65;
	int h = (int)w * 0.4;
	int x = (int)((App::camera.w - w) * 0.02);
	int y = (int)((App::camera.h - h) * 0.1);
	createButton(&sdlutils().images().at("atras_boton"), Vector2D(x, y), Vector2D(w, h), volverMenu);
	
	// FullScreen Button
	w = (int)sdlutils().width() * 0.25 * App::camera_Zoom_Out * 0.65;
	h = (int)w * 0.4;
	x = (int)((App::camera.w - w) * 0.95);
	y = (int)((App::camera.h - h) * 0.1); 
	fullscreen_button = createBasicEntity(Vector2D(x, y), Vector2D(w, h), 0.0f, Vector2D(0, 0));
	fullscreen_button->addComponent<Button>(&sdlutils().images().at("fullscreen_off"), changeFullScreen, app, manager_->getSoundMngr());

	//Tamaño de las texturas del Slider
	std::pair<Vector2D, Vector2D> size = { Vector2D(600, 30), Vector2D(95, 100) };
	Vector2D pos = Vector2D(Vector2D((cam.w * 0.5), cam.h * 0.2));

	//Volumen
	Texture* textures[2] = {&sdlutils().images().at("barra") , &sdlutils().images().at("barco")};
	createVolume(manager_, pos, size, textures, manager_->getSoundMngr()->PauseVolume(), app);

	//Efectos
	pos = Vector2D(Vector2D((cam.w * 0.5), cam.h * 0.55));
	Texture* textures1[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };
	createEffects(manager_, pos, size, textures1, manager_->getSoundMngr()->EffectsVolume(), app);

	//Brillo
	pos = Vector2D(Vector2D((cam.w * 0.5), cam.h * 0.85));
	Texture* textures2[2] = {&sdlutils().images().at("barra") , &sdlutils().images().at("barco")};
	createBrightness(manager_, pos, size, textures2, SDL_GetWindowBrightness(sdlutils().window()), app);
}

void OptionsState::createButton(Texture* imageTexture, Vector2D pos, Vector2D size, void(*callback)(App*, SoundManager*)) {
	auto* button = createBasicEntity(Vector2D(pos.getX(), pos.getY()), Vector2D(size.getX(), size.getY()), 0.0f, Vector2D(0, 0));
	button->addComponent<Button>(imageTexture, callback, app, manager_->getSoundMngr());
}

/// <summary>
///  Las entidades de createVolume, Effects y Brightness estan separadas en metodos para poder utilizarlos en otros sitios como PauseState
/// </summary>


Entity* OptionsState::createVolume(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textures[2], const int& volume, App* a){
	Entity* sliderVolume = mngr->addEntity();
	Slider* slider = sliderVolume->addComponent<Slider>(pos, sizes, textures, controlVolume, &sdlutils().images().at("volumen"), a, 0.5f, 50.0f);

	float newVol = (float)volume / (float)(mngr->getSoundMngr()->getMaxVol()/2);
	slider->setSlider(newVol);

	return sliderVolume;
}

Entity* OptionsState::createEffects(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textures[2], const int& volume, App* a) {
	Entity* sliderEffects = mngr->addEntity();
	Slider* slider =  sliderEffects->addComponent<Slider>(pos, sizes, textures, controlEffects, &sdlutils().images().at("efectos"), a, 0.5f, 50.0f);

	float newVol = (float)volume / (float)(mngr->getSoundMngr()->getMaxVol() / 2);
	slider->setSlider(newVol);

	return sliderEffects;
}

Entity* OptionsState::createBrightness(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textures[2], const float& brightness, App* a){
	Entity* sliderBrightness = mngr->addEntity();
	Slider* slider = sliderBrightness->addComponent<Slider>(pos, sizes, textures, controlBrightness, &sdlutils().images().at("brillo"), a, 0.5f, 30.0f);

	float value = brightness;
	slider->setSlider(value);
	return sliderBrightness;
}

void OptionsState::update() {
	GameState::update(); 
	if (fullscreen_button != nullptr) {
		auto flags = SDL_GetWindowFlags(sdlutils().window());
		if (flags & SDL_WINDOW_FULLSCREEN) {
			fullscreen_button->getComponent<Button>()->setTex(&sdlutils().images().at("fullscreen_on"));
		}
	}
}

void OptionsState::controlVolume(float value, Entity* ent){
	SoundManager* snd = ent->getMngr()->getSoundMngr();
	int newVol = value * (float)(snd->getMaxVol()/2);
	snd->setMusicVolume(newVol);
}

void OptionsState::controlEffects(float value, Entity* ent) {
	SoundManager* snd = ent->getMngr()->getSoundMngr();

	int newVol = value * (float)(snd->getMaxVol() / 2);
	snd->setEffectsVolume(newVol);
}


void OptionsState::controlBrightness(float value, Entity* ent){
	SDL_Window* window = sdlutils().window();
	float newValue = value * (1 - 0.26);
	//Pongo 0.26 porque apartir de ese numero no baja m�s
	if (SDL_SetWindowBrightness(window, newValue + 0.26) != 0){
		std:cout << "Can't lower the brightness";
	}
}

void OptionsState::volverMenu(App* app, SoundManager* snd){
	snd->playSoundEffect("gaviota",0);
	app->getStateMachine()->popState();
}

void OptionsState::changeFullScreen(App* app, SoundManager* snd)
{
	sdlutils().toggleFullScreen();
}
