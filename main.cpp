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
const int WORLD_HEIGHT = 1;

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
    BOX_COLLIDER,
    BLANK_GATE,
    BLANK,
    TERRAIN_COUNT //This one serve no purpose other than to have the number of STATE available
};

//This are just an array on strings for check the against the enum
const std::string terrainTypeName[COMBO_COUNT] =
{
    "DIRT",
    "GRASS",
    "TREE_TRUNK_SMALL",
    "BLANK_GATE",
    "BOX_COLLIDER",
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

const std::string collisionTypeName[COLLISION_COUNT] =
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

        floatPoint position;

        float rotation;

        int sideNumber;

    private:
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
};*/

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

};

class circle
{
    public:

        circle();

        circle(floatPoint position, float radius);

        float radius;

        floatPoint position;

    private:

};*/

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
		void render(SDL_Renderer* renderer, int posX, int posY);

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

        //Does this terrain block the player
        collisionType collide;

        //For terrain destruction
        float health;
        float resPiercing;
        float resCuting;
        float resSmashing;

        //If the terrain is destroyed, what replace it
        terrainType decayTo;

        //the Oreder in what the terrain must be rendered
        int zLayer;
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

        bool checkCollisionTerrain(std::vector <intPoint> collider);

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

    private:

};

class player: public character
{
    public:

        //Function

        player();

        bool checkCollisionTerrain(std::vector <intPoint> collider);

        void evaluateKeyInput(const Uint8* currentKeyStates);

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

//Different function to evaluate distance between different type of point.
float evalDistance(float x1, float y1, float x2, float y2);
float evalDistance(floatPoint point1, floatPoint point2);
float evalDistance(intPoint point1, intPoint point2);
float evalDistance(floatPoint point1, intPoint point2);
float evalDistance(intPoint point1, floatPoint point2);

bool pointOnVectorCheck(floatPoint point, floatPoint vecStart, floatPoint vecEnd);

int threePointOrientation(floatPoint point1, floatPoint point2, floatPoint point3);

bool vectorCrossCheck(floatPoint vec1Start, floatPoint vec1End, floatPoint vec2Start, floatPoint vec2End);

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
    sideNumber = 0;
}

rect::rect()
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
        std::cout << "Player state changed to: " << characterStateName[state] << std::endl;

        std::cout << "Distance from character to mouse: " << evalDistance(position, gMouse) << " Angle: " << angle << std::endl;
    }
    return stateChanged;
}

bool character::checkCollisionTerrain(std::vector <intPoint> collider)
{
    bool collide = false;

    //Here I'm defining some arbitrary collision for the character.  I'll need to change that on something like collision per weapon or per animation.

    float charRadius = fmin(MAP_TILE_HEIGHT, MAP_TILE_WIDTH)/2 - 2; //I added an arbitrary -2 just to give it some tolerance.

    for (int i = 0; i < collider.size(); i++)
    {
        floatPoint currColliderPos = {(collider[i].x * MAP_TILE_WIDTH + MAP_TILE_WIDTH / 2), (collider[i].y * MAP_TILE_HEIGHT + MAP_TILE_HEIGHT / 2)};

        collisionType currCollide = gWorld[gCurWorldCoord.x][gCurWorldCoord.y].tileMap[collider[i].x][collider[i].y][MIDGROUND].collide;

        if (currCollide == BOX && currCollide == GATE)
        {
            floatPoint objMin = {currColliderPos.x - MAP_TILE_WIDTH / 2, currColliderPos.y - MAP_TILE_HEIGHT / 2};
            floatPoint objMax = {currColliderPos.x + MAP_TILE_WIDTH / 2, currColliderPos.y + MAP_TILE_HEIGHT / 2};

            floatPoint nearObj;

            if (position.x < objMin.x)
            {
                nearObj.x = objMin.x;
            }
            else if (position.x > objMax.x)
            {
                nearObj.x = objMax.x;
            }
            else
            {
                nearObj.x = position.x;
            }

            if (position.y < objMin.y)
            {
                nearObj.y = objMin.y;
            }
            else if (position.y > objMax.y)
            {
                nearObj.y = objMax.y;
            }
            else
            {
                nearObj.y = position.y;
            }

            if (evalDistance(position, nearObj) < charRadius)
            {
                collide = true;

                std::cout << "Collision detected: " << collisionTypeName[currCollide] << std::endl;
            }
        }
        else if (currCollide == RADIAL)
        {
            if (evalDistance(position, currColliderPos) < (charRadius + fmin(MAP_TILE_HEIGHT, MAP_TILE_WIDTH)/2))
            {
                collide = true;

                std::cout << "Collision detected: " << collisionTypeName[currCollide] << std::endl;
            }
        }

    }

    return collide;
}

