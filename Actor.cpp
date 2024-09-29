#include "Actor.h"
#include "StudentWorld.h"

void Actor::takedamage(int hp){
    m_health -= hp;
}
//Live actor movements
void LiveActor::newloc(int dir, double& movex, double& movey){      //get coordinates of the new square the actor wants to move in
    double x = getX();
    double y = getY();
    switch (dir){
        case up:
            y+=1;
            break;
        case down:
            y-=1;
            break;
        case left:
            x-=1;
            break;
        case right:
            x+=1;
        default:
            break;
    }
    movex = x;
    movey = y;

}
bool LiveActor::canmove(int dir){                   //check if that square is a valid spot
        double x;
        double y;
        newloc(dir, x, y);
        return getWorld()->isValidPos(x, y, this);
}

bool LiveActor::move(int dir){                  //move to new square if valid
        double x;
        double y;
        newloc(dir, x, y);
        moveTo(x, y);
        return true;

}

//Player commands start
bool Player::canmove(int dir){      //player movement is different, as it can push marbles, other live actors cant, so it needs a different check
        double x;
        double y;
        newloc(dir, x, y);          //click key, go into this fucntion. get new location
    
                                    //we are checking the location where the player wants to move
    if (getWorld()->isValidPos(x, y, this) || moveablemarble(x, y, dir)){        //if its valid spot or if its valid marble
        return true;
    }
    return false;
}

bool Player::moveablemarble(double x, double y, int dir){
    if(getWorld()->canpushobject(x, y, this)){                      //is this object pushable?
        Actor* b = getWorld()->getOverlappingDamageableObject(x, y, this);
        if(b->canpush(dir)){
            b->pushmarble(dir);
            return true;
        }
    }
    return false;
}


void Player::doSomething(){
    
    int ch;
     if (getWorld()->getKey(ch))            //movement commands, have position checks before player actually moves
     {
         // user hit a key this tick!
         switch (ch)
         {
             case KEY_PRESS_TAB:
             case KEY_PRESS_ENTER:
                 setDirection(right);
                 break;
             case KEY_PRESS_ESCAPE:
                 setHealth(0);
                 getWorld()->playSound(SOUND_PLAYER_DIE);
                 break;
             case KEY_PRESS_LEFT:
                 setDirection(left);
                 if(canmove(left)){
                     move(left);
                 }
                 break;
             case KEY_PRESS_RIGHT:
                 setDirection(right);
                 if(canmove(right)){
                     move(right);
                 }
                 break;
             case KEY_PRESS_UP:
                 setDirection(up);
                 if(canmove(up)){
                     move(up);
                 }
                 break;
             case KEY_PRESS_DOWN:
                 setDirection(down);
                 if(canmove(down)){
                     move(down);
                 }
                 break;
             case KEY_PRESS_SPACE:
                 shootpeas();
                 break;
             default:
                 break;
         }
     }
}


void Player::shootpeas(){
    if (peas > 0) {

        
        int dir = getDirection();
        double px = getX(), py = getY();
        switch (dir) {                              //add pea in the direction player is facing, 1 square in front
            case up:
                getWorld()->addpea(px, py+1 , up);
                break;
            case down:
                getWorld()->addpea(px, py-1, down);
                break;
            case left:
                getWorld()->addpea(px-1, py, left);
                break;
            case right:
                getWorld()->addpea(px+1, py, right);
                break;
            default:
                cerr << "INVALID PLAYER DIRECTION" << endl;
        }
        peas--;
        getWorld()->playSound(SOUND_PLAYER_FIRE);
    }
}
// Pea start
void Pea::doSomething(){
    if(!islive()){
        return;
    }

    int dir = getDirection();                       //find direction it wants to move in, and move to that spot
    if(justcreated){                                //flag to prevent pea from moving on same tick its created
        switch (dir) {
            case up:
                moveTo(getX(), getY()+1);
                break;
            case down:
                moveTo(getX(), getY()-1);
                break;
            case left:
                moveTo(getX()-1, getY());
                break;
            case right:
                moveTo(getX()+1, getY());
                break;
            default:
                cerr << "Invalid direction";
                break;
        }
    }
    if(getWorld()->overlappingDamageable(this) == 1 || getWorld()->overlappingDamageable(this) == 3){        //check if obejects on new current square it just moved to
       Actor* a = getWorld()->getOverlappingDamageableObject(getX(), getY(), this);                            //same algorithm as above
       a->takedamage(2);
        setHealth(0);
       return;
   }
     else if(getWorld()->overlappingDamageable(this) == 2){             //if the object overlapping cannot be damaged, pea does nothing
         setHealth(0);
         return;
      }
    justcreated = true;
}


