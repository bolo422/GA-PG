#pragma once
#include "Object.h"
class Player :
    public Object
{
public:
    Player() {}
    ~Player() {}

    //Verifica se está no chão
    inline bool grounded() { if (position.y <= ground) { return true; } else { return false; } }

    //Pular
    void jump(float force, float speed);
    inline void jump(bool _jumping) { jumping = _jumping; }

    inline bool getFalling() { return falling; }
    inline bool getJumped() { return jumped; }


private:

    bool jumping = false;
    bool falling = false;
    bool jumped = false;

    float jumpHeight = 0;
    const int ground = 62; //Altura do chão + alturado do player/2


};

