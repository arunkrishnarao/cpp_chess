#include <GL/glut.h>
#include <iostream>
#define WHITE 1
#define BLACK -1
#define VALID 1
#define INVALID -1
using namespace std;

int move_count = 1;
bool promotion = false;
bool white_casling = true;
bool black_casling = true;

// Initial chess pieces positions
int squares[8][8] =
{
	{ 4,  3,  2,  5,  6,  2,  3,  4},
	{ 1,  1,  1,  1,  1,  1,  1,  1},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0,  0,  0,  0,  0},
	{ 0,  0,  0,  0 , 0,  0,  0,  0},
	{-1, -1, -1, -1, -1, -1, -1, -1},
	{-4, -3, -2, -5, -6, -2, -3, -4}
};

void drawText(char text, const unsigned int x, const unsigned int y)
{
    glColor3f(0, 0, 0);
    glRasterPos2i(x, y);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)text);
}

class Window
{
public:

	int width, height;

	Window(int w, int h)
	{
		width = w;
		height = h;
	}

	void init()
	{
		glutInitWindowSize(width, height);
		glutInitWindowPosition(75,  125);

		glutCreateWindow("Chessboard");
		glutInitDisplayMode(GLUT_SINGLE | GL_RGB);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, width, 0, height);
	}	

	int getHeight() 
	{ 
		return height; 
	}

	int getWidth() 
	{ 
		return width; 
	}

};

class Board
{
public:
    Board(Window window)
    {
        drawBoard(window);
    }


