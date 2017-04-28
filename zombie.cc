#include "clazzes.h"
#include <ncurses.h>
#include <queue>
#include <unistd.h>

using namespace std;

const unsigned char OPEN = '.';
const unsigned char WALL = '#';
const unsigned char ZOMB = 'Z';
const unsigned char PLAYER = 'P';
const unsigned char TP_LEFT = '<';
const unsigned char TP_RIGHT = '>';
const unsigned char TREAS = '@'; 

const int UP = 65;
const int DOWN = 66;
const int LEFT = 67;
const int RIGHT = 68;

const unsigned int TIME = 300;
const int WIN_GOLD = 100; 
const int ZOMB_LOOT = 20;
const int DMG_DIST = 2;
const int CLOSE_DIST = 5;   //this is where you adjust constants to tweak gameplay
const int GUN_RANGE = 3;
const int RESPWN_TIME = 15;
const int RELOAD_TIME = 4;
const int ZOMB_REFRESH = 5;
const int ZOMB_DMG = 5;
const int MAX_ZOMBIES = 3; //not used, probably should be

vector<string> map = {                 //the map
	"####################", //0
	"#....#...##........#",
	"#.#..###.##.###.##.>",
	"#.##...#.##.###.##.#",
	"#........##.....##.#",
	"#.###..............#", //5
	"#.####.######.####.#",
	"#.#............##..#",
	"#...##.##..##..##..#",
	"#.#.#..###.##.##.#.#",
	"#..............#.###", //10
	"####..########...###",
	"#.................##",
	"#.........###..##..#",
	"#.##..###.###......#", //14
	"#.##.####.##..#....#",
	"#.#...........#.##.#",
	"<.#....####..##.##.#",
	"#......##....##....#",
	"####################", //19
};

/*		0 1 2 3 4 5 6 7 8 9 10111213141516171819
 *0 	# # # # # # # # # # # # # # # # # # # #
 *1 	#         #       # #                 #
 *2 	#   #     # # #   # #   # # #   # #   >
 *3 	#   # #       #   # #   # # #   # #   #
 *4 	#                 # #           # #   #
 *5 	#   # # #                             #
 *6 	#   # # # #   # # # # # #   # # # #   #
 *7 	#   #                         # #     #
 *8 	#       # #   # #     # #     # #     #
 *9 	#   #   #     # # #   # #   # #   #   #
 *10	#                             #   # # #
 *11	# # # #     # # # # # # # #       # # #
 *12	#                                   # #
 *13	#                   # # #     # #     #
 *14	#   # #     # # #   # # #             #
 *15	#   # #   # # # #   # #     #         #
 *16	#   #                       #   # #   #
 *17	<   #         # # # #     # #   # #   #
 *18	#             # #         # #         #
 *19	# # # # # # # # # # # # # # # # # # # #
 */

bool check(spot &place) {
	if (map.at(place.get_y()).at(place.get_x()) != OPEN) return false;
	return true;
}

bool check(spot &entity, const int dir, int &lastmove) { //this function prevents walking through walls
	if (dir == UP) {
		if (map.at(entity.get_y() - 1).at(entity.get_x()) == OPEN)
			return true;
		else return false;
	} else if (dir == DOWN) {
		if (map.at(entity.get_y() + 1).at(entity.get_x()) == OPEN)
			return true;
		else return false;
	} else if (dir == LEFT) {
		if (map.at(entity.get_y()).at(entity.get_x() + 1) == OPEN)
			return true;
		if (map.at(entity.get_y()).at(entity.get_x() + 1) == TP_LEFT) { //for some reason this doesnt work. You should fix it
			entity.set_x(18);                                           //its supposed to teleport you when you walk into a '<' or '>'
			entity.set_y(2);
			lastmove = 0;
		}
		return false;
	} else if (dir == RIGHT) {
		if (map.at(entity.get_y()).at(entity.get_x() - 1) == OPEN)
			return true;
		if (map.at(entity.get_y()).at(entity.get_x() - 1) == TP_RIGHT) { //ditto
			entity.set_x(1);
			entity.set_y(17);
			lastmove = 0;
		}
		return false;
	}
	return false;
	//if it makes it here with out finding a direction false

}

