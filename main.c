#include "../engine/engine.h"

#include <stdio.h>

enum
{
    DECAL_DOG,
    NUM_DECALS
};

typedef struct dogActor
{
    actor a;
    sprite s;
} dogActor;

typedef struct herdingSheepsEngine
{
    engine * engine;

    decal decals[NUM_DECALS];

    dogActor dog;
} herdingSheepsEngine;

void dogRenderHandler(actor *a)
{
    dogActor * d = a->owner;
    engineSpriteRender(a->eng, &d->s);
}

void dogLogicHandler(actor *a)
{
}

void dogUpdateSprite(dogActor * dog)
{
    juint w = dog->s.d->rect.tr[0] - dog->s.d->rect.bl[0];
    juint h = dog->s.d->rect.tr[1] - dog->s.d->rect.bl[1];
    dog->s.rect.bl[0] = 100 - w/2;
    dog->s.rect.tr[0] = 100 + w/2;

    dog->s.rect.bl[1] = 100 - h/2;
    dog->s.rect.tr[1] = 100 + h/2;
}

void initDog(dogActor * dog, herdingSheepsEngine * eng)
{
    dog->a.owner = dog;
    dog->a.renderHandler = dogRenderHandler;
    dog->a.logicHandler = dogLogicHandler;
    engineActorReg(eng->engine, &dog->a);
    dog->s.d = &eng->decals[DECAL_DOG];
    dogUpdateSprite(dog);
}

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng)
{
    eng->engine = createEngine(800, 600, eng);

    juint dogTexture = engineLoadTexture(eng->engine, "assets/imgs/dog/E_still.png");
    decalInit(&eng->decals[DECAL_DOG], eng->engine,
            dogTexture, engineGetTextureRect(eng->engine, dogTexture));

    initDog(&eng->dog, eng);

    return eng;
}



int main()
{
    herdingSheepsEngine eng;
    initHerdingSheepsEngine(&eng);

    engineStart(eng.engine);
}
