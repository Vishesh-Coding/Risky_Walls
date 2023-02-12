#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<time.h>
#include<stdio.h> 

#define ScWidth 1400
#define ScHeight 700
#define OpWidth 400
#define SPACEBAR 32

using namespace std;

// Global Variables
char option, txtScore[5];
int score = 1;
int gameRunning = 1;

void GameOver(){
	cleardevice(); // clear screen

	// Print Game Over Text
	settextstyle(4,0,9);
	outtextxy(300,300,"Game Over");

	settextstyle(4,0,2);
	outtextxy(500,420,"Press any key to quit...");
	getch();
	score = 0; // reset score
}

int main(){
	srand((unsigned) time(0));

	int gd = DETECT, gm, err, tmp;
	initgraph( & gd, & gm, "C:\\tc\\bgi");

	// Enemy Variables
	// Enemy (x, y, height, type)
	int Enemy[4][4] = { {0,0,0,0}, {0,0,0,1}, {0,0,0,0}, {0,0,0,1} };
	// ei-Enemy Index, eGap-Enemy Gap, ew-Enemy Width, eStep-Enemy Step
	int ei = 1, eGap = 100, ew = 20, eStep = 5;

	// Fruit Variables
	// Fruit (x,y,IsHidden), fSize - Fruit Size
	int Fruit[4][3] = {{0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}};
	int fSize = 20;

	// Player Variables
	// px-Player X, py-Player Y, pSize-Player Size, pStep-Player Step
	int px = (ScWidth-400)/2, py = 300, pSize = 20, pStep = 10;

	// Generate Enemies
	for(int i=0; i<4; i++){
	    Enemy[i][0] = ScWidth-OpWidth; // set Initial X Pos of Enemy
		Fruit[i][0] = Enemy[i][0];     // set Initial X Pos of Enemy
		if( i%2 == 0 ){ // for enemies
			Enemy[i][1] = ScHeight/3 + rand()%(ScHeight/3); // generate random y position of enemy
			Enemy[i][2] = ScHeight - Enemy[i][1]; // set height of enemy

			Fruit[i][1] = Enemy[i][1] - 70; // set Y position of fruit
		}else{ // for odd enemies
		    Enemy[i][1] = 0; // set initial position of enemy
			Enemy[i][2] = ScHeight/3 + rand()%(ScHeight/3); // set random height of odd enemies

			Fruit[i][1] = Enemy[i][1]+Enemy[i][2] + 50; // set Y position of fruit
		}	
	}


	while(gameRunning){
		setcolor(WHITE);
		//draw board
		rectangle(0,0,ScWidth-OpWidth,ScHeight);
		rectangle(ScWidth-OpWidth,0,ScWidth,ScHeight);
		// print score
		// itoa function converts number to character array
		settextstyle(4,0,9);
		if( score<10 )
			outtextxy(ScWidth-270, 250, itoa(score,txtScore,10));
		else if( score>=10 )
			outtextxy(ScWidth-320, 250, itoa(score,txtScore,10));

		settextstyle(4,0,1);
		outtextxy(ScWidth-375, 500, "Use Spacebar to Jump"); 

		//Draw Enemies & Fruit
		for(int i=0; i<ei; i++){

		    // Draw Fruit if fruit is not already taken
			if( Fruit[i][1] != 1){	
			    setcolor(LIGHTBLUE);	
			    setfillstyle(1, LIGHTBLUE);
			    bar(Fruit[i][0], Fruit[i][1], Fruit[i][0]+fSize, Fruit[i][1]+fSize);
			}

			//Draw Enemy
		    setfillstyle(1, YELLOW);
		    bar(Enemy[i][0], Enemy[i][1], Enemy[i][0]+ew, Enemy[i][1]+Enemy[i][2]);

			Enemy[i][0] -= eStep; // update enemy position 
 			Fruit[i][0] -= eStep; // update fruit position  

 			// if enemy covers 1/4 distance generate new enemy
			if( Enemy[i][0] <= 750 && Enemy[i][0] > 750-eStep ){
				if( ei < 4 ) // generate enemy if enemies are less than 4
					ei++; // increment enemy index
				break;
			}

			// if enemy goes through left border of screen
			if( Enemy[i][0] < 0 ){
				Enemy[i][0] = ScWidth-OpWidth; //reset enemy x postion
				Fruit[i][0] = Enemy[i][0];  // reset fruit
				Fruit[i][2] = 0; // reset enemy state

				// for even enemies generate random enemy & Fruit
				if( i%2 == 0 ){ 
					Enemy[i][1] = ScHeight/3 + rand()%(ScHeight/3);
					Enemy[i][2] = ScHeight - Enemy[i][1];

					Fruit[i][1] = Enemy[i][1] - 70;
				}else{ //for odd enemies generate random enemy & Fruit
				    Enemy[i][1] = 0;
					Enemy[i][2] = ScHeight/3 + rand()%(ScHeight/3);

					Fruit[i][1] = Enemy[i][1]+Enemy[i][2] + 50;
				}	
			}
		}
		if(kbhit()){ // if any key is pressed
			option = getch(); // catch character in options variable
			if(option == SPACEBAR){ // if right arrow key is pressed
				if( py > 10){ // restrict player on left border
					py -= pStep*10;
				}
			}
		}

		// Draw Player  
		setcolor(WHITE);
		setfillstyle(1,WHITE);
		bar(px,py,px+pSize,py+pSize);

		// Player hit Wall
		for(int i=0; i<4; i++){
			if( px+pSize > Enemy[i][0] && px < Enemy[i][0]+ew ){ 
				if( Enemy[i][3] == 0 ){ // for even enemies
					if( py >= Enemy[i][1]){
						GameOver();
						gameRunning = 0;
						break;
					}
				}else{  // for odd enemies
					if( py <= Enemy[i][1]+Enemy[i][2] ){
						GameOver();
						gameRunning = 0;
						break;
					}
				}
			}
		}

		// Player hit Fruit
		for(int i=0; i<4; i++){
			if( Fruit[i][1] != 1){
				if( px+pSize > Fruit[i][0] && px < Fruit[i][0]+ew ){ 
					if( Enemy[i][3] == 0 ){
						if( py >= Fruit[i][1]){
							score += 5;
							Fruit[i][1] = 1;
						}
					}else{ 
						if( py <= Fruit[i][1]+Fruit[i][2] ){
							score += 5;
							Fruit[i][1] = 1;
						}
					}
				}
			}
		}

		// if player goes avoid a hurdle then increment score
		for(int i=0; i<4; i++){
			if( px+pSize == Enemy[i][0] ){ 
				if( Enemy[i][3] == 0 ){
					if( py < Enemy[i][1]){
						score++;
					}
				}else{ 
					if( py > Enemy[i][1]+Enemy[i][2] ){
						score++;
					}
				}
			}
		}

		// for free fall motion of player
 		if( py <= 700 ){
 			py += pStep;
 		}else{ // if player hit ground then call gameover method
 			GameOver();
			gameRunning = 0;
			break;
 		}

		delay(40);
		cleardevice();
	}

	getch();
	closegraph();
}
