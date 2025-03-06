#include<iostream> //used as a stream of input and output
#include<conio.h>  //Perform console input and console output operations (like getch( ) to get character from the keyboard)
#include<dos.h> //contains functions for handling interrupts, producing sound, date and time functions
#include<Windows.h> // contains all the common macros used by functions and subsystems
#include<time.h> // performs functions related to date and time
#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define MENU_WIDTH 20
#define GAP_SIZE 7
#define ENEMY_DIF 45

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);		//FUNCTION PROVIDES MACHANISM FOR RETRIVING THE STANDARD OUTPUT
COORD CursorPosition;

int enemyY[3];									//HEIGHT OF ENEMY
int enemyX[3];									//WIDTH OF ENEMY
int enemyFlag[3];
char bird[3][5] = { ' ', ' ', '+', ' ', ' ',	//DESIGN OF BIRD
'|', '+', '+', '+', '|',
'+', '+', '+', '+', '+' };
int birdpos = WIN_WIDTH / 2;
int score = 0;
int bullets[20][4];
int bulletlife[20];
int bindex = 0;

void gotoxy(int x, int y){						//TO TAKE POSITION OF CURSOR AND ENEMY OR BIRD
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void SetCursor(bool visible, DWORD size){
	if (size == 0)
		size = 20;

	CONSOLE_CURSOR_INFO lpcursor;
	lpcursor.bVisible = visible;
	lpcursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpcursor);
}

void drawborder(){
	for (int i = 0; i < SCREEN_WIDTH; i++){
		gotoxy(i, SCREEN_HEIGHT);
		cout << "*";
	}
	for (int i = 0; i < SCREEN_HEIGHT; i++){
		gotoxy(0, i);
		cout << "*";
		gotoxy(SCREEN_WIDTH, i);
		cout << "*";
	}
	for (int i = 0; i < SCREEN_HEIGHT; i++){
		gotoxy(SCREEN_WIDTH, i);
		cout << "*";
	}
}

void genenemy(int ind){							//ENEMY GENERATION
	enemyX[ind] = 1 + rand() % (WIN_WIDTH - 10);		//RANDOM VISIBLE OF ENEMY

}
void drawenemy(int ind){						//DRAWING ENEMY
	if (enemyFlag[ind] == TRUE){
		gotoxy(enemyX[ind], enemyY[ind]);    cout << " ** ";
		gotoxy(enemyX[ind], enemyY[ind] + 1);  cout << "****";
		gotoxy(enemyX[ind], enemyY[ind] + 2);  cout << "****";
		gotoxy(enemyX[ind], enemyY[ind] + 3);  cout << " ** ";
	}
}
void eraseenemy(int ind){						//ERASE ENEMY
	if (enemyFlag[ind] == TRUE){
		gotoxy(enemyX[ind], enemyY[ind]);    cout << "    ";
		gotoxy(enemyX[ind], enemyY[ind] + 1);  cout << "    ";
		gotoxy(enemyX[ind], enemyY[ind] + 2);  cout << "    ";
		gotoxy(enemyX[ind], enemyY[ind] + 3);  cout << "    ";
	}
}

void resetenemy(int ind){   //WHEN AN ENEMY IS OUT RESETING ENEMY
	eraseenemy(ind);
	enemyY[ind] = 4;
	genenemy(ind);
}
void generatebullet(){				//GENERATING BULLETS
	bullets[bindex][0] = 22;
	bullets[bindex][1] = birdpos;
	bullets[bindex][2] = 22;
	bullets[bindex][3] = birdpos + 4;
	bindex++;
	if (bindex == 20)
		bindex = 0;
}
void movebullets(){						//MOVING BULLETS FROM DOWN TO UP
	for (int i = 0; i < 20; i++){
		if (bullets[i][0]>2)
			bullets[i][0]--;
		else
			bullets[i][0] = 0;
		if (bullets[i][2]>2)
			bullets[i][2]--;
		else
			bullets[i][2] = 0;
	}
}
void drawbullets(){						//DRAW BULLETS
	for (int i = 0; i < 20; i++){
		if (bullets[i][0]>1){
			gotoxy(bullets[i][1], bullets[i][0]); cout << "^";
			gotoxy(bullets[i][3], bullets[i][2]); cout << "^";
		}
	}
}
void erasebullets(){					//ERASE BULLETS WHEN BOTH ARE HIT
	for (int i = 0; i < 20; i++){
		if (bullets[i][0] >= 1){
			gotoxy(bullets[i][1], bullets[i][0]); cout << " ";
			gotoxy(bullets[i][3], bullets[i][2]); cout << " ";
		}
	}
}
void erasebullet(int i){
	gotoxy(bullets[i][1], bullets[i][0]); cout << " ";
	gotoxy(bullets[i][3], bullets[i][2]); cout << " ";
}
void drawbird(){					//DRAW FIRING BIRD
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 5; j++){
			gotoxy(j + birdpos, i + 22); cout << bird[i][j];
		}
	}
}
void erasebird(){					//ERASE BIRD WHEN  HIT
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 5; j++){
			gotoxy(j + birdpos, i + 22); cout << " ";
		}
	}
}