void Die(/*spot no, int nope*/) { //just in case (variables to be used later)
	endwin();              //the useless parameters allow it to be pushed into the heap
	system("clear");
	cout << "Bad function" << endl;
	exit(0);
}

void respawn(spot location, int zomb_type) {
	;//respawn a zombie, idk
}

void dig(player &p1, const spot treas) { //note: player passed by ref
	int dist = sqrt( (p1.get_x() - treas.get_x())*(p1.get_x() - treas.get_x()) + (p1.get_y() - treas.get_y())*(p1.get_y() - treas.get_y()) ); //Good ol' Pythagorean distance formula
	if (dist == 0) p1.add_g(WIN_GOLD);
	else if (dist > CLOSE_DIST) {
		;//Tell player they are close, somehow
	}                                             //this is for when the treasure is NOT displayed on the map. 
	                                              //right now I have it set to display the treasure so that you can make sure the treasure isnt spawning in walls or something
	else {
		;//Tell player they are not close
	}
}

void player_attack(player p1, const int dir, vector<zombie> THEBOYZ) {
	if (!p1.can_shoot()) return;
	int range = 1;
	if (p1.gun()) {
		range = GUN_RANGE;
		p1.shot(default_spot, dir);
	}//checks which weapon the player has and which direction theyre attacking in
	//then checks if there is a zombie there

	if (dir == UP) {
		for (int i = 1; i <= range; i++) {
			if (map.at(p1.get_y() -i).at(p1.get_x()) == ZOMB) {//find a zombie
				for (zombie BOI : THEBOYZ) {
					if ((BOI.get_x() == p1.get_x()) && (BOI.get_y() == p1.get_y() -i)) //find out WHICH zombie we found by comparing it to the map, brute force style
						BOI.die(); //set zombie flag to 'dead'. Main will pick this up later and actually erase the zombie
				}
			}       
		}
	} 
	else if (dir == DOWN) {
		for (int i = 1; i <= range; i++) {
			if (map.at(p1.get_y() +i).at(p1.get_x()) == ZOMB) {
				for (zombie BOI : THEBOYZ) {
					if ((BOI.get_x() == p1.get_x()) && (BOI.get_y() == p1.get_y() +i))
						BOI.die();
				}
			} //quintuple nesting ftw!!1!
		}

	}
	else if (dir == LEFT) {
		for (int i = 1; i <= range; i++) {
			if (map.at(p1.get_y()).at(p1.get_x() +i) == ZOMB) {
				for (zombie BOI : THEBOYZ) {
					if ((BOI.get_x() == p1.get_x() +i) && (BOI.get_y() == p1.get_y()))
						BOI.die();
				}
			}
		}

	}
	else if (dir == RIGHT) {
		for (int i = 1; i <= range; i++) {
			if (map.at(p1.get_y()).at(p1.get_x() -i) == ZOMB) {
				for (zombie BOI : THEBOYZ) {
					if ((BOI.get_x() == p1.get_x() -i) && (BOI.get_y() == p1.get_y()))
						BOI.die();
				}
			}
		}

	}
	return;
}

int dothething(zombie Zelda) { //zombie control center, not a member function or heap-supported anymore
if (Zelda.isasleep()) return 0;
	//type 0 is for zombies that dont track player
if (Zelda.get_type() == 0) return rand() % 4 + 4;// returns 4, 5, 6, or 7, which correspond to directions the zombie is aimlessly moving in
else if (Zelda.get_dist() < DMG_DIST) return 1; //1 is for attacking player
else if (Zelda.get_dist() < CLOSE_DIST) return 2; //2 is for close pursuit, unused
else return 3; //3 is for long distance pathfinding, unused
}