    void drawBoard(Window window)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.0, 0.0, 0.0);
        
        int X = window.getWidth() - 20;
        int Y = window.getHeight() - 20;

        // The dimensions of individual squeares
        int square_width = X / 8;
        int square_height = Y / 8;

        for (int x=0; x<=X; x+=square_width)
        {
            for (int y=0; y<=Y; y+=square_height)
            {
                if (((x + y) / square_width) & 1)
                {
                    glColor3f(1, 0.9f, 0.6f);
                }
				else
				{
                    glColor3f(0.8f, 0.55f, 0.3f);
                }
			 	// draw the individual square
                glRecti(x, y, (x + square_width), (y + square_height));
            }
        }
		int labels[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
		int indexes[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};
		for (int x=0; x<8; x++)
		{
			glColor3f(0.0, 0.0, 0.0);
			drawText(labels[x], (x*100 + 47), 804.0);
		}

		for (int x=8; x>=0; x--)
		{
			glColor3f(0.0, 0.0, 0.0);
			drawText(indexes[x], 806.0, (x*100 + 45));
		}
		
        glFlush();
    }

	static int covert_label(char label)
	{
		switch(label)
		{
			case 'A':
			case 'a':
				return 1;
				break;
			case 'B':
			case 'b':	
				return 2;	
				break;	
			case 'C':
			case 'c':	
				return 3;	
				break;
			case 'D':
			case 'd':	
				return 4;	
				break;
			case 'E':
			case 'e':	
				return 5;	
				break;
			case 'F':
			case 'f':	
				return 6;	
				break;
			case 'G':
			case 'g':	
				return 7;	
				break;				
			case 'H':
			case 'h':	
				return 8;	
				break;
			default:
				cout<<"Invaild Label";
				return -1;
		}
	}

	int inputCheck(int x)
	{
		if(x<=-1 || x>=8)
			return -1;
		else
			return 1;
	}

	int checkValidMove(int (&squares)[8][8], int x1, int y1, int x2, int y2)
	{
		if(inputCheck(x1) == -1)
			return INVALID;
		else if(inputCheck(y1) == -1)
			return INVALID;
		else if(inputCheck(x2) == -1)
			return INVALID;
		else if(inputCheck(y2) == -1)
			return INVALID;

		switch(squares[x1][y1])
		{
			case 1:
				if(x2 - x1 == 2 && x1 == 1 && squares[x2][y2] == 0)
				{
					if(squares[x1+1][y1] == 0)
					{
						return VALID;
					}
					return INVALID;
				}
				if(x2 - x1 == 1 && y1 == y2 && squares[x2][y2] == 0)
				{
					if (x2 == 7)
					{
						promotion = true;
					}
					return VALID;
				}
				else if (x2 - x1 == 1 && squares[x2][y2] < 0 && (y2-y1 == 1 || y2-y1 == -1))
				{
					if (x2 == 7)
					{
						promotion = true;
					}
					return VALID;
				}
				else
					return INVALID;
			break;

			case -1:
				if(x1 - x2 == 2 && x1 == 6 && squares[x2][y2] == 0)
				{
					if(squares[x1-1][y1] == 0)
					{
						return VALID;
					}
					return INVALID;
				}
				if(x1 - x2 == 1 && y1 == y2)
				{
					if (x2 == 0)
					{
						promotion = true;
					}
					return VALID;
				}
				else if (x1 - x2 == 1 && squares[x2][y2] > 0 && (y2-y1 == 1 || y2-y1 == -1))
				{
					if (x2 == 0)
					{
						promotion = true;
					}					
					return VALID;
				}
				else
					return INVALID;
			break;

			case 2:
				if(inputCheck(x1) == -1)
					return INVALID;
				else if(inputCheck(y1) == -1)
					return INVALID;
				else if(inputCheck(x2) == -1)
					return INVALID;
				else if(inputCheck(y2) == -1)
					return INVALID;
				
				if(abs(x2 - x1) == abs(y2 - y1) && squares[x2][y2] < 0)
				{
					if(x2 > x1 && y2 > y1)
					{
						int i = x1+1;
						int j = y1+1;
						
						for(; i<x2, j<y2; i++, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x2 < x1 && y2 < y1)
					{
						int i = x1-1;
						int j = y1-1;
						
						for(; i>x2, j>y2; i--, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x2 > x1 && y2 < y1)
					{
						int i = x1+1;
						int j = y1-1;
						
						for(; i<x2, j>y2; i++, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x2 < x1 && y2 > y1)
					{
						int i = x1-1;
						int j = y1+1;
						
						for(; i>x2, j<y2; i--, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}
					return VALID;					
				}

				else if (abs(x2 - x1) == abs(y2 - y1))
				{
					if(x2 > x1 && y2 > y1)
					{
						int i = x1+1;
						int j = y1+1;
						
						for(; i<x2+1, j<y2+1; i++, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x2 < x1 && y2 < y1)
					{
						int i = x1-1;
						int j = y1-1;
						
						for(; i>x2-1, j>y2-1; i--, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x2 > x1 && y2 < y1)
					{
						int i = x1+1;
						int j = y1-1;
						
						for(; i<x2+1, j>y2-1; i++, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x2 < x1 && y2 > y1)
					{
						int i = x1-1;
						int j = y1+1;
						
						for(; i>x2-1, j<y2+1; i--, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}
					return VALID;
				}
			break;
			case -2:
				if(inputCheck(x1) == -1)
					return INVALID;
				else if(inputCheck(y1) == -1)
					return INVALID;
				else if(inputCheck(x2) == -1)
					return INVALID;
				else if(inputCheck(y2) == -1)
					return INVALID;
				
				if(abs(x2 - x1) == abs(y2 - y1) && squares[x2][y2] > 0)
				{
					if(x2 > x1 && y2 > y1)
					{
						int i = x1+1;
						int j = y1+1;
						
						for(; i<x2, j<y2; i++, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x2 < x1 && y2 < y1)
					{
						int i = x1-1;
						int j = y1-1;
						
						for(; i>x2, j>y2; i--, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x2 > x1 && y2 < y1)
					{
						int i = x1+1;
						int j = y1-1;
						
						for(; i<x2, j>y2; i++, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x2 < x1 && y2 > y1)
					{
						int i = x1-1;
						int j = y1+1;
						
						for(; i>x2, j<y2; i--, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}
					return VALID;					
				}

				else if (abs(x2 - x1) == abs(y2 - y1))
				{
					if(x2 > x1 && y2 > y1)
					{
						int i = x1+1;
						int j = y1+1;
						
						for(; i<x2+1, j<y2+1; i++, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x2 < x1 && y2 < y1)
					{
						int i = x1-1;
						int j = y1-1;
						
						for(; i>x2-1, j>y2-1; i--, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x2 > x1 && y2 < y1)
					{
						int i = x1+1;
						int j = y1-1;
						
						for(; i<x2+1, j>y2-1; i++, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x2 < x1 && y2 > y1)
					{
						int i = x1-1;
						int j = y1+1;
						
						for(; i>x2-1, j<y2+1; i--, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}
					return VALID;
				}
			break;
			case 3:
				if(inputCheck(x1) == -1)
					return INVALID;
				else if(inputCheck(y1) == -1)
					return INVALID;
				else if(inputCheck(x2) == -1)
					return INVALID;
				else if(inputCheck(y2) == -1)
					return INVALID;

				if((abs(x2 - x1) == 2 && abs(y2 - y1) == 1) || (abs(x2 - x1) == 1 && abs(y2 - y1) == 2) && squares[x2][y2] == 0)
					return VALID;
				else if(abs(x2 - x1) == 2 && abs(y2 - y1) == 1 || (abs(x2 - x1) == 1 && abs(y2 - y1) == 2) && squares[x2][y2] < 0)
					return VALID;
				else
					return INVALID;	
			break;
			case -3:
				if(inputCheck(x1) == -1)
					return INVALID;
				else if(inputCheck(y1) == -1)
					return INVALID;
				else if(inputCheck(x2) == -1)
					return INVALID;
				else if(inputCheck(y2) == -1)
					return INVALID;

				if(abs(x2 - x1) == 2 && abs(y2 - y1) == 1 || (abs(x2 - x1) == 1 && abs(y2 - y1) == 2) && squares[x2][y2] == 0)
					return VALID;
				else if(abs(x2 - x1) == 2 && abs(y2 - y1) == 1 || (abs(x2 - x1) == 1 && abs(y2 - y1) == 2) && squares[x2][y2] > 0)
					return VALID;
				else
					return INVALID;	
			break;
			case 4:
				if(inputCheck(x1) == -1)
					return INVALID;
				else if(inputCheck(y1) == -1)
					return INVALID;
				else if(inputCheck(x2) == -1)
					return INVALID;
				else if(inputCheck(y2) == -1)
					return INVALID;
				
				if ((abs(x2 - x1) > 0 && (y2 == y1)) || (abs(y2 - y1) > 0 && (x2 == x1)) && squares[x2][y2] < 0)
				{
					if(x2 > x1)
					{
						int j = y1;
						for(int i=x1+1; i<x2; i++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x1 > x2)
					{
						int j = y1;
						for(int i=x1-1; i>x2; i--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(y2 > y1)
					{
						int i = x1;
						for(int j=y1+1; j<y2; j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(y1 > y2)
					{
						int i = x1;
						for(int j=y1-1; j>y2; j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}
					return VALID;
				}

				else if ((abs(x2 - x1) > 0 && (y2 == y1)) || (abs(y2 - y1) > 0 && (x2 == x1)))
				{
					if(x2 > x1)
					{
						int j = y1;
						for(int i=x1+1; i<x2+1; i++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x1 > x2)
					{
						int j = y1;
						for(int i=x1-1; i>x2-1; i--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(y2 > y1)
					{
						int i = x1;
						for(int j=y1+1; j<y2+1; j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(y1 > y2)
					{
						int i = x1;
						for(int j=y1-1; j>y2-1; j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}
					return VALID;					
				}
			break;
			case -4:
				if(inputCheck(x1) == -1)
					return INVALID;
				else if(inputCheck(y1) == -1)
					return INVALID;
				else if(inputCheck(x2) == -1)
					return INVALID;
				else if(inputCheck(y2) == -1)
					return INVALID;
				
				if ((abs(x2 - x1) > 0 && (y2 == y1)) || (abs(y2 - y1) > 0 && (x2 == x1)) && squares[x2][y2] > 0)
				{
					if(x2 > x1)
					{
						int j = y1;
						for(int i=x1+1; i<x2; i++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x1 > x2)
					{
						int j = y1;
						for(int i=x1-1; i>x2; i--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(y2 > y1)
					{
						int i = x1;
						for(int j=y1+1; j<y2; j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(y1 > y2)
					{
						int i = x1;
						for(int j=y1-1; j>y2; j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}
					return VALID;
				}

				else if ((abs(x2 - x1) > 0 && (y2 == y1)) || (abs(y2 - y1) > 0 && (x2 == x1)))
				{
					if(x2 > x1)
					{
						int j = y1;
						for(int i=x1+1; i<x2+1; i++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(x1 > x2)
					{
						int j = y1;
						for(int i=x1-1; i>x2-1; i--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(y2 > y1)
					{
						int i = x1;
						for(int j=y1+1; j<y2+1; j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					if(y1 > y2)
					{
						int i = x1;
						for(int j=y1-1; j>y2-1; j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}
					return VALID;					
				}
			break;
			case 5:
				if(inputCheck(x1) == -1)
					return INVALID;
				else if(inputCheck(y1) == -1)
					return INVALID;
				else if(inputCheck(x2) == -1)
					return INVALID;
				else if(inputCheck(y2) == -1)
					return INVALID;

				if((abs(x2 - x1) == abs(y2 - y1)) || (abs(x2 - x1) > 0 && (y2 == y1)) || (abs(y2 - y1) > 0 && (x2 == x1))  && squares[x2][y2] < 0)
				{
					if(x2 > x1 && y2 > y1)
					{
						int i = x1+1;
						int j = y1+1;
						
						for(; i<x2, j<y2; i++, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 < x1 && y2 < y1)
					{
						int i = x1-1;
						int j = y1-1;
						
						for(; i>x2, j>y2; i--, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 > x1 && y2 < y1)
					{
						int i = x1+1;
						int j = y1-1;
						
						for(; i<x2, j>y2; i++, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 < x1 && y2 > y1)
					{
						int i = x1-1;
						int j = y1+1;
						
						for(; i>x2, j<y2; i--, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 > x1)
					{
						int j = y1;
						for(int i=x1+1; i<x2; i++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x1 > x2)
					{
						int j = y1;
						for(int i=x1-1; i>x2; i--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(y2 > y1)
					{
						int i = x1;
						for(int j=y1+1; j<y2; j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(y1 > y2)
					{
						int i = x1;
						for(int j=y1-1; j>y2; j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}
					return VALID;					
				}
				else if((abs(x2 - x1) == abs(y2 - y1)) || (abs(x2 - x1) > 0 && (y2 == y1)) || (abs(y2 - y1) > 0 && (x2 == x1)))
				{
					if(x2 > x1 && y2 > y1)
					{
						int i = x1+1;
						int j = y1+1;
						
						for(; i<x2+1, j<y2+1; i++, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 < x1 && y2 < y1)
					{
						int i = x1-1;
						int j = y1-1;
						
						for(; i>x2-1, j>y2-1; i--, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 > x1 && y2 < y1)
					{
						int i = x1+1;
						int j = y1-1;
						
						for(; i<x2+1, j>y2-1; i++, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 < x1 && y2 > y1)
					{
						int i = x1-1;
						int j = y1+1;
						
						for(; i>x2-1, j<y2+1; i--, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 > x1)
					{
						int j = y1;
						for(int i=x1+1; i<x2+1; i++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x1 > x2)
					{
						int j = y1;
						for(int i=x1-1; i>x2-1; i--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(y2 > y1)
					{
						int i = x1;
						for(int j=y1+1; j<y2+1; j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(y1 > y2)
					{
						int i = x1;
						for(int j=y1-1; j>y2-1; j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}
					return VALID;			
				}
			break;
			case -5:
				if(inputCheck(x1) == -1)
					return INVALID;
				else if(inputCheck(y1) == -1)
					return INVALID;
				else if(inputCheck(x2) == -1)
					return INVALID;
				else if(inputCheck(y2) == -1)
					return INVALID;

				if((abs(x2 - x1) == abs(y2 - y1)) || (abs(x2 - x1) > 0 && (y2 == y1)) || (abs(y2 - y1) > 0 && (x2 == x1))  && squares[x2][y2] > 0)
				{
					if(x2 > x1 && y2 > y1)
					{
						int i = x1+1;
						int j = y1+1;
						
						for(; i<x2, j<y2; i++, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 < x1 && y2 < y1)
					{
						int i = x1-1;
						int j = y1-1;
						
						for(; i>x2, j>y2; i--, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 > x1 && y2 < y1)
					{
						int i = x1+1;
						int j = y1-1;
						
						for(; i<x2, j>y2; i++, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 < x1 && y2 > y1)
					{
						int i = x1-1;
						int j = y1+1;
						
						for(; i>x2, j<y2; i--, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 > x1)
					{
						int j = y1;
						for(int i=x1+1; i<x2; i++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x1 > x2)
					{
						int j = y1;
						for(int i=x1-1; i>x2; i--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(y2 > y1)
					{
						int i = x1;
						for(int j=y1+1; j<y2; j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(y1 > y2)
					{
						int i = x1;
						for(int j=y1-1; j>y2; j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}
					return VALID;					
				}
				else if((abs(x2 - x1) == abs(y2 - y1)) || (abs(x2 - x1) > 0 && (y2 == y1)) || (abs(y2 - y1) > 0 && (x2 == x1)))
				{
					if(x2 > x1 && y2 > y1)
					{
						int i = x1+1;
						int j = y1+1;
						
						for(; i<x2+1, j<y2+1; i++, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 < x1 && y2 < y1)
					{
						int i = x1-1;
						int j = y1-1;
						
						for(; i>x2-1, j>y2-1; i--, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 > x1 && y2 < y1)
					{
						int i = x1+1;
						int j = y1-1;
						
						for(; i<x2+1, j>y2-1; i++, j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 < x1 && y2 > y1)
					{
						int i = x1-1;
						int j = y1+1;
						
						for(; i>x2-1, j<y2+1; i--, j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x2 > x1)
					{
						int j = y1;
						for(int i=x1+1; i<x2+1; i++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(x1 > x2)
					{
						int j = y1;
						for(int i=x1-1; i>x2-1; i--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(y2 > y1)
					{
						int i = x1;
						for(int j=y1+1; j<y2+1; j++)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}

					else if(y1 > y2)
					{
						int i = x1;
						for(int j=y1-1; j>y2-1; j--)
						{
							if(squares[i][j] != 0)
							{
								return INVALID;
							}
						}
					}
					return VALID;			
				}
			break;

			case 6:
			if((x1 == 0 && y1 == 4) && ((x2 == 0 && y2 == 6) || (x2 == 0 && y2 == 2)) && white_casling == true)
			{
				if(verifyCheckWhite() == true)
					return INVALID;

				else if (x2 == 0 && y2 == 6)
				{
					if (squares[0][5] == 0 && squares[0][6] == 0 && squares[0][7] == 4)
					{
						squares[0][5] = 4;
						squares[0][7] = 0;
						cout<<"\nRule applied: White King-side Castling\n";
						return VALID;
					}
				}
				else if (x2 == 0 && y2 == 2)
				{
					if (squares[0][0] == 4 && squares[0][1] == 0 && squares[0][2] == 0 && squares[0][3] == 0)
					{
						squares[0][0] = 0;
						squares[0][3] = 4;
						cout<<"\nRule applied: White Queen-side Castling\n";
						return VALID;
					}
				}
				return INVALID;
			}
			else
			{
				white_casling = false;
			}

			if(((abs(x2 - x1) == 1 && (y2 == y1)) || (abs(y2 - y1) == 1  && (x2 == x1))) && squares[x2][y2] == 0)
				return VALID;
			else if (((abs(x2 - x1) == 1 && (y2 == y1)) || (abs(y2 - y1) == 1  && (x2 == x1))) && squares[x2][y2] < 0)
				return VALID;
			else
				return INVALID;
			break;

			case -6:
			if((x1 == 7 && y1 == 4) && ((x2 == 7 && y2 == 6) || (x2 == 7 && y2 == 2)) && black_casling == true)
			{
				if(verifyCheckBlack() == true)
					return INVALID;

				else if (x2 == 7 && y2 == 6)
				{
					if (squares[7][5] == 0 && squares[7][6] == 0 && squares[7][7] == -4)
					{
						squares[7][5] = -4;
						squares[7][7] = 0;
						cout<<"\nRule applied: Black King-side Castling\n";
						return VALID;
					}
				}
				else if (x2 == 7 && y2 == 2)
				{
					if (squares[7][0] == -4 && squares[7][1] == 0 && squares[7][2] == 0 && squares[7][3] == 0)
					{
						squares[7][0] = 0;
						squares[7][3] = -4;
						cout<<"\nRule applied: Black Queen-side Castling\n";
						return VALID;
					}
				}
				return INVALID;
			}
			else
			{
				white_casling = false;
			}
			if(((abs(x2 - x1) == 1 && (y2 == y1)) || (abs(y2 - y1) == 1  && (x2 == x1))) && squares[x2][y2] == 0)
				return VALID;
			else if (((abs(x2 - x1) == 1 && (y2 == y1)) || (abs(y2 - y1) == 1  && (x2 == x1))) && squares[x2][y2] > 0)
				return VALID;
			else
				return INVALID;
			break;
		}
		return INVALID;
	}

	bool verifyCheckWhite()
	{
		for(int p=0; p<8; p++)
		{
			for(int q=0; q<8; q++)
			{
				if(squares[p][q] < 0)
				{
					for(int x=0; x<8; x++)
					{
						for(int y=0; y<8; y++)
						{
							if(abs(squares[x][y]) == 6)
							{
								if(checkValidMove(squares, p, q, x, y) == VALID)
								{
									return true;
								}
							}
						}
					}
				}
			}
		}
		return false;
	}

	bool verifyCheckBlack()
	{
		for(int p=0; p<8; p++)
		{
			for(int q=0; q<8; q++)
			{
				if(squares[p][q] > 0)
				{
					for(int x=0; x<8; x++)
					{
						for(int y=0; y<8; y++)
						{
							if(abs(squares[x][y]) == 6)
							{
								if(checkValidMove(squares, p, q, x, y) == VALID)
								{
									return true;
								}
							}
						}
					}
				}
			}
		}
		return false;
	}

	void makeMove(int (&squares)[8][8] , char label_start, int x1, char label_end, int x2)
	{
		int y1 = Board::covert_label(label_start) -1;
		int y2 = Board::covert_label(label_end) -1;
		x1--;
		x2--;
		cout<<"\n\nMove: ("<<x1<<", "<<y1<<") -> ("<<x2<<", "<<y2<<")\n";

		if(abs(squares[x2][y2]) != 6)
		{
			if(move_count % 2 == 0 && squares[x1][y1] < 0 || move_count % 2 == 1 && squares[x1][y1] > 0)
			{
				if(checkValidMove(squares, x1, y1, x2, y2) == VALID)
				{
					if(squares[x1][y1] == 1 || squares[x1][y1] == -1)
					{
						if(promotion == true)
						{
							int ch=1;
							cout<<"\nRule: Pawn Promotion applied\nChoose piece to promote:\n 1 = Queen\n 2 = Rook\n 3 = Knight\n 4 = Bishop\n> ";
							do { cin>>ch; } while (ch<1 || ch>4);
							switch(ch)
							{
								case 1: if(squares[x1][y1] == 1) ::squares[x1][y1] = 5; else ::squares[x1][y1] = -5;
								break;
								case 2: if(squares[x1][y1] == 1) ::squares[x1][y1] = 4; else ::squares[x1][y1] = -4;
								break;
								case 3: if(squares[x1][y1] == 1) ::squares[x1][y1] = 3; else ::squares[x1][y1] = -3;
								break;
								case 4: if(squares[x1][y1] == 1) ::squares[x1][y1] = 2; else ::squares[x1][y1] = -2;
								break;	
								default: if(squares[x1][y1] == 1) ::squares[x1][y1] = 5; else ::squares[x1][y1] = -5;
							}
							promotion = false;
						}
					}
					int temp_piece = squares[x2][y2];
					squares[x2][y2] = squares[x1][y1];
					squares[x1][y1] = 0;

					if(verifyCheckWhite() == false && move_count%2 == 1  || verifyCheckBlack() == false && move_count%2 == 0)
					{
						move_count++;
					}
					else
					{
						squares[x1][y1] = squares[x2][y2];
						squares[x2][y2] = temp_piece;
						cout<<"Invlid Move! King is under Check!";
					}
				}
				else
				{
					cout<<"\nInvalid Move\n";
				}
			}
			else
			{
				if(move_count % 2 == 0)
					cout<<"\nIt is now Black's turn.\n";
				else
					cout<<"\nIt is now White's turn.\n";
			}
		}
		else
		{
			cout<<"\nInvalid Move: King under Check!\n";			
		}
	}
};

class ChessPieces
{
public:
	ChessPieces(int chessboard[8][8])
	{
		for(int p=0; p<8; p++)
		{
			for(int q=0; q<8; q++)
			{
				switch(chessboard[q][p])
				{
					case 1:
						drawPawn(p*100, q*100, WHITE);
					break;
					case -1:
						drawPawn(p*100, q*100, BLACK);
					break;

					case 2:
						drawBishop(p*100, q*100, WHITE);
					break;
					case -2:
						drawBishop(p*100, q*100, BLACK);
					break;

					case 3:
						drawKnight(p*100, q*100, WHITE);
					break;
					case -3:
						drawKnight(p*100, q*100, BLACK);
					break;

					case 4:
						drawRook(p*100, q*100, WHITE);
					break;
					case -4:
						drawRook(p*100, q*100, BLACK);
					break;

					case 5:
						drawQueen(p*100, q*100, WHITE);
					break;
					case -5:
						drawQueen(p*100, q*100, BLACK);
					break;		

					case 6:
						drawKing(p*100, q*100, WHITE);
					break;
					case -6:
						drawKing(p*100, q*100, BLACK);
					break;																				
				}
			}
		}
	}
	void drawPawn(int x, int y, int color)
	{
		if(color == BLACK)
		{
			glColor3f(0, 0, 0); // Black Piece
		}
		else
		{

			glColor3f(1, 1, 1); // White Piece
		}
		// Base of the Pawn
		glBegin(GL_POLYGON);
		glVertex2d(x+20, y+10);
		glVertex2d(x+20, y+20);
		glVertex2d(x+25, y+18);
		glVertex2d(x+25, y+25);
		glVertex2d(x+40, y+30);
		glVertex2d(x+60, y+30);
		glVertex2d(x+75, y+25);
		glVertex2d(x+75, y+18);
		glVertex2d(x+80, y+20);
		glVertex2d(x+80, y+10);
		glVertex2d(x+20, y+10);
		glEnd();

		// Neck of the Pawn
		glBegin(GL_POLYGON);
		glVertex2d(x+40, y+30);
		glVertex2d(x+45, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+60, y+30);
		glVertex2d(x+40, y+30);
		glEnd();

		// Head of the Pawn
		glBegin(GL_POLYGON);
		glVertex2d(x+45, y+75);
		glVertex2d(x+40, y+78);
		glVertex2d(x+40, y+90);
		glVertex2d(x+60, y+90);
		glVertex2d(x+60, y+78);
		glVertex2d(x+55, y+75);
		glVertex2d(x+45, y+75);
		glEnd();

		if(color == BLACK)
		{
			glColor3f(1, 1, 1); // Black Piece - White Outline
		}
		else
		{
			glColor3f(0, 0, 0); // White Piece - White Outline
		}
		glLineWidth(2);

		// Base-Outline of the Pawn
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+20, y+10);
		glVertex2d(x+20, y+20);
		glVertex2d(x+25, y+18);
		glVertex2d(x+25, y+18);
		glVertex2d(x+75, y+18);
		glVertex2d(x+25, y+18);		
		glVertex2d(x+25, y+25);
		glVertex2d(x+75, y+25);
		glVertex2d(x+25, y+25);	
		glVertex2d(x+40, y+30);
		glVertex2d(x+60, y+30);
		glVertex2d(x+75, y+25);
		glVertex2d(x+75, y+18);
		glVertex2d(x+80, y+20);
		glVertex2d(x+80, y+10);
		glVertex2d(x+20, y+10);
		glEnd();

		// Neck-Outline of the Pawn
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+40, y+30);
		glVertex2d(x+45, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+60, y+30);
		glVertex2d(x+40, y+30);
		glEnd();

		// Head-Outline of the Pawn
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+45, y+75);
		glVertex2d(x+40, y+78);
		glVertex2d(x+40, y+90);
		glVertex2d(x+60, y+90);
		glVertex2d(x+60, y+78);
		glVertex2d(x+55, y+75);
		glVertex2d(x+45, y+75);
		glEnd();
		
		glFlush();
	}

	void drawBishop(int x, int y, int color)
	{
		if(color == BLACK)
		{
			glColor3f(0, 0, 0); // Black Piece
		}
		else
		{

			glColor3f(1, 1, 1); // White Piece
		}
		// Base of the Bishop
		glBegin(GL_POLYGON);
		glVertex2d(x+20, y+10);
		glVertex2d(x+20, y+20);
		glVertex2d(x+25, y+18);
		glVertex2d(x+25, y+25);
		glVertex2d(x+40, y+30);
		glVertex2d(x+60, y+30);
		glVertex2d(x+75, y+25);
		glVertex2d(x+75, y+18);
		glVertex2d(x+80, y+20);
		glVertex2d(x+80, y+10);
		glVertex2d(x+20, y+10);
		glEnd();

		// Neck of the Bishop
		glBegin(GL_POLYGON);
		glVertex2d(x+40, y+30);
		glVertex2d(x+45, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+60, y+30);
		glVertex2d(x+40, y+30);
		glEnd();

		// Head of the Bishop
		glBegin(GL_POLYGON);
		glVertex2d(x+45, y+75);
		glVertex2d(x+40, y+82);
		glVertex2d(x+50, y+98);
		glVertex2d(x+60, y+82);
		glVertex2d(x+55, y+75);
		glVertex2d(x+45, y+75);
		glEnd();
		if(color == BLACK)
		{
			glColor3f(1, 1, 1); // Black Piece - White Outline
		}
		else
		{
			glColor3f(0, 0, 0); // White Piece - White Outline
		}
		glLineWidth(2);

		// Base-Outline of the Bishop
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+20, y+10);
		glVertex2d(x+20, y+20);
		glVertex2d(x+25, y+18);
		glVertex2d(x+25, y+18);
		glVertex2d(x+75, y+18);
		glVertex2d(x+25, y+18);
		glVertex2d(x+25, y+25);
		glVertex2d(x+75, y+25);
		glVertex2d(x+25, y+25);
		glVertex2d(x+40, y+30);
		glVertex2d(x+60, y+30);
		glVertex2d(x+75, y+25);
		glVertex2d(x+75, y+18);
		glVertex2d(x+80, y+20);
		glVertex2d(x+80, y+10);
		glVertex2d(x+20, y+10);
		glEnd();

		// Neck-Outline of the Bishop
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+40, y+30);
		glVertex2d(x+45, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+60, y+30);
		glVertex2d(x+40, y+30);
		glEnd();


		// Head-Outline of the Bishop
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+45, y+75);
		glVertex2d(x+40, y+82);
		glVertex2d(x+60, y+82);
		glVertex2d(x+40, y+82);
		glVertex2d(x+50, y+98);
		glVertex2d(x+60, y+82);
		glVertex2d(x+55, y+75);
		glVertex2d(x+45, y+75);
		glEnd();

		glFlush();
	}

	void drawKnight(int x, int y, int color)
	{
		if(color == BLACK)
		{
			glColor3f(0, 0, 0); // Black Piece
		}
		else
		{
			glColor3f(1, 1, 1); // White Piece
		}
		// Base of the Knight
		glBegin(GL_POLYGON);
		glVertex2d(x+20, y+10);
		glVertex2d(x+20, y+20);
		glVertex2d(x+25, y+18);
		glVertex2d(x+25, y+25);
		glVertex2d(x+40, y+30);
		glVertex2d(x+60, y+30);
		glVertex2d(x+75, y+25);
		glVertex2d(x+75, y+18);
		glVertex2d(x+80, y+20);
		glVertex2d(x+80, y+10);
		glVertex2d(x+20, y+10);
		glEnd();

		// Neck of the Knight
		glBegin(GL_POLYGON);
		glVertex2d(x+40, y+30);
        glVertex2d(x+60, y+30);
        glVertex2d(x+60, y+50);
        glVertex2d(x+40, y+50);
        glVertex2d(x+40, y+30);
		glEnd();

		// Head of the Knight
		glBegin(GL_POLYGON);
		glVertex2d(x+55, y+85);
        glVertex2d(x+50, y+80);
        glVertex2d(x+30, y+70);
        glVertex2d(x+32, y+65);
        glVertex2d(x+50, y+68);
        glVertex2d(x+50, y+60);
        glVertex2d(x+40, y+50);
        glVertex2d(x+60, y+50);
        glVertex2d(x+60, y+60);
        glVertex2d(x+60, y+68);
        glVertex2d(x+63, y+80);
        glVertex2d(x+60, y+80);
        glVertex2d(x+55, y+85);
		glEnd();

		if(color == BLACK)
		{
			glColor3f(1, 1, 1); // Black Piece - White Outline
		}
		else
		{
			glColor3f(0, 0, 0); // White Piece - White Outline
		}
		glLineWidth(2);

		// Base-Outline of the Knight
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+20, y+10);
		glVertex2d(x+20, y+20);
		glVertex2d(x+25, y+18);
		glVertex2d(x+25, y+18);
		glVertex2d(x+75, y+18);
		glVertex2d(x+25, y+18);		
		glVertex2d(x+25, y+25);
		glVertex2d(x+75, y+25);
		glVertex2d(x+25, y+25);	
		glVertex2d(x+40, y+30);
		glVertex2d(x+60, y+30);
		glVertex2d(x+75, y+25);
		glVertex2d(x+75, y+18);
		glVertex2d(x+80, y+20);
		glVertex2d(x+80, y+10);
		glVertex2d(x+20, y+10);
		glEnd();

		// Neck-Outline of the Knight
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+40, y+30);
        glVertex2d(x+60, y+30);
        glVertex2d(x+60, y+50);
        glVertex2d(x+40, y+50);
        glVertex2d(x+40, y+30);
		glEnd();


		// Head-Outline of the Knight
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+55, y+85);
        glVertex2d(x+50, y+80);
        glVertex2d(x+30, y+70);
        glVertex2d(x+32, y+65);
        glVertex2d(x+50, y+68);
        glVertex2d(x+45, y+60);
        glVertex2d(x+40, y+50);
        glVertex2d(x+60, y+50);
        glVertex2d(x+60, y+60);
        glVertex2d(x+60, y+68);
        glVertex2d(x+63, y+80);
        glVertex2d(x+60, y+80);
        glVertex2d(x+55, y+85);
		glEnd();
		glFlush();
	}

	void drawRook(int x, int y, int color)
	{
		if(color == BLACK)
		{
			glColor3f(0, 0, 0); // Black Piece
		}
		else
		{

			glColor3f(1, 1, 1); // White Piece
		}
		// Base of the Rook
		glBegin(GL_POLYGON);
		glVertex2d(x+20, y+10);
		glVertex2d(x+20, y+20);
		glVertex2d(x+25, y+18);
		glVertex2d(x+25, y+25);
		glVertex2d(x+40, y+30);
		glVertex2d(x+60, y+30);
		glVertex2d(x+75, y+25);
		glVertex2d(x+75, y+18);
		glVertex2d(x+80, y+20);
		glVertex2d(x+80, y+10);
		glVertex2d(x+20, y+10);
		glEnd();

		// Neck of the Rook
		glBegin(GL_POLYGON);
		glVertex2d(x+40, y+30);
		glVertex2d(x+45, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+60, y+30);
		glVertex2d(x+40, y+30);
		glEnd();

		// Head of the Rook
		glBegin(GL_POLYGON);
		glVertex2d(x+45, y+75);
		glVertex2d(x+30, y+75);
		glVertex2d(x+30, y+90);
		glVertex2d(x+37, y+90);
		glVertex2d(x+37, y+85);
		glVertex2d(x+42, y+85);
		glVertex2d(x+42, y+90);
		glVertex2d(x+49, y+90);
		glVertex2d(x+49, y+85);
		glVertex2d(x+54, y+85);
		glVertex2d(x+54, y+90);
		glVertex2d(x+61, y+90);
		glVertex2d(x+61, y+85);
		glVertex2d(x+66, y+85);
		glVertex2d(x+66, y+90);
		glVertex2d(x+73, y+90);
		glVertex2d(x+73, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+45, y+75);
		glEnd();

		if(color == BLACK)
		{
			glColor3f(1, 1, 1); // Black Piece - White Outline
		}
		else
		{
			glColor3f(0, 0, 0); // White Piece - White Outline
		}
		glLineWidth(2);

		// Base-Outline of the Rook
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+20, y+10);
		glVertex2d(x+20, y+20);
		glVertex2d(x+25, y+18);
		glVertex2d(x+25, y+18);
		glVertex2d(x+75, y+18);
		glVertex2d(x+25, y+18);		
		glVertex2d(x+25, y+25);
		glVertex2d(x+75, y+25);
		glVertex2d(x+25, y+25);	
		glVertex2d(x+40, y+30);
		glVertex2d(x+60, y+30);
		glVertex2d(x+75, y+25);
		glVertex2d(x+75, y+18);
		glVertex2d(x+80, y+20);
		glVertex2d(x+80, y+10);
		glVertex2d(x+20, y+10);
		glEnd();

		// Neck-Outline of the Rook
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+40, y+30);
		glVertex2d(x+45, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+60, y+30);
		glVertex2d(x+40, y+30);
		glEnd();

		// Head-Outline of the Rook
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+45, y+75);
		glVertex2d(x+30, y+75);
		glVertex2d(x+30, y+90);
		glVertex2d(x+37, y+90);
		glVertex2d(x+37, y+85);
		glVertex2d(x+42, y+85);
		glVertex2d(x+42, y+90);
		glVertex2d(x+49, y+90);
		glVertex2d(x+49, y+85);
		glVertex2d(x+54, y+85);
		glVertex2d(x+54, y+90);
		glVertex2d(x+61, y+90);
		glVertex2d(x+61, y+85);
		glVertex2d(x+66, y+85);
		glVertex2d(x+66, y+90);
		glVertex2d(x+73, y+90);
		glVertex2d(x+73, y+75);
		glVertex2d(x+45, y+75);
		glEnd();

		glFlush();
	}

	void drawQueen(int x, int y, int color)
	{
		if(color == BLACK)
		{
			glColor3f(0, 0, 0); // Black Piece
		}
		else
		{

			glColor3f(1, 1, 1); // White Piece
		}
		// Base of the Queen
		glBegin(GL_POLYGON);
		glVertex2d(x+20, y+10);
		glVertex2d(x+20, y+20);
		glVertex2d(x+25, y+18);
		glVertex2d(x+25, y+25);
		glVertex2d(x+40, y+30);
		glVertex2d(x+60, y+30);
		glVertex2d(x+75, y+25);
		glVertex2d(x+75, y+18);
		glVertex2d(x+80, y+20);
		glVertex2d(x+80, y+10);
		glVertex2d(x+20, y+10);
		glEnd();

		// Neck of the Queen
		glBegin(GL_POLYGON);
		glVertex2d(x+40, y+30);
		glVertex2d(x+45, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+60, y+30);
		glVertex2d(x+40, y+30);
		glEnd();

		// Head of the Queen
		glBegin(GL_POLYGON);
		glVertex2d(x+45, y+75);
		glVertex2d(x+39, y+75);
		glVertex2d(x+41, y+80);
		glVertex2d(x+38, y+86);
		glVertex2d(x+45, y+86);
		glVertex2d(x+50, y+98);
		glVertex2d(x+55, y+86);
		glVertex2d(x+62, y+86);
		glVertex2d(x+59, y+80);
		glVertex2d(x+61, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+45, y+75);
		glEnd();

		if(color == BLACK)
		{
			glColor3f(1, 1, 1); // Black Piece - White Outline
		}
		else
		{
			glColor3f(0, 0, 0); // White Piece - White Outline
		}
		glLineWidth(2);

		// Base-Outline of the Queen
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+20, y+10);
		glVertex2d(x+20, y+20);
		glVertex2d(x+25, y+18);
		glVertex2d(x+25, y+18);
		glVertex2d(x+75, y+18);
		glVertex2d(x+25, y+18);		
		glVertex2d(x+25, y+25);
		glVertex2d(x+75, y+25);
		glVertex2d(x+25, y+25);	
		glVertex2d(x+40, y+30);
		glVertex2d(x+60, y+30);
		glVertex2d(x+75, y+25);
		glVertex2d(x+75, y+18);
		glVertex2d(x+80, y+20);
		glVertex2d(x+80, y+10);
		glVertex2d(x+20, y+10);
		glEnd();

		// Neck-Outline of the Queen
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+40, y+30);
		glVertex2d(x+45, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+60, y+30);
		glVertex2d(x+40, y+30);
		glEnd();

		// Head-Outline of the Queen
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+45, y+75);
		glVertex2d(x+39, y+75);
		glVertex2d(x+41, y+80);
		glVertex2d(x+38, y+86);
		glVertex2d(x+45, y+86);
		glVertex2d(x+50, y+98);
		glVertex2d(x+55, y+86);
		glVertex2d(x+62, y+86);
		glVertex2d(x+59, y+80);
		glVertex2d(x+61, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+45, y+75);
		glEnd();

		glFlush();
	}

	void drawKing(int x, int y, int color)
	{
		if(color == BLACK)
		{
			glColor3f(0, 0, 0); // Black Piece
		}
		else
		{
			glColor3f(1, 1, 1); // White Piece
		}

		// Base of the King
		glBegin(GL_POLYGON);
		glVertex2d(x+20, y+10);
		glVertex2d(x+20, y+20);
		glVertex2d(x+25, y+18);
		glVertex2d(x+25, y+25);
		glVertex2d(x+40, y+30);
		glVertex2d(x+60, y+30);
		glVertex2d(x+75, y+25);
		glVertex2d(x+75, y+18);
		glVertex2d(x+80, y+20);
		glVertex2d(x+80, y+10);
		glVertex2d(x+20, y+10);
		glEnd();

		// Neck of the King
		glBegin(GL_POLYGON);
		glVertex2d(x+40, y+30);
		glVertex2d(x+45, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+60, y+30);
		glVertex2d(x+40, y+30);
		glEnd();

		// Head of the King
		glBegin(GL_POLYGON);
		glVertex2d(x+45, y+75);
		glVertex2d(x+39, y+75);
		glVertex2d(x+41, y+80);
		glVertex2d(x+38, y+86);
		glVertex2d(x+62, y+86);
		glVertex2d(x+59, y+80);
		glVertex2d(x+61, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+45, y+75);
		glEnd();

		// Crown of the King
		glBegin(GL_POLYGON);
		glVertex2d(x+47, y+86);
		glVertex2d(x+47, y+90);
		glVertex2d(x+41, y+90);
		glVertex2d(x+42, y+93);
		glVertex2d(x+41, y+96);
		glVertex2d(x+47, y+96);
		glVertex2d(x+47, y+100);
		glVertex2d(x+50, y+99);
		glVertex2d(x+53, y+100);
		glVertex2d(x+53, y+96);
		glVertex2d(x+59, y+96);
		glVertex2d(x+58, y+93);
		glVertex2d(x+59, y+90);
		glVertex2d(x+53, y+90);
		glVertex2d(x+53, y+86);		
		glVertex2d(x+47, y+86);			
		glEnd();

		if(color == BLACK)
		{
			glColor3f(1, 1, 1); // Black Piece - White Outline
		}
		else
		{
			glColor3f(0, 0, 0); // White Piece - White Outline
		}
		glLineWidth(2);

		// Base-Outline of the King
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+20, y+10);
		glVertex2d(x+20, y+20);
		glVertex2d(x+25, y+18);
		glVertex2d(x+25, y+18);
		glVertex2d(x+75, y+18);
		glVertex2d(x+25, y+18);		
		glVertex2d(x+25, y+25);
		glVertex2d(x+75, y+25);
		glVertex2d(x+25, y+25);	
		glVertex2d(x+40, y+30);
		glVertex2d(x+60, y+30);
		glVertex2d(x+75, y+25);
		glVertex2d(x+75, y+18);
		glVertex2d(x+80, y+20);
		glVertex2d(x+80, y+10);
		glVertex2d(x+20, y+10);
		glEnd();

		// Neck-Outline of the King
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+40, y+30);
		glVertex2d(x+45, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+60, y+30);
		glVertex2d(x+40, y+30);
		glEnd();

		// Head-Outline of the King
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+45, y+75);
		glVertex2d(x+39, y+75);
		glVertex2d(x+41, y+80);
		glVertex2d(x+38, y+86);
		glVertex2d(x+45, y+86);
		glVertex2d(x+50, y+94);
		glVertex2d(x+55, y+86);
		glVertex2d(x+62, y+86);
		glVertex2d(x+59, y+80);
		glVertex2d(x+61, y+75);
		glVertex2d(x+55, y+75);
		glVertex2d(x+45, y+75);
		glEnd();

		// Crown-Outline of the King
		glBegin(GL_LINE_LOOP);
		glVertex2d(x+47, y+86);
		glVertex2d(x+47, y+90);
		glVertex2d(x+41, y+90);
		glVertex2d(x+42, y+93);
		glVertex2d(x+41, y+96);
		glVertex2d(x+47, y+96);
		glVertex2d(x+47, y+100);
		glVertex2d(x+50, y+99);
		glVertex2d(x+53, y+100);
		glVertex2d(x+53, y+96);
		glVertex2d(x+59, y+96);
		glVertex2d(x+58, y+93);
		glVertex2d(x+59, y+90);
		glVertex2d(x+53, y+90);
		glVertex2d(x+53, y+86);		
		glVertex2d(x+47, y+86);		
		glEnd();

		glFlush();
	}
};

int main()
{
	int width, height;

	// Initialize OpenGL
	int temp_argc = 0;
	glutInit(&temp_argc, NULL);
	
    // Dimensions of the window
	width = 820;
	height = 820;

	// Window with overloaded constructor
	Window window = Window(width, height);
	window.init();

	Board chessboard = Board(window);
	ChessPieces pieces = ChessPieces(squares);
	char label_start, label_end;
	int index_start, index_end;

	while(1)
	{
		cout<<"\n\nCurrent State: \n";
		for(int p=0; p<8; p++)
		{
			cout<<"\n";
			for(int q=0; q<8; q++)
			{
				cout<<squares[p][q]<<"("<<p<<q<<")\t";
			}
		}

		cout<<"\n\nMove : "<<move_count;
		cout<<"\nEnter the label and index of the chess piece:\n> ";
		cin>>label_start;
		if(label_start == 'x' || label_start == 'X')
			break;
		cin>>index_start;
		cout<<"Enter the label and index to move the chess piece:\n> ";
		cin>>label_end>>index_end;

		chessboard.makeMove(squares, label_start, index_start, label_end, index_end);

		for(int p=0; p<8; p++)
		{
			cout<<"\n";
			for(int q=0; q<8; q++)
			{
				cout<<squares[p][q]<<"("<<p<<q<<")\t";
			}
		}
		chessboard = Board(window);
		pieces = ChessPieces(squares);
	}
	if(glutGetWindow()) 			
		glutDestroyWindow(glutGetWindow());
	return 0;
}