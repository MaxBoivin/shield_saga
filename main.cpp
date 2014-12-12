#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include<vector>
//#include <tinyxml2.h>

//Global Constant

const float PI = 3.14159265f;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 480;

const int MAP_TILE_WIDTH = 48;
const int MAP_TILE_HEIGHT = 48;

const int TILE_NUMBER_WIDTH = ceil((float)SCREEN_WIDTH/(float)MAP_TILE_WIDTH);
const int TILE_NUMBER_HEIGHT = ceil((float)SCREEN_HEIGHT/(float)MAP_TILE_HEIGHT);

const int WORLD_WIDTH = 1;
const int WORLD_HEIGHT = 2;

const int FRAME_RATE = 1000;

const std::string APPLICATION_NAME = "Shield Saga";

//Path for the sprite sheet of the character.
const std::string CHARACTER_SPRITESHEET_PATH = "ressource/player-sprite_sheet";

const std::string TERRAIN_SPRITESHEET_PATH = "ressource/terrain-sprite_sheet";

//Path for the texture for the menu
const std::string DASHLEY_FONT_PATH = "ressource/Dashley.ttf";
const int DASHLEY_FONT_POINT = 500;

enum zPlane
{
    BACKGROUND,
    MIDGROUND,
    FOREGROUND,
    Z_PLANE_COUNT
};

const std::string zPlaneName[Z_PLANE_COUNT]
{
    "BACKGROUND",
    "MIDGROUND",
    "FOREGROUND"
};

//Character state constants
enum characterState
{
    IDLE,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    FORWARD_LEFT,
    FORWARD_RIGHT,
    BACKWARD_LEFT,
    BACKWARD_RIGHT,
    STATE_COUNT //This one serve no purpose other than to have the number of STATE available
};

//This are just an array on strings for check the against the enum
const std::string characterStateName[STATE_COUNT] =
{
    "IDLE",
    "FORWARD",
    "BACKWARD",
    "LEFT",
    "RIGHT",
    "FORWARD_LEFT",
    "FORWARD_RIGHT",
    "BACKWARD_LEFT",
    "BACKWARD_RIGHT"
};

//Weapon combo constants
enum weaponCombo
{
    UNARMED,
    SHIELD_SPEAR,
    SHIELD_SWORD,
    SHIELD_AXE,
    SPEAR,
    SWORD,
    AXE,
    SHIELD,
    COMBO_COUNT  //This one serve no purpose other than to have the number of STATE available
};

//This are just an array on strings for check the against the enum
const std::string weaponComboName[COMBO_COUNT] =
{
    "UNARMED",
    "SHIELD_SPEAR",
    "SHIELD_SWORD",
    "SHIELD_AXE",
    "SPEAR",
    "SWORD",
    "AXE",
    "SHIELD"
};

enum terrainType
{
    DIRT,
    GRASS,
    TREE_TRUNK_SMALL,
    TREE_TRUNK_BIG,
    BLANK_GATE,
    BOX_COLLIDER,
    HV_BOX_COLLIDER,
    HH_BOX_COLLIDER,
    HHV_BOX_COLLIDER,
    BLANK,
    TERRAIN_COUNT //This one serve no purpose other than to have the number of STATE available
};

//This are just an array on strings for check the against the enum
const std::string terrainTypeName[TERRAIN_COUNT] =
{
    "DIRT",
    "GRASS",
    "TREE_TRUNK_SMALL",
    "TREE_TRUNK_BIG",
    "BLANK_GATE",
    "BOX_COLLIDER",
    "HV_BOX_COLLIDER",
    "HH_BOX_COLLIDER",
    "HHV_BOX_COLLIDER",
    "-"
};

enum collisionType
{
    NONE,
    RADIAL,
    BOX,
    GATE,
    COLLISION_COUNT
};

const std::string collisionTypeName[COLLISION_COUNT]
{
    "NONE",
    "RADIAL",
    "BOX",
    "GATE"
};

//class declaration

//some shape class to simply things

class intPoint
{

    public:

       intPoint();

       intPoint(int setX, int setY);

        int x;

        int y;

    private:

};

class floatPoint
{

    public:

       floatPoint();

       floatPoint(float setX, float setY);

        float x;

        float y;

    private:

};

class polygon
{
    public:

        polygon();

        polygon(floatPoint initPosition);

        void addVertex(floatPoint newVertexRelPosition);

        bool delVertex(int vertexNumber);

        bool setVertexRelPos(int vertexNumber, floatPoint vertexNewRelativePosition);

        floatPoint getVertexRelPos(int vertexNumber);

        floatPoint getVertexAbsPos(int vertexNumber);

        int getSideNumber();

        void cleanPolygon();

        std::vector <polygon> convexDecomposition();

        floatPoint satCollision(polygon Collider);

        void draw(SDL_Renderer* renderer);

        floatPoint position;

        float rotation;

    private:

        std::vector <floatPoint> vertex;
};

/*class triangle: public polygon
{
    public:

        triangle();

        triangle(floatPoint position, float base, float side1, float side2);

        //Function to create different type of triangle.
        void triangleIso(float base, float height);
        void triangleRect(float base, float height);
        void triangleEqui(float base);

        floatPoint vertex[3];

    private:
};

class rect: public polygon
{
    public:

        rect();

        rect(floatPoint initPosition, floatPoint initDimension);

        floatPoint dimension;

    private:

};

/*class quad: public polygon
{
    public:

        quad();

        quad(floatPoint position, float base, float height);
        void box(float base);

        floatPoint vertex[4];

    private:

};*/

class circle
{
    public:

        circle();

        circle(floatPoint setPosition, float setRadius);

        void draw(SDL_Renderer* renderer);

        floatPoint nearestPoint(floatPoint point);

        float radius;

        floatPoint position;

    private:

};

//Game specific classes

class timer
{
    public:

    //Class function

    //Constructor
    timer();

    void start();
    void pause();
    void resume();

    Uint32 getTime();

    //Class variable
    bool isPaused;

    private:

        //Class private variable

        //The clock time when the timer started
		Uint32 startTime;

		//The ticks stored when the timer was paused
		Uint32 pauseTime;

		//The time the app has been paused
		Uint32 pausedTime;
};

class mapGate //This is to manage the gate in the terrainMap.  I wish I could used something that already existed but everything would take too much memory for nothing.
{
    public:

        mapGate();

        mapGate( intPoint initPosition);

        intPoint position;

        intPoint targetWorldCoord;

        intPoint targetPosCoord;

    private:

};

class terrain
{
    public:

        //Function

        //Initializes variables
        terrain();

        //Deallocates memory
        ~terrain();

        //Render the character to a specified renderer
		void render(SDL_Renderer* renderer, int posX, int posY, int variation = 0);

        //Class variables

        //String name of the terrain
        terrainType terrainName;

        //The dimension of the piece of terrain
        int spriteW;
        int spriteH;

        //The point from where the terrain should be drawn
        int spriteCenterW;
        int spriteCenterH;

        //The position on the sprite sheet
        int spritePosX;
        int spritePosY;

        //The number of different sprite  for this type of terrain
        int spriteVariation;

        //Does this terrain block the player
        collisionType collide;

        //For terrain destruction
        float health;
        float resPiercing;
        float resCuting;
        float resSmashing;

        //If the terrain is destroyed, what replace it
        terrainType decayTo;

        //The Order in what the terrain must be rendered
        int zLayer;

        //The collision size, if applicable
        intPoint collision; //This one is also the collision radius for radial collision.
};

class terrainMap
{
    public:

        //Function

        //Initializes variables
        terrainMap();

        //Load a terrain map form a file
        bool loadMap(int mapCoordX, int mapCoordY);

        //Rendering the terrain
        void render(SDL_Renderer* renderer, zPlane plane, int zLayerBottom = 0, int zLayerTop = 9);

        //Class variables

        intPoint mapSize;

        terrain tileMap[TILE_NUMBER_WIDTH][TILE_NUMBER_HEIGHT][Z_PLANE_COUNT];

        std::vector <intPoint> vTerrainCollide;

        std::vector <polygon> vCollisionPolygon;

        std::vector <circle> vCollisionRadial;

        std::vector <mapGate> vGate;

    private:

};

class animation
{
    public:

        //Class function

        //Initializes variables
        animation();

        //Function that return a render quad
        SDL_Rect renderQuad(int posX, int posY);

        //Function that return a render sprite
        SDL_Rect renderSprite(Uint32 stateTimer);


        //Class variables

        //The number of frames for the animation
        int frameCount;

        //The size of each frame
        int frameW;
        int frameH;

        //The center of the frame
        int frameCenterW;
        int frameCenterH;

        //The position of the first frame of the spritesheet
        int framePosX;
        int framePosY;

        //The duration of the animation
        Uint32 animDuration;


    private:


};

class character
{
    public:

        //Function

        //Initializes variables
        character();

        //Deallocates memory
        ~character();

        //Deallocates texture
		void free();

        //Render the character to a specified renderer
		void render(SDL_Renderer* renderer);

        void loadAnim();

        bool changeState(characterState newState);

        floatPoint checkCollisionTerrain(terrainMap currMap);


		//Class variables

		//The sprite sheet for the character
		SDL_Texture* spriteSheet;

		//A tab of animation, one for every state
		animation stateAnim[COMBO_COUNT][STATE_COUNT];

        //The current state of the character
        characterState state;

        //The equip weapon combo of the character
        weaponCombo weapon;

        //A timer to calculate how long the character has been in a state
        Uint32 stateTimer;

		//Character position
		floatPoint position;

        floatPoint prevPosition;

        int zLayer;

        Uint32 lastUpdate;

        //Character orientation
        float angle;

        float speed;

        //Hack until I find a better way to do this.
        polygon collision;

    private:

};

class player: public character
{
    public:

        //Function

        player();

        void evaluateKeyInput(const Uint8* currentKeyStates);

        floatPoint checkCollisionTerrain(terrainMap currMap);

        void updatePos();

        bool justGated;

        intPoint curWorld;

        //Variables

    private:

};

//Global Variable

//Quit game
bool gQuit = false;

//Mouse coordinate
intPoint gMouse = {0,0};

//Prev mouse coordinate
intPoint gPrevMouse = {0,0};

timer gTimer;

//The player
player gPlayer;

//The information about the type of each type of terrain
terrain gTerrain[TERRAIN_COUNT];

terrainMap gWorld[WORLD_WIDTH][WORLD_HEIGHT];
intPoint gCurWorldCoord = {0,0};

//The texture that is going all the sprite of the terrains
SDL_Texture* gTerrainSprite = NULL;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFontDashley = NULL;


//Function declaration

bool init();

void close();

int pseudoRand(int seed);

