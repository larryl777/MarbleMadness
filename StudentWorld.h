#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"

using namespace std;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
    
    //adding objects
    bool addExit(double x, double y);
    bool addPlayer(double x, double y);
    bool addWall(double x, double y);
    bool addhrage(double x, double y);
    bool addvrage(double x, double y);
    bool addtbotf(double x, double y);
    bool addmtbotf(double x, double y);
    bool addmar(double x, double y);
    bool addpit(double x, double y);
    bool addcrys(double x, double y);
    bool addreshp(double x, double y);
    bool addexlife(double x, double y);
    bool addammo(double x, double y);
    bool addpea(double x, double y, int dir);
    bool addmeanbot(double x, double y);
    bool addtbot(double x, double y);
  
    //position helper functions
    bool isValidPos(double x, double y);
    bool isValidPos(double x, double y, Actor* avatar);
    bool overlapping(Actor* a, Actor* b);
    bool intersecting(Actor* a, Actor* b);
    bool intersecting(double x1, double y1, double x2, double y2);
    
    //object overlap and intersection checks
    bool overlappingPlayer(Actor* avatar);
    int overlappingDamageable(Actor* avatar);
    Actor* getOverlappingDamageableObject(double x, double y, Actor* avatar);

    //marble functionalities
    bool isvalidmarble(double x, double y, Actor* avatar);
    bool canpushobject(double x, double y, Actor* avatar);
   
    //robot functionalities
    bool stealgoodie(Actor* avatar);
    int countthiefbots(double x, double maxs, double miny, double maxy);
    Actor* thiefgoodie(Actor* avatar);
    Actor* playerinsight();
    bool isObstacle(double x, double y, int dir);

        
    void applyHp();
    void applyammo();
    
    ~StudentWorld();
    StudentWorld();
    
    void finishedlevel(){lvlcomplete = true;}
    int getBonus(){return bonus;}
    int numcrystals();
    void setDisplayText();
    
    
private:
    int crystals;
    bool lvlcomplete = true;
    int bonus;
    Player* p;
    vector<Actor*> actors;
};


#endif // STUDENTWORLD_H_