void zombie_attack(player &targ) { //zombie damages player
	targ.hurt(ZOMB_DMG);		//make sure to check that zombie is very close to player before you call this
}

void print(const player p1, const vector<zombie> zombs, const spot treasure) {
	map.at(p1.get_y()).at(p1.get_x()) = PLAYER;
	map.at(treasure.get_y()).at(treasure.get_x()) = TREAS;
	for (zombie Zed : zombs) {                        //put the players and zombies on the map
		if (!Zed.isasleep()) map.at(Zed.get_y()).at(Zed.get_x()) = ZOMB; //only prints zombies that have officially spawned
	}
	for (string s : map) { //for every string that makes up the map
		for (char c : s) { //and for every character in that string
			int colour = 1;
			if (c == WALL) colour = 2;
			else if (c == PLAYER) colour = 5;
			else if (c == ZOMB) colour = 3;
			else if (c == '|' || c == '-' || c == TREAS) colour = 6; //apply colors to characters, as defined in main
			else if (c != OPEN) colour = 4;               //same as dino_panic
			attron(COLOR_PAIR(colour));
			addch(c);
			addch(' ');
			attroff(COLOR_PAIR(colour));
			//equal to (cout << c << " ";), but allows color
		}
		addch('\n');
		//equal to (cout << endl;), but works with curses
	}
}


/*class action { //the hardest class to compile ever
  public:
	int time = 1;
	//void (*funcptr)(spot x, int y); //this member variable is pointer to a function
	action() {}                           //if that sounds annoying, its actually 10x more annoying than you think
	action(int timev, void ptr(spot xx, int yy)) { 
		time = timev;
		//funcptr = ptr;
	}
	//add constructor or modify current constructor that

	

	bool operator< (const action& y) const {
		if (time < y.time) return true;
		return false;
	}                                           //these operators let us use "priority_queue" heap
	bool operator== (const action rhs) {
		if (time == rhs.time) return true;
		return false;
	}
};*/
class action_re {//rewriting the action class?
	private:
		//switching funcptr to vector ints (posibly break it down to a sport and an int)
		int spot_x;
		int spot_y;
		int call_function;
		int zed_type;
	public:
		int time = 1;
		action_re(){;}
		//use int to tell which function to run when at the top of the priority queue rather than trying to pass the function itself
		action_re(int timev, vector<int> spot, int function_num) {
			time = timev;
			spot_x = spot.at(0);
			spot_y = spot.at(1);
			call_function = function_num;
		}
		action_re(int timev, int function_num){
			time = timev;
			call_function = function_num;
		}
		action_re(int timev, int function_num, int new_zed_type) {
			zed_type = new_zed_type;
			time = timev;
			call_function = function_num;
		}
		bool operator< (const action_re& y) const {
			if (time < y.time) return true;
			return false;
		}
		bool operator== (const action_re rhs) {
			if (time == rhs.time) return true;
			return false;
		}
		int get_time() {return time;}
		int get_function() {return call_function;}
};

