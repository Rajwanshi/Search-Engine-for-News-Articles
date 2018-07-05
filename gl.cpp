#include <GLFW/glfw3.h>
#include <bits/stdc++.h>

using namespace std;

struct RGB{
	double R;
	double G;
	double B;
	RGB():R(0),G(0),B(0){};
	RGB(double _r, double _g, double _b) : R(_r), G(_b), B(_b) {}; 
} Colors[10];

void PlotPixel(GLfloat* PV)
{
	// glColor3f(rand()%2, rand()%2, rand()%2);
    glEnableClientState(GL_VERTEX_ARRAY);
    glPointSize(1);
    glVertexPointer(2, GL_FLOAT, 0, PV);
    glDrawArrays(GL_POINTS, 0, 1);
    glDisableClientState(GL_VERTEX_ARRAY);
}

int findOctant(int dx, int dy)
{
	int octant = 0;
	// cout << dy << "  " << dx << endl;
	if(dx>=0)
	{
		if(dy>=0 && dy<=dx){octant = 0;}
		else if(dy>=0 && dy>dx){octant = 1;}
		else if(dy<0 && abs(dy)<=abs(dx)){octant = 7;}
		else{octant = 6;}
	}
	else
	{
		if(dy>=0 && abs(dy) > abs(dx)){octant = 2;}
		else if(dy>=0 && abs(dy) <= abs(dx)){octant = 3;}
		else if(dy<0 && abs(dy)<=abs(dx)){octant = 4;}
		else{octant = 5;}
	}
	return octant;
}

tuple<int, int, int, int> switchOctantToZero(int x1, int y1, int x2, int y2, int n)
{
	switch(n)
	{
		case 0: return make_tuple(x1, y1, x2, y2);
		case 1: return make_tuple(y1, x1, y2, x2);
		case 2: return make_tuple(y1, -x1, y2, -x2);
		case 3: return make_tuple(-x1, y1, -x2, y2);
		case 4: return make_tuple(-x1, -y1, -x2, -y2);
		case 5: return make_tuple(-y1, -x1, -y2, -x2);
		case 6: return make_tuple(-y1, x1, -y2, x2);
		case 7: return make_tuple(x1, -y1, x2, -y2);
	}
}
tuple<int, int> switchOctantFromZero(int x1, int y1, int n)
{
	switch(n)
	{
		case 0: return make_tuple(x1, y1);
		case 1: return make_tuple(y1, x1);
		case 2: return make_tuple(-y1, x1);
		case 3: return make_tuple(-x1, y1);
		case 4: return make_tuple(-x1, -y1);
		case 5: return make_tuple(-y1, -x1);
		case 6: return make_tuple(y1, -x1);
		case 7: return make_tuple(x1, -y1);
	}
}

// Bresenhem's Line Drawing Algorithm
void drawLine(int x1, int y1, int x2, int y2, RGB color)
{
	if(x1 == x2)
	{
		GLfloat pointerVertices[2];
		pointerVertices[0] = x1;
		
		int ystart = y2 > y1 ? y1 : y2;
		int yend = y2 > y1 ? y2 : y1;
		while(ystart <= yend)
		{
			pointerVertices[1] = ystart;
			PlotPixel(pointerVertices);
			ystart++;
		}
	}
	else
	{
		int n = findOctant(x2-x1, y2-y1);
		// cout << n << endl;
		tie(x1, y1, x2, y2) = switchOctantToZero(x1, y1, x2, y2, n);
		GLfloat pointerVertices[] = {0,0};
		int dy = y2 - y1;
		int dx = x2 - x1;
		int d_param = 2*dy - dx;
		int x = x1;
		int y = y1;
		int incr_NE = dy - dx;
		int incr_E = dy;
		while(x != x2)
		{
			if(d_param >= 0)
			{
				d_param += incr_NE;
				tie(pointerVertices[0], pointerVertices[1]) = switchOctantFromZero(x,y,n);
				PlotPixel(pointerVertices);
				y++;
			}
			else
			{
				d_param += incr_E;
				tie(pointerVertices[0], pointerVertices[1]) = switchOctantFromZero(x,y,n);
				PlotPixel(pointerVertices);
			}
			x++;
		}
	} 
}


void drawSpiral(int W, int H)
{
	GLfloat pointerVertices[] = {0,0};
	float x = 0;
	float y = 0;
	float angle = 0.0f;

	// Space between the spirals
	int a = 2, b = 1;

	for (int i = 0; i < 1000; i++)
	{
	    angle = 0.1 * i;
	    x = (a + b * angle) * cos(angle);
	    y = (a + b * angle) * sin(angle);
	    pointerVertices[0] = W/2+x;
	    pointerVertices[1] = H/2+y;
	    if(i%100==0)
	    {
	    	glColor3f((rand()%2+1)*0.5, (rand()%2+1)*0.5, (rand()%2+1)*0.5);
	    }
	    PlotPixel(pointerVertices);
	    int z = 0;
	}
}
void drawGrass(int W)
{

	glColor3f(0,0.4,0);
	int i=0;
	while(i<W)
	{
		int height = 5 + (rand() % static_cast<int>(25 - 5 + 1));
		drawLine(i,0, i+1, height,Colors[0]);
		i++;
	}
}

void drawSunset(int W, int H)
{
	int j = 0;
	while(j <= H)
	{
		glColor3f(0.40,0.40*j/H,0);
		drawLine(0,j,W,j+1,Colors[0]);
		j++;
	}
}
void drawNightSky(int W, int H)
{
	int j = 0;
	while(j <= H)
	{
		glColor3f(0.02,0.02,.20*j/H);
		drawLine(0,j,W,j+1,Colors[0]);
		j++;
	}
}
void startDraw(int W, int H)
{
	 
	 drawSunset(W,H);
	 drawGrass(W);
	 drawLine(W/2,0, W/2, H/2, Colors[0]);
	 // drawLine(0,0,W/2,H);
	 // drawLine(W/2,0,0,H);
	 // drawLine(W,0,0,H/2);
	 // drawLine(W,H,0,H/2);
	 // drawLine(W/2,H,0,0);
	 // drawLine(W/2,H,W,0);
	 // drawLine(0,H,W,H/2);
	 // drawSpiral(W,H);
}

void glConfig(int W, int H)
{
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	glOrtho(0, W, 0, H, 0, 1);
	glColor3f(0,0,1);
	glfwSwapInterval(1);
}

void startWindow()
{
	//check glfw
    if(glfwInit() == false)
    {
        fprintf(stderr, "Error initializing GLFW\n");
        return;
    }

    int H = 600;
    int W = 600;
    
    //create a glfw window
    GLFWwindow* window = glfwCreateWindow(W, H, "OpenGL Example", NULL, NULL);
    if(!window)
    {
        glfwTerminate();
        fprintf(stderr, "Error while creating a window\n");
        return;
    }

    //set context
    glfwMakeContextCurrent(window);

    // configure gl window
    glConfig(W,H);

    //function call to start drawing
    startDraw(W,H);

    //run event loop
    while(!glfwWindowShouldClose(window))
    {
        
        glfwSwapBuffers(window);
        glfwWaitEvents();
        
    }
    //destroy glfw window instance when the event loop exits
    glfwDestroyWindow(window);
    glfwTerminate();
}