#include "../components/Player_Health.h"

void Player_Health::render()
{
	float vidaRestante = vidas;
	float currentVidas = vidas+0.5;
	src = build_sdlrect(frame, frameSize.getX(), frameSize.getY());
	Vector2D aux = Vector2D(10 + 35 * 0, 10);
	float zoom = g->camera_Zoom_Out;

	for (int i = 0; i < vidas-0.5; i++)
	{
		aux = Vector2D(10 + 35 * i * zoom, 10);
		SDL_Rect dest = build_sdlrect(aux, (src.w / 4) * zoom, (src.h / 4)* zoom);

		fVida->render(src, dest);
		vidaRestante--;
	}
	
	// Si hay media vida, renderizamos medio anzuelo
	if (vidaRestante > 0)
	{
		aux = Vector2D(10 + 35 * (vidas-0.5) * zoom, 10);
		SDL_Rect dest = build_sdlrect(aux, (src.w / 4) * zoom, (src.h / 4)* zoom);
		hVida->render(src, dest);
	}
	
	if (currentVidas < maxVidas)
	{
		aux = Vector2D(aux.getX() + (zoom * 50), 10);
		SDL_Rect dest = build_sdlrect(aux, (src.w / 6) * zoom, (src.h / 5) * zoom);
		vVida->render(dest);
		for (int i = 1; i < (maxVidas - currentVidas); i++)
		{
			aux = Vector2D(aux.getX() + (zoom * 50) - 10, 10);
			SDL_Rect dest = build_sdlrect(aux, (src.w / 6 )* zoom, (src.h / 5)*zoom);
			vVida->render(dest);
		}
	}

	// Si pasa mas de X tiempo, pasamos al siguiente frame
	if (sdlutils().currRealTime() > time + tiempoanimacion)
	{
		time = sdlutils().currRealTime();

		// Avanzamos de frame
		frame = Vector2D(frame.getX() + 194, frame.getY()); // 194 es la cantidad exacta a avanzar al siguiente frame sin que ocurra nada
		nFrame++;

		// Si llegamos al ultimo frame, volvemos al primero
		if (nFrame > 7) { frame = Vector2D(0, 15); nFrame = 0; }
	}
	
}