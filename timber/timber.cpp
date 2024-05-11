////////////////////////////////////////////////////////////////////////////////////////////////

//  g++ timber.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 

// for all the modules

//   g++ timber.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio  -lsfml-network

//////////////////////////////////////////////////////////////////////////////////////////////////

// include important libraries 
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<sstream>

using namespace std;
using namespace sf;

// function declaration 
void updateBranchs(int seed);

const int NumberOfBranches = 6;

Sprite branches[NumberOfBranches]; //six Sprite instances.

// where is the player/branch
// left or right 
enum class side {LEFT , RIGHT , NONE};
side branchPosition[NumberOfBranches];
//side types with a size of NUM_BRANCHES (6).


int main(){

    // video mode object 
    VideoMode vm(1600, 1080);
    // VideoMode vm(1200 , 800);

    // create and open window for game 
    RenderWindow window(vm , "Timber Game"); // , Style::Fullscreen

    //create texture to holds a graphic on the GPU
    Texture textureBackground;

    // load graphics (bg) into the texture 
    textureBackground.loadFromFile("graphics/background.png");

    // create sprite 
    Sprite spriteBackground;

    // attach texture to the sprite
    spriteBackground.setTexture(textureBackground);

    // SET SPRITEBACKGROUND  to cover the screen 
    spriteBackground.setPosition(0,0);

    //---------------------------- make tree sprite 
    Texture texturetree;
    texturetree.loadFromFile("graphics/tree.png");
    Sprite spritetree;
    spritetree.setTexture(texturetree);
    spritetree.setPosition(810,0);
    //----------------------------------------------
    //-------------------------- make bees , who moves
    Texture texturebee;
    texturebee.loadFromFile("graphics/bee.png");
    Sprite spritebee;
    spritebee.setTexture(texturebee);
    spritebee.setPosition(0,800);
       // is bee is currently moving 
       bool beeActive = false;

       // how fast can bee fly
       float beespeed = 0.0f;
    //--------------------------------------------------
    
    //---------------  clouds --------------------------
    // Making 3 clouds sprites from 1 texture
        Texture texturecloud;
        //  load 1 new texture
        texturecloud.loadFromFile("graphics/cloud.png");

        // 3 new sprites with same texture 
        Sprite spritecloud1;
        Sprite spritecloud2;
        Sprite spritecloud3;

        spritecloud1.setTexture(texturecloud);
        spritecloud2.setTexture(texturecloud);
        spritecloud3.setTexture(texturecloud);

        spritecloud1.setPosition(0,0);
        spritecloud2.setPosition(0,250);
        spritecloud3.setPosition(0,500);

        // are clouds currently on screen 
        bool cloud1Active = false;
        bool cloud2Active = false;
        bool cloud3Active = false;

        // how fast is cloud 
        float cloud1speed = 0.0f;
        float cloud2speed = 0.0f;
        float cloud3speed = 0.0f;
    //---------------------------------------------------------
    //---------  varaiable to control time itself
    Clock clock;

    // time bar 
    RectangleShape timebar;
    float timeBarStartwidth = 400;
    float timeBarHeight = 80;
    timebar.setSize(Vector2f(timeBarStartwidth ,timeBarHeight));
    timebar.setFillColor(Color::Red);
    timebar.setPosition((1920/2)-timeBarStartwidth/2 ,980);

    Time gametimetotal;
    float timeremaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartwidth / timeremaining;
    // we use some key for PAUSE and START button in middle of game
    // track game is running or not 
    bool paused = true;

    // ----------------------------- Draw some text 
    int score = 0;

    Text messagetext;
    Text scoretext;

    // choose font 
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    // set the font to our message 
    messagetext.setFont(font);
    scoretext.setFont(font);

    // ASsign actual message 
    messagetext.setString("Press Enter to start");
    scoretext.setString("Score = 0");

    // make it really big 
    messagetext.setCharacterSize(75);
    scoretext.setCharacterSize(100);

    // choose color 
    messagetext.setFillColor(Color::White);
    scoretext.setFillColor(Color::White);

    // position for text 
    FloatRect textrect = messagetext.getLocalBounds();

    messagetext.setOrigin(textrect.left + (textrect.width / 2.0f) , textrect.top + (textrect.height / 2.0f));
    
    messagetext.setPosition(1920/2.0f , 1080/2.0f);

    scoretext.setPosition(20 ,20);

    // preparing branches 
    Texture texturebranch;
    texturebranch.loadFromFile("graphics/branch.png");

    // set the texture of each branch sprite 
    for (int i = 0; i < NumberOfBranches; i++)
    {
        branches[i].setTexture(texturebranch);
        branches[i].setPosition(-2000,-20000);

        // Set the sprite's origin to dead centre
        // We can then spin it round without changing its position
        branches[i].setOrigin(220,20);
    }

    //---------- player
    Texture textureplayer;
    textureplayer.loadFromFile("graphics/player.png");
    Sprite spriteplayer;
    spriteplayer.setTexture(textureplayer);
    spriteplayer.setPosition(580 , 720);

    // player start on left 
    side playeside = side::LEFT;

    //Preapare gravestone RIP 
    // Prepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);
    // Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    //line the axe up with tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    // some other useful log related variable 
    bool logActive = false;
    float logSpeedx = 1000;
    float logspeedy = -1500;

    // acceptInput , which will be used to determine when to listen for chops and when to ignore them
    bool acceptINput = false;
    
    // ====== SOUNDS ====================
    // the player chopping sound
    SoundBuffer chopbuffer;
    chopbuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopbuffer);

    // The player has met his end under a branch
        SoundBuffer deathBuffer;
        deathBuffer.loadFromFile("sound/death.wav");
        Sound death;
        death.setBuffer(deathBuffer);

    // Out of time
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);

    // stay on screen till user not close 
    while (window.isOpen())
    {
        //------------------------  handle player input 
        // uses the method of detecting when a key is released
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
                // listen for key press again
                acceptINput = true;

                // hide the axe
                spriteAxe.setPosition(2000 , spriteAxe.getPosition().y);
            }
            
        }
        
        
        if(Keyboard::isKeyPressed(Keyboard::Escape)){
            window.close();
        }

        // start the game by keypress
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;

            // reset the time and the score 
            score =0;
            timeremaining=6;

            // make all branches disappear
            // start in second postion
            for (int i = 1; i < NumberOfBranches; i++)
            {
                branchPosition[i] = side::NONE;
            }

            // MAKE SURE the gravestone is hidden
            spriteRIP.setPosition(675,2000);

            // move the player into position 
            spriteplayer.setPosition(580,720);

            acceptINput = true;
            
        }

        // wrap player control to 
        // make sure we are aceepting input
        if (acceptINput)
        {
            // what happens when the player presses the right cursor key on the keyboard:
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                // make sure player is on right
                playeside = side::RIGHT;
                score++;

                // add to the amount of time remaining
                timeremaining +=(2/score) + .15;

                spriteAxe.setPosition(AXE_POSITION_RIGHT , spriteAxe.getPosition().y);
                spriteplayer.setPosition(1200 ,720);

                //update branches
                updateBranchs(score); // branches down one place and
                    //spawn a new random branch (or space) a

                //set the log flying to the left
                spriteLog.setPosition(810,720);
                logSpeedx=-5000;
                logActive=true;

                acceptINput=false;

                // play a chop sound 
                chop.play();
            }

            // handle the left cursor key
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                // make sure player is on right
                playeside = side::LEFT;
                score++;

                // add to the amount of time remaining
                timeremaining +=(2/score) + .15;

                spriteAxe.setPosition(AXE_POSITION_LEFT , spriteAxe.getPosition().y);
                spriteplayer.setPosition(580 ,720);

                //update branches
                updateBranchs(score); // branches down one place and
                    //spawn a new random branch (or space) a

                //set the log flying to the left
                spriteLog.setPosition(810,720);
                logSpeedx=5000;
                logActive=true;

                acceptINput=false;
            }
        }
        
        

        //-------------------------- update scenes 
            // if paused is truee then we play games
        if (!paused)
        {
            // measure time 
            // dt stands for delta time, which is the time between two updates.
            Time dt = clock.restart(); // restart evrytime

            // substract from amount of time remining
            timeremaining -=dt.asSeconds();

            // size up the time bar (resize)
            timebar.setSize(Vector2f(timeBarWidthPerSecond*timeremaining , timeBarHeight));

            if (timeremaining <= 0.0f)
            {
              // pause the game 
              paused = true;

              // change the message showm to the player
              messagetext.setString("Out of time !!");

              // reposition of text based on its new size

              FloatRect textrect = messagetext.getLocalBounds();
              messagetext.setOrigin(textrect.left + textrect.width / 2.0f , textrect.top + textrect.height / 2.0f);
              messagetext.setPosition(1920/2.0f , 1080/2.0f);

              // play the out of time sound 
              outOfTime.play();
            }
            

            // setup bee 
            if (!beeActive)
            {
                // how fast is bee 
                srand((int)time(0)); // random number from currnt time

                beespeed = (rand() % 200) + 200; //random number between 200 and 399
                
                /*
                    why 200 because 
                    1920 / 10  approx 200 pexel per second 
                    completed in 10 
                    10 sec taken by bee to go from to 1920 pexel
                */

               // how high is bee 
               srand((int)time(0)*10);
               float height = (rand() % 500) + 500; // random number between 500 and 999
               spritebee.setPosition(2000,height); //bee to 2000 on the x axis (just off-screen to the right , height any
               beeActive = true;
            
            } else {
                 // move bee 
                 spritebee.setPosition(spritebee.getPosition().x - (beespeed*dt.asSeconds()) , spritebee.getPosition().y);
                 
                 // has the bee reached left hand edge of screen
                 if (spritebee.getPosition().x < -100 )
                 {
                    // set it up ready to be a whole new bee next time
                    beeActive = false ;
                 }
                 
            }

            // moving  clouds
                // cloud1
                if(!cloud1Active){
                    //how fast is cloud 
                    srand((int)time(0) * 10);
                    cloud1speed = (rand() % 200); // why  200 as mention above

                    // how high is the cloud
                    srand((int)time(0)*10);
                    /*
                    ( * 10 ) to the result returned by time(0) so that we are
                    always guaranteed to get a different seed for each of the clouds.
                    */
                    float height = (rand() % 150);
                    spritecloud1.setPosition(-200 , height);
                    cloud1Active = true;
                }else {
                    spritecloud1.setPosition( spritecloud1.getPosition().x + (cloud1speed*dt.asSeconds()) , spritecloud1.getPosition().y);
                    
                    // has the cloud reached right hand edge of the screen 
                    if (spritecloud1.getPosition().x >1920)
                    {
                        // set it up ready to a whole new cloud
                        cloud1Active = false;
                    }   
                }

                // cloud2
                if(!cloud2Active){
                    //how fast is cloud 
                    srand((int)time(0) * 20);
                    cloud2speed = (rand() % 200); // why  200 as mention above

                    // how high is the cloud
                    srand((int)time(0)*20);
                    /*
                    ( * 10 ) to the result returned by time(0) so that we are
                    always guaranteed to get a different seed for each of the clouds.
                    */
                    float height = (rand() % 300)-150;
                    spritecloud2.setPosition(-200 , height);
                    cloud2Active = true;
                }else {
                    spritecloud2.setPosition( spritecloud2.getPosition().x + (cloud2speed*dt.asSeconds()) , spritecloud2.getPosition().y);
                    
                    // has the cloud reached right hand edge of the screen 
                    if (spritecloud2.getPosition().x >1920)
                    {
                        // set it up ready to a whole new cloud
                        cloud2Active = false;
                    }   
                }

                 // cloud3
                if(!cloud3Active){
                    //how fast is cloud 
                    srand((int)time(0) * 30);
                    cloud3speed = (rand() % 200); // why  200 as mention above

                    // how high is the cloud
                    srand((int)time(0)*30);
                    /*
                    ( * 10 ) to the result returned by time(0) so that we are
                    always guaranteed to get a different seed for each of the clouds.
                    */
                    float height = (rand() % 450)-150;
                    spritecloud3.setPosition(-200 , height);
                    cloud3Active = true;
                }else {
                    spritecloud3.setPosition( spritecloud3.getPosition().x + (cloud3speed*dt.asSeconds()) , spritecloud3.getPosition().y);
                    
                    // has the cloud reached right hand edge of the screen 
                    if (spritecloud3.getPosition().x >1920)
                    {
                        // set it up ready to a whole new cloud
                        cloud3Active = false;
                    }   
                }

                // update score text 
                stringstream ss;
                ss<<"Score = "<<score;
                scoretext.setString(ss.str());

                // update the branch sprite
                for (int i = 0; i < NumberOfBranches; i++)
                {
                    float height = i*150;
                    if (branchPosition[i]==side::LEFT)
                    {
                        //move the sprite to the left side
                        branches[i].setPosition(610,height);

                        // flip the sprite around the other way
                        branches[i].setRotation(180);
                    } else if (branchPosition[i]==side::RIGHT)
                    {
                        //move the sprite to the left side
                        branches[i].setPosition(1330,height);

                        // flip the sprite around the other way
                        branches[i].setRotation(0);
                    } else {
                        //hide the branch
                        branches[i].setPosition(3000,height);
                    }
                    
                }

                // handle a flying log
                if(logActive){
                    spriteLog.setPosition(spriteLog.getPosition().x+(logSpeedx*dt.asSeconds()),spriteLog.getPosition().y+(logspeedy*dt.asSeconds()));
                    // has the log reached right hand edge
                    if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
                        {
                        // Set it up ready to be a whole new log next frame
                        logActive = false;
                        spriteLog.setPosition(810, 720);
                        }                    
                    }
                
                // has the plaayer beeen squished 
                if (branchPosition[5] == playeside)
                {
                    /// death 
                    paused = true;
                    acceptINput=false;

                    // draw gravestone 
                    spriteRIP.setPosition(525,760);

                    // hide the player 
                    spriteplayer.setPosition(200,600);

                    // change the text of the message
                    messagetext.setString("Game Over !!");

                    // center it on the screen 
                    FloatRect textrect = messagetext.getLocalBounds();

                    messagetext.setOrigin(textrect.left + textrect.width/2.0f , textrect.top + textrect.height/2.0f);
                    messagetext.setPosition(1920 / 2.0f,1080 / 2.0f);
                    death.play();
                }
                
                
                
        }
        //-------------------------- draw scene 

        // clear everythong at last rub out the previous frame of animation
        window.clear();

        //--------------------------- draw our game scene here
        window.draw(spriteBackground); 
        
       
        window.draw(spritecloud1);
        window.draw(spritecloud2);
        window.draw(spritecloud3);

        //branch
        for (int i = 0; i < NumberOfBranches; i++)
        {
            window.draw(branches[i]);
        }
        

        window.draw(spritetree);

        // draw player 
        window.draw(spriteplayer);

        // draw axe
         window.draw(spriteAxe);
        
        // Draw the flying log
        window.draw(spriteLog);
        // Draw the gravestone
        window.draw(spriteRIP);

        window.draw(spritebee);

        // draw score 
        window.draw(scoretext);

        // draw timebar 
        window.draw(timebar);

        if (paused)
        {
            window.draw(messagetext);
        }

        //---------------------------- show all things we drew
        window.display();

    }

    return 0;
}

// function definition 
void updateBranchs(int seed){
    // move all branches down one place
    for (int j = NumberOfBranches; j > 0; j--)
    {
        branchPosition[j] = branchPosition[j-1];
    }

    // spawn a new branch at position 0 
    // LEFT , RIGHT ,NONE
    srand((int)time(0) + seed);
    int r = (rand() % 5);

    switch (5)
    {
    case 0:
        branchPosition[0] = side::LEFT;
        break;
    case 1:
        branchPosition[0] = side::RIGHT;
        break;
    
    default:
        branchPosition[0] =side::NONE;
        break;
    }
    
}