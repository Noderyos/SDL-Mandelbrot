#include <SDL2/SDL.h>
#include <complex>

#define WINDOW_WIDTH 500

void putPixelRGB(SDL_Renderer* renderer, int x, int y, unsigned char r, unsigned char g, unsigned char b);

void renderFractal(SDL_Renderer* renderer,int zoom,float x, float y, int iter);
void renderMinimap(SDL_Renderer* renderer,float x, float y, int zoom);

std::complex<double> mandelbrot(std::complex<double> c,int N);

int main() {

    int zoomLevel = 0;
    float xDec = 0;
    float yDec = 0;
    int iter = 10;

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderPresent(renderer);
    renderFractal(renderer,zoomLevel,xDec,yDec,iter);
    renderMinimap(renderer,xDec,yDec,zoomLevel);
    bool close;
    while (!close) {
        SDL_WaitEvent(&event);
        switch (event.type){
            case SDL_QUIT:
                close = true;
            case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_KP_PLUS:
                            iter <<= 1;
                            break;
                        case SDLK_KP_MINUS:
                            iter >>= 1;
                            break;
                        case SDLK_KP_ENTER:
                            iter = 10;
                            break;
                        case SDLK_LEFT:
                            xDec--;
                            break;
                        case SDLK_RIGHT:
                            xDec++;
                            break;
                        case SDLK_UP:
                            yDec--;
                            break;
                        case SDLK_DOWN:
                            yDec++;
                            break;
                        case SDLK_BACKSPACE:
                            xDec *= 0.75;
                            yDec *= 0.75;
                            zoomLevel--;
                            break;
                        case SDLK_RETURN:
                            zoomLevel++;
                            xDec *= 1.5;
                            yDec *= 1.5;
                            break;
                    }
                renderFractal(renderer,zoomLevel,xDec,yDec,iter);
                renderMinimap(renderer,xDec,yDec,zoomLevel);
                break;
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}


void putPixelRGB(SDL_Renderer* renderer, int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    SDL_SetRenderDrawColor(renderer, (Uint8)r, (Uint8)g, (Uint8)b, 255);
    SDL_RenderDrawPoint(renderer, x, y);
}

void renderFractal(SDL_Renderer* renderer,int zoom,float x, float y, int iter) {

    double Xmin = -2 / pow(2,zoom) + (x/5.0) / pow(2,zoom);
    double Ymin = -2 / pow(2,zoom) + (y/5.0) / pow(2,zoom);
    double Xmax = 2 / pow(2,zoom) + (x/5.0) / pow(2,zoom);
    double Ymax = 2 / pow(2,zoom) + (y/5.0) / pow(2,zoom);
    double Xstep = (Xmax-Xmin)/WINDOW_WIDTH;
    double Ystep = (Ymax-Ymin)/WINDOW_WIDTH;

    double X=Xmin,Y=0;

    for (int i = 0; i < WINDOW_WIDTH; i++) {
        Y=Ymin;
        for (int j = 0; j < WINDOW_WIDTH; j++) {
            std::complex<double> n = mandelbrot(std::complex<double>(X,Y),iter);
            unsigned char gb = n.imag()*n.imag()+n.real()*n.real() * 100;
            putPixelRGB(renderer,i,j,gb,gb,gb);
            Y+=Ystep;
        }
        X+=Xstep;
        SDL_RenderPresent(renderer);
    }

}

std::complex<double> mandelbrot(std::complex<double> c,int N){
    double n = 0;
    std::complex<double> z(0,0);
    while (n<N && abs(z) < 2){
        z = pow(z,2) + c;
        n+=1;
    }
    return z;
}

void renderMinimap(SDL_Renderer* renderer,float x, float y, int zoom){

    double Xmin = -2 / pow(2,zoom) + (x/5.0) / pow(2,zoom);
    double Ymin = -2 / pow(2,zoom) + (y/5.0) / pow(2,zoom);
    double Xmax = 2 / pow(2,zoom) + (x/5.0) / pow(2,zoom);
    double Ymax = 2 / pow(2,zoom) + (y/5.0) / pow(2,zoom);

    double X=-2,Y;

    for (int i = 0; i < 100; i++) {
        Y=-2;
        for (int j = 0; j < 100; j++) {
            std::complex<double> n = mandelbrot(std::complex<double>(X,Y),10);
            unsigned char gb = n.imag()*n.imag()+n.real()*n.real() * 100;
            putPixelRGB(renderer,i,j,gb,gb,gb);
            Y+=0.04;
        }
        X+=0.04;
        SDL_RenderPresent(renderer);
    }

    int thickness = 5;

    for (int i = 0; i <= 100; i++) {
        for (int j = 0; j <= 100; j++) {
            if(i < thickness || i > (100-thickness) || j < thickness || j > (100-thickness))
                putPixelRGB(renderer,i,j,255,0,0);
        }
    }

    for (int i = (Xmin+2)*25; i <= (Xmax+2)*25; i++) {
        for (int j = (Ymin+2)*25; j <= (Ymax+2)*25; j++) {
            putPixelRGB(renderer,i,j,0,255,0);
        }
    }

    SDL_RenderPresent(renderer);
}
