/* System and SDK libraries */
#include <gccore.h>
#include <stdlib.h>

/* Generated assets headers */
#include "hovercraft_bmb.h"
#include "plane_bmb.h"
#include "terrain_bmb.h"
#include "ray_bmb.h"
#include "textures.h"

#include "model.h"
#include "object.h"
#include "game.h"
#include "gxutils.h"
#include "input.h"

/* Model info */
model_t *modelHover, *modelTerrain, *modelPlane, *modelRay;
object_t *objectTerrain, *objectPlane, *planeRay;

/* Texture vars */
GXTexObj hoverTexObj, terrainTexObj, waterTexObj, rayTexObj;

/* Light */
static GXColor lightColor[] = {
		{ 0x4f, 0x4f, 0x4f, 0xff }, // Light color
		{ 0x0f, 0x0f, 0x0f, 0xff }, // Ambient color
		{ 0xff, 0xff, 0xff, 0xff }  // Mat color
};

BOOL firstFrame = TRUE;
guVector speedVec;

void SCENE_load() {
	GXU_init();
	//playMod();
	GXU_loadTexture(hovercraftTex, &hoverTexObj);
	GXU_loadTexture(terrainTex, &terrainTexObj);
	GXU_loadTexture(waterTex, &waterTexObj);
	GXU_loadTexture(rayTex, &rayTexObj);

	modelHover = MODEL_setup(hovercraft_bmb);
	modelTerrain = MODEL_setup(terrain_bmb);
	modelPlane = MODEL_setup(plane_bmb);
	modelRay = MODEL_setup(ray_bmb);

	MODEL_setTexture(modelHover, &hoverTexObj);
	MODEL_setTexture(modelTerrain, &terrainTexObj);
	MODEL_setTexture(modelPlane, &waterTexObj);
	MODEL_setTexture(modelRay, &rayTexObj);

	objectTerrain = OBJECT_create(modelTerrain);
	OBJECT_scaleTo(objectTerrain, 200, 200, 200);

	objectPlane = OBJECT_create(modelPlane);
	OBJECT_scaleTo(objectPlane, 1000, 1, 1000);
	OBJECT_moveTo(objectPlane, -500, 6.1f, -500);

	planeRay = OBJECT_create(modelRay);
	OBJECT_scaleTo(planeRay, 1.5f, 4, 1.5f);

	GAME_init(objectTerrain, objectPlane);

	/* Wait for controllers */
	INPUT_waitForControllers();

	u8 i, split = 0;
	for (i = 0; i < MAX_PLAYERS; i++) {
		if (INPUT_isConnected(i) == TRUE) {
			split++;
			//guVector position = { rand() % 200, 30.f, rand() % 200 };
			guVector position = { -5, 30.f, -5 };
			GAME_createPlayer(i, modelHover, position);
		}
	}

	/* We went through all players, so we know how to split the screen */
	u8 splitCur = 0;
	for (i = 0; i < MAX_PLAYERS; i++) {
		if (INPUT_isConnected(i) == TRUE) {
			GXU_setupCamera(&GAME_getPlayerData(i)->camera, split, ++splitCur);
		}
	}
}

void SCENE_render() {
	/* Render time */
	GX_SetNumChans(1);

	GAME_updateWorld();

	u8 i;
	for (i = 0; i < MAX_PLAYERS; i++) {
		player_t* player = GAME_getPlayerData(i);
		if (player->isPlaying == TRUE) {
			GAME_updatePlayer(i);
			GAME_renderPlayerView(i);
		}
	}

	/* Flip framebuffer */
	GXU_done();
}

void SCENE_renderPlayer(Mtx viewMtx) {
	/* Enable Light */
	GXU_setLight(viewMtx, lightColor);

	/* Draw terrain */
	/* Lighting on*/
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GX_SetTevOp(GX_TEVSTAGE0, GX_BLEND);
	GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);
	OBJECT_render(objectTerrain, viewMtx);
	OBJECT_render(objectPlane, viewMtx);

	u8 i;
	for (i = 0; i < MAX_PLAYERS; i++) {
		player_t* player = GAME_getPlayerData(i);
		if (player->isPlaying == TRUE) {
			OBJECT_render(player->hovercraft, viewMtx);
		}
	}

	/* Draw ray */
	/* Lighting off*/
	GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
	GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
	GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);
	OBJECT_render(planeRay, viewMtx);

}