//Different function to evaluate distance between different type of point.
float evalDistance(float x1, float y1, float x2, float y2);
float evalDistance(floatPoint point1, floatPoint point2);
float evalDistance(intPoint point1, intPoint point2);
float evalDistance(floatPoint point1, intPoint point2);
float evalDistance(intPoint point1, floatPoint point2);

bool pointOnVectorCheck(floatPoint point, floatPoint vecStart, floatPoint vecEnd);

int threePointOrientation(floatPoint point1, floatPoint point2, floatPoint point3);

float threePointAngle(floatPoint point1, floatPoint point2, floatPoint point3);

bool vectorCrossCheck(floatPoint vec1Start, floatPoint vec1End, floatPoint vec2Start, floatPoint vec2End);

floatPoint vectorCrossPoint(floatPoint vec1Start, floatPoint vec1End, floatPoint vec2Start, floatPoint vec2End);

floatPoint projectPointToVector(floatPoint point, floatPoint vec1Start, floatPoint vec1End);

floatPoint pointInsidePolygon(floatPoint point, polygon collider);

floatPoint pointInsideCircle(floatPoint point, circle collider);

SDL_Texture* loadTexture(std::string path);

bool loadTerrain();

bool loadWorld();

bool renderText(std::string text, TTF_Font* font, int posX, int posY, SDL_Color textColor, int pointSize = 125);

bool pause();

void menuDisplay();

void newGame();

//Class function definition

//Shape class
intPoint::intPoint()
{
    x = 0;
    y = 0;
}

intPoint::intPoint(int setX, int setY)
{
    x = setX;
    y = setY;
}

floatPoint::floatPoint()
{
    x = 0;
    y = 0;
}

floatPoint::floatPoint(float setX, float setY)
{
    x = setX;
    y = setY;
}

polygon::polygon()
{
    position = {0,0};
    rotation = 0;
}

polygon::polygon(floatPoint initPosition)
{
    position = initPosition;
    rotation = 0;
}

int polygon::getSideNumber()
{
    return vertex.size();
}

void polygon::cleanPolygon()
{
    for (int i = 0; i < vertex.size(); i++)
    {
        for (int j = 0; j < vertex.size(); j++)
        {
            if (j != i && vertex[i].x == vertex[j].x && vertex[i].y == vertex[j].y)
            {
                vertex.erase(vertex.begin() + j);
            }
        }
    }
    for (int i = 0; i < vertex.size(); i++)
    {
        if (threePointOrientation(vertex[i % vertex.size()], vertex[(i +1) % vertex.size()], vertex[(i +2) % vertex.size()]) == 0)
        {
            vertex.erase(vertex.begin() + (i +1) % vertex.size());
        }
    }
    for (int i = 0; i < vertex.size(); i++)
    {
        for (int j = 0; j < vertex.size(); j++)
        {
            if (pointOnVectorCheck(vertex[i % vertex.size()], vertex[(j) % vertex.size()], vertex[(j +1) % vertex.size()])
                && i != j && i != (j + 1) % vertex.size())
            {
                vertex.erase(vertex.begin() + i % vertex.size());
            }
        }
    }

    vertex.shrink_to_fit();
}

std::vector <polygon> polygon::convexDecomposition()
{
    std::vector <polygon> polygonAssembly;
    std::vector <int> concaveAngle;
    std::vector <int> closingPoint;
    std::vector <bool> visitedVertex;

    for (int i = 0; i < vertex.size(); i++)
    {
        if (threePointAngle(vertex[(i+1)%vertex.size()], vertex[i], vertex[(vertex.size()-1+i)%vertex.size()]) > 180)
        {
            polygonAssembly.push_back(position);
            concaveAngle.push_back(i);
        }

        visitedVertex.push_back(false);
    }

    if (polygonAssembly.size() > 0)
    {
        //std::cout << "A polygon needs " << polygonAssembly.size() << " splitting\n";
    }

    for (int i = 0; i < polygonAssembly.size(); i++)
    {
        int pointNext = ((concaveAngle[i]+1)%vertex.size());
        int pointCentral = concaveAngle[i];
        int pointPrev = (vertex.size()-1+concaveAngle[i])%vertex.size();

        //std::cout << "Concave angle: " << vertex[concaveAngle[i]].x << ", " << vertex[concaveAngle[i]].y << std::endl;

        for (int j = 0; j < vertex.size(); j++)
        {
            if (!visitedVertex[pointNext] &&
                threePointAngle(vertex[pointNext], vertex[pointCentral], vertex[pointPrev]) <= 180)
            {
                polygonAssembly[i].addVertex(vertex[pointCentral]);

                if (pointCentral == concaveAngle[i])
                {
                    closingPoint.push_back(pointNext);
                    //std::cout << "Closing point: " << vertex[closingPoint[closingPoint.size()-1]].x << ", " << vertex[closingPoint[closingPoint.size()-1]].y << std::endl;
                }
                else if (pointCentral != closingPoint[i])
                {
                    //std::cout << "Added point: " << vertex[pointCentral].x << ", " << vertex[pointCentral].y << std::endl;
                    visitedVertex[pointCentral] = true;
                }

                pointPrev = pointCentral;
                pointCentral = pointNext;
            }

            pointNext = (pointNext + 1) % vertex.size();
        }
    }

    polygonAssembly.push_back(position);

    for (int i = 0; i < vertex.size(); i++)
    {
        if ( !visitedVertex[i] )
        {
            visitedVertex[i] = true;
            polygonAssembly[polygonAssembly.size()-1].addVertex(vertex[i]);
        }
    }

    for (int i = 0; i < polygonAssembly.size(); i++)
    {
        polygonAssembly[i].cleanPolygon();
    }

    return polygonAssembly;
}

floatPoint polygon::satCollision(polygon collider)
{
    floatPoint mtv = {SCREEN_WIDTH, SCREEN_HEIGHT};

    //I first need to create a perpendicular axis for every side of the two polygons
    std::vector <floatPoint> axisStart;
    std::vector <floatPoint> axisEnd;
    std::vector <floatPoint> tempMTV;

    for (int i = 0; i < getSideNumber(); i++)
    {
        floatPoint vecStart = getVertexAbsPos(i);
        floatPoint vecEnd = getVertexAbsPos((i+1)%getSideNumber());
        floatPoint normalStart = {vecStart.x + (vecEnd.x - vecStart.x)/2, vecStart.y + (vecEnd.y - vecStart.y)/2};
        floatPoint normalEnd = {normalStart.x + (vecEnd.y - vecStart.y), normalStart.y + (vecStart.x - vecEnd.x)};

        if (normalStart.y == normalEnd.y)
        {
            normalStart = vectorCrossPoint(normalStart, normalEnd, floatPoint{0,0}, floatPoint{0, SCREEN_HEIGHT});
            normalEnd = vectorCrossPoint(normalStart, normalEnd, floatPoint{SCREEN_WIDTH,0}, floatPoint{SCREEN_WIDTH, SCREEN_HEIGHT});
        }
        else if (normalStart.x == normalEnd.x)
        {
            normalStart = vectorCrossPoint(normalStart, normalEnd, floatPoint{0,0}, floatPoint{SCREEN_WIDTH, 0});
            normalEnd = vectorCrossPoint(normalStart, normalEnd, floatPoint{SCREEN_HEIGHT,0}, floatPoint{SCREEN_WIDTH, SCREEN_HEIGHT});
        }
        else
        {
            normalStart = vectorCrossPoint(normalStart, normalEnd, floatPoint{0,0}, floatPoint{SCREEN_WIDTH, 0});
            normalEnd = vectorCrossPoint(normalStart, normalEnd, floatPoint{0,SCREEN_HEIGHT}, floatPoint{SCREEN_WIDTH, SCREEN_HEIGHT});
        }

        axisStart.push_back(normalStart);
        axisEnd.push_back(normalEnd);
    }
    for (int i = 0; i < collider.getSideNumber(); i++)
    {
        floatPoint vecStart = collider.getVertexAbsPos(i);
        floatPoint vecEnd = collider.getVertexAbsPos((i+1)%collider.getSideNumber());
        floatPoint normalStart = {vecStart.x + (vecEnd.x - vecStart.x)/2, vecStart.y + (vecEnd.y - vecStart.y)/2};
        floatPoint normalEnd = {normalStart.x + (vecEnd.y - vecStart.y), normalStart.y + (vecStart.x - vecEnd.x)};

        if (normalStart.y == normalEnd.y)
        {
            normalStart = vectorCrossPoint(normalStart, normalEnd, floatPoint{0,0}, floatPoint{0, SCREEN_HEIGHT});
            normalEnd = vectorCrossPoint(normalStart, normalEnd, floatPoint{SCREEN_WIDTH,0}, floatPoint{SCREEN_WIDTH, SCREEN_HEIGHT});
        }
        else if (normalStart.x == normalEnd.x)
        {
            normalStart = vectorCrossPoint(normalStart, normalEnd, floatPoint{0,0}, floatPoint{SCREEN_WIDTH, 0});
            normalEnd = vectorCrossPoint(normalStart, normalEnd, floatPoint{SCREEN_HEIGHT,0}, floatPoint{SCREEN_WIDTH, SCREEN_HEIGHT});
        }
        else
        {
            normalStart = vectorCrossPoint(normalStart, normalEnd, floatPoint{0,0}, floatPoint{SCREEN_WIDTH, 0});
            normalEnd = vectorCrossPoint(normalStart, normalEnd, floatPoint{0,SCREEN_HEIGHT}, floatPoint{SCREEN_WIDTH, SCREEN_HEIGHT});
        }

        axisStart.push_back(normalStart);
        axisEnd.push_back(normalEnd);
    }

    //Now I need to project each shape on every axis

    for (int i = 0; i < axisStart.size(); i++)
    {
        floatPoint shape1Point1 = {0,0};
        floatPoint shape1Point2 = {0,0};
        float shape1Dist = 0;

        floatPoint shape2Point1 = {0,0};
        floatPoint shape2Point2 = {0,0};
        float shape2Dist = 0;

        for (int j = 0; j < getSideNumber(); j++)
        {
            floatPoint tempPoint1 = projectPointToVector(getVertexAbsPos(j), axisStart[i], axisEnd[i]);

            for (int k = 0; k < getSideNumber(); k++)
            {
                if ( true /*j != k*/ )
                {
                    floatPoint tempPoint2 = projectPointToVector(getVertexAbsPos(k), axisStart[i], axisEnd[i]);

                    float tempDist = evalDistance(tempPoint1, tempPoint2);

                    if (tempDist > shape1Dist)
                    {
                        shape1Point1 = tempPoint1;
                        shape1Point2 = tempPoint2;
                        shape1Dist = tempDist;
                    }
                }
            }
        }

        for (int j = 0; j < collider.getSideNumber(); j++)
        {
            floatPoint tempPoint1 = projectPointToVector(collider.getVertexAbsPos(j), axisStart[i], axisEnd[i]);

            for (int k = 0; k < collider.getSideNumber(); k++)
            {
                if ( true /*j != k*/ )
                {
                    floatPoint tempPoint2 = projectPointToVector(collider.getVertexAbsPos(k), axisStart[i], axisEnd[i]);

                    float tempDist = evalDistance(tempPoint1, tempPoint2);

                    if (tempDist > shape2Dist)
                    {
                        shape2Point1 = tempPoint1;
                        shape2Point2 = tempPoint2;
                        shape2Dist = tempDist;
                    }
                }
            }
        }

        float distS1P1S2P1 = evalDistance(shape1Point1, shape2Point1);
        float distS1P1S2P2 = evalDistance(shape1Point1, shape2Point2);
        float distS1P2S2P1 = evalDistance(shape1Point2, shape2Point1);
        float distS1P2S2P2 = evalDistance(shape1Point2, shape2Point2);

        float maxDistance = fmax(fmax(distS1P1S2P1, distS1P1S2P2),fmax(distS1P2S2P1, distS1P2S2P2));

        if (shape1Dist + shape2Dist > maxDistance)
        {
            if (distS1P1S2P1 <= distS1P1S2P2 && distS1P1S2P1 <= distS1P2S2P1 && distS1P1S2P1 <= distS1P2S2P2)
            {
                tempMTV.push_back((floatPoint){shape2Point1.x - shape1Point1.x, shape2Point1.y - shape1Point1.y});
            }
            else if (distS1P1S2P2 <= distS1P1S2P1 && distS1P1S2P2 <= distS1P2S2P1 && distS1P1S2P2 <= distS1P2S2P2)
            {
                tempMTV.push_back((floatPoint){shape2Point2.x - shape1Point1.x, shape2Point2.y - shape1Point1.y});
            }
            else if (distS1P2S2P1 <= distS1P1S2P1 && distS1P2S2P1 <= distS1P1S2P2 && distS1P2S2P1 <= distS1P2S2P2)
            {
                tempMTV.push_back((floatPoint){shape2Point1.x - shape1Point2.x, shape2Point1.y - shape1Point2.y});
            }
            else if (distS1P2S2P2 <= distS1P1S2P1 && distS1P2S2P2 <= distS1P1S2P2 && distS1P2S2P2 <= distS1P2S2P1)
            {
                tempMTV.push_back((floatPoint){shape2Point2.x - shape1Point2.x, shape2Point2.y - shape1Point2.y});
            }
        }
        else
        {
            return mtv = {0,0};
        }
    }

    for (int i = 0; i < tempMTV.size(); i++)
    {
        if (evalDistance(tempMTV[i], (floatPoint){0,0}) < evalDistance(mtv, (floatPoint){0,0}))
        {
            mtv = tempMTV[i];
        }
    }

    return mtv;

}

