#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
//#include <tinyxml2.h>

//Global Constant

const float PI = 3.14159f;

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
    BLANK_COLIDER,
    BLANK,
    TERRAIN_COUNT //This one serve no purpose other than to have the number of STATE available
};

//This are just an array on strings for check the against the enum
const std::string terrainTypeName[COMBO_COUNT] =
{
    "DIRT",
    "GRASS",
    "TREE_TRUNK_SMALL",
    "BLANK_COLIDER",
    "BLANK"
};

//class declaration

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
        std::string terrainName;

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
        bool colide;

        //For terrain destruction
        float health;
        float resPiercing;
        float resCuting;
        float resSmashing;

        //If the terrain is destroyed, what replace it
        std::string decayTo;

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
        void render(int zLayerBottom, int zLayerTop, SDL_Renderer* renderer);

        //Class variables

        terrain tileMap[TILE_NUMBER_WIDTH][TILE_NUMBER_HEIGHT];

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

class player
{
    public:

        //Function

        //Initializes variables
        player();

        //Deallocates memory
        ~player();

        //Deallocates texture
		void free();

        //Render the character to a specified renderer
		void render(SDL_Renderer* renderer);

        void loadAnim();

        void evaluateKeyInput(const Uint8* currentKeyStates);

        bool changeState(characterState newState);

        void updatePos();

        bool checkColisionWorld(terrainMap currMap);

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
        float posX;
        float posY;

        int worldCoordX;
        int worldCoordY;

        float prevPosX;
        float prevPosY;

        Uint32 lastUpdate;

        //Character orientation
        int angle;

        float speed;

    private:

};

//Global Variable

//Quit game
bool gQuit = false;

//Mouse coordinate
int gMouseX = 0;
int gMouseY = 0;
//Prev mouse coordinate
int gPrevMouseX = 0;
int gPrevMouseY = 0;

timer gTimer;

//The player
player gPlayer;

//The information about the type of each type of terrain
terrain gTerrain[TERRAIN_COUNT];

terrainMap gWorld[WORLD_WIDTH][WORLD_HEIGHT];

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

float evalDistance(float x1, float y1, float x2, float y2);

SDL_Texture* loadTexture(std::string path);

bool loadTerrain();

bool loadWorld();

bool renderText(std::string text, TTF_Font* font, int posX, int posY, SDL_Color textColor, int pointSize = 125);

bool pause();

void menuDisplay();

void newGame();

//Class function definition

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
player::player()
{
    //Initialize the position and orientation
    free();
    loadAnim();
    SDL_DestroyTexture(spriteSheet);

    //this is a temporary hack
    weapon = SHIELD_SPEAR;
}

//Player destructor
player::~player()
{
    free();
}

void player::free()
{
    posX = SCREEN_WIDTH/2;
    posY = SCREEN_HEIGHT/2;
    worldCoordX = 0;
    worldCoordY = 0;
    prevPosX = posX;
    prevPosY = posY;
    angle = 0;
    speed = 0.1;
    lastUpdate = gTimer.getTime();

    SDL_DestroyTexture(spriteSheet);
    spriteSheet = NULL;
}

void player::loadAnim()
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

