#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


class StudentWorld;

//Actor
    //LiveActors
        //player
        //robots
    //marble
    //pea
    //goodies
        //goodies,exit, crystal


class Actor: public GraphObject{
public:
    Actor(StudentWorld* world, int ID, double x_, double y_, int dir): GraphObject(ID, x_, y_, dir), m_world(world){
        setVisible(true);
    }
    void setinvalmarb() {invalmarb = true;}
    bool marinvalid() {return invalmarb;}
    void setHealth(int health){m_health = health;}
    int getHealth(){return m_health;}
    bool islive(){return m_health > 0;}
    void newloc(int dir, double& movex, double& movey){return;}
   
    virtual int remembergoodie(){return 0;}
    virtual void takedamage(int hp);
    virtual bool cantshootat() const {return false;}
    virtual bool stealable() const {return false;}
    virtual bool spawnable() const {return false;}
    virtual bool consumable() const {return false;}
    virtual bool allcrystals(){return false;}
    virtual bool blockMarble() const {return true;}
    virtual bool ispushable() const {return false;}
    virtual bool blocksPeas() const {return false;}
    virtual bool isDamagable() const {return false;}
    virtual bool pushmarble(int dir){return false;}
    virtual bool canpush(int dir){return false;}
    virtual void doSomething() = 0;
    virtual bool blockmovement() = 0;
    StudentWorld* getWorld() const {return m_world;}
    
    void setToRemove() {markedForRemoval = true;}
    bool markedForRemove() {return markedForRemoval;}
  
private:
    int m_health;
    bool invalmarb = false;
    bool markedForRemoval = false;
    StudentWorld* m_world;
};

class LiveActor: public Actor{
public:
    LiveActor(StudentWorld* world, int ID, double x_, double y_, int dir):
    Actor(world, ID, x_, y_, dir){};
       
    bool checkoverlap(int dir);
    virtual bool blocksPeas() const {return false;}
    virtual bool blockmovement(){return true;}
   
    void fullrestore(){setHealth(20);}
    virtual bool isDamagable() const {return true;}
    
    void newloc(int dir, double& movex, double& movey);
    virtual bool canmove(int dir);
    bool move(int dir);
};

class Player: public LiveActor{
    const int startpeas = 20;
public:
    Player(StudentWorld* world, double x_, double y_):
    LiveActor(world, IID_PLAYER, x_,  y_, right){
        setHealth(20);
        peas = startpeas;
    }
    
    bool moveablemarble(double x, double y, int dir);
    virtual bool canmove(int dir);
    void takedamage(int hp);
    virtual void doSomething();

    int getAmmo(){return peas;}
    void addAmmo(int x){peas += x;}
    bool checkoverlap(int dir);

private:
    void shootpeas();
    void validmove(double x, double y);
    int peas;
};

class Wall: public Actor{
public:
    Wall(StudentWorld* world, double x_, double y_):
    Actor(world, IID_WALL,  x_,  y_, none){
        setHealth(1);
        
    }
    virtual bool cantshootat() const {return true;}
    virtual bool blocksPeas() const {return true;}
    virtual bool blockmovement(){return true;}
    virtual void doSomething(){return;}
};

class Marble: public LiveActor{
    public:
    Marble(StudentWorld* world, double x_, double y_):
    LiveActor(world, IID_MARBLE, x_,  y_, none){
        setHealth(10);
    }
    virtual bool cantshootat() const {return true;}
    virtual bool blockmovement(){return true;}
    virtual bool ispushable() const {return true;}
    virtual void doSomething(){return;}
    virtual bool pushmarble(int dir);
    virtual bool canpush(int dir);
    
private:
    bool validmar;
    };



class Pits: public Actor{
public:
    Pits(StudentWorld* world, double x_, double y_):
    Actor(world, IID_PIT, x_ , y_ , none){
        setHealth(1);
    }
    virtual bool blockMarble() const {return false;}
    virtual bool blockmovement(){return true;}
    virtual void doSomething();
    void action();
    
private:
    
};

class Pea: public Actor{
public:
    Pea(StudentWorld* world, double x_, double y_ , int dir):
    Actor(world, IID_PEA, x_, y_, dir){
        startx = x_;
        starty = y_;
        setDirection(dir);
        setHealth(1);
        justcreated = false; 
    };
    virtual bool blockmovement(){return false;}
    virtual void doSomething();
   bool obstacleonspawn();
 
private:
    int startingx(){return startx;}
    int startingy(){return starty;}
    int startx;
    int starty;
    bool justcreated;
};


class Interact: public Actor{
public:
    Interact(StudentWorld* world, int ID, double x_, double y_, int dir):
    Actor(world, ID, x_, y_, dir){
        setHealth(1);
    };
    
    virtual bool consumable() const {return true;}
    virtual bool blockmovement(){return false;}
};