void polygon::addVertex(floatPoint newVertexRelPosisition)
{
    vertex.push_back(newVertexRelPosisition);
}

bool polygon::delVertex(int vertexNumber)
{
    if(vertexNumber <= vertex.size() && vertexNumber >= 0)
    {
        vertex.erase(vertex.begin()+vertexNumber);
        return true;
    }
    else
    {
        return false;
    }
}

bool polygon::setVertexRelPos(int vertexNumber, floatPoint vertexNewRelativePosition)
{
    if(vertexNumber < vertex.size() && vertexNumber >= 0)
    {
        vertex[vertexNumber] = vertexNewRelativePosition;
        return true;
    }
    else
    {
        return false;
    }
}

floatPoint polygon::getVertexRelPos(int vertexNumber)
{
    if(vertexNumber <= vertex.size() && vertexNumber >= 0)
    {
        return vertex[vertexNumber];
    }
    else
    {
        return (floatPoint){0,0};
    }
}

floatPoint polygon::getVertexAbsPos(int vertexNumber)
{
    floatPoint absolutePosition = position;

    if(vertexNumber <= vertex.size() && vertexNumber >= 0)
    {
        float originalAngle = atan2(vertex[vertexNumber].y,vertex[vertexNumber].x)*180/PI;
        float hypothenuse = evalDistance((floatPoint){0,0}, vertex[vertexNumber]);

        absolutePosition.x = position.x + cos((originalAngle+rotation)*PI/180) *  hypothenuse;
        absolutePosition.y = position.y + sin((originalAngle+rotation)*PI/180) *  hypothenuse;

        return absolutePosition;
    }
    else
    {
        return position;
    }
}

void polygon::draw(SDL_Renderer* renderer)
{
    for (int i = 0; i < getSideNumber(); i++)
    {
        SDL_RenderDrawLine( renderer, getVertexAbsPos(i).x,getVertexAbsPos(i).y, getVertexAbsPos((i+1)%getSideNumber()).x, getVertexAbsPos((i+1)%getSideNumber()).y);
    }
}

/*rect::rect()
{
    position = {0,0};
    rotation = 0;
    sideNumber = 4;

    dimension = {1,1};
}

rect::rect(floatPoint initPosition, floatPoint initDimension)
{
    position = initPosition;
    rotation = 0;
    sideNumber = 4;

    dimension = initDimension;
}*/

circle::circle()
{
    position = {0,0};
    radius = 1;
}

circle::circle(floatPoint setPosition, float setRadius)
{
    position = setPosition;
    radius = setRadius;
}

floatPoint circle::nearestPoint(floatPoint point)
{
    floatPoint result = position;

    float angle = (atan2(position.x - point.x, position.y - point.y)*-180/PI);

    result.x = position.x + sin((angle)*PI/180)*radius;
    result.y = position.y + cos((angle+180)*PI/180)*radius;

    return result;
}

void circle::draw(SDL_Renderer* renderer)
{
    int circumference = abs(radius * PI * 2);

    float degPerPoint = 360.0 /(float)circumference;

    for (int i = 0; i < circumference; i++)
    {
        floatPoint result = position;
        result.x = round(position.x + sin((i*degPerPoint)*PI/180)*radius);
        result.y = round(position.y + cos((i*degPerPoint+180)*PI/180)*radius);
        SDL_RenderDrawPoint(renderer, result.x, result.y);
    }
}

timer::timer()
{
    startTime = 0;
    pauseTime = 0;
    pausedTime = 0;

    isPaused = true;
}

void timer::start()
{
    startTime = SDL_GetTicks();
    pausedTime = 0;

    isPaused = false;
}

void timer::pause()
{
    if(!isPaused)
    {
        isPaused = true;
        pauseTime = SDL_GetTicks();
    }
}

void timer::resume()
{
    if(isPaused)
    {
        isPaused = false;
        pausedTime = pausedTime + (SDL_GetTicks() - startTime)-pauseTime;
    }
}

Uint32 timer::getTime()
{
    return (SDL_GetTicks() -startTime -pausedTime);
}

//Animation constructor
animation::animation()
{
    frameCount = 0;
    frameW = 0;
    frameH = 0;
    frameCenterW = 0;
    frameCenterH = 0;
    framePosX = 0;
    framePosY = 0;
    animDuration = 0;
}

SDL_Rect animation::renderQuad(int posX, int posY)
{
    SDL_Rect quad = {(posX - frameCenterW), (posY - frameCenterH), frameW, frameH};
    return quad;
}

SDL_Rect animation::renderSprite(Uint32 stateTimer)
{
    //std::cout << "Globale timer - stateTime: "<< (gTimer.getTime()- stateTimer) << " AnimDuration: " << animDuration << " FrameCount: " << frameCount << "\n";
    int currentFrame = 0;
    if (animDuration != 0 && frameCount != 0)
    {
    currentFrame = ((gTimer.getTime()- stateTimer) % animDuration)/(animDuration/frameCount);
    }
    //std::cout << "Rendering frame: " << currentFrame << "\n";
    SDL_Rect quad = {(framePosX+(currentFrame*frameW)), framePosY, frameW, frameH};
    return quad;
}

//Player constructor
character::character()
{
    //Initialize the position and orientation
    free();
    loadAnim();
    SDL_DestroyTexture(spriteSheet);

    //this is a temporary hack
    weapon = SHIELD_SPEAR;
}

//Player destructor
character::~character()
{
    free();
}

void character::free()
{
    position.x = SCREEN_WIDTH/2;
    position.y = SCREEN_HEIGHT/2;
    zLayer = 5;
    angle = 0;
    speed = 0.1;
    lastUpdate = gTimer.getTime();

    SDL_DestroyTexture(spriteSheet);
    spriteSheet = NULL;

    //Collision hack
    collision.position = position;
    collision.rotation = angle;
    collision.addVertex((floatPoint){4-24, 25-43});
    collision.addVertex((floatPoint){40-24, 0-43});
    collision.addVertex((floatPoint){46-24, 53-43});
    collision.addVertex((floatPoint){4-24, 53-43});
    collision.cleanPolygon();
}

void character::loadAnim()
{
    std::string path = CHARACTER_SPRITESHEET_PATH + ".ani";

    //Open file for reading
    std::ifstream animFile(path.c_str());

	if (animFile.is_open())
	{
        while (!animFile.eof())
        {
            std::string weaponName;
            std::string animName;

            animFile >> weaponName;
            animFile >> animName;

            for (int i = 0; i < COMBO_COUNT; i++)
            {
                if (weaponName == weaponComboName[i])
                {
                    for (int j = 0; j< STATE_COUNT; j++)
                    {
                        if (animName == characterStateName[j])
                        {
                            animFile >> stateAnim[i][j].frameCount
                                >> stateAnim[i][j].frameW
                                >> stateAnim[i][j].frameH
                                >> stateAnim[i][j].frameCenterW
                                >> stateAnim[i][j].frameCenterH
                                >> stateAnim[i][j].framePosX
                                >> stateAnim[i][j].framePosY
                                >> stateAnim[i][j].animDuration;
                        }
                    }
                }
            }
        }
    }

    //Closing the file
    animFile.close();
}

void character::render(SDL_Renderer* renderer)
{
    SDL_Point center {(stateAnim[weapon][state].frameCenterW), (stateAnim[weapon][state].frameCenterH)};
    SDL_Rect spriteQuad = (stateAnim[weapon][state].renderSprite(stateTimer));
    SDL_Rect renderQuad = (stateAnim[weapon][state].renderQuad((int)round(position.x), (int)round(position.y)));

    SDL_RenderCopyEx( renderer, spriteSheet, &spriteQuad, &renderQuad,  angle, &center, SDL_FLIP_NONE );
}

