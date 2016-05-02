//
// Ardu3D : A small Arduboy 3D demo that displays a 3D Cube or a 3D "HELLO WORLD" text
//
// Copyright (C) 2016 Jerome Perrot (Initgraph)
//
// Version : 0.1
//
// Notes :
//
//   You need the Arduboy library in order to compile this program, please see the link below for more explanations :
//   
//   https://github.com/arduboy/arduboy
// 
// Special thanks :
//
//   - "Programmation graphique en C", a book by Walter Siracusa (ISBN : 2-7361-2136-8) : this book contains very good tricks about 2D and 3D  
//     graphics.
//   - "Direct3D 3D temps réel sous Windows", a book by Denis Duplan and Stéphane Bontemps (ISBN : 2-212-09061-7) : this book contains a very
//     good chapter about 3D matrix used by Direct3D.
//
// Licence :  
//
//   This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
//   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc.,
//   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// For any remarks, suggestions or questions you can contact me with the email address contact@initgraph.com or via Twitter : @InitgraphDotCom
// You can also visit my website : http://www.initgraph.com 
//
// Enjoy !
//

#include <SPI.h>
#include <EEPROM.h>
#include "Arduboy.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// Constants
// ------------------------------------------------------------------------------------------------------------------------------------------------------

// Arduboy device constants
#define SCREEN_WIDTH 128      // Arduboy screen width.     
#define SCREEN_HEIGHT 64      // Arduboy screen height. 
#define HALF_SCREEN_WIDTH 64  // Half Arduboy screen width. 
#define HALF_SCREEN_HEIGHT 32 // Half Arduboy screen height.

// 3D rendering constants
#define MULTIPLY_BY_32 5     // Can be used in a bit shift operation in order to multiply a value by 32.
#define DIVIDE_BY_32 5       // Can be used in a bit shift operation in order to divide a value by 32.
#define FRONT 5              // The "Front" value used for projection. Also called "Near" in some 3D engine.
#define FRONT_BY_64 320      // The "Front" value multiplied by 64.
#define MODEL_CUBE 1         // Cube model ID.
#define MODEL_HELLO_WORLD 2  // "HELLO WORLD" model ID.

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// 3D vertex object used by models
// ------------------------------------------------------------------------------------------------------------------------------------------------------
class A3DVertex {
  
  public:

    // Position
    int16_t x;
    int16_t y;
    int16_t z;

    A3DVertex();
    A3DVertex(int16_t x, int16_t y, int16_t z);
};

A3DVertex::A3DVertex() { }

