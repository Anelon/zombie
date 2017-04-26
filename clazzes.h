#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <ncurses.h>
#include <cmath>
#include <ctime>
#include <utility>
#include <limits>
#include <set>
#include <unistd.h>

using namespace std;

//We only made this header file to make a compiler error go away

class spot{
	protected:
		int x = 0;
		int y = 0;
	public:
		spot() {}
		spot(int nux, int nuy) {x = nux; y = nuy;}
		int get_y() {return y;}
		int get_x() {return x;}
		int get_x() const {return x;} //It likes to complain that this is redundant, but it didnt work until it was like this, so idgaf
		int get_y() const {return y;}
		void change_x(int nu) {x += nu;}
		void change_y(int nu) {y += nu;}
		void set_x(int xx) {x = xx;}
		void set_y(int yy) {y = yy;}
};
spot default_spot(0,0); //In case this gets used in main, for useless shit


class node : public spot{
	spot parent; //keeping track of parent allows the path algorithm to not go backwards
	public:      //or at least it WOULD if there was a path algorithm
	node(spot mom) {parent = mom;}
};

class zombie : public spot{ //lots of inheritance, scary
	int pathfindtype = 0;
	unsigned int dist;
	bool undead = true;
	bool asleep = true;
	public:
	zombie(int xx, int yy) {x = xx; y = yy;}
	zombie(int type, int xx, int yy) {x = xx; y = yy; pathfindtype = type;}
	int get_type() {return pathfindtype;}
	unsigned int get_dist() {return dist;}
	//void awaken(spot useless, int moreuseless) {asleep = false;} //not sure what this was for =(
	void awaken() {asleep = false;}
	void die() {undead = !undead;}
	bool isded() {return !undead;}
	bool isasleep() { return asleep;}
	void set_dist(int d) {dist = d;}
	
	void pathfind() {;}
	void pathfind(spot target, int lastmove) {//'lastmove' is so that zombies of certain pathfinding types can intelligently move to where youre moving, instead of just where youre at
		if (pathfindtype == 0) return;
		vector<node> closed;    //the legendary pathfinding function
		vector<node> open;     
		//int heur = 0;          //I was going to use the "A star" algorithm. 'Heur' is for heuristic
		                       //check this out: www.policyalmanac.org/games/aStarTutorial.htm
                               //select the link with your mouse and copy with ctrl+shift

	}
	void operator=(zombie rhs) {
		pathfindtype = rhs.get_type(); 
		dist = rhs.get_dist(); 
		undead = !rhs.isded();
		asleep = rhs.isasleep();
	}
};

class player : public spot{
	private:
		bool has_gun = false;
		bool reloaded = true;
		int gold = 0;
		int health = 100;
	public:
		player(int nux, int nuy) {x = nux; y = nuy;}
		int get_h() { return health;}
		int get_g() {return gold;}
		bool gun() {return has_gun;}
		bool can_shoot() {return reloaded;}
		void add_g(int booty) {
			gold += booty;
		}

		void hurt(int dmg) {
			health -= dmg;
		}
		void shot(spot useless, int moreuseless) {reloaded = !reloaded;}
		//"useless" is so that I'm allowed to pass this function to the heap queue in main
		void shot() {reloaded = !reloaded;}
		void reload(int time_reloaded, int direction) {
		}
		void weapon() {
			if (!has_gun) has_gun = true;
			else has_gun = false;
		}
};