int main() {

	initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_BLACK, COLOR_YELLOW);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK); //initialize curses
	clear();
	noecho();
	cbreak();
	timeout(TIME);

	priority_queue<action_re> event_mgr;
	//priority_queue<proxy_action> event_mgr;

	player p1(9, 12);              //initialize the game 
	vector<zombie> zombs;
	zombie z1(4, 13), z2(16, 15), z3(11, 5);
	zombs.push_back(z1);
	zombs.push_back(z2);
	zombs.push_back(z3);

	srand(time(NULL));
	bool victory = false;

	int treasure_x = rand() % 19 + 1; //x and y must be within the map (1-19)
	int treasure_y = rand() % 19 + 1;
	spot treasure(treasure_x, treasure_y);
	while (!check(treasure)) { //keep trying until the treasure is in an open tile
		treasure.set_x(rand() % 19 + 1);
		treasure.set_y(rand() % 19 + 1);
	}
	spot lastmove(0,0); //keep track of players last move. For pathfinding, so not used
	//initialized to 0,0
	int direction = 0; //to tell which direction the player moved

	
	for (zombie z : zombs) z.awaken();
	int frame = 0;
	while (true) {
		clear();
		print(p1, zombs, treasure);
		/*
		cout << "Frame: " << frame << endl;
		cout << "Health: " << p1.get_h() << endl;
		cout << "Gold: " << p1.get_g() << endl;
		cout << "Weapon: ";
		if (p1.gun()) cout << "gun" << endl;
		else cout << "knife" << endl;
		*/
		printw("Arows to Move \nx to shoot\nd to dig\ns to switch weapon\n");
		printw("Frame: %i \nHealth: %i \nGold: %i \nWeapon: ", frame, p1.get_h(), p1.get_g()); //same as the above, in C form (works with curses)
		if (p1.gun()) printw("Gun\n");
		else printw("Knife\n");
		printw("segfault here\n");
		int c = getch();
		printw("not like getch");
		int d;
		if (c == 'Q' || c == 'q') break;
		else if (c == ERR) ;//do nothing
		else if (c == UP) {
			if (check(p1, UP, direction)) {
				map.at(p1.get_y()).at(p1.get_x()) = OPEN;
				p1.change_y(-1);
				direction = UP;
			}                                                       //movement
		} else if (c == DOWN) {
			if (check(p1, DOWN, direction)) {
				map.at(p1.get_y()).at(p1.get_x()) = OPEN; 
				p1.change_y(1);
				direction = DOWN;
			}
		} else if (c == LEFT) {
			if (check(p1, LEFT, direction)) {
				map.at(p1.get_y()).at(p1.get_x()) = OPEN;
				p1.change_x(1);
				direction = LEFT;
			}
		} else if (c == RIGHT) {
			if (check(p1, RIGHT, direction)) {
				map.at(p1.get_y()).at(p1.get_x()) = OPEN;
				p1.change_x(-1);
				direction = RIGHT;
			}
		} else if (c == 'x') { //attack
			timeout(100000); //freeze time, so player can choose attack direction
			int q = 0;	//bad practice throughout this code: variable has no purpose, just fills in function parameter
			if (check(p1, UP, q)) map.at(p1.get_y() - 1).at(p1.get_x()) = '|';
			if (check(p1, DOWN, q)) map.at(p1.get_y() + 1).at(p1.get_x()) = '|'; //display which directions the player can attack in
			if (check(p1, LEFT, q)) map.at(p1.get_y()).at(p1.get_x() + 1) = '-';
			if (check(p1, RIGHT, q)) map.at(p1.get_y()).at(p1.get_x() - 1) = '-';
			clear();
			print(p1, zombs, treasure);  //reprint the map 
			printw("Choose Attack Direction\n");
			d = getch();
			if (d == UP || d == DOWN || d == LEFT || d == RIGHT) {
				player_attack(p1, d, zombs); //shoot from player in direction
				action_re reload(frame +RELOAD_TIME, 2);
				//p1.reload(frame +RELOAD_TIME, d);
				event_mgr.push(reload);
			}
			if (map.at(p1.get_y() - 1).at(p1.get_x()) == '|') map.at(p1.get_y() - 1).at(p1.get_x()) = OPEN;
			if (map.at(p1.get_y() + 1).at(p1.get_x()) == '|') map.at(p1.get_y() + 1).at(p1.get_x()) = OPEN;
			if (map.at(p1.get_y()).at(p1.get_x() + 1) == '-') map.at(p1.get_y()).at(p1.get_x() + 1) = OPEN;
			if (map.at(p1.get_y()).at(p1.get_x() - 1) == '-') map.at(p1.get_y()).at(p1.get_x() - 1) = OPEN;
			timeout(TIME);
		} else if (c == 'd') { //dig
			dig(p1, treasure);
		} else if (c == 's') { //switch weapon
			p1.weapon();
		}
		printw("start win check\n");
		if (p1.get_g() >= WIN_GOLD) { //check victory
			victory = true;
			break;
		}
		for (auto Zed : zombs) { //did player kill a zombie?
			if (Zed.isded()) {
				spot x(Zed.get_x(), Zed.get_y() );
				int zomb_type = Zed.get_type();
				//zombs.erase(Zed);                  //mark zombie as dead
				p1.add_g(ZOMB_LOOT);               //give player gold
				zombie New_Zed(zomb_type, Zed.get_x(), Zed.get_y() ); //create a new zombie, in main because we actually have access to the vector of zombies here
				zombs.push_back(New_Zed);
				action_re spawn(frame + RESPWN_TIME, 3, zomb_type); //the vector access thing is why i added the "asleep" member to zombie
				//event_mgr.push(spawn); //send respawn command to the heap
			}
		}
		if (!event_mgr.empty()) {//check if its not empty to avoid segfault
			while (frame == event_mgr.top().time)  {//check the top of the heap
				action_re funct = event_mgr.top();
				event_mgr.pop();
				if (funct.get_function() == 0) break;
				if (funct.get_function() == 1) {
					//funct.zboe->pathfind();
				}
				else if (funct.get_function() == 2) {
					printw("BANG\n");
					//funct.ihatethis->shot();  //this is how you would handle events if you decide against the function pointer route
				}                             //but honestly the function pointer is almost working, just ask danial or kerney whats wrong with the 4 lines that wont compile
				else if (funct.get_function() == 3) {
					//funct.zboe->awaken();
					for (auto Zed : zombs) {
						if (Zed.isded()) {
							spot x(Zed.get_x(), Zed.get_y());
							int zomb_type = Zed.get_type();
							zombie New_Zed(zomb_type, Zed.get_x(), Zed.get_y());
							zombs.push_back(New_Zed);
							New_Zed.awaken();
						}
					}
				}
			}
		}



		if (frame % ZOMB_REFRESH == 0) { //every 5 frames, run zombie AI
			for (zombie Zander : zombs) {
				Zander.set_dist(sqrt( (p1.get_x() -Zander.get_x())*(p1.get_x() -Zander.get_x()) + (p1.get_y() -Zander.get_y())*(p1.get_y() -Zander.get_y()) )); 
				//how far the zombie is from the player, as the bird flies

				int x = dothething(Zander);
				if (x == 0) continue;
				if (x == 1) {
					zombie_attack(p1);
					continue;
				}
				action_re thing;
				int placeholder;
				if (x == 2) {
					thing.time = frame + ZOMB_REFRESH;
					//thing.funcptr = &Zander.pathfind(p1, 0);
					event_mgr.push(thing);
				}
				else if (x == 3) {
					thing.time = frame + ZOMB_REFRESH;
					//thing.funcptr = &Zander.pathfind(p1, lastmove); //these are for the tracking algorithm that I never wrote
					event_mgr.push(thing);                          //You could get by without writing it; it was going to be rather complicated
				}
				else if (x == 4) {
					if (check(Zander, UP, placeholder)) Zander.change_y(-1);
				}
				else if (x == 5) {
					if (check(Zander, DOWN, placeholder)) Zander.change_y(1); //these 4 are for zombies that move randomly instead of track the player
				}                                                            
				else if (x == 6) {
					if (check(Zander, LEFT, placeholder)) Zander.change_x(1);
				}
				else if (x == 7) {
					if (check(Zander, RIGHT, placeholder)) Zander.change_x(-1);
				}
				else exit(0);
		
			}
		}
		printw("End of loop\n");
		usleep(16667);
		
		if (p1.get_h() <= 0) break;  //check death


		frame++;
	} //while

	endwin();
	system("clear");
	if (victory) cout << "A WINNER IS YOU.\n";
	else cout << "You got nommed.\n";

}//main