bool player::checkCollisionTerrain(std::vector <intPoint> collider)
{
    bool collide = false;

    bool collideWithGate = false;

    //Here I'm defining some arbitrary collision for the character.  I'll need to change that on something like collision per weapon or per animation.

    float charRadius = fmin(MAP_TILE_HEIGHT, MAP_TILE_WIDTH)/2 - 2; //I added an arbitrary -2 just to give it some tolerance.

    for (int i = 0; i < collider.size(); i++)
    {
        floatPoint currColliderPos = {(collider[i].x * MAP_TILE_WIDTH + MAP_TILE_WIDTH / 2), (collider[i].y * MAP_TILE_HEIGHT + MAP_TILE_HEIGHT / 2)};

        collisionType currCollide = gWorld[gCurWorldCoord.x][gCurWorldCoord.y].tileMap[collider[i].x][collider[i].y][MIDGROUND].collide;

        if (currCollide == GATE)
        {
            if (evalDistance(position, currColliderPos) < charRadius)
            {
                if (!justGated)
                {
                    for (int j = 0; j < gWorld[gCurWorldCoord.x][gCurWorldCoord.y].vGate.size(); j++)
                    {
                        if ( (collider[i].x == gWorld[gCurWorldCoord.x][gCurWorldCoord.y].vGate[j].position.x) && (collider[i].y == gWorld[gCurWorldCoord.x][gCurWorldCoord.y].vGate[j].position.y))
                        {
                            gCurWorldCoord = gWorld[gCurWorldCoord.x][gCurWorldCoord.y].vGate[j].targetWorldCoord;
                            position.x = gWorld[gCurWorldCoord.x][gCurWorldCoord.y].vGate[j].targetPosCoord.x * MAP_TILE_WIDTH + MAP_TILE_WIDTH / 2;
                            position.y = gWorld[gCurWorldCoord.x][gCurWorldCoord.y].vGate[j].targetPosCoord.y * MAP_TILE_HEIGHT + MAP_TILE_HEIGHT / 2;

                            std::cout << "Collision detected: " << collisionTypeName[currCollide] << std::endl;
                        }
                    }
                    justGated = true;
                }
                collideWithGate = true;
            }
        }
        else if (currCollide == BOX)
        {
            floatPoint objMin = {currColliderPos.x - MAP_TILE_WIDTH / 2, currColliderPos.y - MAP_TILE_HEIGHT / 2};
            floatPoint objMax = {currColliderPos.x + MAP_TILE_WIDTH / 2, currColliderPos.y + MAP_TILE_HEIGHT / 2};

            floatPoint nearObj;

            if (position.x < objMin.x)
            {
                nearObj.x = objMin.x;
            }
            else if (position.x > objMax.x)
            {
                nearObj.x = objMax.x;
            }
            else
            {
                nearObj.x = position.x;
            }

            if (position.y < objMin.y)
            {
                nearObj.y = objMin.y;
            }
            else if (position.y > objMax.y)
            {
                nearObj.y = objMax.y;
            }
            else
            {
                nearObj.y = position.y;
            }

            if (evalDistance(position, nearObj) < charRadius)
            {
                collide = true;

                std::cout << "Collision detected: " << collisionTypeName[currCollide] << std::endl;
            }
        }
        else if (currCollide == RADIAL)
        {
            if (evalDistance(position, currColliderPos) < (charRadius + fmin(MAP_TILE_HEIGHT, MAP_TILE_WIDTH)/2))
            {
                collide = true;

                std::cout << "Collision detected: " << collisionTypeName[currCollide] << std::endl;
            }
        }

    }

    if (collideWithGate == false)
    {
        justGated = false;
    }

    return collide;
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

    /*if (currentKeyStates[ SDL_SCANCODE_LSHIFT] )
    {
        gPlayer.speed = 0.4;
    }
    else
    {
        gPlayer.speed = 0.1;
    }*/
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

    if (checkCollisionTerrain(gWorld[gCurWorldCoord.x][gCurWorldCoord.y].vTerrainCollide))
    {
        position.x = prevPosition.x;
        position.y = prevPosition.y;
    }

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
    collide = NONE;
    health = -1;
    resPiercing = -1;
    resCuting = -1;
    resSmashing = -1;
    decayTo = BLANK;
    zLayer = 0;
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
    collide = NONE;
    health = -1;
    resPiercing = -1;
    resCuting = -1;
    resSmashing = -1;
    decayTo = BLANK;
    zLayer = 0;
}

