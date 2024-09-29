#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}


//maze displays and character standing in proper position

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    
    p = nullptr;
    actors.clear();
    bonus = 1000;                   //bonus starts at 1000 every time we start a game
}

int StudentWorld::init()
{
    lvlcomplete = false;            //lvl in progress
    
    string lvl;
    if(getLevel() < 10){
        lvl = "0" + to_string(getLevel());
    }
    else if(getLevel() >= 10){
        lvl = to_string(getLevel());
    }
    else{
        return GWSTATUS_LEVEL_ERROR;
    }

    Level lev(assetPath());
    
    string curLevel =  "level" + lvl + ".txt";
    Level::LoadResult result = lev.loadLevel(curLevel);

    if (result == Level::load_fail_file_not_found || getLevel() == 100){
        cerr << "Could not find level00.txt data file\n";
        return GWSTATUS_PLAYER_WON;
    }

    else if (result == Level::load_fail_bad_format){
        cerr << "Your level was improperly formatted\n";
        return GWSTATUS_LEVEL_ERROR;
    }

    else if (result == Level::load_success){
        cerr << "Successfully loaded level\n";
        for(int x = 0; x < VIEW_WIDTH; x++){
            for(int y = 0; y < VIEW_HEIGHT; y++){
                Level::MazeEntry data = lev.getContentsOf(x, y);    //add functions to add if its vlaid
                switch (data){
                    case Level::empty:
                        break;
                    case Level::exit:
                        addExit(x,y);
                        break;
                    case Level::player:
                        addPlayer(x, y);
                        break;
                    case Level::horiz_ragebot:
                        addhrage(x, y);
                        break;
                    case Level::vert_ragebot:
                        addvrage(x,y);
                        break;
                    case Level::thiefbot_factory:
                        addtbotf(x, y);
                        break;
                    case Level::mean_thiefbot_factory:
                        addmtbotf(x, y);
                        break;
                    case Level::wall:
                        addWall(x, y);
                        break;
                    case Level::marble:
                        addmar(x, y);
                        break;
                    case Level::pit:
                        addpit(x, y);
                        break;
                    case Level::crystal:
                        addcrys(x, y);
                        break;
                    case Level::restore_health:
                        addreshp(x, y);
                        break;
                    case Level::extra_life:
                        addexlife(x,y);
                        break;
                    case Level::ammo:
                        addammo(x, y);
                        break;
                    
                }
            }
        }
     
    }
    return GWSTATUS_CONTINUE_GAME;
}
//adding avatars into the game
bool StudentWorld::addExit(double x, double y){
    if(isValidPos(x, y)){
        actors.push_back(new Exit(this, x, y));
        return true;
    }
    return false;
}
bool StudentWorld::addPlayer(double x, double y){
    if(isValidPos(x, y)){
        p = new Player(this, x ,y);
        actors.push_back(p);
        return true;
    }
    return false;
}
bool StudentWorld::addWall(double x, double y){
    if(isValidPos(x, y)){
        actors.push_back(new Wall(this, x, y));
        return true;
    }
    return false;
}

bool StudentWorld::addhrage(double x, double y){
    if(isValidPos(x, y)){
        actors.push_back(new ragebot(this, x, y, 0));       //horizontal face right
        return true;
    }
    return false;
}
bool StudentWorld::addvrage(double x, double y){
    if(isValidPos(x, y)){
        actors.push_back(new ragebot(this, x, y, 270));     //vert face down
        return true;
    }
    return false;
}
bool StudentWorld::addtbotf(double x, double y){
    if(isValidPos(x, y)){
        actors.push_back(new regfactory(this, x, y));
        return true;
    }
    return false;
}
bool StudentWorld::addmtbotf(double x, double y){
    if(isValidPos(x, y)){
        actors.push_back(new meanfactory(this, x, y));
        return true;
    }
    return false;
}

bool StudentWorld::addpea(double x, double y, int dir){
        actors.push_back(new Pea(this, x, y, dir));
        return true;
}
bool StudentWorld::addmeanbot(double x, double y){
    actors.push_back(new meanthiefbot(this, x, y));
    return true;
}
bool StudentWorld::addtbot(double x, double y){
    actors.push_back(new thiefbot(this, x, y));
    return true;
}
bool StudentWorld::addmar(double x, double y){
    if(isValidPos(x, y)){
        actors.push_back(new Marble(this, x, y));
        return true;
    }
    return false;
}
bool StudentWorld::addpit(double x, double y){
    if(isValidPos(x, y)){
        actors.push_back(new Pits(this, x, y));
        return true;
    }
    return false;
}
bool StudentWorld::addcrys(double x, double y){
    if(isValidPos(x, y)){
        actors.push_back(new Crystal(this, x, y));
        return true;
    }
    return false;
}
bool StudentWorld::addexlife(double x, double y){
    if(isValidPos(x, y)){
        actors.push_back(new ExtraLifeGoodie(this, x, y));
        return true;
    }
    return false;
}
bool StudentWorld::addreshp(double x, double y){
    if(isValidPos(x, y)){
        actors.push_back(new HealthGoodie(this, x, y));
        return true;
    }
    return false;
}
bool StudentWorld::addammo(double x, double y){
    if(isValidPos(x, y)){
        actors.push_back(new AmmoGoodie(this, x, y));
        return true;
    }
    return false;
}

