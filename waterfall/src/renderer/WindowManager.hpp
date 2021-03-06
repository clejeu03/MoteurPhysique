#ifndef _IMAC3_WINDOWMANAGER_HPP
#define _IMAC3_WINDOWMANAGER_HPP

#include <SDL/SDL.h>

namespace imac3 {

class WindowManager {
public:
    WindowManager(uint32_t w, uint32_t h, const char* title);

    ~WindowManager();

    WindowManager(const WindowManager&) = delete;

    WindowManager& operator =(const WindowManager&) = delete;

    bool pollEvent(SDL_Event& e) const {
        return SDL_PollEvent(&e);
    }

    // A appeler en début de boucle de rendu
    void startMainLoop() {
        m_nStartTime = SDL_GetTicks();
    }

    // Met à jour la fenetre et renvoit le temps écoulé depuis le dernier appel à startMainLoop (en secondes)
    float update();

    void setFramerate(uint32_t fps) {
        m_nFPS = fps;
        m_nFrameDuration = 1000.f / m_nFPS;
    }

    uint32_t getFramerate() const {
        return m_nFPS;
    }
private:
    uint32_t m_nFPS;
    uint32_t m_nFrameDuration;
    uint32_t m_nStartTime;
};

}

#endif // IMAC3_SMP_WINDOWMANAGER_HPP