bool character::changeState(characterState newState)
{
    bool stateChanged = false;
    if (state != newState)
    {
        state = newState;
        stateTimer = gTimer.getTime();
        stateChanged = true;

        //std::cout << "Player state changed to: " << characterStateName[state] << std::endl;

        //std::cout << "Distance from character to mouse: " << evalDistance(position, gMouse) << " Angle: " << angle << std::endl;
    }
    return stateChanged;
}

floatPoint player::checkCollisionTerrain(terrainMap currMap)
{
    bool collideWithGate = false;

    floatPoint mtv = {0,0};

    //Here I'm defining some arbitrary collision for the character.  I'll need to change that on something like collision per weapon or per animation.

    float charRadius = fmin(MAP_TILE_HEIGHT, MAP_TILE_WIDTH)/2 - 2; //I added an arbitrary -2 just to give it some tolerance.

    for (int i = 0; i < currMap.vGate.size(); i++)
    {
        floatPoint gatePos = {(currMap.vGate[i].position.x * MAP_TILE_WIDTH + MAP_TILE_WIDTH/2), (currMap.vGate[i].position.y * MAP_TILE_HEIGHT + MAP_TILE_HEIGHT/2)};
        if (evalDistance(position, gatePos) <= charRadius)
        {
            //std::cout << "Player touching a gate." << std::endl;
            collideWithGate = true;
            if (!justGated)
            {
                gCurWorldCoord = currMap.vGate[i].targetWorldCoord;
                position.x = currMap.vGate[i].targetPosCoord.x * MAP_TILE_WIDTH + MAP_TILE_WIDTH/2;
                position.y = currMap.vGate[i].targetPosCoord.y * MAP_TILE_HEIGHT + MAP_TILE_HEIGHT/2;
                justGated = true;
            }
        }
    }
    if (collideWithGate == false)
    {
        justGated = false;
    }

    std::vector <floatPoint> tempMTV;

    for (int i = 0; i < currMap.vCollisionRadial.size(); i++)
    {
        floatPoint posMTV = pointInsideCircle(position, currMap.vCollisionRadial[i]);

        if (posMTV.x != 0 || posMTV.y != 0)
        {
            tempMTV.push_back(posMTV);
        }
        for (int j = 0; j < collision.getSideNumber(); j++)
        {
            floatPoint loopMTV = pointInsideCircle(collision.getVertexAbsPos(j), currMap.vCollisionRadial[i]);
            if (loopMTV.x != 0 || loopMTV.y != 0)
            {
                tempMTV.push_back(loopMTV);
            }
            floatPoint projectedPoint = projectPointToVector(currMap.vCollisionRadial[i].position, collision.getVertexAbsPos(j), collision.getVertexAbsPos((j+1)%collision.getSideNumber()));
            floatPoint nearestPoint = currMap.vCollisionRadial[i].nearestPoint(projectedPoint);

            loopMTV = pointInsidePolygon(nearestPoint, collision);
            loopMTV.x = loopMTV.x * -1;
            loopMTV.y = loopMTV.y * -1;
            if (loopMTV.x != 0 || loopMTV.y != 0)
            {
                tempMTV.push_back(loopMTV);
            }
        }
    }

    for (int i = 0; i < currMap.vCollisionPolygon.size(); i++)
    {
        floatPoint posMTV = pointInsidePolygon(position, currMap.vCollisionPolygon[i]);

        if (posMTV.x != 0 || posMTV.y != 0)
        {
            tempMTV.push_back(posMTV);
        }

        for (int j = 0; j < collision.getSideNumber(); j++)
        {
            floatPoint loopMTV = pointInsidePolygon(collision.getVertexAbsPos(j), currMap.vCollisionPolygon[i]);
            if (loopMTV.x != 0 || loopMTV.y != 0)
            {
                tempMTV.push_back(loopMTV);
            }
        }
        for (int j = 0; j < currMap.vCollisionPolygon[i].getSideNumber(); j++)
        {
            floatPoint loopMTV = pointInsidePolygon(currMap.vCollisionPolygon[i].getVertexAbsPos(j), collision);
            loopMTV.x = loopMTV.x * -1;
            loopMTV.y = loopMTV.y * -1;
            if (loopMTV.x != 0 || loopMTV.y != 0)
            {
                tempMTV.push_back(loopMTV);
            }
        }
    }

    /*for (int i = 0; i < currMap.vCollisionPolygon.size(); i++)
    {
        floatPoint loopMTV = collision.satCollision(currMap.vCollisionPolygon[i]);

        if (loopMTV.x != 0 || loopMTV.y != 0)
        {
            tempMTV.push_back(loopMTV);
        }

        loopMTV = currMap.vCollisionPolygon[i].satCollision(collision);

        loopMTV.x = loopMTV.x * -1;
        loopMTV.y = loopMTV.y * -1;
        if (loopMTV.x != 0 || loopMTV.y != 0)
        {
            tempMTV.push_back(loopMTV);
        }
    }*/

    for (int i = 0; i < tempMTV.size(); i++)
    {
        if(evalDistance((floatPoint){0,0}, tempMTV[i]) > evalDistance((floatPoint){0,0}, mtv))
        {
            mtv = tempMTV[i];
        }
    }

    return mtv;
}

player::player()
{
    justGated = false;
}

void player::evaluateKeyInput(const Uint8* currentKeyStates)
{
    if( currentKeyStates[ SDL_SCANCODE_ESCAPE ] )
    {
        pause();
    }
    else if( currentKeyStates[ SDL_SCANCODE_W ]
        && !currentKeyStates[ SDL_SCANCODE_S ]
        && !currentKeyStates[ SDL_SCANCODE_A ]
        && !currentKeyStates[ SDL_SCANCODE_D ]
        && evalDistance(gMouse, position) > stateAnim[weapon][state].frameCenterH/2)
    {
        changeState(FORWARD);
    }
    else if ( !currentKeyStates[ SDL_SCANCODE_W ]
        && currentKeyStates[ SDL_SCANCODE_S ]
        && !currentKeyStates[ SDL_SCANCODE_A ]
        && !currentKeyStates[ SDL_SCANCODE_D ])
    {
        changeState(BACKWARD);
    }
    else if ( !currentKeyStates[ SDL_SCANCODE_W ]
        && !currentKeyStates[ SDL_SCANCODE_S ]
        && currentKeyStates[ SDL_SCANCODE_A ]
        && !currentKeyStates[ SDL_SCANCODE_D ])
    {
        changeState(LEFT);
    }
    else if ( !currentKeyStates[ SDL_SCANCODE_W ]
        && !currentKeyStates[ SDL_SCANCODE_S ]
        && !currentKeyStates[ SDL_SCANCODE_A ]
        && currentKeyStates[ SDL_SCANCODE_D ])
    {
        changeState(RIGHT);
    }
    else if ( currentKeyStates[ SDL_SCANCODE_W ]
        && !currentKeyStates[ SDL_SCANCODE_S ]
        && currentKeyStates[ SDL_SCANCODE_A ]
        && !currentKeyStates[ SDL_SCANCODE_D ]
        && evalDistance(gMouse, position) > stateAnim[weapon][state].frameCenterH/2)
    {
        changeState(FORWARD_LEFT); //Need to be change for a diagonal
    }
    else if ( currentKeyStates[ SDL_SCANCODE_W ]
        && !currentKeyStates[ SDL_SCANCODE_S ]
        && !currentKeyStates[ SDL_SCANCODE_A ]
        && currentKeyStates[ SDL_SCANCODE_D ]
        && evalDistance(gMouse, position) > stateAnim[weapon][state].frameCenterH/2)
    {
        changeState(FORWARD_RIGHT); //Need to be change for a diagonal
    }
    else if ( !currentKeyStates[ SDL_SCANCODE_W ]
        && currentKeyStates[ SDL_SCANCODE_S ]
        && currentKeyStates[ SDL_SCANCODE_A ]
        && !currentKeyStates[ SDL_SCANCODE_D ])
    {
        changeState(BACKWARD_LEFT); //Need to be change for a diagonal
    }
    else if ( !currentKeyStates[ SDL_SCANCODE_W ]
        && currentKeyStates[ SDL_SCANCODE_S ]
        && !currentKeyStates[ SDL_SCANCODE_A ]
        && currentKeyStates[ SDL_SCANCODE_D ])
    {
        changeState(BACKWARD_RIGHT); //Need to be change for a diagonal
    }
    else
    {
        changeState(IDLE);
    }

    if (currentKeyStates[ SDL_SCANCODE_LSHIFT] )
    {
        gPlayer.speed = 0.4;
    }
    else
    {
        gPlayer.speed = 0.1;
    }
}

void player::updatePos()
{
    angle = (atan2(position.x - gMouse.x, position.y - gMouse.y)*-180/PI);

    prevPosition.x = position.x;
    prevPosition.y = position.y;

    Uint32 timeMulti = gTimer.getTime() - lastUpdate;

    /*if (evalDistance(position.x, position.y, gMouse.x, gMouse.y) > SCREEN_HEIGHT/4) //A test where a link the speed of the character to the distance the mouse is.
    {
        speed = 0.2;
    }
    else
    {
        speed = 0.1;
    }*/

    switch(state)
    {
        case FORWARD:
            position.x = position.x + sin((angle)*PI/180)*timeMulti*speed;
            position.y = position.y + cos((angle+180)*PI/180)*timeMulti*speed;

            lastUpdate = gTimer.getTime();
            break;

        case BACKWARD:
            position.x = position.x + sin((angle+180)*PI/180)*timeMulti*speed;
            position.y = position.y + cos((angle+180+180)*PI/180)*timeMulti*speed;

            lastUpdate = gTimer.getTime();
            break;

        case LEFT:
            position.x = position.x + sin((angle-90)*PI/180)*timeMulti*speed;
            position.y = position.y + cos((angle+180-90)*PI/180)*timeMulti*speed;

            lastUpdate = gTimer.getTime();
            break;

        case RIGHT:
            position.x = position.x + sin((angle+90)*PI/180)*timeMulti*speed;
            position.y = position.y + cos((angle+180+90)*PI/180)*timeMulti*speed;

            lastUpdate = gTimer.getTime();
            break;

        case FORWARD_LEFT:
            position.x = position.x + sin((angle-45)*PI/180)*timeMulti*speed;
            position.y = position.y + cos((angle+180-45)*PI/180)*timeMulti*speed;

            lastUpdate = gTimer.getTime();
            break;

        case FORWARD_RIGHT:
            position.x = position.x + sin((angle+45)*PI/180)*timeMulti*speed;
            position.y = position.y + cos((angle+180+45)*PI/180)*timeMulti*speed;

            lastUpdate = gTimer.getTime();
            break;

         case BACKWARD_LEFT:
            position.x = position.x + sin((angle+180-45)*PI/180)*timeMulti*speed;
            position.y = position.y + cos((angle+180+180-45)*PI/180)*timeMulti*speed;

            lastUpdate = gTimer.getTime();
            break;

        case BACKWARD_RIGHT:
            position.x = position.x + sin((angle+180+45)*PI/180)*timeMulti*speed;
            position.y = position.y + cos((angle+180+180+45)*PI/180)*timeMulti*speed;

            lastUpdate = gTimer.getTime();
            break;

        default:
            lastUpdate = gTimer.getTime();
            break;
    }

    if (position.x < 0 || position.x > SCREEN_WIDTH)
    {
        position.x = prevPosition.x;

    }
    if (position.y  < 0 || position.y > SCREEN_HEIGHT)
    {
        position.y = prevPosition.y;
    }

    floatPoint mtv = checkCollisionTerrain(gWorld[gCurWorldCoord.x][gCurWorldCoord.y]);

    std::vector <floatPoint> tempMTV;

    position = (floatPoint){position.x + mtv.x, position.y + mtv.y};
    //Collision hack
    collision.position = position;
    collision.rotation = angle;
}