void Player::takedamage(int hp){
    Actor::takedamage(hp);
    if(islive()){                                   //play impact when damaged if alive if you die from it, play sound player die
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
    }
    else{
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }

}
// Marble Start

bool Marble::canpush(int dir){                          //as function name states
    double newX, newY;
    newloc(dir, newX, newY);
    return getWorld()->isvalidmarble(newX, newY, this);
}
bool Marble::pushmarble(int dir){                           //find new location of marble, check if its a valid spot, return true
        double newX, newY;
        newloc(dir, newX, newY);
        if (getWorld()->isvalidmarble(newX, newY, this)){
                moveTo(newX, newY);
                return true;
        }
    return false;
}

//pit
void Pits::doSomething(){
    if(!islive()){
        return;
    }
    
    if(getWorld()->overlappingDamageable(this) == 1){                 //can use same fucntion that the pea uses as only a marble (damageable), can be pushed ontop
       Actor* b = getWorld()->getOverlappingDamageableObject(getX(), getY(), this);   //find the marble overlapping with it, and delete both
        setToRemove();
        b->setToRemove();
        return;
    }
}

//robots


Robot::Robot(StudentWorld* world, int ID, double x_, double y_, int dir)
    :LiveActor(world, ID, x_, y_, dir) {
    setHealth(10);
    gettickstillaction();
}
//ragebots
void Robot::gettickstillaction() {
    int ticks = (28 - getWorld()->getLevel()) / 4;      //get ticks until they act according to level
    if (ticks < 3) {
        tickstillact = 3;
    } else {
        tickstillact = ticks-1;
    }
}

void Robot::doSomething() {
    if (!islive()) {
        return;
    }
    if (tickstillact == 0) {            //move/turn when ticks = 0
        action();
        gettickstillaction();
    } else {
        tickstillact--;
    }
}

void ragebot::action(){
    bool movedOrShot = false;           //flag to determine if robot shoots or moves
    int dir = getDirection();
    if (shouldShoot()) {                //if it can shoot, shoot
        robotpeas();
        movedOrShot = true;
        return;
    }
    else {
        if(canmove(dir)){                   //if next spot is valid
            movedOrShot = move(dir);
        }
    }
    if (!movedOrShot) {                             //if it either hasnt moved nor shot, change direction
        setDirection(reversedirection(dir));
    }
}

void ragebot::deatheffect(){            //increase score by 100
    getWorld()->increaseScore(100);
}

//general robot commands
bool Robot::shouldShoot() {
    Actor* player = getWorld()->playerinsight();
    if (player != nullptr) {
        int x = player->getX();
        int y = player->getY();
        int requiredDir = none;         //set directions according to where the player is in front of robots direction
        if (getX() == x) {
            if (getY() < y) {
                requiredDir = up;
            }
            else {
                requiredDir = down;
            }
        }
        else if (getY() == y) {
            if (getX() < x) {
                requiredDir = right;
            }
            else {
                requiredDir = left;
            }
        }
        if(getDirection() == requiredDir && !getWorld()->isObstacle(getX(), getY(), requiredDir)){      //no obstacle and player is in front of robot direction
            return true;
        }
    }
    return false;
}

int Robot::reversedirection(int dir){       //reversing robots direction when it hits an obstacle
    switch(dir){
        case up:
            return down;
        case down:
            return up;
        case left:
            return right;
        case right:
            return left;
        default:
            return none;
    }
}
void Robot::robotpeas(){

    getWorld()->playSound(SOUND_ENEMY_FIRE);
    
    int dir = getDirection();
    double px = getX(), py = getY();
    switch (dir) {                              //add pea in the direction player is facing, 1 square in front
        case up:
            getWorld()->addpea(px, py+1 , dir);
            break;
        case down:
            getWorld()->addpea(px, py-1, dir);
            break;
        case left:
            getWorld()->addpea(px-1, py, dir);
            break;
        case right:
            getWorld()->addpea(px+1, py, dir);
            break;
        default:
            break;
    }
}


