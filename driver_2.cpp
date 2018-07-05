#include "GLFW/glfw3.h"
#include "bits/stdc++.h"
#include "DrawUtil_2.h"
using namespace std;

const double INITIAL_DECAY_RATE = 0.91;
const double INITIAL_DECAY_DELTA = 0.7;
const double INITIAL_LENGTH = 105;
const double MAX_GENERATIONS = 6;
const double ROTATE_ANGLE = 25.12;

// define the variables in the rule string
#define OPENBRACKET '['
#define CLOSEDBRACKET ']'
#define COLORCHANGE 'C'
#define	TURNLEFT '+'
#define TURNRIGHT '-'
#define DRAW 'F'
#define CIRCLE 'c'

// create a class pointer of the DrawUtil Class
DrawUtil* DU;
struct Point{
	double X;
	double Y;
	Point() : X(0), Y(0) {};
	Point(double _x, double _y) : X(_x), Y(_y) {};
};

// create a map for the rule variable and associated definition <rule character, definition>
map<char, string> Rule;
class Tree{
	string Str;
	Point Pos, tempPos;
	int Generation;
	double Length;
	double Dir;
	int c_index;
	double DecayRate;
	double DecayDelta;
public:
	Tree(string _str, Point _pos, double _len, double _dir, int _gen, int _col, double _dr, double _dd){	//constructor function for the Tree class
		for (auto Char : _str){ 	// generate the string for the drawing rule in the current generation
			if (Rule.find(Char) == Rule.end()){
				Str.push_back(Char);
			}
			else{
				Str += Rule[Char];
			}
		}
		Pos = _pos;
		tempPos=_pos;
		Length = _len;
		Dir = _dir;
		Generation = _gen;
		c_index = _col;
		DecayRate = _dr;
		DecayDelta = _dd;
	}
	void Process(GLFWwindow* window){		// Vital function to check the characters and draw the fractal structure recursively
		if (Generation > MAX_GENERATIONS) return;
		int i = 0, n = Str.size();
		double ODir = Dir;
		while (i < n){			//while Str[i] is a valid String character
			char Ch = Str[i];	// get the ith character of the string
			switch(Ch){
				case OPENBRACKET : {	//get the subtring lying between the current bracket and its matching counterpart
					int OpenBrackets = 1;
					int j = i;
					while (OpenBrackets){
					 	i++;
						if (Str[i] == CLOSEDBRACKET) OpenBrackets--;
						else if (Str[i] == OPENBRACKET) OpenBrackets++;
					}
					if (Generation < MAX_GENERATIONS) {		// if not on the last generation
															// keep generating new tree objects at each node
						Tree child = *(new Tree(Str.substr(j + 1, i-j-1), Pos, Length * (1.0 - DecayRate), Dir, Generation + 1, c_index, DecayRate * (1.0 - DecayDelta), DecayDelta));
						child.Process(window);
					}
					break;
				}
				case TURNLEFT : {	//rotate the branch left by defined angle
					Dir += ROTATE_ANGLE;
					if (Dir >= 360) Dir -= 360;		// if the angle increases beyond 360, subtract 360
					break;
				}
				case TURNRIGHT : {	//rotate the branch right by defined angle
					Dir -= ROTATE_ANGLE;
					if (Dir < 0) Dir += 360;	// if angle decreases beyond 0, add 360
					break;
				}
				case DRAW : {
					if (abs(Dir - ODir) <= 0.00001 && Generation != MAX_GENERATIONS){//If main branch, make it brown.
						DU->SetColor(0.26,0.13,0.09);	
					}
					Point NewPos = *(new Point(Pos.X + cos((M_PI / 180.0) * Dir) * Length, Pos.Y + sin((M_PI / 180.0) * Dir) * Length));
					DU->DrawLine((int)Pos.X, (int)Pos.Y, (int)NewPos.X, (int)NewPos.Y, 0);
					Pos = NewPos;
					if (abs(Dir - ODir) <= 0.00001 && Generation != MAX_GENERATIONS){//Change the color back to the designated color
						DU->SetColor(DU->Colors[c_index].R, DU->Colors[c_index].G, DU->Colors[c_index].B);
					}
					break;
				}
				case COLORCHANGE : {
					if(Generation <= 3)
					{
						DU->SetColor(0.26,0.13,0.09);
						break;
					}
					else
					{
						c_index = Str[++i] - '0';
						DU->SetColor(DU->Colors[c_index].R, DU->Colors[c_index].G, DU->Colors[c_index].B);
						break;
					}
				}
				case CIRCLE : {
					if(Generation == MAX_GENERATIONS){
						// DU->SetColor(0,0.5, 0);
						DU->ChangePointSize(1);
						DU->FillCircle(tempPos.X, tempPos.Y,1);
					}
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
	DU->InitializeColors();
	int H = 1080, W = 1920;
	GLFWwindow* window = DU->StartWindow(H, W);
	cout << DU->Colors[0].R << endl;
	DU->DrawNightSky(W, H);
	DU->DrawStars(W, H, 600);
	DU->DrawGrass(W);
	DU->SetColor(1,1,1);
	DU->FillCircle(0.15*W, 0.85*H, 50);
	DU->SetColor(0.26,0.13,0.09);

	Tree tree2(s, Point(W*0.85,0), INITIAL_LENGTH, 90, 1, 0, INITIAL_DECAY_RATE, INITIAL_DECAY_DELTA);
	tree2.Process(window);
	// Tree tree3(s, Point(W*0.9,0), 90, 90, 1, 0);
	// tree3.Process();
	DU->StayAwake(window);
}