mapGate::mapGate()
{
    position = {0,0};

    targetWorldCoord = {0,0};

    targetPosCoord = {0,0};
}

mapGate::mapGate( intPoint initPosition)
{
    position = initPosition;
}

terrain::terrain()
{
    terrainName = BLANK;
    spriteW = 58;
    spriteH = 58;
    spriteCenterW = 29;
    spriteCenterH = 29;
    spritePosX = 0;
    spritePosY = 0;
    spriteVariation = 1;
    collide = NONE;
    health = -1;
    resPiercing = -1;
    resCuting = -1;
    resSmashing = -1;
    decayTo = BLANK;
    zLayer = 0;
    collision.x = MAP_TILE_WIDTH;
    collision.y = MAP_TILE_HEIGHT;
}

terrain::~terrain()
{
    terrainName = BLANK;
    spriteW = 58;
    spriteH = 58;
    spriteCenterW = 29;
    spriteCenterH = 29;
    spritePosX = 0;
    spritePosY = 0;
    spriteVariation = 1;
    collide = NONE;
    health = -1;
    resPiercing = -1;
    resCuting = -1;
    resSmashing = -1;
    decayTo = BLANK;
    zLayer = 0;
    collision.x = MAP_TILE_WIDTH;
    collision.y = MAP_TILE_HEIGHT;
}

void terrain::render(SDL_Renderer* renderer, int posX, int posY, int variation)
{
    SDL_Point center {spriteCenterW, spriteCenterH};
    SDL_Rect spriteQuad = {spritePosX + variation * spriteW, spritePosY, spriteW, spriteH};
    SDL_Rect renderQuad = {(posX - spriteCenterW), (posY - spriteCenterH), spriteW, spriteH};

    SDL_RenderCopyEx( renderer, gTerrainSprite, &spriteQuad, &renderQuad, 0, &center, SDL_FLIP_NONE );
}

terrainMap::terrainMap()
{
    for (int h = 0;  h < Z_PLANE_COUNT; h++)
    {
        for (int i = 0; i < TILE_NUMBER_HEIGHT; i++)
        {
            for (int j = 0; j < TILE_NUMBER_WIDTH; j++)
            {
                tileMap[j][i][h] = gTerrain[BLANK];
            }
        }
    }
}

