//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// base class for every creature
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////


#ifndef __creature_h
#define __creature_h


#include "thing.h"
#include "position.h"

#include "magic.h"

enum fight_t {
	FIGHT_MELEE,
	FIGHT_DIST,
	FIGHT_MAGICDIST
};
// Macros
#define CREATURE_SET_OUTFIT(c, type, head, body, legs, feet) c->looktype = type; \
c->lookhead = head; \
c->lookbody = body; \
c->looklegs = legs; \
c->lookfeet = feet;

enum playerLooks
{
	PLAYER_MALE_1=0x80,
	PLAYER_MALE_2=0x81,
	PLAYER_MALE_3=0x82,
	PLAYER_MALE_4=0x83,
	PLAYER_FEMALE_1=0x88,
	PLAYER_FEMALE_2=0x89,
	PLAYER_FEMALE_3=0x8A,
	PLAYER_FEMALE_4=0x8B,
};


class Map;

class Item;

class Thing;
class Player;

//////////////////////////////////////////////////////////////////////
// Defines the Base class for all creatures and base functions which 
// every creature has


class Creature : public Thing
{
public:
  Creature(const char *name);
  virtual ~Creature() {};

  virtual const std::string& getName() const {return name; };

  unsigned long getID() const { return id; }
  unsigned long getExpForLv(const int& lv) const { 
		return (int)((50*lv*lv*lv)/3 - 100 * lv * lv + (850*lv) / 3 - 200);
    //return (int)(((50.0/3.0) * pow((double)lv ,3)) - (100 * pow((double)lv, 2)) + ((850.0/3.0) * lv) - 200);
	}
  Direction getDirection() const { return direction; }
  void setDirection(Direction dir) { direction = dir; }

  virtual fight_t getFightType(){return FIGHT_MELEE;};

  virtual void drainHealth(int);
  virtual void drainMana(int);
  virtual void die(){};
  virtual std::string getDescription(bool self = false);
  virtual void setAttackedCreature(unsigned long id);

  virtual int getWeaponDamage() const {
	return 1+(int)(10.0*rand()/(RAND_MAX+1.0));
  }

  unsigned long attackedCreature;

  virtual bool isAttackable() { return true; };

  virtual int sendInventory(){return 0;};
  virtual int addItem(Item* item, int pos){return 0;};
  virtual Item* getItem(int pos){return NULL;}
  virtual Direction getDirection(){return direction;}
	void addMagicDamage(const MagicDamageContainer& dmgContainer, bool skipfirst = true);
	MagicDamageVec* getMagicDamageVec(MagicDamageType md);

  int lookhead, lookbody, looklegs, lookfeet, looktype, lookcorpse, lookmaster;
  int mana, manamax, manaspent;
  bool pzLocked;
  //bool poisoned, burning, energized, drunk, paralyzed;
  long inFightTicks, exhaustedTicks, manaShieldTicks, hasteTicks, paralyzeTicks;
	long burningTicks, energizedTicks, poisonedTicks;
	long curburningTicks, curenergizedTicks, curpoisonedTicks;
  unsigned long experience;
  Position masterPos;

  int health, healthmax;

  uint64_t lastmove;

  int lastDamage;

  unsigned short getSpeed() const {            
           return speed; 
           };

  virtual int getStepDuration(int underground) { return (1000*120*100)/(getSpeed()*underground); };
  void setNormalSpeed()
  {
  if(access!=0){
           speed = 900;     
           return;    
           } 
  speed = 220 + (2* (level - 1)); 
  }
  int getNormalSpeed()
  {
      if(access!=0){     
           return 900;    
           }
  return 220 + (2* (level - 1)); 
  }
  int access; //access level
  // magic level
  int maglevel;
  // level
  int level;
  int speed;
  virtual bool canMovedTo(Tile *tile);

  virtual void sendCancel(const char *msg) { };
  virtual void sendCancelWalk(const char *msg) { };

private:
	virtual void onThink(){};
  virtual void onThingMove(const Creature *player, const Thing *thing, const Position *oldPos, unsigned char oldstackpos) { };
  virtual void onCreatureAppear(const Creature *creature) { };
  virtual void onCreatureDisappear(const Creature *creature, unsigned char stackPos) { };
  virtual void onCreatureTurn(const Creature *creature, unsigned char stackPos) { };
  virtual void onCreatureSay(const Creature *creature, unsigned char type, const std::string &text) { };

  virtual void onCreatureChangeOutfit(const Creature* creature) { };
	virtual void onTileUpdated(const Position *Pos) { };
	virtual void onContainerUpdated(Item *item, unsigned char from_id, unsigned char to_id, unsigned char from_slot, unsigned char to_slot, bool remove) {};

	std::map<MagicDamageType, MagicDamageVec> MagicDamageMap;

  friend class Game;
  friend class Map;
	friend class MapState;

  Direction direction;

  unsigned long id;
  std::string        name;
};


#endif // __creature_h
