#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

//gets the index of an x and y coordinate
int XYtoIndex(Vector2* v2);

struct placedBlock{
    short tile;
    int age;
};

int main(){
    //techncal setup (vars, new window etc)
  float scale = 4;
  InitWindow(1280, 720, "Tile Editor");
  SetTargetFPS(60);
 
  unsigned short* tiles = malloc(sizeof(unsigned short)*(32*32));
  int memoryINC = 0;
  struct placedBlock* memory = malloc(sizeof(struct placedBlock)*(32*32));
  int i = 0;
  while(i < 32*32){
      tiles[i] = 0;
      i++;
  }
  Vector2 pointer = (Vector2){0, 0};
  Vector2 TilePointer = (Vector2){0, 0};

  int inc = 0;
  int secondCounter = 0;
  int frameCounter = 0;
  int shouldColapsTimer = 0;
  int colapsSpeed = 8;
  int numberOfTiles = 20;
  int numberOfPowerUps = 0;
  
    InitAudioDevice();              // Initialize audio device

    Music music = LoadMusicStream("city_blues_redfarn.mp3");
    PlayMusicStream(music);
  
  //sets up camera

  Camera2D cam;
    cam.target = pointer;
    cam.offset = (Vector2){ 1280/2.0f, 720/2.0f };
    cam.rotation = 0.0f;
    cam.zoom = 1.0f;
    
    //sets up player
    pointer.x = 16;
    pointer.y = 16;
    
    tiles[XYtoIndex(&pointer)] = 1;
    

    //main while loop
    
  while (!WindowShouldClose()) {
      UpdateMusicStream(music); 
      //prevents the tile deleater from accsesing unalocated memory
      if(memoryINC >= 32*32)
      
        // Timing

        // Clears screen and sets up camera
        BeginDrawing();
        ClearBackground(WHITE);

        BeginMode2D(cam);


    cam.zoom = scale;
    
    inc = 0;
    TilePointer.x = 0;
    TilePointer.y = 0;
    //sees if you picked up a powerup
    if(tiles[XYtoIndex(&pointer)] == 2){
        numberOfTiles = numberOfTiles + 8;
        numberOfPowerUps--;
    }
    
    //renders tiles
    while(inc < 32*32){
      
        DrawRectangleLines(TilePointer.x*16, TilePointer.y*16, 16, 16, GRAY);
        if(tiles[inc] == 0);
        else if(tiles[inc] == 1){
            DrawRectangle(TilePointer.x*16, TilePointer.y*16, 16, 16, RED);
        }
        else if(tiles[inc] == 2){
            DrawRectangle(TilePointer.x*16, TilePointer.y*16, 16, 16, BLUE);
        }
        else DrawRectangle(TilePointer.x*16, TilePointer.y*16, 16, 16, BLACK);


      TilePointer.x++;
      if(TilePointer.x >= 32){
        TilePointer.x = 0;
        TilePointer.y++;
      }
      inc++;
    }
    //kills old tiles
    if(shouldColapsTimer <= secondCounter){
        int incc = memoryINC;
        while(incc >= 0){
            if(memory[incc].age < secondCounter-colapsSpeed){
                tiles[memory[incc].tile] = 0;
            }
            incc--;
        }
    }
    else shouldColapsTimer = secondCounter + colapsSpeed;
    //sees if Enter has been pressed and if so, places a tile on that spot;
    
    if(tiles[XYtoIndex(&pointer)] != 1 && numberOfTiles > 0){
        /*//int res = 32*pointer.y;
        int index = pointer.x;
        index = index + pointer.x*32;*/

        if(XYtoIndex(&pointer) < 32*32 && XYtoIndex(&pointer) > 0){
            tiles[XYtoIndex(&pointer)] = 1;
            memory[memoryINC].tile = XYtoIndex(&pointer);
            memory[memoryINC].age = secondCounter;
            memoryINC++;
        }
        else printf("Out Of Bounds!\n");
        numberOfTiles--;
    }
    //destroys old blocks
    
    //draws player
    DrawRectangleLines(pointer.x*16, pointer.y*16, 16, 16, RED);
    
    //draws stats

    
    EndMode2D();
        char textBuff[20];
        sprintf(textBuff, "%d", numberOfTiles);
        DrawRectangle(30, 30, 32, 32, RED);
        DrawText(textBuff, 64, 64, 10, BLACK);

    EndDrawing();
  
    //fancy camera movment to track the player
    if(cam.target.x < pointer.x*16) cam.target.x++;
    if(cam.target.x > pointer.x*16) cam.target.x--;

    if(cam.target.y < pointer.y*16) cam.target.y++;
    if(cam.target.y > pointer.y*16) cam.target.y--;

    if(IsKeyPressed(KEY_D)) pointer.x++;
    if(IsKeyPressed(KEY_A)) pointer.x--;

    if(IsKeyPressed(KEY_W)) pointer.y--;
    if(IsKeyPressed(KEY_S)) pointer.y++;
    //time and space
        frameCounter++;
        if (frameCounter >= 60) {
            frameCounter = 0;
            secondCounter++;
            printf("%d\n", secondCounter);
            if(numberOfPowerUps <= 5){
                srand(time(NULL));
                //adds random powerup
                Vector2 asinine;
                asinine.x = rand() % 32;
                asinine.y = rand() % 32;
                tiles[XYtoIndex(&asinine)] = 2;
                numberOfPowerUps++;
            }
        }
    //tries to kill you each frame
    if(tiles[XYtoIndex(&pointer)] == 0 && numberOfTiles <= 0){
        printf("You Died!\nYour Score %d\n", numberOfTiles);
        char array[2];
        int crashINC = 0;
        //kills your game :)
        
        while(1){
            crashINC++;
            array[crashINC] = crashINC;
        }
    }



  //ends
  }
    UnloadMusicStream(music);   // Unload music stream buffers from RAM

    CloseAudioDevice(); 
  CloseWindow();
  return 0;
}

int XYtoIndex(Vector2* v2){
    return v2->y*32+v2->x;
}