bool terrainMap::loadMap(int mapCoordX, int mapCoordY)
{
    bool success = false;

    std::string path = "ressource/map/" + std::to_string(mapCoordX) + "-" + std::to_string(mapCoordY) + ".map";

    std::ifstream mapFile(path.c_str());

    if  (mapFile.is_open())
    {
        std::string tempRead;

        int gateNumber = 0;

        collisionType collisionMap[TILE_NUMBER_WIDTH][TILE_NUMBER_HEIGHT][Z_PLANE_COUNT];

        mapFile >> mapSize.x >> mapSize.y;

        if  (mapSize.x > TILE_NUMBER_WIDTH)
        {
            mapSize.x = TILE_NUMBER_WIDTH;
        }

        if  (mapSize.y > TILE_NUMBER_HEIGHT)
        {
            mapSize.y = TILE_NUMBER_HEIGHT;
        }

        for (int h = 0; h < Z_PLANE_COUNT; h++)
        {
            mapFile >> tempRead;

            if (tempRead == zPlaneName[h])
            {
                for (int i = 0; i < mapSize.y; i++)
                {
                    for (int j = 0; j  < mapSize.x; j++)
                    {
                        if (!mapFile.eof())
                        {
                            mapFile >> tempRead;
                            for (int k = 0; k < TERRAIN_COUNT; k++)
                            {
                                if (tempRead == terrainTypeName[gTerrain[k].terrainName])
                                {
                                    tileMap[j][i][h] = gTerrain[k];
                                    if (gTerrain[k].collide != NONE)
                                    {
                                        vTerrainCollide.push_back((intPoint){j,i});
                                        if (gTerrain[k].collide == BOX)
                                        {
                                            collisionMap[j][i][h] = BOX;
                                            //std::cout << "Adding a BOX on map " << mapCoordX << ", " << mapCoordY << " at pos: " << j << ", " << i << ", " << h << std::endl;
                                        }
                                        else if (gTerrain[k].collide == RADIAL)
                                        {
                                            circle tempCircle((floatPoint){j*MAP_TILE_WIDTH + MAP_TILE_WIDTH/2, i*MAP_TILE_HEIGHT + MAP_TILE_HEIGHT/2}, gTerrain[k].collision.x);
                                            vCollisionRadial.push_back(tempCircle);
                                        }
                                        else if (gTerrain[k].collide == GATE)
                                        {
                                            gateNumber++;
                                            vGate.push_back((mapGate)((intPoint){j,i}));
                                        }
                                    }
                                    else
                                    {
                                        collisionMap[j][i][h] = NONE;
                                    }
                                }
                            }
                        }
                        else
                        {
                            return success;
                        }
                    }
                }
            }
        }

        mapFile >> tempRead;

        if ( tempRead == "GATE")
        {
            //std::cout << "Loading up the gate from the map File" << std::endl;
            //std::cout << "Expected number of gate : " << gateNumber << std::endl;
            for (int i = 0; i < gateNumber; i++)
            {
                //std::cout << "Reading the gate " << i << std::endl;
                mapFile >> vGate[i].targetWorldCoord.x >> vGate[i].targetWorldCoord.y >> vGate[i].targetPosCoord.x >> vGate[i].targetPosCoord.y;
            }
            //std::cout << "Found number of gate : " << vGate.size() << std::endl;
        }

        success = true;
        mapFile.close();

        //Creating a vector of polygon of collision

        std::vector <polygon> fileCollisionPolygon;

        for (int h = 0; h < Z_PLANE_COUNT; h++)
        {
            for (int i = 0; i < mapSize.y; i++)
            {
                for (int j = 0; j < mapSize.x; j++)
                {
                    if (collisionMap[j][i][h] == BOX)
                    {

                        intPoint startTile = {j,i};
                        intPoint currTile = startTile;
                        int prevDirection = 3; //0 = LEFT, 1 = UP, 2 = RIGHT, 3 = BOTTOM

                        int doubleTakeVerifivation = 0;

                        std::vector <intPoint> visitedTile;
                        visitedTile.push_back(currTile);

                        fileCollisionPolygon.push_back((floatPoint){0,0});

                        floatPoint tileCollisionPointOffset = {round((float)(MAP_TILE_WIDTH/2)-(float)(tileMap[currTile.x][currTile.y][h].collision.x)*(float)(tileMap[currTile.x][currTile.y][h].spriteCenterW)/(float)(tileMap[currTile.x][currTile.y][h].spriteW)),
                                                                round((float)(MAP_TILE_HEIGHT/2)-(float)(tileMap[currTile.x][currTile.y][h].collision.y)*(float)(tileMap[currTile.x][currTile.y][h].spriteCenterH)/(float)(tileMap[currTile.x][currTile.y][h].spriteH))};
                        //tileCollisionPointOffset = {0,0};

                        //Adding the first vertex on the top left corner.
                        fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x) * MAP_TILE_WIDTH + tileCollisionPointOffset.x, (currTile.y) * MAP_TILE_HEIGHT + tileCollisionPointOffset.y});

                        do
                        {
                            for (int k = 0; k < 4; k++)
                            {
                                if ((k + prevDirection)%4 == 0)
                                {
                                    tileCollisionPointOffset = {round((float)(MAP_TILE_WIDTH/2)-(float)(tileMap[currTile.x][currTile.y][h].collision.x)*(float)(tileMap[currTile.x][currTile.y][h].spriteCenterW)/(float)(tileMap[currTile.x][currTile.y][h].spriteW)),
                                                                round((float)(MAP_TILE_HEIGHT/2)-(float)(tileMap[currTile.x][currTile.y][h].collision.y)*(float)(tileMap[currTile.x][currTile.y][h].spriteCenterH)/(float)(tileMap[currTile.x][currTile.y][h].spriteH))};

                                    if (currTile.y -1 >= 0 && collisionMap[currTile.x][currTile.y - 1][h] == BOX) //Looking UP
                                    {
                                        if (prevDirection != 3) //It does not come from the bottom
                                        {
                                            /*tileCollisionPointOffset = {(float)(MAP_TILE_WIDTH/2)-(float)(tileMap[j][i][h].collision.x)*(float)(tileMap[j][i][h].spriteCenterW)/(float)(tileMap[j][i][h].spriteW),
                                                                    (float)(MAP_TILE_HEIGHT/2)-(float)(tileMap[j][i][h].collision.y)*(float)(tileMap[j][i][h].spriteCenterH)/(float)(tileMap[j][i][h].spriteH)};*/

                                            if (prevDirection == 0) //Coming form the left
                                            {
                                                //Adding a vertex on the top left corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x) * MAP_TILE_WIDTH + tileCollisionPointOffset.x, (currTile.y) * MAP_TILE_HEIGHT + tileCollisionPointOffset.y});
                                            }
                                            else if (prevDirection == 1) //Coming for the top, backtracking
                                            {
                                                //Adding a vertex on the bottom right corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x + 1) * MAP_TILE_WIDTH - tileCollisionPointOffset.x, (currTile.y + 1) * MAP_TILE_HEIGHT - tileCollisionPointOffset.y});
                                                //Adding a vertex on the bottom left corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x) * MAP_TILE_WIDTH + tileCollisionPointOffset.x, (currTile.y + 1) * MAP_TILE_HEIGHT - tileCollisionPointOffset.y});
                                            }
                                            else if (prevDirection == 2) //Coming from the right
                                            {
                                                //Adding a vertex on the bottom left corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x) * MAP_TILE_WIDTH + tileCollisionPointOffset.x, (currTile.y + 1) * MAP_TILE_HEIGHT - tileCollisionPointOffset.y});
                                            }
                                        }
                                        prevDirection = 3;
                                        visitedTile.push_back((intPoint){currTile.x, currTile.y - 1});
                                        currTile.y--;
                                        break;
                                    }
                                }
                                else if ((k + prevDirection)%4 == 1)
                                {
                                    if (currTile.x +1 < mapSize.x && collisionMap[currTile.x+1][currTile.y][h] == BOX) //Looking RIGHT
                                    {
                                        if (prevDirection != 0) //It does not come from the left
                                        {
                                            /*tileCollisionPointOffset = {(float)(MAP_TILE_WIDTH/2)-(float)(tileMap[j+1][i][h].collision.x)*(float)(tileMap[j+1][i][h].spriteCenterW)/(float)(tileMap[j+1][i][h].spriteW),
                                                                    (float)(MAP_TILE_HEIGHT/2)-(float)(tileMap[j+1][i][h].collision.y)*(float)(tileMap[j+1][i][h].spriteCenterH)/(float)(tileMap[j+1][i][h].spriteH)};*/
                                            if (prevDirection == 1) //Coming from the top
                                            {
                                                //Adding a vertex on the top right corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x + 1) * MAP_TILE_WIDTH - tileCollisionPointOffset.x, (currTile.y) * MAP_TILE_HEIGHT + tileCollisionPointOffset.y});
                                            }
                                            else if (prevDirection == 2) //Coming for the right, backtracking
                                            {
                                                //Adding a vertex on the bottom left corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x) * MAP_TILE_WIDTH + tileCollisionPointOffset.x, (currTile.y + 1) * MAP_TILE_HEIGHT - tileCollisionPointOffset.y});
                                                //Adding a vertex on the top left corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x) * MAP_TILE_WIDTH + tileCollisionPointOffset.x, (currTile.y) * MAP_TILE_HEIGHT + tileCollisionPointOffset.y});
                                            }
                                            else if (prevDirection == 3) //Coming from the bottom
                                            {
                                                //Adding a vertex on the top left corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x) * MAP_TILE_WIDTH + tileCollisionPointOffset.x, (currTile.y) * MAP_TILE_HEIGHT + tileCollisionPointOffset.y});
                                            }
                                        }
                                        prevDirection = 0;
                                        visitedTile.push_back((intPoint){currTile.x + 1, currTile.y});
                                        currTile.x++;
                                        break;
                                    }
                                }
                                else if ((k + prevDirection)%4 == 2)
                                {
                                    if (currTile.y +1 < mapSize.y && collisionMap[currTile.x][currTile.y+1][h] == BOX) //Looking DOWN
                                    {
                                        if (prevDirection != 1) //It does not come from the top
                                        {
                                            /*tileCollisionPointOffset = {(float)(MAP_TILE_WIDTH/2)-(float)(tileMap[j][i+1][h].collision.x)*(float)(tileMap[j][i+1][h].spriteCenterW)/(float)(tileMap[j][i+1][h].spriteW),
                                                                    (float)(MAP_TILE_HEIGHT/2)-(float)(tileMap[j][i+1][h].collision.y)*(float)(tileMap[j][i+1][h].spriteCenterH)/(float)(tileMap[j][i+1][h].spriteH)};*/

                                            if (prevDirection == 2) //Coming from the right
                                            {
                                                //Adding a vertex on the bottom right corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x + 1) * MAP_TILE_WIDTH - tileCollisionPointOffset.x, (currTile.y + 1) * MAP_TILE_HEIGHT - tileCollisionPointOffset.y});
                                            }
                                            else if (prevDirection == 3) // Coming from the bottom, backtracking
                                            {
                                                //Adding a vertex on the top right corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x + 1) * MAP_TILE_WIDTH - tileCollisionPointOffset.x, (currTile.y) * MAP_TILE_HEIGHT + tileCollisionPointOffset.y});
                                                //Adding a vertex on the bottom right corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x + 1) * MAP_TILE_WIDTH - tileCollisionPointOffset.x, (currTile.y + 1) * MAP_TILE_HEIGHT - tileCollisionPointOffset.y});
                                            }
                                            else if (prevDirection == 0) //Coming from the left
                                            {
                                                //Adding a vertex on the top right corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x + 1) * MAP_TILE_WIDTH - tileCollisionPointOffset.x, (currTile.y) * MAP_TILE_HEIGHT + tileCollisionPointOffset.y});
                                            }
                                        }
                                        prevDirection = 1;
                                        visitedTile.push_back((intPoint){currTile.x, currTile.y + 1});
                                        currTile.y++;
                                        break;
                                    }
                                }
                                else if ((k + prevDirection)%4 == 3)
                                {
                                    if (currTile.x -1 >= 0 && collisionMap[currTile.x-1][currTile.y][h] == BOX) //Looking LEFT
                                    {
                                        if (prevDirection != 2) //It does not come from the right
                                        {
                                            /*tileCollisionPointOffset = {(float)(MAP_TILE_WIDTH/2)-(float)(tileMap[j-1][i][h].collision.x)*(float)(tileMap[j-1][i][h].spriteCenterW)/(float)(tileMap[j-1][i][h].spriteW),
                                                                    (float)(MAP_TILE_HEIGHT/2)-(float)(tileMap[j-1][i][h].collision.y)*(float)(tileMap[j-1][i][h].spriteCenterH)/(float)(tileMap[j-1][i][h].spriteH)};*/

                                            if (prevDirection == 3) //Coming from the bottom
                                            {
                                                //Adding a vertex on the bottom left corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x) * MAP_TILE_WIDTH + tileCollisionPointOffset.x, (currTile.y + 1) * MAP_TILE_HEIGHT - tileCollisionPointOffset.y});
                                            }
                                            else if (prevDirection == 0) //Coming from the left, backtracking
                                            {
                                                //Adding a vertex on the top right corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x + 1) * MAP_TILE_WIDTH - tileCollisionPointOffset.x, (currTile.y) * MAP_TILE_HEIGHT + tileCollisionPointOffset.y});
                                                //Adding a vertex on the bottom right corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x + 1) * MAP_TILE_WIDTH - tileCollisionPointOffset.x, (currTile.y + 1) * MAP_TILE_HEIGHT - tileCollisionPointOffset.y});
                                            }
                                            else if (prevDirection == 1) //Coming from the top
                                            {
                                                //Adding a vertex on the bottom right corner
                                                fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x + 1) * MAP_TILE_WIDTH - tileCollisionPointOffset.x, (currTile.y + 1) * MAP_TILE_HEIGHT - tileCollisionPointOffset.y});
                                            }
                                        }
                                        prevDirection = 2;
                                        visitedTile.push_back((intPoint){currTile.x - 1, currTile.y});
                                        currTile.x--;
                                        break;
                                    }
                                }
                                else
                                {
                                    std::cout << "Something is wrong" << std::endl;
                                }

                            }
                            if (currTile.x == startTile.x && currTile.y == startTile.y)
                            {
                                doubleTakeVerifivation++;
                            }

                        }
                        while (doubleTakeVerifivation < 2);

                        if (prevDirection == 2)
                        {
                            //Adding a vertex on the bottom left corner
                            fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x) * MAP_TILE_WIDTH + tileCollisionPointOffset.x, (currTile.y + 1) * MAP_TILE_HEIGHT - tileCollisionPointOffset.y});
                        }

                        if (visitedTile.size() == 1)
                        {
                            //The box is by itself

                            //The offset is gonna be cut in half so I can have the offset on each side
                            /*tileCollisionPointOffset.x = round(tileCollisionPointOffset.x - (float)MAP_TILE_WIDTH/4);
                            tileCollisionPointOffset.y = round(tileCollisionPointOffset.y - (float)MAP_TILE_HEIGHT/4);*/

                            //Moving the original point to the half the offset.  I don't know why I have to add a -1 at the end
                            //fileCollisionPolygon[fileCollisionPolygon.size()-1].setVertexRelPos(0, (floatPoint){(currTile.x) * MAP_TILE_WIDTH + tileCollisionPointOffset.x - 1, (currTile.y) * MAP_TILE_HEIGHT + tileCollisionPointOffset.y - 1});

                            //Adding a vertex on the top right corner
                            fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x + 1) * MAP_TILE_WIDTH - tileCollisionPointOffset.x, (currTile.y) * MAP_TILE_HEIGHT + tileCollisionPointOffset.y});
                            //Adding a vertex on the bottom right corner
                            fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x + 1) * MAP_TILE_WIDTH - tileCollisionPointOffset.x, (currTile.y + 1) * MAP_TILE_HEIGHT - tileCollisionPointOffset.y});
                            //Adding a vertex on the bottom left corner
                            fileCollisionPolygon[fileCollisionPolygon.size()-1].addVertex((floatPoint){(currTile.x) * MAP_TILE_WIDTH + tileCollisionPointOffset.x, (currTile.y + 1) * MAP_TILE_HEIGHT - tileCollisionPointOffset.y});
                        }

                        for (int k = 0; k < visitedTile.size(); k++)
                        {
                            collisionMap[visitedTile[k].x][visitedTile[k].y][h] = NONE;
                        }

                        fileCollisionPolygon[fileCollisionPolygon.size()-1].cleanPolygon();
                    }
                }
            }
        }

        for (int i = 0; i < fileCollisionPolygon.size(); i++)
        {
            std::vector <polygon> tempColisionPolygon = fileCollisionPolygon[i].convexDecomposition();

            for (int j = 0; j < tempColisionPolygon.size(); j++)
            {
                vCollisionPolygon.push_back(tempColisionPolygon[j]);
            }
        }

    }
    else
    {
        std::cout << "Could not find map file " << std::to_string(mapCoordX) << "-" << std::to_string(mapCoordY) << ".map" << std::endl;
    }

    return success;
}

void terrainMap::render(SDL_Renderer* renderer, zPlane plane, int zLayerBottom, int zLayerTop)
{
    int variation = pseudoRand(zLayerBottom+zLayerTop+gCurWorldCoord.x*(gCurWorldCoord.x+ gCurWorldCoord.y));

    for (int i = zLayerBottom; i <= zLayerTop; i++)
    {
        for (int j = 0; j < TILE_NUMBER_HEIGHT; j++)
        {
            for (int k = 0; k < TILE_NUMBER_WIDTH; k++)
            {
                if (tileMap[k][j][plane].zLayer == i)
                {
                    variation = pseudoRand(variation+i+j*k+gCurWorldCoord.x*(gCurWorldCoord.x+ gCurWorldCoord.y))%tileMap[k][j][plane].spriteVariation;
                    tileMap[k][j][plane].render(renderer, (k*MAP_TILE_WIDTH+MAP_TILE_WIDTH/2),(j*MAP_TILE_HEIGHT+MAP_TILE_HEIGHT/2), variation);
                }
            }
        }
    }
}