void terrain::render(SDL_Renderer* renderer, int posX, int posY)
{
    SDL_Point center {spriteCenterW, spriteCenterH};
    SDL_Rect spriteQuad = {spritePosX, spritePosY, spriteW, spriteH};
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

    std::string path = "ressource/map/0-0.map";// + std::to_string(mapCoordX) + "-" + std::to_string(mapCoordY) + ".map";

    std::ifstream mapFile(path.c_str());

    if  (mapFile.is_open())
    {
        std::string tempRead;

        int gateNumber = 0;

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
                                    }
                                    if (gTerrain[k].collide == GATE)
                                    {
                                        gateNumber++;
                                        vGate.push_back((mapGate)((intPoint){j,i}));
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
            for (int i = 0; i < gateNumber; i++)
            {
                mapFile >> vGate[i].targetWorldCoord.x >> vGate[i].targetWorldCoord.y >> vGate[i].targetPosCoord.x >> vGate[i].targetPosCoord.y;
            }
        }

        success = true;
        mapFile.close();
    }
    else
    {
        //std::cout << "Could not find map file " << std::to_string(mapCoordX) << "-" << std::to_string(mapCoordY) << ".map" << std::endl;
    }

    return success;
}

void terrainMap::render(SDL_Renderer* renderer, zPlane plane, int zLayerBottom, int zLayerTop)
{

    for (int i = zLayerBottom; i <= zLayerTop; i++)
    {
        for (int j = 0; j < TILE_NUMBER_HEIGHT; j++)
        {
            for (int k = 0; k < TILE_NUMBER_WIDTH; k++)
            {
                if (tileMap[k][j][plane].zLayer == i)
                {
                    tileMap[k][j][plane].render(renderer, (k*MAP_TILE_WIDTH+MAP_TILE_WIDTH/2),(j*MAP_TILE_HEIGHT+MAP_TILE_HEIGHT/2));
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
        point.y <= fmax(vecStart.y, vecEnd.y) && point.y >= fmin(vecStart.y, vecEnd.y))
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
    gTimer.start();
    gPlayer.free();
    gPlayer.spriteSheet = loadTexture((CHARACTER_SPRITESHEET_PATH + ".png").c_str());
}

//Main function

int main(int argc, char* argv[])
{
   intPoint position(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

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
        pause();

        //Event handler
        SDL_Event e;

        Uint32 drawTimer = SDL_GetTicks();

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

            if ((SDL_GetTicks() - drawTimer)/(1000/FRAME_RATE) >= 1)
            {
                gPlayer.updatePos();

                //Draw stuff on the renderer
                SDL_RenderClear( gRenderer );

                gWorld[gCurWorldCoord.x][gCurWorldCoord.y].render(gRenderer, BACKGROUND);

                gWorld[gCurWorldCoord.x][gCurWorldCoord.y].render(gRenderer, MIDGROUND, 0, gPlayer.zLayer);

                gPlayer.render(gRenderer);

                gWorld[gCurWorldCoord.x][gCurWorldCoord.y].render(gRenderer, MIDGROUND, gPlayer.zLayer, 9);

                gWorld[gCurWorldCoord.x][gCurWorldCoord.y].render(gRenderer, FOREGROUND);

                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );

                SDL_RenderDrawLine( gRenderer, gPlayer.position.x, gPlayer.position.y, gMouse.x, gMouse.y);

                //std::cout << "Distance from character to mouse: " << evalDistance(gPlayer.position.x, gPlayer.position.y, gMouse.x, gMouse.y) << " Angle: " << gPlayer.angle << std::endl;

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