void Robot::takedamage(int hp){                     //robot taking damage
    Actor::takedamage(hp);
    if(islive()){
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
    else{
        getWorld()->playSound(SOUND_ROBOT_DIE);
        deatheffect();
    }
    
    
}
//thief bots
void thiefbotbase::action(){
    if(!islive()){
        return;
    }
    bool stealoraction = false;
    bool pickedupgoodie = false;
    if(getWorld()->stealgoodie(this) && !pickedupgoodie){                                   //if overlap with a goodie
        Actor* a = getWorld()->thiefgoodie(this);
        if(randInt(1, 10) == 1){                            //1/10 chacne to steal
            a->setToRemove();
            getWorld()->playSound(SOUND_ROBOT_MUNCH);
            stealoraction = true;
            pickedupgoodie = true;
            setgoodiestolen(a->remembergoodie());           //remmeber the goodie it stole
            return;
        }
    }
    if(!stealoraction){                             //if didn't steal, move
        updates();
    }
}
void thiefbotbase::randommove(){
    int dir = getDirection();
    if(canmove(dir) && distanceBeforeTurning > 0){              //if can move in direction and distancebeforeturning is not 0 yet
        move(dir);
        distanceBeforeTurning--;
    }
    else if(!canmove(dir) || distanceBeforeTurning <= 0){           //if obstacle or distancebeforeturning is all used
        distanceBeforeTurning = randInt(1, 6);                      //reset distance
        int newDir = randInt(0, 3) * 90;                            //get new direction
        for(int i =0; i < 4; i++){                                   //loop through each direction and see if u can move
            if(canmove(newDir)){
                setDirection(newDir);
                move(newDir);
                return;
            }
            newDir = (newDir + 90) % 360;                   //try different direction not considered during the loop
        }
        setDirection(newDir);                               //if cant move in any direction in loop , just set direction
    }
}
void thiefbot::updates(){               //regular thiefbots jsut move
    randommove();
}

void meanthiefbot::updates(){           //mean thiefbots can either shoot or move
    bool movedOrShot = false;
    if (shouldShoot()) {
        robotpeas();
        movedOrShot = true;
        return;                     //shoot and do nothing else for the tick
    }
    if(!movedOrShot){               //if didnt shoot, move instead
        randommove();
    }
}
void thiefbotbase::deatheffect(){               //create goodie if stolen based on what it remembered
    switch(goodiestolen){
        case -1:
            break;
        case 1:
            getWorld()->addexlife(getX(), getY());
            break;
        case 2:
            getWorld()->addreshp(getX(), getY());
            break;
        case 3:
            getWorld()->addammo(getX(), getY());
    }
    increasescore();
}

void thiefbot::increasescore(){
    getWorld()->increaseScore(10);
}

void meanthiefbot::increasescore(){
    getWorld()->increaseScore(20);
}
//thiefbot factory
void factory::doSomething(){
    int x = getX();
    int y = getY();
    
    if(getWorld()->countthiefbots(x-3, x+3, y-3, y+3) < 3 && getWorld()->overlappingDamageable(this) !=3){     //get bounds that are 3 up,down,left,right from factory
        if (randInt(1, 50) == 1) {                              //1/50 chance to create bot
            create();
        }
    }
}
void meanfactory::create(){
    getWorld()->addmeanbot(getX(), getY());           //create mean thiefbot
    getWorld()->playSound(SOUND_ROBOT_BORN);
}

void regfactory::create(){                              //create thiefbot
    getWorld()->addtbot(getX(), getY());
    getWorld()->playSound(SOUND_ROBOT_BORN);
}
//goodies, crystal, exit
void InteractGoodie::action(){      //all goodies and crystal share same mechanics, so can group their do somethings
    if(getWorld()->overlappingPlayer(this)){
        if(consumable()){
            getWorld()->playSound(SOUND_GOT_GOODIE);
            setToRemove();
            applyGoodie();
            return;
        }
    }
}
void InteractGoodie::doSomething(){
    if(!islive()){
        return;
    }
    action();
    

}
//applying goodie effects
void ExtraLifeGoodie::applyGoodie(){
    getWorld()->increaseScore(1000);
    getWorld()->incLives();
}

void HealthGoodie::applyGoodie(){
    getWorld()->applyHp();
}

void AmmoGoodie::applyGoodie(){
    getWorld()->applyammo();
}

void Crystal::applyGoodie(){
    getWorld()->increaseScore(50);
    
}

void Exit::doSomething(){       //0 crystals left, reveal exit
    if (getWorld()->numcrystals() == 0 && !hasRevealedExit) {
           getWorld()->playSound(SOUND_REVEAL_EXIT);
           setVisible(true);
           hasRevealedExit = true;
       }
    else if (getWorld()->numcrystals() == 0){
        if (getWorld()->overlappingPlayer(this)) {
            getWorld()->increaseScore(2000);
            getWorld()->finishedlevel();
        }
    }

}
//had to implement two different if statements because sound reveal exit would keep playing every tick
//if numcrystals function kept returning 0
