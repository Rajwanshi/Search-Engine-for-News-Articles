#include "GLFW/glfw3.h"
#include "bits/stdc++.h"
#include "DrawUtil.h"
using namespace std;

const double DECAY_RATE = 0.1;
const double INITIAL_LENGTH = 50;
const double MAX_GENERATIONS = 8;
const double ROTATE_ANGLE = 72;
 
#define OPENBRACKET '['
#define CLOSEDBRACKET ']'
#define COLORCHANGE 'C'
#define	TURNLEFT '+'
#define TURNRIGHT '-'
#define DRAW 'F'

DrawUtil* DU;
struct Point{
	double X;
	double Y;
	Point() : X(0), Y(0) {};
	Point(double _x, double _y) : X(_x), Y(_y) {};
};
map<char, string> Rule;
class Tree{
	string Str;
	Point Pos;
	int Generation;
	double Length;
	double Dir;
	int Color;
public:
	Tree(string _str, Point _pos, double _len, double _dir, int _gen, int _col){
		// cout << _str << endl;
		for (auto Char : _str){
			if (Rule.find(Char) == Rule.end()){
				Str.push_back(Char);
			}
			else{
				Str += Rule[Char];
			}
		}
		// cout << Str << endl << endl;
		Pos = _pos;
		Length = _len;
		Dir = _dir;
		Generation = _gen;
		Color = _col;
		cout << Generation << endl;
		// cout << Str << endl;
	}
	void Process(){
		if (Generation > MAX_GENERATIONS) return;
		int i = 0, n = Str.size();
		while (i < n){
			char Ch = Str[i];
			switch(Ch){
				case OPENBRACKET : {
					int OpenBrackets = 1;
					int j = i;
					while (OpenBrackets){
					 	i++;
						if (Str[i] == CLOSEDBRACKET) OpenBrackets--;
						else if (Str[i] == OPENBRACKET) OpenBrackets++;
					}
					if (Generation < MAX_GENERATIONS) {
						Tree child = *(new Tree(Str.substr(j + 1, i-j-1), Pos, Length * (1.0 - DECAY_RATE), Dir, Generation + 1, Color));
						child.Process();
					}
					break;
				}
				case TURNLEFT : {
					Dir += ROTATE_ANGLE;
					if (Dir >= 360) Dir -= 360;
					break;
				}
				case TURNRIGHT : {
					Dir -= ROTATE_ANGLE;
					if (Dir < 0) Dir += 360;
					break;
				}
				case DRAW : {
					Point NewPos = *(new Point(Pos.X + cos((M_PI / 180.0) * Dir) * Length, Pos.Y + sin((M_PI / 180.0) * Dir) * Length));
					// cout << (int)Pos.X << ' ' << (int)Pos.Y << ' ' << (int)NewPos.X << ' ' <<  (int)NewPos.Y << endl;
					DU->drawLine((int)Pos.X, (int)Pos.Y, (int)NewPos.X, (int)NewPos.Y, 0);
					Pos = NewPos;
					break;
				}
				case COLORCHANGE : {
					Color = Str[++i] - '0';
					break;
				}
				default : break;
			}
			i++;
		}
	}
};

int main(){
	freopen("in", "r", stdin);
	string s;
	cin >> s;
	string _tmp; char _ch;
	while (cin >> _ch >> _tmp){
		Rule[_ch] = _tmp;
	}
	DU = new DrawUtil();
	int H = 800, W = 800;
	GLFWwindow* window = DU->StartWindow(H, H);
	DU->drawNightSky(H,W);
	DU->drawGrass(H);
	Tree obj(s, Point(W/1.5,H/2), INITIAL_LENGTH, 90, 1, 0);
	obj.Process();
	DU->Stayawake(window);
}