/* Maze Generation
* Vu Do
* This following maze generating algorithm is recursive backtracker which is taken from https://en.wikipedia.org/wiki/Maze_generation_algorithm
*/
#include <GL/freeglut.h>
#include <vector>
#include <stack> 
#include <set>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>
#include <thread>
#include <algorithm>
#include <chrono>
#include <iostream>
using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

int r, t, csize; // width and length and scale
int currentCellI; // current cell index
int nextCellI; //next cell index
int firstWall; //starting wall
vector<int> leftv, rightv;
bool firstEnd=true,stop=false;

/*
***** Notes: 1 cell only have 4 neighbors and neighbor related function go with this order: top, right, bottom, left
*/
class Cell { //helper class which constuct a cell giving its walls info and origin ( lower left corner of the square )
public:
	int top, bottom, left, right, x, y, index;
	Cell(int xc, int yc, int i) {
		x = xc;
		y = yc;
		top = 1;
		bottom = 1;
		left = 1;
		right = 1;
		index = i;
	}

	void reWall(Cell c) { //removing wall
		if (c.index == index + (r / csize))
			top = 0;
		else if (c.index == index + 1)
			right = 0;
		else if (c.index == index - (r / csize))
			bottom = 0;
		else if (c.index == index - 1)
			left = 0;
	}


};

vector<Cell> cells; // list of cells

void setup() // initializations
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // makes everything white 
	glMatrixMode(GL_PROJECTION); // sets the matrix to turn world 
	glLoadIdentity();
	gluOrtho2D(0, r + 1, 0, t + 1); // world extends from 0-100 in 
	glPointSize(1.0); // how big to make dots


}


void startCell() { //randomly get an entrance at a wall
	int random = (int)rand() % 4;
	//currentCellI = random;
	if (random == 0) {
		currentCellI = (int)rand() % (r / csize) + (t / csize * r / csize - r/csize);
		cells[currentCellI].top = 0;
		firstWall = 0;
	}
	else if (random == 1) {
		int ran = (int)rand() % (t / csize);
		currentCellI =ran * (r / csize - 1) + ran - 1;
		cells[currentCellI].right = 0;
		firstWall = 1;
	}
	else if (random == 2) {
		currentCellI = (int)rand() % (r / csize) ;
		cells[currentCellI].bottom = 0;
		firstWall = 2;
	}
	else {
		currentCellI = ((int)rand() % (t / csize)) * r / csize;
		cells[currentCellI].left = 0;
		firstWall = 3;
	}
	
	cout << currentCellI;
}


int randomPick(vector<int> vc) { // recursively and randomly pick a neighbor cell
	int rng = (int)rand() % vc.size();

	while (vc[rng] == -1) {
		rng = (int)rand() % vc.size();
	}
	//cout << "size " << vc.size() << endl;
	for(int i =0; i < vc.size();i++)
		cout << vc[i] << endl;
	cout << "done " << currentCellI << endl;

	nextCellI = vc[rng];
	cout << " next:" << nextCellI << endl;
	
	
	
	return rng;
	
}