int StudentWorld::move()
{
//    setDisplayText();
//    int size = static_cast<int>(actors.size());
//                  
//    p->doSomething();               //have player do something first
//    vector<Actor*>::iterator it = actors.begin();
//
//    for(int i =0; i< size; i++){
//      
//        if(!p->islive()){                                           //if player dies during said tick
//            decLives();
//            bonus = 1000;
//            return GWSTATUS_PLAYER_DIED;
//        }
//        if(lvlcomplete){                                            //lvl finished
//            playSound(SOUND_FINISHED_LEVEL);
//            increaseScore(getBonus());
//            bonus = 1000;
//            return GWSTATUS_FINISHED_LEVEL;
//        }
//        if (i == size){
//            break;
//        }
//
//        if((*it)->markedForRemove()){                       //set character to dead/remove them at end of current tick
//            (*it)->setHealth(0);
//        }
//        (*it)->doSomething();
//        it++;
//   
//    }
//    
//    for(auto it = actors.begin(); it != actors.end();){         //removing actors that are no longer active
//        if(!(*it)->islive()){
//            delete *it;
//            it = actors.erase(it);
//        }
//        else {
//            it++;
//        }
//    }
//    if(bonus > 0){
//        bonus--;                                //reduce current bonus each tick
//    }
//    return GWSTATUS_CONTINUE_GAME;
 
    setDisplayText();
    if(p->islive()){                //if player is live, do something
        p->doSomething();
    }
    else if (!p->islive()) {            //if player is not alive, decrease lives, reset bonus
         decLives();
         bonus = 1000;
         return GWSTATUS_PLAYER_DIED;
     }
     if (lvlcomplete) {                         //finished level, reset bonus
         playSound(SOUND_FINISHED_LEVEL);
         increaseScore(getBonus());
         bonus = 1000;
         return GWSTATUS_FINISHED_LEVEL;
     }

     int size = static_cast<int>(actors.size());
     vector<Actor*>::iterator it = actors.begin();
     for (int i = 0; i < size; i++) {                   //loop through container size
         if ((*it)->markedForRemove()) {                //if an actor was marked for remove, set its health to 0 so it can be removed
             (*it)->setHealth(0);
         }
         (*it)->doSomething();                          //rest of actors do something
         it++;
     }

     for (auto it = actors.begin(); it != actors.end();) {      //loop through list of actors again
         if (!(*it)->islive()) {                                //if its not live (health = 0), delete
             delete *it;
             it = actors.erase(it);
         } else {
             it++;
         }
     }
     if (bonus > 0) {                               //while bonus is greater than 0, decrement it each tick
         bonus--;
     }

     return GWSTATUS_CONTINUE_GAME;

  
}

int StudentWorld::numcrystals(){        //count current crystals on map each tick
    int crystals = 0;
    for(auto a: actors){
        if(a->allcrystals()){           //count amount of crystals, if all crystals are collected, it will return 0
            crystals++;
        }
    }

    return crystals;
}



