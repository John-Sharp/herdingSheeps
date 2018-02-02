#include "../engine/engine.h"

#include <stdio.h>

typedef struct dogActor
{
    actor a;
    sprite * s;
} dogActor;

void dogRenderHandler(actor *a)
{
    dogActor * d = a->owner;

    engineSpriteRender(a->eng, d->s);
}

void dogLogicHandler(actor *a)
{
}

int main()
{
    engine * e = createEngine(800, 600);

    juint dogTexture = engineLoadTexture(e, "assets/imgs/dog/E_still.png");
    decal dogDecal;
    decalInit(&dogDecal, e, dogTexture, engineGetTextureRect(e, dogTexture));

    sprite dogSprite = {d : &dogDecal, rect: engineGetTextureRect(e, dogTexture)};

    dogActor dogActor = {s : &dogSprite, a : {renderHandler : dogRenderHandler,
        logicHandler : dogLogicHandler }};
    dogActor.a.owner = &dogActor;

    engineActorReg(e, &dogActor.a);

    engineStart(e);
}