vector<int> neighborVisit(set<int> visit) { //help to find if there is any neighbor of the current is not visited	
	vector<int> nb; 
	vector<int> dirs = { currentCellI + (r / csize) ,currentCellI + 1 ,currentCellI - (r / csize) ,currentCellI - 1 };
	
	if (find(leftv.begin(), leftv.end(), currentCellI) != leftv.end())
	{
		for (int i = 0; i < dirs.size(); i += 1) {		
			if (i == 3)
				nb.push_back(-1);
			else if (visit.find(dirs[i]) == visit.end() && dirs[i] < cells.size() && dirs[i] >= 0) {
				nb.push_back(dirs[i]);
			}
			else {
				nb.push_back(-1);
			}
		}
	}
	else if (find(rightv.begin(), rightv.end(), currentCellI) != rightv.end())
	{
		for (int i = 0; i < dirs.size() ; i += 1) {
			if (i == 1)
				nb.push_back(-1);
			else if (visit.find(dirs[i]) == visit.end() && dirs[i] < cells.size() && dirs[i] >= 0) {
				nb.push_back(dirs[i]);
			}
			else {
				nb.push_back(-1);
			}
		}
	}
	else
		for (int i = 0; i < dirs.size(); i += 1) {
			if (visit.find(dirs[i]) == visit.end() && dirs[i] < cells.size() && dirs[i] >= 0) {
				nb.push_back(dirs[i]);
			}
			else {
				nb.push_back(-1);		
			}
		}



	/*
	if (currentCellI - 10 >= 0 && currentCellI + r/10 < cells.size()) {
		if ((checkBorder(currentCellI) == 2 || checkBorder(currentCellI) == 3 || checkBorder(currentCellI) == 7) && visit.find(currentCellI + (r / 10)) == visit.end() && visit.find(currentCellI + 1) == visit.end())
		{
			nb.push_back(currentCellI + (r / 10));
			nb.push_back(currentCellI + 1);
			nb.push_back(-1);
			nb.push_back(-1);
		}
		else if ((checkBorder(currentCellI)==2 || checkBorder(currentCellI) == 1 || checkBorder(currentCellI) == 6) && visit.find(currentCellI + (r / 10)) == visit.end() && visit.find(currentCellI - 1) == visit.end())
		{
			nb.push_back(currentCellI + (r / 10));
			nb.push_back(-1);
			nb.push_back(-1);
			nb.push_back(currentCellI - 1);
		}
		else if ((checkBorder(currentCellI) == 0 || checkBorder(currentCellI) == 1 || checkBorder(currentCellI) == 5) && visit.find(currentCellI - (r / 10)) == visit.end() && visit.find(currentCellI - 1) == visit.end())
		{
			nb.push_back(-1);
			nb.push_back(-1);
			nb.push_back(currentCellI - (r / 10));
			nb.push_back(currentCellI - 1);
		}
		else if ((checkBorder(currentCellI) == 0 || checkBorder(currentCellI) == 3 || checkBorder(currentCellI) == 4) && visit.find(currentCellI - (r / 10)) == visit.end() && visit.find(currentCellI + 1) == visit.end())
		{
			nb.push_back(-1);
			nb.push_back(currentCellI + 1);
			nb.push_back(currentCellI - (r / 10));
			nb.push_back(-1);
		}
		else if ( checkBorder(currentCellI) == 0 && visit.find(currentCellI - 1) == visit.end() && visit.find(currentCellI + 1) == visit.end() && visit.find(currentCellI - (r / 10)) == visit.end()) {
			nb.push_back(-1);
			nb.push_back(currentCellI + 1);
			nb.push_back(currentCellI - (r / 10));
			nb.push_back(currentCellI - 1);
		}
		else if ( checkBorder(currentCellI) == 1 && visit.find(currentCellI - 1) == visit.end() && visit.find(currentCellI - (r / 10)) == visit.end() && visit.find(currentCellI + (r / 10)) == visit.end()) {
			nb.push_back(currentCellI + (r / 10));
			nb.push_back(-1);
			nb.push_back(currentCellI - (r / 10));
			nb.push_back(currentCellI - 1);
		}
		else if ( checkBorder(currentCellI) == 2 && visit.find(currentCellI + (r / 10)) == visit.end() && visit.find(currentCellI + 1) == visit.end() && visit.find(currentCellI - 1) == visit.end()) {
			nb.push_back(currentCellI + (r / 10));
			nb.push_back(currentCellI + 1);
			nb.push_back(-1);
			nb.push_back(currentCellI - 1);
		}
		else if ( checkBorder(currentCellI) == 3 && visit.find(currentCellI - (r / 10)) == visit.end() && visit.find(currentCellI + 1) == visit.end() && visit.find(currentCellI + (r / 10)) == visit.end()) {
			nb.push_back(currentCellI + (r / 10));
			nb.push_back(currentCellI + 1);
			nb.push_back(currentCellI - (r / 10));
			nb.push_back(-1);
		}
		else if (visit.find(currentCellI + (r / 10)) == visit.end() && visit.find(currentCellI - (r / 10)) == visit.end() && visit.find(currentCellI + 1) == visit.end() && visit.find(currentCellI - 1) == visit.end()) {
			nb.push_back(currentCellI + (r / 10));
			nb.push_back(currentCellI + 1);
			nb.push_back(currentCellI - (r / 10));
			nb.push_back(currentCellI - 1);
		}
		else if (visit.find(currentCellI + (r/10)) == visit.end() && visit.find(currentCellI + 1) != visit.end() && visit.find(currentCellI - (r / 10)) != visit.end() && visit.find(currentCellI - 1) != visit.end()) {
			nb.push_back(currentCellI + (r / 10));
			nb.push_back(-1);
			nb.push_back(- 1);
			nb.push_back(- 1);
		}
		else if (visit.find(currentCellI + 1) == visit.end() && visit.find(currentCellI + (r/10)) != visit.end() && visit.find(currentCellI - (r / 10)) != visit.end() && visit.find(currentCellI - 1) != visit.end()) {
			nb.push_back(-1);
			nb.push_back(currentCellI + 1);
			nb.push_back(- 1);
			nb.push_back(- 1);
		}
		else if (visit.find(currentCellI - (r / 10)) == visit.end() && visit.find(currentCellI + 1) != visit.end() && visit.find(currentCellI + (r / 10)) != visit.end() && visit.find(currentCellI - 1) != visit.end()) {
			nb.push_back(-1);
			nb.push_back(- 1);
			nb.push_back(currentCellI - (r/10));
			nb.push_back(- 1);
		}
		else if (visit.find(currentCellI - 1) == visit.end() && visit.find(currentCellI + 1) != visit.end() && visit.find(currentCellI + (r / 10)) != visit.end() && visit.find(currentCellI - (r / 10)) != visit.end()) {
			nb.push_back(-1);
			nb.push_back(- 1);
			nb.push_back(- 1);
			nb.push_back(currentCellI - 1);
		}
		else if (visit.find(currentCellI + (r / 10)) != visit.end())
		{
			nb.push_back(-1);
			nb.push_back(currentCellI + 1);
			nb.push_back(currentCellI - (r / 10));
			nb.push_back(currentCellI - 1);
		}
		else if (visit.find(currentCellI + 1) != visit.end()) {
			nb.push_back(currentCellI + (r / 10));
			nb.push_back(-1);
			nb.push_back(currentCellI - (r / 10));
			nb.push_back(currentCellI - 1);
		}
		else if (visit.find(currentCellI - (r / 10)) != visit.end()) {
			nb.push_back(currentCellI + (r / 10));
			nb.push_back(currentCellI + 1);
			nb.push_back(-1);
			nb.push_back(currentCellI - 1);
		}
		else if (visit.find(currentCellI - 1) != visit.end()) {
			nb.push_back(currentCellI + (r / 10));
			nb.push_back(currentCellI + 1);
			nb.push_back(currentCellI - (r / 10));
			nb.push_back(-1);
		}
		else {
			nb.push_back(-1);
			nb.push_back(-1);
			nb.push_back(-1);
			nb.push_back(-1);
		}
	}
	else {
		nb.push_back(-1);
		nb.push_back(-1);
		nb.push_back(-1);
		nb.push_back(-1);
	}
	/*
	if (checkBorder(currentCellI + (r / 10)) == -1 && ((currentCellI + (r / 10)) < cells.size()))
		nb.push_back(currentCellI + (r / 10));
	else// if (checkBorder(currentCellI + (r / 10)) == 0 )
		nb.push_back(-1);

	if (checkBorder(currentCellI + 1) == -1 && ((currentCellI + 1) < cells.size()))
		nb.push_back(currentCellI + 1);
	else// if (checkBorder(currentCellI + 1) == 1)
		nb.push_back(-1);

	if (checkBorder(currentCellI - (r / 10)) == -1 && (currentCellI - (r / 10)) > 0)
		nb.push_back(currentCellI - (r / 10));
	else// if (checkBorder(currentCellI - (r / 10)) == 2)
		nb.push_back(-1);

	if (checkBorder(currentCellI -1) == -1 && (currentCellI - 1) > 0)
		nb.push_back(currentCellI -1);
	else// if (checkBorder(currentCellI -0) == 3)
		nb.push_back(-1);
	/*
		if (visit.find(currentCellI + (r / 10)) == visit.end() && ((currentCellI + (r / 10)) < cells.size()) ) {
			nb.push_back(currentCellI + (r / 10));
		}
		else {
			nb.push_back(-1);
		}
		if (visit.find(currentCellI + 1) == visit.end() && ((currentCellI + 1) < cells.size())) {
			nb.push_back(currentCellI + 1);
		}
		else {
			nb.push_back(-1);
		}
		if (visit.find(currentCellI - (r / 10)) == visit.end() && (currentCellI - (r / 10)) > 0) {
			nb.push_back(currentCellI - (r / 10));
		}
		else {
			nb.push_back(-1);
		}
		if (visit.find(currentCellI - 1) == visit.end() && (currentCellI - 1) > 0) {
			nb.push_back(currentCellI - 1);
		}
		else {
			nb.push_back(-1);
		}
	*/
	return nb;
}

