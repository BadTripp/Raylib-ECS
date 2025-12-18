#include<stdio.h>
#include<raylib.h>
#include<stdint.h>

#define MAX_ENTITIES 64
#define GAME_W  800
#define GAME_H  600
typedef int Entity;

typedef  enum { 
	C_ALIVE    = 1u << 0, // 0000 0001
	C_POSITION = 1u << 1, // 0000 0010
	C_VELOCITY = 1u << 2, // 0000 0100	
} flags;

typedef enum {
	T_PLAYER = 0,
	T_NPC    = 1,
	T_PROJECTILE = 2,	
} type;

typedef enum {	
	S_RECTANGLE = 0,
	S_CIRCLE = 1,
	S_TRIANGLE = 2,
} shape;

typedef struct {
	float x, y;
	int h,w;	
} Position;

typedef struct {
	float vx,vy;
} Velocity;

typedef struct {
	uint8_t   flag[MAX_ENTITIES]; 
	uint8_t   type_shape[MAX_ENTITIES];
	Position  pos[MAX_ENTITIES];
	Velocity  vel[MAX_ENTITIES];
} World;

Entity CreateEntity(World *w) {
	for(int i = 0; i < MAX_ENTITIES; i++) {
		if(!(w->flag[i] & C_ALIVE)) {
			w->flag[i] = w->flag[i] | C_ALIVE;	
			return i;
		}	
	}	
		return -1;
}

void AddPosition(World *w, Entity e, float x, float y, int he, int we) {
	w->pos[e].h = he;
	w->pos[e].w = we;
	w->pos[e].x = x;
	w->pos[e].y = y;
	w->flag[e] |= C_POSITION;	
}

void AddVelocity(World *w, Entity e, float vx, float vy) {
	w->vel[e].vx = vx;
	w->vel[e].vy = vy;	
	w->flag[e] |= C_VELOCITY ;
}

void AddTypeShape(World *w,Entity e, type t, shape s) {
	w->type_shape[e] = ((t & 15u)<<4 | s & 15u );	
}
uint8_t getType(uint8_t x){ return ((x >> 4) & 15u);
}
uint8_t getShape(uint8_t x){ return (x & 15u);
}

void SystemRender(World *w) {
	for(int i = 0; i < MAX_ENTITIES; i++) {
		if(!(w->flag[i] & C_ALIVE) || !(w->flag[i] & C_POSITION)) continue;	

		DrawRectangle((int)w->pos[i].x,(int)w->pos[i].y,w->pos[i].h,w->pos[i].w,BLUE);
	}
	
}
	
bool WindowCollision(World *w, int i, int direction) {

	if(direction == 0 && w->pos[i].x + w->vel[i].vx > GAME_W - w->pos[i].w) return 0;	
	if(direction == 1 && w->pos[i].x - w->vel[i].vx < 1) return 0;
	if(direction == 2 && w->pos[i].y + w->vel[i].vy > GAME_H - w->pos[i].h) return 0;
	if(direction == 3 && w->pos[i].y - w->vel[i].vy < 1) return 0;
	
	return 1;
}

void SystemMovement(World *w) {
	for(int i = 0; i < MAX_ENTITIES; i++) {
		if(!(w->flag[i] & C_ALIVE) || !(w->flag[i] & C_VELOCITY) || !(getType(w->type_shape[i]) ==  T_PLAYER )) continue;
		
		if(IsKeyDown(KEY_RIGHT) && WindowCollision(w,i,0))  w->pos[i].x += w->vel[i].vx;			
		if(IsKeyDown(KEY_LEFT)  && WindowCollision(w,i,1))  w->pos[i].x -= w->vel[i].vx;
		if(IsKeyDown(KEY_UP)    && WindowCollision(w,i,3))  w->pos[i].y -= w->vel[i].vy;
		if(IsKeyDown(KEY_DOWN)  && WindowCollision(w,i,2))  w->pos[i].y += w->vel[i].vy;
	}
}



int main (void) {
	InitWindow(GAME_W,GAME_H,"Game");
	SetTargetFPS(60);

	World world = {0};

	Entity player = CreateEntity(&world);
	AddPosition(&world,player,40,GAME_H - 52,20,20);	
	AddVelocity(&world,player,4.5,4.5);
	AddTypeShape(&world,player,0,0);	

	Entity enemy = CreateEntity(&world);
	AddPosition(&world,enemy,40,50,100,100);
	AddTypeShape(&world,enemy,1,0);

	while(!WindowShouldClose()) {
	
		
		BeginDrawing();	
		ClearBackground(RAYWHITE);

		SystemRender(&world);
		SystemMovement(&world);
		EndDrawing();

	}	
	CloseWindow();
	return 1;
}