void player::render(SDL_Renderer* renderer)
{
    SDL_Point center {(stateAnim[weapon][state].frameCenterW), (stateAnim[weapon][state].frameCenterH)};
    SDL_Rect spriteQuad = (stateAnim[weapon][state].renderSprite(stateTimer));
    SDL_Rect renderQuad = (stateAnim[weapon][state].renderQuad(posX, posY));

    SDL_RenderCopyEx( renderer, spriteSheet, &spriteQuad, &renderQuad,  angle, &center, SDL_FLIP_NONE );
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
        && evalDistance(gMouseX, gMouseY, posX, posY) > stateAnim[weapon][state].frameCenterH/2)
    {
        gPlayer.changeState(FORWARD);
    }
    else if ( !currentKeyStates[ SDL_SCANCODE_W ]
        && currentKeyStates[ SDL_SCANCODE_S ]
        && !currentKeyStates[ SDL_SCANCODE_A ]
        && !currentKeyStates[ SDL_SCANCODE_D ])
    {
        gPlayer.changeState(BACKWARD);
    }
    else if ( !currentKeyStates[ SDL_SCANCODE_W ]
        && !currentKeyStates[ SDL_SCANCODE_S ]
        && currentKeyStates[ SDL_SCANCODE_A ]
        && !currentKeyStates[ SDL_SCANCODE_D ])
    {
        gPlayer.changeState(LEFT);
    }
    else if ( !currentKeyStates[ SDL_SCANCODE_W ]
        && !currentKeyStates[ SDL_SCANCODE_S ]
        && !currentKeyStates[ SDL_SCANCODE_A ]
        && currentKeyStates[ SDL_SCANCODE_D ])
    {
        gPlayer.changeState(RIGHT);
    }
    else if ( currentKeyStates[ SDL_SCANCODE_W ]
        && !currentKeyStates[ SDL_SCANCODE_S ]
        && currentKeyStates[ SDL_SCANCODE_A ]
        && !currentKeyStates[ SDL_SCANCODE_D ]
        && evalDistance(gMouseX, gMouseY, posX, posY) > stateAnim[weapon][state].frameCenterH/2)
    {
        gPlayer.changeState(FORWARD_LEFT); //Need to be change for a diagonal
    }
    else if ( currentKeyStates[ SDL_SCANCODE_W ]
        && !currentKeyStates[ SDL_SCANCODE_S ]
        && !currentKeyStates[ SDL_SCANCODE_A ]
        && currentKeyStates[ SDL_SCANCODE_D ]
        && evalDistance(gMouseX, gMouseY, posX, posY) > stateAnim[weapon][state].frameCenterH/2)
    {
        gPlayer.changeState(FORWARD_RIGHT); //Need to be change for a diagonal
    }
    else if ( !currentKeyStates[ SDL_SCANCODE_W ]
        && currentKeyStates[ SDL_SCANCODE_S ]
        && currentKeyStates[ SDL_SCANCODE_A ]
        && !currentKeyStates[ SDL_SCANCODE_D ])
    {
        gPlayer.changeState(BACKWARD_LEFT); //Need to be change for a diagonal
    }
    else if ( !currentKeyStates[ SDL_SCANCODE_W ]
        && currentKeyStates[ SDL_SCANCODE_S ]
        && !currentKeyStates[ SDL_SCANCODE_A ]
        && currentKeyStates[ SDL_SCANCODE_D ])
    {
        gPlayer.changeState(BACKWARD_RIGHT); //Need to be change for a diagonal
    }
    else
    {
        gPlayer.changeState(IDLE);
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

bool player::changeState(characterState newState)
{
    bool stateChanged = false;
    if (state != newState)
    {
        state = newState;
        stateTimer = gTimer.getTime();
        stateChanged = true;
    }
    return stateChanged;
}

void player::updatePos()
{
    angle = (atan2(posX - gMouseX, posY - gMouseY)*-180/PI);

    prevPosX = posX;
    prevPosY = posY;

    Uint32 timeMulti = gTimer.getTime() - lastUpdate;
    switch(state)
    {
        case FORWARD:
            posX = posX + sin(angle*PI/180)*timeMulti*speed;
            posY = posY - cos(angle*PI/180)*timeMulti*speed;
            lastUpdate = gTimer.getTime();
            break;

        case BACKWARD:
            posX = posX - sin(angle*PI/180)*timeMulti*speed;
            posY = posY + cos(angle*PI/180)*timeMulti*speed;
            lastUpdate = gTimer.getTime();
            break;

        case LEFT:
            posX = posX - sin((angle+90)*PI/180)*timeMulti*speed;
            posY = posY + cos((angle+90)*PI/180)*timeMulti*speed;
            lastUpdate = gTimer.getTime();
            break;

        case RIGHT:
            posX = posX - sin((angle-90)*PI/180)*timeMulti*speed;
            posY = posY + cos((angle-90)*PI/180)*timeMulti*speed;
            lastUpdate = gTimer.getTime();
            break;

        case FORWARD_LEFT:
            posX = posX + sin((angle+45)*PI/180)*timeMulti*speed;
            posY = posY - cos((angle+45)*PI/180)*timeMulti*speed;
            lastUpdate = gTimer.getTime();
            break;

        case FORWARD_RIGHT:
            posX = posX + sin((angle-45)*PI/180)*timeMulti*speed;
            posY = posY - cos((angle-45)*PI/180)*timeMulti*speed;
            lastUpdate = gTimer.getTime();
            break;

         case BACKWARD_LEFT:
            posX = posX - sin((angle+45)*PI/180)*timeMulti*speed;
            posY = posY + cos((angle+45)*PI/180)*timeMulti*speed;
            lastUpdate = gTimer.getTime();
            break;

        case BACKWARD_RIGHT:
            posX = posX - sin((angle-45)*PI/180)*timeMulti*speed;
            posY = posY + cos((angle-45)*PI/180)*timeMulti*speed;
            lastUpdate = gTimer.getTime();
            break;

        default:
            lastUpdate = gTimer.getTime();
            break;
    }

    if (posX < 0 || posX > SCREEN_WIDTH)
    {
        posX = prevPosX;
    }
    if (posY < 0 || posY > SCREEN_HEIGHT)
    {
        posY = prevPosY;
    }

    if (checkColisionWorld(gWorld[gPlayer.worldCoordX][gPlayer.worldCoordY]))
    {
        posX = prevPosX;
        posY = prevPosY;
    }
    //std::cout << "Character position: " << posX << ", " << posY << std::endl;

}

bool player::checkColisionWorld(terrainMap currMap)
{
    bool colide = false;

    bool colideArray[TILE_NUMBER_WIDTH][TILE_NUMBER_HEIGHT];

    for (int i = 0; i < TILE_NUMBER_HEIGHT; i++)
    {
        for (int j = 0; j < TILE_NUMBER_WIDTH; j++)
        {
            colideArray[j][i] = true;
        }
    }

    int charMinX = posX - stateAnim[weapon][state].frameCenterW;
    int charMaxX = posX - stateAnim[weapon][state].frameCenterW + stateAnim[weapon][state].frameW;
    int charMinY = posY - stateAnim[weapon][state].frameCenterH +10;
    int charMaxY = posY - stateAnim[weapon][state].frameCenterH + stateAnim[weapon][state].frameH;

    std::cout << "Character size: " << charMinX << ", " << charMaxX << ", " << charMinY << ", " << charMaxY << std::endl;

    for (int i = 0; i < TILE_NUMBER_HEIGHT; i++)
    {
        for (int j =0; j < TILE_NUMBER_WIDTH; j++)
        {
            if (currMap.tileMap[j][i].colide)
            {
                int objMinX = j * MAP_TILE_WIDTH + MAP_TILE_WIDTH / 2 - currMap.tileMap[j][i].spriteCenterW;
                int objMaxX = j * MAP_TILE_WIDTH + MAP_TILE_WIDTH / 2 - currMap.tileMap[j][i].spriteCenterW + currMap.tileMap[j][i].spriteW;
                int objMinY = i * MAP_TILE_HEIGHT + MAP_TILE_HEIGHT / 2 - currMap.tileMap[j][i].spriteCenterH;
                int objMaxY = i * MAP_TILE_HEIGHT + MAP_TILE_HEIGHT / 2 - currMap.tileMap[j][i].spriteCenterH + currMap.tileMap[j][i].spriteH;

                std::cout << "Evalutating terrain: " << currMap.tileMap[j][i].terrainName << " pos: " << j << ", " << i << std::endl
                    << "Colision size: " << objMinX << ", " << objMaxX << ", " << objMinY << ", " << objMaxY << std::endl;
                /*if (charMinX < objMaxX)
                {
                   if (charMinY < objMaxY || charMaxY > objMinY)
                   {
                        //colide = true;
                        std::cout << "Colision detected \n";
                   }
                }
                if (charMaxX > objMinX)
                {
                   if (charMinY < objMaxY && charMaxY > objMinY)
                   {
                        //colide = true;
                        std::cout << "Colision detected \n";
                   }
                }*/
                if (charMaxX < objMinX)
                {
                    colideArray[j][i] = false;
                }
                if (charMinX > objMaxX)
                {
                    colideArray[j][i] = false;
                }
                if (charMaxY < objMinY)
                {
                    colideArray[j][i] = false;
                }
                if (charMinY > objMaxY)
                {
                    colideArray[j][i] = false;
                }
            }
            else
            {
                colideArray[j][i] = false;
            }
        }
    }

    for (int i = 0; i < TILE_NUMBER_HEIGHT; i++)
    {
        for (int j = 0; j < TILE_NUMBER_WIDTH; j++)
        {
            if (colideArray[j][i] == true)
            {
                colide = true;
                std::cout << "Colision detected on tile: " << j << ", " << i << std::endl;
            }
        }
    }

    return colide;
}

terrain::terrain()
{
    terrainName = "DIRT";
    spriteW = 48;
    spriteH = 48;
    spriteCenterW = 24;
    spriteCenterH = 24;
    spritePosX = 0;
    spritePosY = 0;
    colide = false;
    health = -1;
    resPiercing = -1;
    resCuting = -1;
    resSmashing = -1;
    decayTo = "DIRT";
    zLayer = -1;
}

terrain::~terrain()
{
    spriteW = 48;
    spriteH = 48;
    spriteCenterW = 24;
    spriteCenterH = 24;
    spritePosX = 0;
    spritePosY = 0;
    colide = false;
    health = -1;
    resPiercing = -1;
    resCuting = -1;
    resSmashing = -1;
    decayTo = "DIRT";
    zLayer = -1;
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
    for (int i = 0; i < TILE_NUMBER_HEIGHT; i++)
    {
        for (int j = 0; j < TILE_NUMBER_WIDTH; j++)
        {
            tileMap[i][j] = gTerrain[DIRT];
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
        for (int i = 0; i < TILE_NUMBER_HEIGHT; i++)
        {
            for (int j = 0; j  < TILE_NUMBER_WIDTH; j++)
            {
                if (!mapFile.eof())
                {
                    std::string tileName;
                    mapFile >> tileName;
                    for (int k = 0; k < TERRAIN_COUNT; k++)
                    {
                        if (tileName == gTerrain[k].terrainName)
                        {
                            tileMap[j][i] = gTerrain[k];
                        }
                    }
                }
                else
                {
                    return success;
                }
            }
        }
        success = true;
        mapFile.close();
    }
    else
    {
        std::cout << "Could not find map file " << std::to_string(mapCoordX) << "-" << std::to_string(mapCoordY) << ".map" << std::endl;
    }

    return success;
}

void terrainMap::render(int zLayerBottom, int zLayerTop, SDL_Renderer* renderer)
{
    for (int i = zLayerBottom; i <= zLayerTop; i++)
    {
        for (int j = 0; j < TILE_NUMBER_HEIGHT; j++)
        {
            for (int k = 0; k < TILE_NUMBER_WIDTH; k++)
            {
                if (tileMap[k][j].zLayer == i)
                {
                    tileMap[k][j].render(renderer, (k*MAP_TILE_WIDTH+MAP_TILE_WIDTH/2),(j*MAP_TILE_HEIGHT+MAP_TILE_HEIGHT/2));
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

                    std::string tempColide;
                    std::string tempDecayTo;

                    gTerrain[i].terrainName = terrainName;

                    terrainFile >> gTerrain[i].spriteW
                        >> gTerrain[i].spriteH
                        >> gTerrain[i].spriteCenterW
                        >> gTerrain[i].spriteCenterH
                        >> gTerrain[i].spritePosX
                        >> gTerrain[i].spritePosY
                        >> tempColide
                        >> gTerrain[i].health
                        >> gTerrain[i].resPiercing
                        >> gTerrain[i].resCuting
                        >> gTerrain[i].resSmashing
                        >> gTerrain[i].decayTo
                        >> gTerrain[i].zLayer;

                        for (int j = 0; j < tempColide.length(); j++)
                        {
                            tempColide[j] = std::toupper(tempColide[j]);
                        }
                        if (tempColide == "TRUE")
                        {
                            gTerrain[i].colide = true;
                        }
                        else
                        {
                            gTerrain[i].colide = false;
                        }
                }
            }
        }
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
                SDL_GetMouseState( &gMouseX, &gMouseY );
            }
            else if (e.type == SDL_MOUSEBUTTONUP)
            {
                if (gMouseX < 200 && gMouseX > 20 && gMouseY < 100 && gMouseY > 65)
                {
                    resumeGame = true;
                    gTimer.resume();
                }
                else if (gMouseX < 200 && gMouseX > 20 && gMouseY < 135 && gMouseY > 100)
                {
                    newGame();
                    resumeGame = true;
                }
                else if (gMouseX < 200 && gMouseX > 20 && gMouseY < 275 && gMouseY > 240)
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

    if (gPlayer.spriteSheet != NULL) //I need to find something to check if the game just launch
    {
        if (gMouseX < 200 && gMouseX > 20 && gMouseY < 100 && gMouseY > 65)
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
        if (gMouseX < 200 && gMouseX > 20 && gMouseY < 135 && gMouseY > 100)
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
        if (gMouseX < 200 && gMouseX > 20 && gMouseY < 170 && gMouseY > 135)
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
        if (gMouseX < 200 && gMouseX > 20 && gMouseY < 205 && gMouseY > 170)
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
        if (gMouseX < 200 && gMouseX > 20 && gMouseY < 240 && gMouseY > 205)
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
        if (gMouseX < 200 && gMouseX > 20 && gMouseY < 275 && gMouseY > 240)
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
    //Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else if( !loadTerrain())
	{
        if ( loadWorld())
        {
            pause();

            //Event handler
            SDL_Event e;

            Uint32 drawTimer = SDL_GetTicks();

            while( !gQuit )
            {
                SDL_SetRenderDrawColor( gRenderer, 0xEE, 0xEE, 0xEE, 0x00 );
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
                        gPrevMouseX = gMouseX;
                        gPrevMouseY = gMouseY;
                        SDL_GetMouseState( &gMouseX, &gMouseY );
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

                    gWorld[gPlayer.worldCoordX][gPlayer.worldCoordY].render(0, 4, gRenderer);

                    gPlayer.render(gRenderer);

                    gWorld[gPlayer.worldCoordX][gPlayer.worldCoordY].render(6, 10, gRenderer);

                    //Update screen
                    SDL_RenderPresent( gRenderer );

                    drawTimer = SDL_GetTicks();
                }

            }
        }
    }
    else
    {
        std::cout << "Failed to load terrain.";
    }


    close();
    return 0;
}