void drawCell(int x, int y, int left, int right, int top, int bottom) { // cell draw routine
	glColor3f(0.0, 255.0, 0.0);

	if (left == 1) {
		glBegin(GL_LINES);
		glVertex2i(x, y);
		glVertex2i(x, y + csize);
		glEnd();
	}
	
	if (top == 1) {
		glBegin(GL_LINES);
		glVertex2i(x, y + csize);
		glVertex2i(x + csize, y + csize);
		glEnd();
	}

	if (right == 1) {
		glBegin(GL_LINES);
		glVertex2i(x + csize, y + csize);
		glVertex2i(x + csize, y);
		glEnd();
	}
	
	if (bottom == 1) {
		glBegin(GL_LINES);
		glVertex2i(x + csize, y);
		glVertex2i(x, y);		
		glEnd();
	}
	
}

void MazeGen() { // generating maze then draw
	glClear(GL_COLOR_BUFFER_BIT);
	
	glViewport(5, 5, r+1, t+1);	
	for (int i = 0; i < cells.size(); i++) {
		if (i == currentCellI && stop==false)
		{
			glColor3f(0.0, 255.0, 0.0);
			glRecti(cells[i].x, cells[i].y, cells[i].x + csize, cells[i].y + csize);
		}
		else {
			
			drawCell(cells[i].x, cells[i].y, cells[i].left, cells[i].right, cells[i].top, cells[i].bottom);
		}
	}
	
	glFlush();

}
int nbWall(int rPick) { // to remove from next;
	if (rPick == 0)
		return 2;
	else if (rPick = 1)
		return 3;
	else if (rPick = 2)
		return 0;
	else if (rPick = 3)
		return 1;
}
bool allNe1(vector<int> v) {
	return (v[0] == -1 && v[1] == -1 && v[2] == -1 && v[3] == -1);v.p
}
void dfs() { //recursive dfs to generate maze and redraw maze	
	stack<int> s;
	set<int> visit;
	visit.insert(currentCellI);
	s.push(currentCellI);
	while (visit.size()!=cells.size()) {
		//sleep_for(500ms);
		vector<int> cNeighborVisited = neighborVisit(visit);
		if (!allNe1(cNeighborVisited)) {
			randomPick(cNeighborVisited);			
			s.push(currentCellI);
			visit.insert(nextCellI);
			cells[currentCellI].reWall(cells[nextCellI]);
			cells[nextCellI].reWall(cells[currentCellI]);
			currentCellI = nextCellI;
			
		}
		else if (!s.empty()) {
			if (firstEnd == true && find(rightv.begin(), rightv.end(), currentCellI) != rightv.end() && firstWall != 1) {
				cells[currentCellI].right = 0;
				firstEnd = false;
				
			}
			else if (firstEnd == true && find(leftv.begin(), leftv.end(), currentCellI) != leftv.end() && firstWall != 3) {
				cells[currentCellI].left = 0;
				firstEnd = false;
				
			}
			else if (firstEnd == true && currentCellI >= 0 && currentCellI < r / csize && firstWall != 2) {
				cells[currentCellI].bottom = 0;
				firstEnd = false;
				
			}
			else if (firstEnd == true && currentCellI < cells.size() && currentCellI >= cells.size() - r / csize && firstWall != 0) {
				cells[currentCellI].top = 0;
				firstEnd = false;
				
			}
			currentCellI = s.top();
			s.pop();
		}
		MazeGen();
	}
	stop = true;
	MazeGen();
	cout << "done";
	sleep_for(9999999999999s);
}

