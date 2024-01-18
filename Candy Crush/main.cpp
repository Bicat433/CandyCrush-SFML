#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <string.h>
#include<sstream>
using namespace std;
using namespace sf;


struct building
{
  int x,y,col,row,forrand,match,kami;
  //initialisation of struct
  building(){match=0;kami=255;}
} blocks[12][12];


void assigningposition();
void swapping(building,building);
int main()
{

  int ts = 54; //tile size
  Vector2i offset(48,24);
  int score=0;

  int xtemp,ytemp,x,y;
  int temp=0;
  int menu_choice=0;
  int points=0;
  string temp_score;
  Vector2i position;
  bool swappy=0,movy=0;

  assigningposition();
  //font for score
  Font font;
  font.loadFromFile("fonts/Oxygen-regular");
  Text text_score;
  text_score.setFont(font);
  text_score.setCharacterSize(40);
  text_score.setColor(Color::Black);
  // text_score.setStyle(Text::Bold);
  text_score.setPosition(740 - 75, 480 - 410);
  RenderWindow app(VideoMode(740,480), "Manageria");
  app.setFramerateLimit(120);

  //timer
  Time micro = microseconds(10000);
  Time milli = milliseconds(10);
  Time second = seconds(0.01f);
  Clock clock;
  
  Texture t1,t2,t3;
  t1.loadFromFile("sprites/background.png");
  t2.loadFromFile("sprites/naruto.png");
  t3.loadFromFile("sprites/menu.png");
  Sprite background(t1),gems(t2),menu(t3);

  while(app.isOpen())
  {

    Event ball;
    while(app.pollEvent(ball))
    {
      
      if(ball.type == Event::Closed)
      {
        app.close();
      }
      if(ball.type == Event::MouseButtonPressed)
      {
        if(ball.key.code==Mouse::Left)
        {
          if(!swappy && !movy)
          {
            temp++;
            position= Mouse::getPosition(app)-offset;
          }
        }
      }
      if(ball.type == Event::KeyPressed)
      {
        if(ball.key.code == Keyboard::Enter)
        {
          menu_choice=1;
        }
      }
      if(ball.type == Event::KeyPressed)
      {
        if(ball.type==Keyboard::Escape)
        {
          clock.restart();
        }
      }
    }
    if(score>10000)   //score setting
    {
      app.close();
    }


    if(menu_choice==1)
  {

    app.clear();
    //clicking of mouse
    if(temp==1)
    {
      xtemp=position.x/ts+1;
      ytemp=position.y/ts+1;
    }
    if(temp==2)
    {
      x=position.x/ts+1;
      y=position.y/ts+1;
      if(abs(x-xtemp)+abs(y-ytemp)==1)
      {
        swapping(blocks[ytemp][xtemp],blocks[y][x]);
        temp=0;
        swappy=1;
      }
      else
      {
        temp=1;
      }
    }

    //when match will be found

    for(int i=1;i<=8;i++)
    {
      for(int j=1;j<=8;j++)
      {
        if(blocks[i][j].forrand==blocks[i+1][j].forrand)
        {
          if(blocks[i][j].forrand==blocks[i-1][j].forrand)
          {
            for(int n=-1;n<=1;n++)
            {
              score++;
              temp_score=to_string(score);
              blocks[i+n][j].match++;
            }
          }
        }
        if(blocks[i][j].forrand==blocks[i][j+1].forrand)
        {
          if(blocks[i][j].forrand==blocks[i][j-1].forrand)
          {
            for(int n=-1;n<=1;n++)
            {
              score++;
              temp_score=to_string(score);
              blocks[i][j+n].match++;
            }
          }
        }
      }
    }
    

    //movement
    movy=0;
    for(int i=1;i<=8;i++)
    {
      for(int j=1;j<=8;j++)
      {
        building &b = blocks[i][j];
        int d_ix,d_iy;
        for(int n=0;n<4;n++)
        {
        d_ix = b.x-b.col*ts;
        d_iy = b.y-b.row*ts;
        if(d_ix)
        {
          b.x-=d_ix/abs(d_ix);
        }
        if(d_iy)
        {
          b.y-=d_iy/abs(d_iy);
        }
        }
        if(d_ix||d_iy)
        {
          movy=1;
        }
      }
    }
    //deleting

    if(!movy)
    {
      for(int i=1;i<=8;i++)
      {
        for(int j=1;j<=8;j++)
        {
          if(blocks[i][j].match)
          {
            if(blocks[i][j].kami>10)
            {
              blocks[i][j].kami-=10;
              movy=1;
            }
          }
        }
      }
    }
    //getting score
    points=0;
    for(int i=1;i<=8;i++)
    {
      for(int j=1;j<=8;j++)
      {
        points=points+blocks[i][j].match;
      }
    }
    //second swap
    if(swappy && !movy)
    {
      if(!points)
      swapping(blocks[ytemp][xtemp],blocks[y][x]);
      swappy=0;
    }
    
    //Bringing in more blocks

    if(!movy)
    {
      for(int i=8;i>0;i--)
      {
        for(int j=1;j<=8;j++)
        {
          if(blocks[i][j].match)
          {
            for(int n=i;n>0;n--)
            {
              if(!blocks[n][j].match)
              {
                swapping(blocks[n][j],blocks[i][j]);
                break;
              }
            }
          }
        }
      }
      for(int j=1;j<=8;j++)
      {
        for(int i=8,n=0;i>0;i--)
        {
          if(blocks[i][j].match)
          {
            blocks[i][j].forrand=rand()%7;
            blocks[i][j].y = -ts*n++;
            blocks[i][j].match=0;
            blocks[i][j].kami=255;
          }
        }
      }
    }

    //drawing of the map
    app.draw(background);

    for(int i=1;i<=8;i++)
    {
      for(int j=1;j<=8;j++)
      {
        building b = blocks[i][j];
        gems.setTextureRect(IntRect(b.forrand*49,0,49,49));   // set texture of sprites.
        gems.setColor(Color(225,225,255,225));
        gems.setPosition(b.x,b.y);
        gems.move(offset.x-ts , offset.y-ts);
        app.draw(gems);
        text_score.setString(temp_score);
        app.draw(text_score);
        Time elapsed = clock.getElapsedTime();        // time clock.
        cout<<elapsed.asSeconds()<<endl;
      }
    }
    
    app.display();
  }
    else
    {
      app.draw(menu);
      app.display();
    }
  }
  return 0;
}
void swap1(int &a,int &b)
{
    int temp=0;
    temp=a;
    a=b;
    b=temp;

}
void swapping(building b1,building b2)
{

  swap1(b1.col,b2.col);
  // b1.col=temp_col;
  // b1.col=b2.col;
  // b2.col=temp_col;

  swap1(b1.row,b2.row);
  // b1.row=temp_row;
  // b1.row=b2.row;
  // b2.row=temp_row;

  blocks[b1.row][b1.col]=b1;
  blocks[b2.row][b2.col]=b2;
}
void assigningposition()
{
  int ts=54;
  srand(time(0));
  for(int i=1;i<=8;i++)
  {
    for(int j=1;j<=8;j++)
    {
      blocks[i][j].forrand=rand()%7;    
      blocks[i][j].col=j;
      blocks[i][j].row=i;
      blocks[i][j].x = j*ts;
      blocks[i][j].y = i*ts;
    }
  }
}