//Function definition

bool init()
{
	//Initialization flag
	bool success = false;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else
	{
        //Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( APPLICATION_NAME.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{

                //Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				else
				{
                     //Initialize SDL_ttf
                    if( TTF_Init() == -1 )
                    {
                        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                        success = false;
                    }
                    else
                    {
                        //Load the font
                        gFontDashley = TTF_OpenFont( DASHLEY_FONT_PATH.c_str(), DASHLEY_FONT_POINT );
                        if( gFontDashley == NULL )
                        {
                            printf( "Failed to load Dashley font! SDL_ttf Error: %s\n", TTF_GetError() );
                            success = false;
                        }
                        else
                        {
                            SDL_RenderClear( gRenderer );
                            //Update screen
                            SDL_RenderPresent( gRenderer );
                            success = true;
                        }
                    }
				}
			}
		}
	}

	return success;
}

void close()
{
    //Free loaded images

	//Deallocate surface
    SDL_DestroyTexture( gTerrainSprite);
    gTerrainSprite = NULL;

    //Free global font
    TTF_CloseFont( gFontDashley );
    gFontDashley = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Set blending function
    SDL_SetTextureBlendMode( newTexture, SDL_BLENDMODE_BLEND );

	return newTexture;
}

bool loadTerrain()
{
    bool success = false;

    gTerrainSprite = loadTexture((TERRAIN_SPRITESHEET_PATH + ".png").c_str());

    std::ifstream terrainFile((TERRAIN_SPRITESHEET_PATH + ".ter").c_str());

    if (terrainFile.is_open())
    {
        while (!terrainFile.eof())
        {
            std::string terrainName;

            terrainFile >> terrainName;

            for (int i = 0; i < TERRAIN_COUNT; i++)
            {
                if (terrainName == terrainTypeName[i])
                {

                    std::string tempcollide;
                    std::string tempDecayTo;

                    gTerrain[i].terrainName = (terrainType) i;

                    terrainFile >> gTerrain[i].spriteW
                        >> gTerrain[i].spriteH
                        >> gTerrain[i].spriteCenterW
                        >> gTerrain[i].spriteCenterH
                        >> gTerrain[i].spritePosX
                        >> gTerrain[i].spritePosY
                        >> gTerrain[i].spriteVariation
                        >> tempcollide
                        >> gTerrain[i].health
                        >> gTerrain[i].resPiercing
                        >> gTerrain[i].resCuting
                        >> gTerrain[i].resSmashing
                        >> tempDecayTo
                        >> gTerrain[i].zLayer;

                    for (int j = NONE; j < COLLISION_COUNT; j++)
                    {
                        if (tempcollide == collisionTypeName[j])
                        {
                            gTerrain[i].collide = (collisionType) j;
                        }
                    }

                    if (gTerrain[i].collide == BOX)
                    {
                        terrainFile >> gTerrain[i].collision.x >>  gTerrain[i].collision.y;
                    }
                    else if (gTerrain[i].collide == RADIAL)
                    {
                        terrainFile >> gTerrain[i].collision.x;
                    }

                    for (int j = 0; j < TERRAIN_COUNT; j++)
                    {
                        if (tempDecayTo == terrainTypeName[j])
                        {
                            gTerrain[i].decayTo = (terrainType) j;
                        }
                    }
                }
            }
        }
        success = true;
    }

    terrainFile.close();

    return success;
}

bool loadWorld()
{
    bool success = false;

    std::cout << "Loading the world" << std::endl;

    for (int i = 0; i < WORLD_HEIGHT; i++)
    {
        for (int j = 0; j < WORLD_WIDTH; j++)
        {
            if (!gWorld[j][i].loadMap(j, i))
            {
                std::cout << "Could not load the world map \n";
                return success;
            }
        }
    }

    success = true;

    return success;
}

int pseudoRand(int seed)
{
    int result = (seed * 16807) % 2147483647;

    return result;
}

float evalDistance(float x1, float y1, float x2, float y2)
{
    return (sqrt(pow(x1-x2, 2)+pow(y1-y2, 2)));
}

float evalDistance(floatPoint point1, floatPoint point2)
{
    return (sqrt(pow(point1.x-point2.x, 2)+pow(point1.y-point2.y, 2)));
}

float evalDistance(intPoint point1, intPoint point2)
{
    return (sqrt(pow(point1.x-point2.x, 2)+pow(point1.y-point2.y, 2)));
}

float evalDistance(floatPoint point1, intPoint point2)
{
    return (sqrt(pow(point1.x-point2.x, 2)+pow(point1.y-point2.y, 2)));
}

float evalDistance(intPoint point1, floatPoint point2)
{
    return (sqrt(pow(point1.x-point2.x, 2)+pow(point1.y-point2.y, 2)));
}

bool pointOnVectorCheck(floatPoint point, floatPoint vecStart, floatPoint vecEnd)
{
    bool pointIsOnVector = false;

    if (point.x <= fmax(vecStart.x, vecEnd.x) && point.x >= fmin(vecStart.x, vecEnd.x) &&
        point.y <= fmax(vecStart.y, vecEnd.y) && point.y >= fmin(vecStart.y, vecEnd.y) &&
        threePointOrientation(point, vecStart, vecEnd) == 0)
    {
        pointIsOnVector = true;
    }

    return pointIsOnVector;
}

int threePointOrientation(floatPoint point1, floatPoint point2, floatPoint point3)
{
    int orientation = 0;
    if ((point2.y - point1.y) * (point3.x - point2.x) - (point2.x - point1.x) * (point3.y - point2.y) > 0)
    {
        orientation = 1;
    }
    else if ((point2.y - point1.y) * (point3.x - point2.x) - (point2.x - point1.x) * (point3.y - point2.y) < 0)
    {
        orientation = -1;
    }

    return orientation;
}

float threePointAngle(floatPoint point1, floatPoint point2, floatPoint point3)
{
    floatPoint point1Rel = (floatPoint){point1.x - point2.x, point1.y - point2.y};
    floatPoint point3Rel = (floatPoint){point3.x - point2.x, point3.y - point2.y};

    float anglePoint1toWorld = atan2(point1Rel.y, point1Rel.x)*-180/PI;
    float anglePoint3toWorld = atan2(point3Rel.y, point3Rel.x)*-180/PI;

    float resultAngle = anglePoint1toWorld - anglePoint3toWorld;

    if (resultAngle < 0)
    {
        resultAngle = 360 + resultAngle;
    }

    return resultAngle;
}

bool vectorCrossCheck(floatPoint vec1Start, floatPoint vec1End, floatPoint vec2Start, floatPoint vec2End)
{
    bool crossing = false;

    int orientation1 = threePointOrientation(vec1Start, vec1End, vec2Start);
    int orientation2 = threePointOrientation(vec1Start, vec1End, vec2End);
    int orientation3 = threePointOrientation(vec2Start, vec2End, vec1Start);
    int orientation4 = threePointOrientation(vec2Start, vec2End, vec1End);

    if (orientation1 != orientation2 && orientation3 != orientation4)
    {
        crossing = true;
    }

    if (orientation1 == 0 && pointOnVectorCheck(vec2Start, vec1Start, vec1End))
    {
        crossing = true;
    }

    if (orientation2 == 0 && pointOnVectorCheck(vec2End, vec1Start, vec1End))
    {
        crossing = true;
    }

    if (orientation3 == 0 && pointOnVectorCheck(vec1Start, vec2Start, vec2End))
    {
        crossing = true;
    }

    if (orientation4 == 0 && pointOnVectorCheck(vec1End, vec2Start, vec2End))
    {
        crossing = true;
    }

    return crossing;
}

floatPoint vectorCrossPoint(floatPoint vec1Start, floatPoint vec1End, floatPoint vec2Start, floatPoint vec2End)
{
    floatPoint result = {-1, -1};

    if (true/*vectorCrossCheck(vec1Start, vec1End, vec2Start, vec2End)*/) //I took off the check to see is the point is in the limit of the vector.  Might cause problem down the line.
    {
        float tempA1 = vec1End.y - vec1Start.y;
        float tempB1 = vec1Start.x - vec1End.x;
        float tempC1 = tempA1*vec1Start.x+tempB1*vec1Start.y;

        float tempA2 = vec2End.y - vec2Start.y;
        float tempB2 = vec2Start.x - vec2End.x;
        float tempC2 = tempA2*vec2Start.x+tempB2*vec2Start.y;

        float det = tempA1*tempB2 - tempA2*tempB1;

        if (det != 0)
        {
            result.x = (tempB2*tempC1 - tempB1*tempC2)/det;
            result.y = (tempA1*tempC2 - tempA2*tempC1)/det;
        }
    }
    return result;
}

floatPoint projectPointToVector(floatPoint point, floatPoint vecStart, floatPoint vecEnd)
{
    floatPoint nearestPoint = {vecEnd.x - vecStart.x, vecEnd.y - vecStart.y};

    float angleLineToWorld = atan2(vecEnd.y - vecStart.y, vecEnd.x - vecStart.x)*-180/PI;

    float perpendicularAngle = (angleLineToWorld + 90);

    floatPoint perpendicularPoint = {point.x + cos((perpendicularAngle)*PI/180)*100, point.y + sin((perpendicularAngle)*PI/-180)*100};

    nearestPoint = vectorCrossPoint(vecStart, vecEnd, point, perpendicularPoint);

    return nearestPoint;
}