A3DVertex::A3DVertex(int16_t vertexX, int16_t vertexY, int16_t vertexZ) {

  this->x = vertexX;
  this->y = vertexY;
  this->z = vertexZ;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// 2D vertex object used by models
// ------------------------------------------------------------------------------------------------------------------------------------------------------
class A2DVertex {
  
  public:

    // Position
    int16_t x;
    int16_t y;

    A2DVertex();
};

A2DVertex::A2DVertex() { }

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// Cube model
// ------------------------------------------------------------------------------------------------------------------------------------------------------
class A3DCube {
  
  public:
 
    // ID
    uint8_t id = MODEL_CUBE;

    // Position
    int16_t x = 0;
    int16_t y = 0;
    int16_t z = 750;

    // Vertices
    uint8_t verticesCount = 8;
    A2DVertex screenVertices[8];
    A3DVertex vertices[8] = {

      A3DVertex(-50, 50, 50),
      A3DVertex(50, 50, 50),
      A3DVertex(50, -50, 50),
      A3DVertex(-50, -50, 50),
      A3DVertex(-50, 50, -50),
      A3DVertex(50, 50, -50),
      A3DVertex(50, -50, -50),
      A3DVertex(-50, -50, -50)
    };

    A3DCube(); 
};

A3DCube::A3DCube() { }

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// "HELLO WORLD" model
// ------------------------------------------------------------------------------------------------------------------------------------------------------
class A3DHelloWorld {
  
  public:

    // ID
    uint8_t id = MODEL_HELLO_WORLD;

    // Position
    int16_t x = 0;
    int16_t y = 0;
    int16_t z = 350;

    // Vertices
    uint8_t verticesCount = 44;
    A2DVertex screenVertices[44];
    A3DVertex vertices[44] = {

      // H letter
      A3DVertex(-56, 28, 0),
      A3DVertex(-56, 4, 0),
      A3DVertex(-40, 28, 0),
      A3DVertex(-40, 4, 0),
      A3DVertex(-56, 16, 0),
      A3DVertex(-40, 16, 0), 

      // E letter
      A3DVertex(-32, 28, 0),
      A3DVertex(-32, 4, 0),
      A3DVertex(-16, 28, 0),
      A3DVertex(-32, 16, 0),
      A3DVertex(-16, 16, 0),
      A3DVertex(-16, 4, 0),

      // First L letter
      A3DVertex(-8, 28, 0),
      A3DVertex(-8, 4, 0),
      A3DVertex(8, 4, 0),

      // Second L letter
      A3DVertex(16, 28, 0),
      A3DVertex(16, 4, 0),
      A3DVertex(32, 4, 0),

      // First O letter
      A3DVertex(40, 28, 0),
      A3DVertex(40, 4, 0),
      A3DVertex(56, 4, 0),
      A3DVertex(56, 28, 0),
      
      // W letter
      A3DVertex(-56, -4, 0),
      A3DVertex(-52, -28, 0),
      A3DVertex(-48, -16, 0),
      A3DVertex(-44, -28, 0),
      A3DVertex(-40, -4, 0),
      
      // Second O letter
      A3DVertex(-32, -4, 0),
      A3DVertex(-32, -28, 0),
      A3DVertex(-16, -28, 0),
      A3DVertex(-16, -4, 0),
      
      // R letter
      A3DVertex(-8, -4, 0),
      A3DVertex(-8, -28, 0),
      A3DVertex(8, -4, 0),
      A3DVertex(8, -16, 0),
      A3DVertex(-4, -16, 0),
      A3DVertex(8, -28, 0),
      
      // Third L letter
      A3DVertex(16, -4, 0),
      A3DVertex(16, -28, 0),
      A3DVertex(32, -28, 0),
      
      // D letter
      A3DVertex(40, -4, 0),
      A3DVertex(40, -28, 0),
      A3DVertex(56, -5, 0),
      A3DVertex(56, -27, 0)
    };

    A3DHelloWorld();
};

A3DHelloWorld::A3DHelloWorld() { }

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// Globals variables
// ------------------------------------------------------------------------------------------------------------------------------------------------------
Arduboy arduboy;

// Rotation angle expressed in degrees with a PI/128 step.
// (10 value means 10 * PI/128)
uint8_t rotAngle = 0;    

// Model to display (enable one and disable other)
//A3DCube model;
A3DHelloWorld model;

// Cosinus array for rotations
// Each cosinus value is multiplied by 32 in order to use integers instead of floats.
PROGMEM const int8_t cosBy32[256] = {
  
  32, 32, 32, 32, 32, 32, 32, 32, 31, 31, 
  31, 31, 31, 30, 30, 30, 30, 29, 29, 29, 
  28, 28, 27, 27, 27, 26, 26, 25, 25, 24, 
  24, 23, 23, 22, 21, 21, 20, 20, 19, 18, 
  18, 17, 16, 16, 15, 14, 14, 13, 12, 12, 
  11, 10, 9, 9, 8, 7, 6, 5, 5, 4, 
  3, 2, 2, 1, 0, -1, -2, -2, -3, -4, 
  -5, -5, -6, -7, -8, -9, -9, -10, -11, -12, 
  -12, -13, -14, -14, -15, -16, -16, -17, -18, -18, 
  -19, -20, -20, -21, -21, -22, -23, -23, -24, -24, 
  -25, -25, -26, -26, -27, -27, -27, -28, -28, -29, 
  -29, -29, -30, -30, -30, -30, -31, -31, -31, -31, 
  -31, -32, -32, -32, -32, -32, -32, -32, -32, -32, 
  -32, -32, -32, -32, -32, -32, -31, -31, -31, -31, 
  -31, -30, -30, -30, -30, -29, -29, -29, -28, -28, 
  -27, -27, -27, -26, -26, -25, -25, -24, -24, -23, 
  -23, -22, -21, -21, -20, -20, -19, -18, -18, -17, 
  -16, -16, -15, -14, -14, -13, -12, -12, -11, -10, 
  -9, -9, -8, -7, -6, -5, -5, -4, -3, -2, 
  -2, -1, 0, 1, 2, 2, 3, 4, 5, 5, 
  6, 7, 8, 9, 9, 10, 11, 12, 12, 13, 
  14, 14, 15, 16, 16, 17, 18, 18, 19, 20, 
  20, 21, 21, 22, 23, 23, 24, 24, 25, 25, 
  26, 26, 27, 27, 27, 28, 28, 29, 29, 29, 
  30, 30, 30, 30, 31, 31, 31, 31, 31, 32, 
  32, 32, 32, 32, 32, 32
}; 

// Sinus array for rotations
// Each sinus value is multiplied by 32 in order to use integers instead of floats.
PROGMEM const int8_t sinBy32[256] = {
  
  0, 1, 2, 2, 3, 4, 5, 5, 6, 7, 
  8, 9, 9, 10, 11, 12, 12, 13, 14, 14, 
  15, 16, 16, 17, 18, 18, 19, 20, 20, 21, 
  21, 22, 23, 23, 24, 24, 25, 25, 26, 26, 
  27, 27, 27, 28, 28, 29, 29, 29, 30, 30, 
  30, 30, 31, 31, 31, 31, 31, 32, 32, 32, 
  32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 
  32, 32, 31, 31, 31, 31, 31, 30, 30, 30, 
  30, 29, 29, 29, 28, 28, 27, 27, 27, 26, 
  26, 25, 25, 24, 24, 23, 23, 22, 21, 21, 
  20, 20, 19, 18, 18, 17, 16, 16, 15, 14, 
  14, 13, 12, 12, 11, 10, 9, 9, 8, 7, 
  6, 5, 5, 4, 3, 2, 2, 1, 0, -1, 
  -2, -2, -3, -4, -5, -5, -6, -7, -8, -9, 
  -9, -10, -11, -12, -12, -13, -14, -14, -15, -16, 
  -16, -17, -18, -18, -19, -20, -20, -21, -21, -22, 
  -23, -23, -24, -24, -25, -25, -26, -26, -27, -27, 
  -27, -28, -28, -29, -29, -29, -30, -30, -30, -30, 
  -31, -31, -31, -31, -31, -32, -32, -32, -32, -32, 
  -32, -32, -32, -32, -32, -32, -32, -32, -32, -32, 
  -31, -31, -31, -31, -31, -30, -30, -30, -30, -29, 
  -29, -29, -28, -28, -27, -27, -27, -26, -26, -25, 
  -25, -24, -24, -23, -23, -22, -21, -21, -20, -20, 
  -19, -18, -18, -17, -16, -16, -15, -14, -14, -13, 
  -12, -12, -11, -10, -9, -9, -8, -7, -6, -5, 
  -5, -4, -3, -2, -2, -1
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// Initializations
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {

  // Initialize Arduboy library
  arduboy.begin();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// Main loop
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {

  int8_t rotCosBy32;  // Cosinus value muliplied by 32 used for rotate the object
  int8_t rotSinBy32;  // Sinus value muliplied by 32 used for rotate the object
  int16_t newVertX;   // New vertex X value after rotation  
  int16_t newVertY;   // New vertex Y value after rotation
  int16_t newVertZ;   // New vertex Z value after rotation
  int16_t projectedX; // Projected vertex X value in the projection surface
  int16_t projectedY; // Projected vertex Y value in the projection surface
  
  // Clear display
  arduboy.clear();

  // Update angle value and read its cosinus and sinus value
  rotAngle++;
  rotAngle = rotAngle & 255; // Equals to "rotAngle = rotAngle % 256"
  rotCosBy32 = pgm_read_byte(cosBy32 + rotAngle);
  rotSinBy32 = pgm_read_byte(sinBy32 + rotAngle);
  
  // Update model coordinates
  for (uint8_t i=0; i<model.verticesCount; i++) {

    // X axis rotation
    /*newVertX = model.vertices[i].x;
    newVertY = (model.vertices[i].y * rotCosBy32 - model.vertices[i].z * rotSinBy32) >> DIVIDE_BY_32;
    newVertZ = (model.vertices[i].y * rotSinBy32 + model.vertices[i].z * rotCosBy32) >> DIVIDE_BY_32;*/

    // Y axis rotation
    newVertX = (model.vertices[i].x * rotCosBy32 - model.vertices[i].z * rotSinBy32) >> DIVIDE_BY_32;
    newVertY = model.vertices[i].y;
    newVertZ = (-model.vertices[i].x * rotSinBy32 + model.vertices[i].z * rotCosBy32) >> DIVIDE_BY_32;
    
    // Z axis rotation
    /*newVertX = (model.vertices[i].x * rotCosBy32 - model.vertices[i].y * rotSinBy32) >> DIVIDE_BY_32;
    newVertY = (model.vertices[i].x * rotSinBy32 + model.vertices[i].y * rotCosBy32) >> DIVIDE_BY_32;
    newVertZ = model.vertices[i].z;*/

    // Update model position on the world
    newVertZ += model.z;

    // Projection of the vertex on a 2D surface (converts 3D coordinates into 2D coordinates)
    // A visible vertex will be between -64 and 64 values.
    projectedX = (newVertX * FRONT_BY_64) / newVertZ;
    projectedY = (newVertY * FRONT_BY_64) / newVertZ;

    // Convert 2D surface coordinates into screen coordinates 
    model.screenVertices[i].x = projectedX + HALF_SCREEN_WIDTH;
    model.screenVertices[i].y = SCREEN_HEIGHT - (projectedY + HALF_SCREEN_HEIGHT);
  }

  // Display the cube model if it has been selected
  if (model.id == MODEL_CUBE) {

    arduboy.drawLine(model.screenVertices[0].x, model.screenVertices[0].y, model.screenVertices[1].x, model.screenVertices[1].y, 1);
    arduboy.drawLine(model.screenVertices[1].x, model.screenVertices[1].y, model.screenVertices[2].x, model.screenVertices[2].y, 1);
    arduboy.drawLine(model.screenVertices[2].x, model.screenVertices[2].y, model.screenVertices[3].x, model.screenVertices[3].y, 1);
    arduboy.drawLine(model.screenVertices[3].x, model.screenVertices[3].y, model.screenVertices[0].x, model.screenVertices[0].y, 1);
    arduboy.drawLine(model.screenVertices[4].x, model.screenVertices[4].y, model.screenVertices[5].x, model.screenVertices[5].y, 1);
    arduboy.drawLine(model.screenVertices[5].x, model.screenVertices[5].y, model.screenVertices[6].x, model.screenVertices[6].y, 1);
    arduboy.drawLine(model.screenVertices[6].x, model.screenVertices[6].y, model.screenVertices[7].x, model.screenVertices[7].y, 1);
    arduboy.drawLine(model.screenVertices[7].x, model.screenVertices[7].y, model.screenVertices[4].x, model.screenVertices[4].y, 1);
    arduboy.drawLine(model.screenVertices[0].x, model.screenVertices[0].y, model.screenVertices[4].x, model.screenVertices[4].y, 1);
    arduboy.drawLine(model.screenVertices[1].x, model.screenVertices[1].y, model.screenVertices[5].x, model.screenVertices[5].y, 1);
    arduboy.drawLine(model.screenVertices[2].x, model.screenVertices[2].y, model.screenVertices[6].x, model.screenVertices[6].y, 1);
    arduboy.drawLine(model.screenVertices[3].x, model.screenVertices[3].y, model.screenVertices[7].x, model.screenVertices[7].y, 1);
  }

  // Display the "HELLO WORLD" model if it has been selected
  else {

    // Display H letter
    arduboy.drawLine(model.screenVertices[0].x, model.screenVertices[0].y, model.screenVertices[1].x, model.screenVertices[1].y, 1);
    arduboy.drawLine(model.screenVertices[2].x, model.screenVertices[2].y, model.screenVertices[3].x, model.screenVertices[3].y, 1);
    arduboy.drawLine(model.screenVertices[4].x, model.screenVertices[4].y, model.screenVertices[5].x, model.screenVertices[5].y, 1);
  
    // Display E letter
    arduboy.drawLine(model.screenVertices[6].x, model.screenVertices[6].y, model.screenVertices[7].x, model.screenVertices[7].y, 1);
    arduboy.drawLine(model.screenVertices[6].x, model.screenVertices[6].y, model.screenVertices[8].x, model.screenVertices[8].y, 1);
    arduboy.drawLine(model.screenVertices[9].x, model.screenVertices[9].y, model.screenVertices[10].x, model.screenVertices[10].y, 1);
    arduboy.drawLine(model.screenVertices[7].x, model.screenVertices[7].y, model.screenVertices[11].x, model.screenVertices[11].y, 1);
  
    // Display fist L letter
    arduboy.drawLine(model.screenVertices[12].x, model.screenVertices[12].y, model.screenVertices[13].x, model.screenVertices[13].y, 1);
    arduboy.drawLine(model.screenVertices[13].x, model.screenVertices[13].y, model.screenVertices[14].x, model.screenVertices[14].y, 1);
  
    // Display second L letter
    arduboy.drawLine(model.screenVertices[15].x, model.screenVertices[15].y, model.screenVertices[16].x, model.screenVertices[16].y, 1);
    arduboy.drawLine(model.screenVertices[16].x, model.screenVertices[16].y, model.screenVertices[17].x, model.screenVertices[17].y, 1);
  
    // Display O letter
    arduboy.drawLine(model.screenVertices[18].x, model.screenVertices[18].y, model.screenVertices[19].x, model.screenVertices[19].y, 1);
    arduboy.drawLine(model.screenVertices[19].x, model.screenVertices[19].y, model.screenVertices[20].x, model.screenVertices[20].y, 1);
    arduboy.drawLine(model.screenVertices[20].x, model.screenVertices[20].y, model.screenVertices[21].x, model.screenVertices[21].y, 1);
    arduboy.drawLine(model.screenVertices[21].x, model.screenVertices[21].y, model.screenVertices[18].x, model.screenVertices[18].y, 1);
  
    // Display W letter
    arduboy.drawLine(model.screenVertices[22].x, model.screenVertices[22].y, model.screenVertices[23].x, model.screenVertices[23].y, 1);
    arduboy.drawLine(model.screenVertices[23].x, model.screenVertices[23].y, model.screenVertices[24].x, model.screenVertices[24].y, 1);
    arduboy.drawLine(model.screenVertices[24].x, model.screenVertices[24].y, model.screenVertices[25].x, model.screenVertices[25].y, 1);
    arduboy.drawLine(model.screenVertices[25].x, model.screenVertices[25].y, model.screenVertices[26].x, model.screenVertices[26].y, 1);
  
    // Display O letter
    arduboy.drawLine(model.screenVertices[27].x, model.screenVertices[27].y, model.screenVertices[28].x, model.screenVertices[28].y, 1);
    arduboy.drawLine(model.screenVertices[28].x, model.screenVertices[28].y, model.screenVertices[29].x, model.screenVertices[29].y, 1);
    arduboy.drawLine(model.screenVertices[29].x, model.screenVertices[29].y, model.screenVertices[30].x, model.screenVertices[30].y, 1);
    arduboy.drawLine(model.screenVertices[30].x, model.screenVertices[30].y, model.screenVertices[27].x, model.screenVertices[27].y, 1);
  
    // Display R letter
    arduboy.drawLine(model.screenVertices[31].x, model.screenVertices[31].y, model.screenVertices[32].x, model.screenVertices[32].y, 1);
    arduboy.drawLine(model.screenVertices[31].x, model.screenVertices[31].y, model.screenVertices[33].x, model.screenVertices[33].y, 1);
    arduboy.drawLine(model.screenVertices[33].x, model.screenVertices[33].y, model.screenVertices[34].x, model.screenVertices[34].y, 1);
    arduboy.drawLine(model.screenVertices[34].x, model.screenVertices[34].y, model.screenVertices[35].x, model.screenVertices[35].y, 1);
    arduboy.drawLine(model.screenVertices[35].x, model.screenVertices[35].y, model.screenVertices[36].x, model.screenVertices[36].y, 1);

    // Display third L letter
    arduboy.drawLine(model.screenVertices[37].x, model.screenVertices[37].y, model.screenVertices[38].x, model.screenVertices[38].y, 1);
    arduboy.drawLine(model.screenVertices[38].x, model.screenVertices[38].y, model.screenVertices[39].x, model.screenVertices[39].y, 1);
  
    // Display D letter
    arduboy.drawLine(model.screenVertices[40].x, model.screenVertices[40].y, model.screenVertices[41].x, model.screenVertices[41].y, 1);
    arduboy.drawLine(model.screenVertices[40].x, model.screenVertices[40].y, model.screenVertices[42].x, model.screenVertices[42].y, 1);
    arduboy.drawLine(model.screenVertices[42].x, model.screenVertices[42].y, model.screenVertices[43].x, model.screenVertices[43].y, 1);
    arduboy.drawLine(model.screenVertices[43].x, model.screenVertices[43].y, model.screenVertices[41].x, model.screenVertices[41].y, 1);
  }

  // Update display
  arduboy.display();
}
