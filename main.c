#include "../coll/coll.h"
#include "../engine/engine.h"

#include <stdio.h>

enum
{
    DECAL_DOG,
    NUM_DECALS
};

enum 
{
    GS_LEFT = 1,
    GS_RIGHT = 2,
    GS_UP = 3,
    GS_DOWN = 4,
};

enum
{
    COLL_GROUP_DOG,
};

enum
{
    MAP_DECAL_SHEET_W = 3,
    MAP_DECAL_SHEET_H =3,
    MAP_TILE_W = 32,
    MAP_TILE_H =32,
    NUM_MAP_DECALS = 9,
    MAP_W = 3,
    MAP_H = 4,
};

typedef struct map
{
    actor a;
    sprite s[MAP_W * MAP_H];
} map;

typedef struct dogActor
{
    actor a;
    sprite s;
    jvec v;

    jcircle collBody;
} dogActor;

typedef struct herdingSheepsEngine
{
    engine * engine;
    jcEng * collEng;

    decal decals[NUM_DECALS];
    decal mapDecals[NUM_MAP_DECALS];

    map map;
    dogActor dog;
} herdingSheepsEngine;

void dogUpdateSprite(dogActor * dog)
{
    juint w = dog->s.d->rect.tr[0] - dog->s.d->rect.bl[0];
    juint h = dog->s.d->rect.tr[1] - dog->s.d->rect.bl[1];
    dog->s.rect.bl[0] = dog->collBody.c[0] - w/2;
    dog->s.rect.tr[0] = dog->collBody.c[0] + w/2;

    dog->s.rect.bl[1] = dog->collBody.c[1] - h/2;
    dog->s.rect.tr[1] = dog->collBody.c[1] + h/2;
}

void dogRenderHandler(actor *a)
{
    dogActor * d = a->owner;
    dogUpdateSprite(d);
    engineSpriteRender(a->eng, &d->s);
}

void dogLogicHandler(actor *a)
{
}

void mapRenderHandler(actor *a)
{
    map * map = a->owner;

    jint i;
    for (i = 0; i < MAP_W * MAP_H; i++)
    {
        engineSpriteRender(a->eng, &map->s[i]);
    }
}

void initMap(map * map, herdingSheepsEngine * eng)
{
    map->a.owner = map;
    map->a.renderHandler = mapRenderHandler;
    map->a.logicHandler = NULL;
    engineActorReg(eng->engine, &map->a);

    jint mapIndicies[] = {6,7,8,
        3,4,5,
        3,4,5,
        0,1,2};

    jint i, j;
    for (j = 0; j < MAP_H; j++)
    {
        for (i = 0; i < MAP_W; i++)
        {
            map->s[j*MAP_W+i].d = &eng->mapDecals[mapIndicies[i+(MAP_H-1-j)*MAP_W]];
            map->s[j*MAP_W+i].rect.bl[0] = i * MAP_TILE_W;
            map->s[j*MAP_W+i].rect.bl[1] = j * MAP_TILE_H;
            map->s[j*MAP_W+i].rect.tr[0] = (i + 1) * MAP_TILE_W;
            map->s[j*MAP_W+i].rect.tr[1] = (j + 1) * MAP_TILE_H;
        }
    }
}

void initDog(dogActor * dog, herdingSheepsEngine * eng)
{
    dog->a.owner = dog;
    dog->a.renderHandler = dogRenderHandler;
    dog->a.logicHandler = dogLogicHandler;
    engineActorReg(eng->engine, &dog->a);
    dog->s.d = &eng->decals[DECAL_DOG];

    dog->collBody.c[0] = 100;
    dog->collBody.c[1] = 100;
    dog->collBody.r = 20;

    dogUpdateSprite(dog);
    registerCircle(eng->collEng, &dog->collBody, &dog->v, COLL_GROUP_DOG, dog);
}

void herdingSheepsPreLogic(engine * e)
{
    herdingSheepsEngine * eng = e->owner;
 
    if (isStateActive(GS_LEFT))
    {
        eng->dog.v[0] = -1;
    }
    else if (isStateActive(GS_RIGHT))
    {
        eng->dog.v[0] = 1;
    }
    else 
    {
        eng->dog.v[0] = 0;
    }
    if (isStateActive(GS_UP))
    {
        eng->dog.v[1] = 1;
    }
    else if (isStateActive(GS_DOWN))
    {
        eng->dog.v[1] = -1;
    }
    else{
        eng->dog.v[1] = 0;
    }

    jcEngDoStep(eng->collEng);
}

void loadMapDecals(herdingSheepsEngine * eng)
{
    juint mapTexture = engineLoadTexture(eng->engine, "assets/imgs/maps/map.png");

    jint i;
    jint j;
    for (j = 0; j < MAP_DECAL_SHEET_H; j++)
    {
        for (i = 0; i < MAP_DECAL_SHEET_W; i++)
        {
            if (i + j == NUM_MAP_DECALS)
                return;
            jintRect tileSrcRect;
            tileSrcRect.bl[0] = i * MAP_TILE_W;
            tileSrcRect.bl[1] = j * MAP_TILE_H;

            tileSrcRect.tr[0] = (i + 1) * MAP_TILE_W;
            tileSrcRect.tr[1] = (j + 1) * MAP_TILE_H;

            decalInit(&eng->mapDecals[j * MAP_DECAL_SHEET_W + i], eng->engine,
                    mapTexture, tileSrcRect);
        }
    }
}

herdingSheepsEngine * initHerdingSheepsEngine(herdingSheepsEngine * eng)
{
    eng->engine = createEngine(800, 600, eng);

    juint dogTexture = engineLoadTexture(eng->engine, "assets/imgs/dog/E_still.png");
    decalInit(&eng->decals[DECAL_DOG], eng->engine,
            dogTexture, engineGetTextureRect(eng->engine, dogTexture));
    loadMapDecals(eng);

    eng->collEng = createJcEng();

    initDog(&eng->dog, eng);
    initMap(&eng->map, eng);

    enginePreLogicCallBackReg(eng->engine, herdingSheepsPreLogic);

    keyStateBinding binding;
    binding.k = SDLK_LEFT;
    binding.s = GS_LEFT;
    binding.t = BINDING_CONTINUOUS;
    addBinding(&binding);

    binding.k = SDLK_RIGHT;
    binding.s = GS_RIGHT;
    binding.t = BINDING_CONTINUOUS;
    addBinding(&binding);

    binding.k = SDLK_UP;
    binding.s = GS_UP;
    binding.t = BINDING_CONTINUOUS;
    addBinding(&binding);

    binding.k = SDLK_DOWN;
    binding.s = GS_DOWN;
    binding.t = BINDING_CONTINUOUS;
    addBinding(&binding);


    return eng;
}



int main()
{
    herdingSheepsEngine eng;
    initHerdingSheepsEngine(&eng);

    engineStart(eng.engine);
}