void StudentWorld::cleanUp()
{
    for(Actor* x: actors){  //delete all actors in vector
        delete x;
    }
    actors.clear();     //clear and set p to null
    p = nullptr;
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

void StudentWorld::setDisplayText(){
    ostringstream oss;
    
    oss << "Score: ";
    oss << setw(7) << setfill('0') << getScore() << "  ";
    
    oss << "Level: ";
    oss << setw(2) << getLevel() << "  ";
    
    oss << "Lives: ";
    oss << setw(2) << setfill(' ') << getLives() << "  ";
    
    oss << "Health: ";
    int max = 20;
    oss << setw(3) << (p->getHealth()*100)/max << "%" << "  ";
    // health
    oss << "Ammo: ";
    oss << setw(3) << p->getAmmo() <<  "  ";
    //ammo
    
    oss << "Bonus: ";
    oss << setw(4) << getBonus();
    
    setGameStatText(oss.str());
}

void StudentWorld::applyHp(){       //restore hp to max
    increaseScore(500);
    p->fullrestore();
}
void StudentWorld::applyammo(){         //give player ammo
    increaseScore(100);
    p->addAmmo(20);
}



// INTERSECTION AND OEVRLAPPING helper functions


bool StudentWorld::overlappingPlayer(Actor* avatar) {               //if player overlaps with an avatar
        if (p->islive() && overlapping(avatar, p)){
            return true;
        }
    return false;
}

bool StudentWorld::intersecting(double x1, double y1, double x2, double y2) {       //intersecting avatars
    if (x1 + SPRITE_WIDTH_GL > x2 && x1 < x2 + SPRITE_WIDTH_GL &&
        y1 + SPRITE_HEIGHT_GL > y2 && y1 < y2 + SPRITE_HEIGHT_GL){
        return true;
    }
    return false;
}

bool StudentWorld::intersecting(Actor* a, Actor* b) {                   //wrapper for passing in two avatars
    return intersecting(a->getX(), a->getY(), b->getX(), b->getY());
}


bool StudentWorld::overlapping(Actor* a, Actor* b) {                    //overlapping for same x and y cords for both actors
    return intersecting(a->getX(), a->getY(), b->getX(), b->getY());
}

//Position of actors check using helper functons above

bool StudentWorld::isValidPos(double x, double y) {
    return isValidPos(x, y, nullptr);
}

bool StudentWorld::isValidPos(double x, double y, Actor* me) {      //dont use overlap if character/marble is moving onto square, as x and y cords will be diff
    for (auto a : actors){                                          //when function is called
        if (a != me){
            if (a->islive() && (a->blockmovement())){
                if (intersecting(x, y, a->getX(), a->getY())){
                    return false;
                }
            }
        }
    }
    return true;
}
bool StudentWorld::canpushobject(double x, double y, Actor* avatar){        //as function name says
        for (auto a : actors){
            if (a != avatar){
                if (a->ispushable() && intersecting(x, y, a->getX(), a->getY())){
                    return true;
                }
            }
        }
        return false;
    }
bool StudentWorld::isvalidmarble(double x, double y, Actor* avatar) {
    for (auto a : actors){
        if (a != avatar){
            if (a->blockMarble() && intersecting(x, y, a->getX(), a->getY())){  //if object blocks marble (wall,factory, live actors) and is intersecting, then
                return false;                                                   //return false
            }
        }
    }
    return true;
}
Actor* StudentWorld::getOverlappingDamageableObject(double x, double y, Actor* avatar){
    for (auto a : actors){
        if (a != avatar){
            if (a->ispushable() && intersecting(x, y, a->getX(), a->getY())){
                return a;
            }
            else if(a->isDamagable() && !a->blocksPeas() && overlapping(avatar, a)){           //get damageable actors
                    return a;
            }
        }
    }
    return nullptr;
}
Actor* StudentWorld::playerinsight(){
    for(auto a: actors){                    //find player to get coordinates
        if(a == p){
            return a;
        }
    }
    return nullptr;
}

int StudentWorld::overlappingDamageable(Actor* avatar){
    bool canoverlap = false;
     bool peablocker = false;
    bool damageable = false;

     for (auto a : actors) {                                    //loop through actors
     
         if (a != avatar) {
             if (a->isDamagable() && !a->consumable() && overlapping(avatar, a)) {
                 damageable = true;                             //damageable objects
                 if (a->spawnable()) {
                     canoverlap = true;                      //if its also a robot that can spawn ontop of other avatars
                 }
             }
             else if (a->blocksPeas() && overlapping(avatar, a)) {      //avatars that blocks peas
                 peablocker = true;
             }

         }
     }
     if (canoverlap || damageable) {                         //if its a robot that overlaps with another avatar or if its just damageable (can damage)
         return 1;
     } 
     else if (peablocker) {                             //wall,factory
         return 2;
     }
     else if (canoverlap) {                             //robot being able to overlap on spawn (factories cant create robot if there already is a robot ontop)
         return 3;
     }

     return 0;
}
bool StudentWorld::stealgoodie(Actor* avatar){                      //if robot overlaps with a stealable goodie
    for(auto a : actors){
        if(a != avatar){
            if(a->stealable() && overlapping(avatar, a)){
                return true;
            }
        }
    }
    return false; 
}

Actor* StudentWorld::thiefgoodie(Actor* avatar){                     //get overlapping goodie
    for(auto a: actors){
        if(a!= avatar){
            if(a->stealable() && !a->isDamagable() && overlapping(avatar, a)){
                return a;
            }
        }
    }
    return nullptr;
}

bool StudentWorld::isObstacle(double x, double y, int dir){
    for (auto a : actors) {
        if (a->cantshootat()) {                                           //check if theres an obstacle in between player and robot
            double actorX = a->getX();
            double actorY = a->getY();
            if ((dir == 90 && actorY > y && actorY < p->getY() && actorX == x) ||             //up
                (dir == 270 && actorY < y && actorY > p->getY() && actorX == x) ||            //down
                (dir == 0 && actorX > x && actorX < p->getX() && actorY == y) ||              //right
                (dir == 180 && actorX < x && 
                 actorX > p->getX() && actorY == y)) {                                          //left
                return true;                                                                    //obstacle found
            }
        }
    }
    return false;
}

int StudentWorld::countthiefbots(double x, double maxs, double y, double maxy){     //how many thiefbots are in square around factory
    int counter = 0;
    for(auto a:actors){
        if(a->spawnable()){
            double robotx = a->getX();
            double roboty = a->getY();
            if (robotx >= x && robotx <= maxs && roboty >= y && roboty<= maxy) {
                    counter++;
            }
        }
    }
    return counter;
}