int collision(){					//WHEN ENEMY HIT BIRD GAME SHOULD END.   THIS WILL HAPPEN WHEN COLLISION GOT " 1 "
	if (enemyY[0] + 4 >= 23){			//when 1st enemy hit game ends
		if (enemyX[0] + 4 - birdpos >= 0 && enemyX[0] + 4 - birdpos < 8){
			return 1;
		}
	}
	if (enemyY[1] + 4 >= 23){			//when 2nd enemy hit game ends
		if (enemyX[1] + 4 - birdpos >= 0 && enemyX[1] + 4 - birdpos < 8){
			return 1;
		}
	}
	return 0;
}

int bullethit(){						//when a bullet hit enemy, enemy should reset automatically
	for (int i = 0; i < 20; i++){
		for (int j = 0; j < 4; j += 2){
			if (bullets[i][j] != 0){
				if (bullets[i][j] >= enemyY[0] && bullets[i][j] <= enemyY[0] + 4){
					if (bullets[i][j + 1] >= enemyX[0] && bullets[i][j + 1] <= enemyX[0] + 4){
						erasebullet(i);
						bullets[i][j] = 0;
						resetenemy(0);
						return 1;
					}
				}
				if (bullets[i][j] >= enemyY[1] && bullets[i][j] <= enemyY[1] + 4){
					if (bullets[i][j + 1] >= enemyX[1] && bullets[i][j + 1] <= enemyX[1] + 4){
						erasebullet(i);
						bullets[i][j] = 0;
						resetenemy(1);
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

void gameover(){
	system("cls");								//DISPLAYS WHEN GAME END
	cout << "\tscore is " << score<<endl;
	cout << "\t-------------------------\n";
	cout << "\t--------Game Over--------\n";
	cout << "\t-------------------------\n";
	cout << "\tPress any key to go back";
	getch();
}
void updatescore(){								//DISPLAYING SCORE WITH UPDATE
	gotoxy(WIN_WIDTH + 7, 5);
	cout << "score: " << score << endl;
}
void instructions(){							//INSTRUCTIONS IN MAIN MENU
	system("cls");
	cout << "Instructions";
	cout << "\n___________________";
	cout << "\nPress space bar to make a shot";
	cout << "\n\nPress any key to go back";
	getch();
}

void play(){							//PLAYING CODE

	birdpos = -1 + WIN_WIDTH / 2;		//BIRD POSITION IN MIDDLE
	score = 0;
	enemyFlag[0] = 1;
	enemyFlag[1] = 1;
	enemyY[0] = enemyY[1] = 4;

	for (int i = 0; i < 20; i++)
	{
		bullets[i][0] = bullets[i][1] = 0;
	}

	system("cls");
	drawborder();
	genenemy(0);
	genenemy(1);
	updatescore();

	gotoxy(WIN_WIDTH + 5, 2); cout << "FIRING CONSOLE";		//INSTRUCTIONS AND DIPLAYS IN GAMEING TIME.
	gotoxy(WIN_WIDTH + 6, 4); cout << "______________";
	gotoxy(WIN_WIDTH + 6, 6); cout << "______________";
	gotoxy(WIN_WIDTH + 7, 12); cout << "control";
	gotoxy(WIN_WIDTH + 7, 13); cout << "______________";
	gotoxy(WIN_WIDTH + 2, 14); cout << "A key    -   left";
	gotoxy(WIN_WIDTH + 2, 15); cout << "D key    -   right";
	gotoxy(WIN_WIDTH + 2, 16); cout << "spacebar -   shoot";

	gotoxy(10, 5); cout << "Press any key to start";
	getch();
	gotoxy(10, 5); cout << "                      ";

	while (1){
		if (kbhit()){
			char ch = getch();
			if (ch == 'a' || ch == 'A'){				// PRESS 'A' TO MOVE LEFT
				if (birdpos > 5)
					birdpos -= 5;
			}
			if (ch == 'd' || ch == 'D'){				//PRESS 'D' TO MOVE RIGHT
				if (birdpos < WIN_WIDTH - 7)
					birdpos += 5;
			}
			if (ch == 32){								//PRESS SPACE TOGENERATE BULLETS
				generatebullet();
			}
			if (ch == 27){								// ESC TO BREAK
				break;
			}
		}
		drawbird();
		drawenemy(0);
		drawenemy(1);
		drawbullets();
		if (collision() == 1){
			gameover();
			return;
		}
		if (bullethit() == 1){
			score++;
			updatescore();
		}
		Sleep(200);
		erasebird();
		eraseenemy(0);
		eraseenemy(1);
		erasebullets();
		movebullets();

		if (enemyFlag[0] == 1)
			enemyY[0] += 1;
		if (enemyFlag[1] == 1)
			enemyY[1] += 1;
		if (enemyY[0] > SCREEN_HEIGHT - 5){
			resetenemy(0);
		}
		if (enemyY[1] > SCREEN_HEIGHT - 5){
			resetenemy(1);
		}
	}
}

int main(){

	SetCursor(0, 0);
	srand((unsigned)time(NULL));

	do{
		system("cls");
		gotoxy(10, 5); cout << "-------------------------";			//FIRST PAGE
		gotoxy(10, 6); cout << "|_____FIRING CONSOLE____|";
		gotoxy(10, 7); cout << "-------------------------";
		gotoxy(10, 9); cout << "1. Start Game";
		gotoxy(10, 10); cout << "2. Instructions";
		gotoxy(10, 11); cout << "3. Quit";
		gotoxy(10, 13); cout << "Select option : ";
		char op = getche();

		if (op == '1')							//MAIN MENU SELECTION
			play();
		else if (op == '2')
			instructions();
		else if (op == '3') exit(0);
	} while (1);

	return 0;
}

