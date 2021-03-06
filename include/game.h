/*! \file game.h
 *  \brief Game logic handling
 */
#ifndef _GAME_H
#define _GAME_H

#include <ogc/gu.h>
#include "object.h"
#include "input.h"
#include "gxutils.h"

/*! Maximum number of players */
#define MAX_PLAYERS 4

typedef enum {
	PICKUP_NONE      = 0,
	PICKUP_SOMETHING = 1
} pickupType;

/*! Player structure */
typedef struct {
	BOOL         isPlaying;     /*< Is the player... playing?  */
	BOOL         isGrounded;    /*< Is it on the ground?       */
	object_t*    hovercraft;    /*< Hovercraft object          */
	guVector     velocity;      /*< Current velocity           */
	camera_t     camera;        /*< Player's camera            */
	controller_t controller;    /*< Player's controller data   */
	pickupType   currentPickup; /*< Current pickup (0 if none) */
} player_t;

/*! Pickup data */
typedef struct {
	object_t*  object;  /*< Pickup object                    */
	BOOL       enable;  /*< Is the pickup enabled?           */
	float      timeout; /*< Time before the pickup reappears */
	pickupType type;    /*< Pickup type                      */
} pickup_t;

/*! \brief Initialize In-game data
 */
void GAME_init();

/*! \brief Create a player
 *  \param controllerInfo  Controller to bind player to
 *  \param hovercraftModel Hovercraft model
 *  \param startPosition   Starting position
 */
void GAME_createPlayer(controller_t controllerInfo, model_t* hovercraftModel, guVector startPosition);

/*! \brief Remove a player
 *  \param player Player to remove
 */
void GAME_removePlayer(player_t* player);

/*! \brief Update player's state (physics/logic)
 *  \param player Player to update
 */
void GAME_updatePlayer(player_t* player);

/*! \brief Update global/world state (physics/logic)
*/
void GAME_updateWorld();

/*! \brief Render a player's view
 *  \param player Player to render
 */
void GAME_renderPlayerView(player_t* player);

/*! Updates the scene and renders all the player views inside it */
void GAME_render();

/*! \brief Renders the scene
 *  \param viewMtx View matrix to use for rendering
 */
void GAME_renderView(Mtx viewMtx);

#endif
