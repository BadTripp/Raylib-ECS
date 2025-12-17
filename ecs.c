#include<stdio.h>
#include<raylib.h>

#define MAX_ENTITIES 64
#define GAME_W  800
#define GAME_H  600
typedef int Entity;

typedef struct {
	float x, y;
	int h,w;
	bool active;
} Position;

typedef struct {
	float vx,vy;
	bool active;
} Velocity;

typedef struct {
	bool alive[MAX_ENTITIES]; 
	Position  pos[MAX_ENTITIES];
	Velocity  vel[MAX_ENTITIES];
} World;

Entity CreateEntity(World *w) {
	for(int i = 0; i < MAX_ENTITIES; i++) {
		if(!w->alive[i]) {
			w->alive[i] = 1;
			w->pos[i].active = 0;
			w->vel[i].active = 0;
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
	w->pos[e].active = 1;	
}

void AddVelocity(World *w, Entity e, float vx, float vy) {
	w->vel[e].vx = vx;
	w->vel[e].vy = vy;	
	w->vel[e].active = 1;
}

void SystemRender(World *w) {
	for(int i = 0; i < MAX_ENTITIES; i++) {
		if(!w->alive[i] || !w->pos[i].active) continue;	

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
		if(!w->alive[i] || !w->vel[i].active) continue;
		
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
	
	while(!WindowShouldClose()) {
		ClearBackground(RAYWHITE);
		
		BeginDrawing();	
		SystemRender(&world);
		SystemMovement(&world);
		EndDrawing();
	}	
	CloseWindow();
	return 1;
}