class InteractGoodie: public Interact{
public:
    InteractGoodie(StudentWorld* world, int ID, double x_, double y_, int dir):
    Interact(world, ID, x_, y_, none){};
    virtual void doSomething();
    virtual bool stealable() const {return true;}
    void action();
    
    virtual void applyGoodie() = 0;
 

};

class ExtraLifeGoodie: public InteractGoodie{
public:
    ExtraLifeGoodie(StudentWorld* world, double x_, double y_):
    InteractGoodie(world, IID_EXTRA_LIFE,  x_, y_, none){};
    
    
private:
    virtual void applyGoodie();
    virtual int remembergoodie(){return 1;}
    
};

class HealthGoodie: public InteractGoodie{
public:
    HealthGoodie(StudentWorld* world, double x_, double y_):
    InteractGoodie(world, IID_RESTORE_HEALTH, x_, y_, none){};
    
private:
    virtual void applyGoodie();
    virtual int remembergoodie(){return 2;}
    
};

class AmmoGoodie: public InteractGoodie{
public:
    AmmoGoodie(StudentWorld* world, double x_, double y_):
    InteractGoodie(world, IID_AMMO, x_, y_, none){};
    
private:
    virtual void applyGoodie();
    virtual int remembergoodie(){return 3;}
    
};

class Crystal: public InteractGoodie{         //pretty much the smae as a goodie??
public:
    Crystal(StudentWorld* world, double x_, double y_):
    InteractGoodie(world, IID_CRYSTAL, x_, y_, none){};
  
    virtual bool allcrystals(){return true;}
    virtual bool stealable() const {return false;}
    
private:
    virtual void applyGoodie();
};


class Exit: public Interact{
public:
    Exit(StudentWorld* world, double x_, double y_):
    Interact(world, IID_EXIT, x_, y_, none){
       setVisible(false);
        hasRevealedExit = false; 
    }
    virtual void doSomething();
    
private:
    bool hasRevealedExit;
};


class Robot: public LiveActor{
public:
    Robot(StudentWorld* world, int ID, double x_, double y_, int dir);
    virtual void takedamage(int hp);
    virtual void doSomething();
    int reversedirection(int dir);
    bool robotmove(int dir);
    bool shouldShoot();
    void robotpeas();
    virtual bool cantshootat() const {return true;}
    bool noObstaclesInPath(int playerX, int playerY);
    

    virtual void action() = 0;
    virtual void deatheffect() = 0;
    
    void gettickstillaction();
    
private:
    int tickstillact;
};


class ragebot: public Robot{
public:
    ragebot(StudentWorld* world, double x_, double y_, int dir):
    Robot(world, IID_RAGEBOT, x_, y_, dir){}

    virtual void action();

    virtual void deatheffect();

};


class thiefbotbase: public Robot{
public:
    thiefbotbase(StudentWorld* world, int ID, double x_, double y_, int dir):Robot(world, ID, x_, y_, dir){
        distanceBeforeTurning = randInt(1, 6);
        goodiestolen = -1;
        }
    virtual void action();
    virtual bool spawnable() const {return true;}
    virtual void deatheffect();
    void randommove();
    virtual void updates() = 0;
    virtual void increasescore() = 0;
    
    void setgoodiestolen(int x){
        goodiestolen = x;
    }
    int getgoodiestolen(){
        return goodiestolen;
    }
    
private:
    int distanceBeforeTurning;
    int goodiestolen;
};

class thiefbot: public thiefbotbase{
public:
    thiefbot(StudentWorld* world, double x_, double y_):
    thiefbotbase(world, IID_THIEFBOT, x_, y_, right){
        setHealth(5);
    }
    virtual void updates();
    virtual void increasescore();

};

class meanthiefbot: public thiefbotbase{
public:
    meanthiefbot(StudentWorld* world, double x_, double y_):
    thiefbotbase(world, IID_THIEFBOT, x_, y_, right){
        setHealth(8);
    }
    virtual void updates();
    virtual void increasescore();

};

class factory: public Actor{
public:
    factory(StudentWorld* world, int ID, double x_, double y_, int dir):
    Actor(world, ID, x_, y_, none){}

    virtual bool blocksPeas() const {return true;}
    virtual bool cantshootat() const {return true;}
    virtual bool blockmovement(){return true;}
    virtual void doSomething();
    virtual void create() = 0;
    
};

class meanfactory: public factory{
public:
    meanfactory(StudentWorld* world, double x_, double y_):
    factory(world, IID_ROBOT_FACTORY, x_, y_, none){
        setHealth(1);
    }
    virtual void create();

};

class regfactory: public factory{
public:
    regfactory(StudentWorld* world, double x_, double y_):
    factory(world, IID_ROBOT_FACTORY, x_, y_, none){
        setHealth(1);
    }
    virtual void create();
};



//create subclass interactable that has its own subclasses that you can interactwith: goodies(ammo, restore health, extra life) , crystal, exit


#endif // ACTOR_H_