floatPoint pointInsidePolygon(floatPoint point, polygon collider)
{
    floatPoint mtv = {SCREEN_WIDTH,SCREEN_HEIGHT};

    if (collider.getSideNumber() < 3)
    {
        return mtv = {0,0};
    }

    floatPoint minDimension =  {SCREEN_WIDTH, SCREEN_HEIGHT};
    floatPoint maxDimension = {0,0};

    for (int i = 0; i < collider.getSideNumber();  i++)
    {
        if (collider.getVertexAbsPos(i).x < minDimension.x)
        {
            minDimension.x = collider.getVertexAbsPos(i).x;
        }
        if (collider.getVertexAbsPos(i).y < minDimension.y)
        {
            minDimension.y = collider.getVertexAbsPos(i).y;
        }
        if (collider.getVertexAbsPos(i).x > maxDimension.x)
        {
            maxDimension.x = collider.getVertexAbsPos(i).x;
        }
        if (collider.getVertexAbsPos(i).y > maxDimension.y)
        {
            maxDimension.y = collider.getVertexAbsPos(i).y;
        }
    }

    if (point.x > maxDimension.x || point.x < minDimension.x || point.y > maxDimension.y || point.y < minDimension.y)
    {
        return mtv = {0,0};
    }

    for (int i = 0; i < collider.getSideNumber(); i++)
    {
        for (int j = 0; j < collider.getSideNumber(); j++)
        {
            if ( i != j && i != (j + 1)%collider.getSideNumber() && vectorCrossCheck(point, collider.getVertexAbsPos(i), collider.getVertexAbsPos(j), collider.getVertexAbsPos((j + 1)%collider.getSideNumber())))
            {
                return mtv = {0,0};
            }
        }
    }

    std::vector <floatPoint> tempMTV;

    for (int i = 0; i < collider.getSideNumber(); i++)
    {
        floatPoint loopTempMTV = projectPointToVector(point, collider.getVertexAbsPos(i), collider.getVertexAbsPos((i+1)%collider.getSideNumber()));
        tempMTV.push_back((floatPoint){loopTempMTV.x - point.x, loopTempMTV.y - point.y});
    }

    for (int i = 0; i < tempMTV.size(); i++)
    {
        if (evalDistance((floatPoint){0,0}, tempMTV[i]) < evalDistance((floatPoint){0,0}, mtv))
        {
            mtv = tempMTV[i];
        }
    }

    return mtv;
}

floatPoint pointInsideCircle(floatPoint point, circle collider)
{
    floatPoint mtv = {SCREEN_WIDTH,SCREEN_HEIGHT};

    if (evalDistance(point, collider.position) > collider.radius)
    {
        return mtv = {0,0};
    }
    else
    {
        floatPoint tempMTV = collider.nearestPoint(point);
        mtv = (floatPoint){tempMTV.x - point.x, tempMTV.y - point.y};
    }

    return mtv;
}

bool renderText(std::string text, TTF_Font* font, int posX, int posY, SDL_Color textColor, int pointSize)
{
    bool success = false;

    float stretch = ((float)pointSize/DASHLEY_FONT_POINT);

    SDL_Surface* textSurface = TTF_RenderText_Solid( font, text.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( textTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //SDL_Rect stretchQuad = {}
            SDL_Rect renderQuad = {posX, posY, (textSurface->w * stretch), (textSurface->h * stretch)};
            SDL_RenderCopy(gRenderer, textTexture, NULL, &renderQuad);
            success = true;
        }

        //Get rid of old surface and texture
        SDL_FreeSurface( textSurface );
        SDL_DestroyTexture( textTexture );
    }
    return success;
}

bool pause()
{
    gTimer.pause();
    bool resumeGame = false;

    Uint32 drawTimer = SDL_GetTicks();

    SDL_Event e;

    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
    SDL_RenderClear( gRenderer );

    while( !gQuit && !resumeGame)
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0)
        {
            //User requests quit
            if( e.type == SDL_QUIT)
            {
                gQuit = true;
            }
            else if(e.type == SDL_MOUSEMOTION )
            {
                SDL_GetMouseState( &gMouse.x, &gMouse.y );
            }
            else if (e.type == SDL_MOUSEBUTTONUP)
            {
                if (gMouse.x < 200 && gMouse.x > 20 && gMouse.y < 100 && gMouse.y > 65)
                {
                    resumeGame = true;
                    gTimer.resume();
                }
                else if (gMouse.x < 200 && gMouse.x > 20 && gMouse.y < 135 && gMouse.y > 100)
                {
                    newGame();
                    resumeGame = true;
                }
                else if (gMouse.x < 200 && gMouse.x > 20 && gMouse.y < 275 && gMouse.y > 240)
                {
                    gQuit = true;
                    resumeGame = true;
                }
            }
        }

        if ((SDL_GetTicks() - drawTimer)/(1000/FRAME_RATE) >= 1)
        {
            SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
            SDL_RenderClear( gRenderer );
            menuDisplay();
            drawTimer = SDL_GetTicks();
        }


        //Update screen
        SDL_RenderPresent( gRenderer );
    }
    gPlayer.evaluateKeyInput(SDL_GetKeyboardState( NULL ));

    SDL_RenderClear( gRenderer );
    SDL_RenderPresent( gRenderer );

    return !resumeGame;
}

void menuDisplay()
{
    SDL_Color textHighlight = {200, 200, 200};
    SDL_Color textAvailable = {125,125,125};
    SDL_Color textUnavailable = {50,50,50};
    renderText("-Menu-", gFontDashley, 20, 20, {185,185,185}, 72);

    if (gPlayer.spriteSheet != NULL) //I need to find something to check if the game just launched
    {
        if (gMouse.x < 200 && gMouse.x > 20 && gMouse.y < 100 && gMouse.y > 65)
        {
            renderText("Resume", gFontDashley, 50, 75, textHighlight, 36);
        }
        else
        {
            renderText("Resume", gFontDashley, 50, 75, textAvailable, 36);
        }

    }
    else
    {
        renderText("Resume", gFontDashley, 50, 75, textUnavailable, 36);
    }
    if (true) //This one is always available
    {
        if (gMouse.x < 200 && gMouse.x > 20 && gMouse.y < 135 && gMouse.y > 100)
        {
            renderText("New Game", gFontDashley, 50, 110, textHighlight, 36);
        }
        else
        {
            renderText("New Game", gFontDashley, 50, 110, textAvailable, 36);
        }
    }
    if (true) //This one is always available
    {
        if (gMouse.x < 200 && gMouse.x > 20 && gMouse.y < 170 && gMouse.y > 135)
        {
            renderText("Load Game", gFontDashley, 50, 145, textHighlight, 36);
        }
        else
        {
            renderText("Load Game", gFontDashley, 50, 145, textAvailable, 36);
        }
    }
    if (gPlayer.spriteSheet != NULL) //I need to find something to check if the game just launch
    {
        if (gMouse.x < 200 && gMouse.x > 20 && gMouse.y < 205 && gMouse.y > 170)
        {
            renderText("Save Game", gFontDashley, 50, 180, textHighlight, 36);
        }
        else
        {
            renderText("Save Game", gFontDashley, 50, 182, textAvailable, 36);
        }
    }
    else
    {
        renderText("Save Game", gFontDashley, 50, 180, textUnavailable, 36);
    }
    if (true) //This one is always available
    {
        if (gMouse.x < 200 && gMouse.x > 20 && gMouse.y < 240 && gMouse.y > 205)
        {
            renderText("Settings", gFontDashley, 50, 215, textHighlight, 36);
        }
        else
        {
            renderText("Settings", gFontDashley, 50, 215, textAvailable, 36);
        }
    }
    if (true) //This one is always available
    {
        if (gMouse.x < 200 && gMouse.x > 20 && gMouse.y < 275 && gMouse.y > 240)
        {
            renderText("Quit", gFontDashley, 50, 250, textHighlight, 36);
        }
        else
        {
            renderText("Quit", gFontDashley, 50, 250, textAvailable, 36);
        }
    }
}

void newGame()
{
    //Start the global timer
    gCurWorldCoord = (intPoint){0,0};
    gTimer.start();
    gPlayer.free();
    gPlayer.spriteSheet = loadTexture((CHARACTER_SPRITESHEET_PATH + ".png").c_str());
}

//Main function

int main(int argc, char* argv[])
{
    std::cout << "Welcome to " << APPLICATION_NAME << std::endl;

    //Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else if( !loadTerrain())
	{
	    std::cout << "Failed to load terrain.";
	}
    else if ( loadWorld())
    {
        //Event handler
        SDL_Event e;

        Uint32 drawTimer = SDL_GetTicks();

        /*********************
        Function testing zone!
        **********************/
        std::cout << "\n************\nFunction testing zone\n************\n\n";

        std::cout << "\n************\nBack to regular programming!\n************\n\n";

        /*********************
        Back to regular programing!
        **********************/

        pause();

        while( !gQuit )
        {
            SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
            SDL_RenderClear( gRenderer );

            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT)
                {
                    gQuit = true;
                }
                else if(e.type == SDL_MOUSEMOTION )
                {
                    gPrevMouse.x = gMouse.x;
                    gPrevMouse.y = gMouse.y;
                    SDL_GetMouseState( &gMouse.x, &gMouse.y );
                }
                else if( e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
                {
                    gPlayer.evaluateKeyInput(SDL_GetKeyboardState( NULL ));
                }
            }

            gPlayer.updatePos();

            if ((SDL_GetTicks() - drawTimer)/(1000/FRAME_RATE) >= 1)
            {
                //Draw stuff on the renderer
                SDL_RenderClear( gRenderer );

                //gWorld[gCurWorldCoord.x][gCurWorldCoord.y].render(gRenderer, BACKGROUND);

                //gWorld[gCurWorldCoord.x][gCurWorldCoord.y].render(gRenderer, MIDGROUND, 0, gPlayer.zLayer);

                //gPlayer.render(gRenderer);

                //gWorld[gCurWorldCoord.x][gCurWorldCoord.y].render(gRenderer, MIDGROUND, gPlayer.zLayer, 9);

                //gWorld[gCurWorldCoord.x][gCurWorldCoord.y].render(gRenderer, FOREGROUND);

                /*********************
                Display testing zone!
                **********************/

                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );

                //SDL_RenderDrawLine( gRenderer, gPlayer.position.x, gPlayer.position.y, gMouse.x, gMouse.y);

                gPlayer.collision.draw(gRenderer);

                for (int i = 0; i < gWorld[gCurWorldCoord.x][gCurWorldCoord.y].vCollisionPolygon.size();i++)
                {
                    //SDL_SetRenderDrawColor( gRenderer, 0xFF + (11111 * i)%256, 0xFF + (33333 * i)%256, 0xFF + (99999 * i)%256, 0xFF );
                    gWorld[gCurWorldCoord.x][gCurWorldCoord.y].vCollisionPolygon[i].draw(gRenderer);
                }

                for (int i = 0; i < gWorld[gCurWorldCoord.x][gCurWorldCoord.y].vCollisionRadial.size();i++)
                {
                    //SDL_SetRenderDrawColor( gRenderer, 0xFF + (11111 * i)%256, 0xFF + (33333 * i)%256, 0xFF + (99999 * i)%256, 0xFF );
                    gWorld[gCurWorldCoord.x][gCurWorldCoord.y].vCollisionRadial[i].draw(gRenderer);
                }

                /*********************
                Back to regular programing!
                **********************/

                //Update screen
                SDL_RenderPresent( gRenderer );

                drawTimer = SDL_GetTicks();
            }
            else
            {
                SDL_Delay( (1000/FRAME_RATE)- (SDL_GetTicks() - drawTimer));
            }
        }
    }

    close();
    return 0;
}