void getI() {
	int count = 0;
	for (int i = 1; i < t + 1; i += csize) { // create a list of cells with wall size of 10;
		for (int j = 1; j < r + 1; j += csize) {
			cells.push_back(Cell(j, i, count));
			count++;
		}
	}
	
	for (int i = 0; i < r / csize; i += 1) {
		leftv.push_back(i*r/ csize);
		
	}
	for (int i = 1; i <= r/ csize; i += 1) {
		rightv.push_back(i*(r/ csize -1)+i-1);
		
	}
}


void runGen() { //init and run the generator
	srand(time(NULL));
	getI();	
	startCell();
	dfs();
	
}

int main(int argc, char **argv)
{
	cout << "Please enter the length of the maze (recommended to be divisable by 10): "; // input Width (r)
	cin >> r;
	cout << endl << "Please enter the width of the maze (recommended to be divisable by 10): "; // input Length (t)
	cin >> t;
	cout << endl << "Please enter the scale of the maze (recommended to be divisable by 10): "; // input scale (csize)
	cin >> csize;
	
	
	glutInit(&argc, argv); // initialize GLUT for window
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(r + 10, t + 10); // width and height of window
	glutInitWindowPosition(100, 100); // coordinates of corner
	glutCreateWindow("MAZE"); // makes a window with given 
	setup();
	
	glutDisplayFunc(runGen);
	glutMainLoop(); // start waiting for events
}