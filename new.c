#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define GAME_TERMINATE 444

///type define
typedef struct{

    float x;
    float y;

    float live;
    float speed;
    int maxFrame;
    int curFrame;
    int frameCount;
    int frameheight;
    int framewidth;
    int animationColumns;
    int animationDirection;

    bool face;
    bool wear_hat;

    ALLEGRO_BITMAP *image_l;
    ALLEGRO_BITMAP *image_r;

    ALLEGRO_BITMAP *image_l_max;
    ALLEGRO_BITMAP *image_r_max;

}Sprite;

typedef struct{

    float bound_x;
    float bound_y;
    int cur_explode;
    int cur_laser;

    ALLEGRO_BITMAP *image_laser_l;
    ALLEGRO_BITMAP *image_laser_r;
    ALLEGRO_BITMAP *image_explode;

}Fire;

typedef struct{

    float bound_x;
    float bound_y;
    int cur;

    ALLEGRO_BITMAP *image_energy_bar_r;
    ALLEGRO_BITMAP *image_energy_bar_l;

}Max_Mode_Fire;

///variables
Sprite A;
Sprite B;
Fire A_F;
Fire B_F;
Max_Mode_Fire A_M_F;
Max_Mode_Fire B_M_F;
int Width=640;
int Height=480;
int cur_a=0;
int cur_win=0;
int hat_x=270;
int hat_y=190;
int Window = 1;
char *title = "Final Project Of 10600003";
bool keys_A[5] = {false, false, false, false, false};//up down left right attack
bool keys_B[5] = {false, false, false, false, false};//up down left right attack
bool game_over=false;
bool judge_next_window = false;
bool A_Win=false;
bool B_Win=false;
bool QuiteGame=false;
bool about=false;
bool get_hat=false;

/// ALLEGRO Variables
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_BITMAP *image = NULL;
ALLEGRO_BITMAP *start_page_a = NULL;
ALLEGRO_BITMAP *start_page_b = NULL;
ALLEGRO_BITMAP *start_page_c = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *disco = NULL;
//ALLEGRO_BITMAP *hat = NULL;
ALLEGRO_BITMAP *hat_about = NULL;
ALLEGRO_BITMAP *angry =NULL;
ALLEGRO_BITMAP *winpage=NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_SAMPLE *song=NULL;
ALLEGRO_SAMPLE *attack_sample=NULL;
ALLEGRO_SAMPLE *attack_max_sample=NULL;
ALLEGRO_SAMPLE *attack_max2_sample=NULL;
ALLEGRO_SAMPLE *bounce_sample=NULL;
ALLEGRO_SAMPLE_INSTANCE *instance1=NULL;
ALLEGRO_SAMPLE_INSTANCE *instance2=NULL;
ALLEGRO_SAMPLE_INSTANCE *instance3=NULL;
ALLEGRO_SAMPLE_INSTANCE *instance4=NULL;
ALLEGRO_SAMPLE_INSTANCE *instance5=NULL;
ALLEGRO_FONT *font = NULL;

///Functions
void game_init();
void game_begin();
void game_destroy();
int process_event();
int game_run();
void Initiate_A();
void Initiate_B();
//SHIFT
void Move_A_up();
void Move_A_down();
void Move_A_left();
void Move_A_right();
void Move_B_up();
void Move_B_down();
void Move_B_left();
void Move_B_right();
//ATTACK
void A_Attack_l();
void B_Attack_l();
void A_Attack_r();
void B_Attack_r();
void A_Attack_MAX_l();
void B_Attack_MAX_l();
void A_Attack_MAX_r();
void B_Attack_MAX_r();

int main(void){
    int msg;
    game_init();
    game_begin();
    while(msg!=GAME_TERMINATE){
        msg=game_run();
        if(msg==GAME_TERMINATE) printf("GAMEOVER!");
    }
    game_destroy();
    return 0;
}

void game_init(){
    if (!al_init()) {
        fprintf(stderr, "Something Wrong!\n");
        game_destroy();
        exit(9);
    }

    // Create display
    display = al_create_display(Width, Height);
    event_queue = al_create_event_queue();
    if (display == NULL || event_queue == NULL) {
        fprintf(stderr, "Something Wrong!\n");
        game_destroy();
        exit(9);
    }
    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");}

    // Initialize Allegro settings
    al_set_window_position(display, 0, 0);
    al_set_window_title(display, title);
    al_install_audio();
    al_init_acodec_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_reserve_samples(1);

    // Register event
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
}

void game_begin(){
    //Load The Song
    song = al_load_sample( "start_music.wav" );
    attack_sample = al_load_sample( "attack.wav" );
    attack_max_sample = al_load_sample("Max_at.wav");
    attack_max2_sample = al_load_sample("Max_at2.wav");
    bounce_sample = al_load_sample("bounce.wav");

    instance1 = al_create_sample_instance(song);
    instance2 = al_create_sample_instance(attack_sample);
    instance3 = al_create_sample_instance(attack_max_sample);
    instance4 = al_create_sample_instance(attack_max2_sample);
    instance5 = al_create_sample_instance(bounce_sample);

    al_attach_sample_instance_to_mixer(instance1,al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance2,al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance3,al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance4,al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance5,al_get_default_mixer());//NOT GOOD

    //load something for Window 1.
    start_page_a=al_load_bitmap("HAND.png");
    start_page_b=al_load_bitmap("HAND2.png");
    start_page_c=al_load_bitmap("BATTLE LLAMA.png");
    font=al_load_ttf_font("WORD.ttf",10,0);
    hat_about = al_load_bitmap("HAT_about.png");

    //Start Page
    al_draw_filled_rectangle(0,0,500,800,al_map_rgb(0,0,0));
    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));//NOT GOOD

    //Initialize Timer
    timer  = al_create_timer(1.0/33.0);
    al_register_event_source(event_queue, al_get_timer_event_source(timer)) ;
    al_start_timer(timer);
}

int process_event(){

    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue,&event);

    if(event.type==ALLEGRO_EVENT_TIMER){

        al_play_sample_instance(instance1);
        if(Window==1){
            //change the position of hat
            hat_x++;hat_y+=2;
            if(hat_x>=450){hat_x=190;}if(hat_y>=370){hat_y=270;}
        }
        if(Window==1||Window==2){cur_a++;if(cur_a>=6){cur_a=0;}}
        if(keys_A[4]&&Window==2){
            if(A.live>=20){
                al_play_sample_instance(instance2);

                A_F.cur_explode++;
                if(A_F.cur_explode>=9){A_F.cur_explode=0;}
                A_F.cur_laser++;
                if(A_F.cur_laser>=6){A_F.cur_laser=0;}
            }
            else{
                al_play_sample_instance(instance3);
                al_play_sample_instance(instance4);
                A_M_F.cur++;
                if(A_M_F.cur>=4){A_M_F.cur=0;}
            }
        }
        if(keys_B[4]&&Window==2){

            if(B.live>=20){
                al_play_sample_instance(instance2);
                B_F.cur_explode++;
                if(B_F.cur_explode>=9){B_F.cur_explode=0;}
                B_F.cur_laser++;
                if(B_F.cur_laser>=6){B_F.cur_laser=0;}
            }
            else{
                al_play_sample_instance(instance3);
                al_play_sample_instance(instance4);
                B_M_F.cur++;
                if(B_M_F.cur>=4){B_M_F.cur=0;}
            }
        }
        if(Window==3){cur_win++;}

        if(keys_A[0]){
            if(Window==2||(Window==3&&A_Win))al_play_sample_instance(instance5);
            Move_A_up();
        }
        else if(keys_A[1]){
            if(Window==2||(Window==3&&A_Win))al_play_sample_instance(instance5);
            Move_A_down();
        }
        if(keys_A[2]){
            if(Window==2||(Window==3&&A_Win))al_play_sample_instance(instance5);
            Move_A_left();
        }
        else if(keys_A[3]){
            if(Window==2||(Window==3&&A_Win))al_play_sample_instance(instance5);
            Move_A_right();
        }
        if(keys_B[0]){
            if(Window==2||(Window==3&&B_Win))al_play_sample_instance(instance5);
            Move_B_up();
        }
        else if(keys_B[1]){
            if(Window==2||(Window==3&&B_Win))al_play_sample_instance(instance5);
            Move_B_down();
        }
        if(keys_B[2]){
            if(Window==2||(Window==3&&B_Win))al_play_sample_instance(instance5);
            Move_B_left();
        }
        else if(keys_B[3]){
            if(Window==2||(Window==3&&B_Win))al_play_sample_instance(instance5);
            Move_B_right();
        }
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch(event.keyboard.keycode)
        {
        case ALLEGRO_KEY_ESCAPE:
            return GAME_TERMINATE;
            break;
        case ALLEGRO_KEY_I:
            keys_A[0] = true;
            break;
        case ALLEGRO_KEY_K:
            keys_A[1] = true;
            break;
        case ALLEGRO_KEY_J:
            keys_A[2] = true;
            break;
        case ALLEGRO_KEY_L:
            keys_A[3] = true;
            break;
        case ALLEGRO_KEY_W:
            keys_B[0] = true;
            break;
        case ALLEGRO_KEY_S:
            keys_B[1] = true;
            break;
        case ALLEGRO_KEY_D:
            keys_B[2] = true;
            break;
        case ALLEGRO_KEY_A:
            keys_B[3] = true;
            break;
        case ALLEGRO_KEY_0:
            keys_A[4]=true;
            break;
        case ALLEGRO_KEY_1:
            keys_B[4]=true;
            break;
        case ALLEGRO_KEY_ENTER:
            if(Window==3)QuiteGame=true;
            else judge_next_window = true;
            break;
        case ALLEGRO_KEY_TAB:
            about = true;
            break;
        }
    }
    else if(event.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch(event.keyboard.keycode)
        {
        case ALLEGRO_KEY_I:
            keys_A[0] = false;
            break;
        case ALLEGRO_KEY_K:
            keys_A[1] = false;
            break;
        case ALLEGRO_KEY_J:
            keys_A[2] = false;
            break;
        case ALLEGRO_KEY_L:
            keys_A[3] = false;
            break;
        case ALLEGRO_KEY_W:
            keys_B[0] = false;
            break;
        case ALLEGRO_KEY_S:
            keys_B[1] = false;
            break;
        case ALLEGRO_KEY_D:
            keys_B[2] = false;
            break;
        case ALLEGRO_KEY_A:
            keys_B[3] = false;
            break;
        case ALLEGRO_KEY_0:
            keys_A[4]=false;
            break;
        case ALLEGRO_KEY_1:
            keys_B[4]=false;
            break;
        case ALLEGRO_KEY_TAB:
            about = false;
            break;
        case ALLEGRO_KEY_ENTER:
            QuiteGame=false;
            break;
        }
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
        return GAME_TERMINATE;
    }
    return 0;
}

int game_run(){

    int error = 0;

    if(Window==1){
        if (!al_is_event_queue_empty(event_queue)) {

            error=process_event();

            if(!about){
                al_draw_bitmap_region(start_page_a,cur_a*150,0,150,150,440,90,0);
                al_draw_bitmap_region(start_page_b,cur_a*150,0,150,150,40,90,0);
                al_draw_bitmap_region(start_page_c,cur_a*200,0,200,150,215,90,0);
                al_draw_text(font, al_map_rgb(255,255,255), Width/2, Height/2+20 , ALLEGRO_ALIGN_CENTRE, "ENTER >> START");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2, Height/2+50 , ALLEGRO_ALIGN_CENTRE, "TAB >> ABOUT");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2, Height/2+80 , ALLEGRO_ALIGN_CENTRE, "ESC >> QUIT");
                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }
            if(about){
                al_draw_filled_rectangle(0,0,500,800,al_map_rgb(0,0,0));
                al_draw_bitmap_region(hat_about, cur_a*32,0,32,32,300,50,0);
                al_draw_text(font, al_map_rgb(255,255,255), Width/2, Height/2-140, ALLEGRO_ALIGN_CENTRE,"This game is about two llamas...");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2, Height/2-110, ALLEGRO_ALIGN_CENTRE,"They are going to fight on a party,");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2, Height/2-80, ALLEGRO_ALIGN_CENTRE,"and wanting to be the party queen.");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2-150, Height/2-10, ALLEGRO_ALIGN_CENTRE, "Player 1 :");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2+150, Height/2-10, ALLEGRO_ALIGN_CENTRE, "Player 2 :");

                al_draw_text(font, al_map_rgb(255,255,255), Width/2-210, Height/2 +50, ALLEGRO_ALIGN_CENTRE, "W");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2-210, Height/2 +90, ALLEGRO_ALIGN_CENTRE, "A  S  D");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2-130, Height/2 +65, ALLEGRO_ALIGN_LEFT, ">>MOVE");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2-210, Height/2 +140, ALLEGRO_ALIGN_CENTRE, "1");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2-130, Height/2 +140, ALLEGRO_ALIGN_LEFT, ">>ATTACK");

                al_draw_text(font, al_map_rgb(255,255,255), Width/2+90, Height/2 +50, ALLEGRO_ALIGN_CENTRE, "I");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2+90, Height/2 +90, ALLEGRO_ALIGN_CENTRE, "J  K  L");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2+170, Height/2 +65, ALLEGRO_ALIGN_LEFT, ">>MOVE");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2+90, Height/2 +140, ALLEGRO_ALIGN_CENTRE, "0");
                al_draw_text(font, al_map_rgb(255,255,255), Width/2+170, Height/2 +140, ALLEGRO_ALIGN_LEFT, ">>ATTACK");

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }

            if(judge_next_window){

                Initiate_A();
                Initiate_B();

                background = al_load_bitmap("BACKGROUND.png");
                disco = al_load_bitmap("DISCO.png");
                //hat = al_load_bitmap("HAT.png");
                angry = al_load_bitmap("cool.png");
                Window = 2;
            }
        }
    }
    else if(Window==2){

        if(!get_hat){
            if(hat_x+40>=A.x&&hat_x<=A.x&&hat_y+40>=A.y&&hat_y<=A.y){A.wear_hat=true;get_hat=true;hat_x=A.x;hat_y=A.y;A.speed=4.5;}
            else if(hat_x+40>=B.x&&hat_x<=B.x&&hat_y+40>=B.y&&hat_y<=B.y){B.wear_hat=true;get_hat=true;hat_x=B.x;hat_y=B.y;B.speed=4.5;}
        }

        al_draw_bitmap(background, 0,0, 0);
        al_draw_bitmap_region(disco,cur_a*100,0,100,100,280,0,0);

        if(A.live>=20){
            if(A.face){al_draw_bitmap_region(A.image_l, A.curFrame * A.framewidth, 0,A.framewidth,A.frameheight,A.x,A.y,0);}
            else{al_draw_bitmap_region(A.image_r, A.curFrame * A.framewidth, 0,A.framewidth,A.frameheight,A.x,A.y,0);}
        }
        else{
            al_draw_bitmap_region(angry,cur_a*65,0,65,65,570,30,0);
            if(A.face){al_draw_bitmap_region(A.image_l_max, A.curFrame * A.framewidth, 0,A.framewidth,A.frameheight,A.x,A.y,0);}
            else{al_draw_bitmap_region(A.image_r_max, A.curFrame * A.framewidth, 0,A.framewidth,A.frameheight,A.x,A.y,0);}
        }
        if(B.live>=20){
            if(B.face){al_draw_bitmap_region(B.image_r, B.curFrame * B.framewidth, 0,B.framewidth,B.frameheight,B.x,B.y,0);}
            else{al_draw_bitmap_region(B.image_l, B.curFrame * B.framewidth, 0,B.framewidth,B.frameheight,B.x,B.y,0);}
        }
        else{
            al_draw_bitmap_region(angry,cur_a*65,0,65,65,15,30,0);
            if(B.face){al_draw_bitmap_region(B.image_r_max, B.curFrame * B.framewidth, 0,B.framewidth,B.frameheight,B.x,B.y,0);}
            else{al_draw_bitmap_region(B.image_l_max, B.curFrame * B.framewidth, 0,B.framewidth,B.frameheight,B.x,B.y,0);}
        }
        al_draw_filled_rectangle(30,20,30+B.live*4,30,al_map_rgb(180,50,255));
        al_draw_filled_rectangle(400,20,400+A.live*4,30,al_map_rgb(255,25,255));
        al_draw_bitmap_region(hat_about,cur_a*32,0,32,32,hat_x+5,hat_y-5,0);

        if(keys_A[4]){
            if(A.live>=20){
                if(A.face){
                    al_draw_bitmap_region(A_F.image_explode,A_F.cur_explode*100,0,100,100,A.x-100,A.y,0);
                    al_draw_bitmap_region(A_F.image_laser_l,A_F.cur_laser*100,0,100,100,A.x-50,A.y,0);
                    A_Attack_l();
                }
                else{
                    al_draw_bitmap_region(A_F.image_explode,A_F.cur_explode*100,0,100,100,A.x+100,A.y+3,0);
                    al_draw_bitmap_region(A_F.image_laser_r,A_F.cur_laser*100,0,100,100,A.x+50,A.y+3,0);
                    A_Attack_r();
                }
            }
            else{
                if(A.face){
                    al_draw_bitmap_region(A_M_F.image_energy_bar_l,A_M_F.cur*400,0,400,100,A.x-380,A.y,0);
                    A_Attack_MAX_l();
                }
                else{
                    al_draw_bitmap_region(A_M_F.image_energy_bar_r,A_M_F.cur*400,0,400,100,A.x+70,A.y,0);
                    A_Attack_MAX_r();
                }
            }
        }
        if(keys_B[4]){
            if(B.live>=20){
                if(B.face){
                    al_draw_bitmap_region(B_F.image_explode,B_F.cur_explode*100,0,100,100,B.x+100,B.y+3,0);
                    al_draw_bitmap_region(B_F.image_laser_r,B_F.cur_laser*100,0,100,100,B.x+50,B.y+3,0);
                    B_Attack_r();
                }
                else{
                    al_draw_bitmap_region(B_F.image_explode,B_F.cur_explode*100,0,100,100,B.x-100,B.y,0);
                    al_draw_bitmap_region(B_F.image_laser_l,B_F.cur_laser*100,0,100,100,B.x-50,B.y,0);
                    B_Attack_l();
                }
            }
            else{
                if(B.face){
                    al_draw_bitmap_region(B_M_F.image_energy_bar_r,B_M_F.cur*400,0,400,100,B.x+70,B.y,0);
                    B_Attack_MAX_l();
                }
                else{
                    al_draw_bitmap_region(B_M_F.image_energy_bar_l,B_M_F.cur*400,0,400,100,B.x-380,B.y,0);
                    B_Attack_MAX_r();
                }
            }
        }

        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));

        if(B.live<=0){
            A_Win=true;
            winpage=al_load_bitmap("WIN.png");
            Window=3;
        }
        else if(A.live<=0){
            B_Win=true;
            winpage=al_load_bitmap("WIN.png");
            Window=3;
        }

        if (!al_is_event_queue_empty(event_queue)) {
            error = process_event();
        }
    }
    else if(Window==3){

        al_draw_filled_rectangle(0,0,500,800,al_map_rgb(0,0,0));
        al_draw_text(font, al_map_rgb(255,255,255),320, 350, ALLEGRO_ALIGN_CENTER, "Esc >> QUIT");
        al_draw_text(font, al_map_rgb(255,255,255),320, 380, ALLEGRO_ALIGN_CENTER, "Enter >> MANU");
        if(A_Win){
            al_draw_bitmap_region(A.image_l, A.curFrame * A.framewidth, 0,A.framewidth,A.frameheight,265,220,0);
            al_draw_text(font, al_map_rgb(255,255,255), Width/2, Height/2-40, ALLEGRO_ALIGN_CENTRE, "Player 2 WIN!");
        }
        if(B_Win){
            al_draw_bitmap_region(B.image_r, B.curFrame * B.framewidth, 0,B.framewidth,B.frameheight,270,220,0);
            al_draw_text(font, al_map_rgb(255,255,255), Width/2, Height/2-40, ALLEGRO_ALIGN_CENTRE, "Player 1 WIN!");

        }
        if(cur_win>=4){cur_win=0;}
        al_draw_bitmap_region(winpage,cur_win*250,0,250,150,180,38,0);

        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));

        if(QuiteGame){judge_next_window=false;A.wear_hat=false;get_hat=false;B.wear_hat=false;A_Win=false;B_Win=false;Window=1;}
        if (!al_is_event_queue_empty(event_queue)) {
            error = process_event();
        }
    }
    return error;
}
void Initiate_A(){

    A.x=Width/2+55;
    A.y=Height/2-20;

    A.live=50;
    A.speed=3;
    A.maxFrame=6;
    A.curFrame=0;
    A.frameCount=0;
    A.framewidth=96;
    A.frameheight=96;
    A.animationColumns=6;
    A.animationDirection=-1;
    A.face=true;
    A.wear_hat=false;
    A.image_l=al_load_bitmap("llama_l.png");
    A.image_r=al_load_bitmap("llama_r.png");
    A.image_l_max=al_load_bitmap("lll.png");
    A.image_r_max=al_load_bitmap("llr.png");

    A_F.bound_x=100;
    A_F.bound_y=50;
    A_F.image_explode=al_load_bitmap("EXPLODE.png");
    A_F.image_laser_l=al_load_bitmap("LASAR_l.png");
    A_F.image_laser_r=al_load_bitmap("LASAR_r.png");

    A_M_F.bound_x=400;
    A_M_F.bound_y=100;
    A_M_F.image_energy_bar_r=al_load_bitmap("ENERGY_BAR_r.png");
    A_M_F.image_energy_bar_l=al_load_bitmap("ENERGY_BAR_l.png");
}
void Initiate_B(){

    B.x=Width/2-100;
    B.y=Height/2+20;

    B.live=50;
    B.speed=3;
    B.maxFrame=6;
    B.curFrame=0;
    B.frameCount=0;
    B.framewidth=96;
    B.frameheight=96;
    B.animationColumns=6;
    B.animationDirection=-1;
    B.face=true;
    B.wear_hat=false;
    B.image_l=al_load_bitmap("llama_l.png");
    B.image_r=al_load_bitmap("llama_r.png");
    B.image_l_max=al_load_bitmap("lll.png");
    B.image_r_max=al_load_bitmap("llr.png");

    B_F.bound_x=100;
    B_F.bound_y=50;
    B_F.image_explode=al_load_bitmap("EXPLODE.png");
    B_F.image_laser_l=al_load_bitmap("LASAR_l.png");
    B_F.image_laser_r=al_load_bitmap("LASAR_r.png");

    B_M_F.bound_x=400;
    B_M_F.bound_y=100;
    B_M_F.image_energy_bar_r=al_load_bitmap("ENERGY_BAR_r.png");
    B_M_F.image_energy_bar_l=al_load_bitmap("ENERGY_BAR_l.png");
}
void Move_A_up(){

    A.curFrame+=A.animationDirection;
    if(A.curFrame>=A.maxFrame){
        A.curFrame=0;
    }
    else if(A.curFrame<=0){
        A.curFrame=A.maxFrame-1;
    }
    A.y-=A.speed;
    if(A.y<=88){A.y=88;}
    if(A.wear_hat){hat_y=A.y;}
}
void Move_A_down(){

    A.curFrame+=A.animationDirection;
    if(A.curFrame>=A.maxFrame){
        A.curFrame=0;
    }
    else if(A.curFrame<=0){
        A.curFrame=A.maxFrame-1;
    }
    A.y+=A.speed;
    if(A.y>=380){A.y=380;}
    if(A.wear_hat){hat_y=A.y;}
}
void Move_A_left(){

    A.face=true;
	A.curFrame-=A.animationDirection;
	if(A.curFrame>=A.maxFrame){
        A.curFrame=0;
    }
    else if(A.curFrame<=0){
        A.curFrame=A.maxFrame-1;
    }
	A.x -= A.speed;
	if(A.x<=8){A.x=8;}
	if(A.wear_hat){hat_x=A.x+30;}
}
void Move_A_right(){

    A.face=false;
	A.curFrame+=A.animationDirection;
	if(A.curFrame>=A.maxFrame){
        A.curFrame=0;
    }
    else if(A.curFrame<=0){
        A.curFrame=A.maxFrame-1;
    }
	A.x += A.speed;

	if(A.x>=548){A.x=548;}
	if(A.wear_hat){hat_x=A.x+30;}
}
void Move_B_up(){

    B.curFrame+=B.animationDirection;
    if(B.curFrame>=B.maxFrame){
        B.curFrame=0;
    }
    else if(B.curFrame<=0){
        B.curFrame=B.maxFrame-1;
    }
    B.y-=B.speed;
    if(B.y<=88){B.y=88;}
    if(B.wear_hat){hat_y=B.y;}
}
void Move_B_down(){

    B.curFrame+=B.animationDirection;
    if(B.curFrame>=B.maxFrame){
        B.curFrame=0;
    }
    else if(B.curFrame<=0){
        B.curFrame=B.maxFrame-1;
    }
    B.y+=B.speed;
    if(B.y>=380){B.y=380;}
    if(B.wear_hat){hat_y=B.y;}
}
void Move_B_left(){

    B.face=true;
	B.curFrame-=B.animationDirection;
	if(B.curFrame>=B.maxFrame){
        B.curFrame=0;
    }
    else if(B.curFrame<=0){
        B.curFrame=B.maxFrame-1;
    }
	B.x += B.speed;
	if(B.x>=548){B.x=548;}
	if(B.wear_hat){hat_x=B.x+30;}
}
void Move_B_right(){

    B.face=false;
	B.curFrame+=B.animationDirection;
	if(B.curFrame>=B.maxFrame){
        B.curFrame=0;
    }
    else if(B.curFrame<=0){
        B.curFrame=B.maxFrame-1;
    }
	B.x -= B.speed;
	if(B.x<=8){B.x=8;}
	if(B.wear_hat){hat_x=B.x+30;}
}
void A_Attack_l(){

    if(A.x>=B.x&&B.x>=A.x-A_F.bound_x&&A.y-40<=B.y&&A.y+A_F.bound_y-40>=B.y){
        if(!A.wear_hat){
            B.live-=0.002;
        }
        else{
            B.live-=0.003;
        }
    }
}
void A_Attack_r(){

    if(A.x<=B.x&&B.x<=A.x+A_F.bound_x&&A.y-40<=B.y&&A.y+A_F.bound_y-40>=B.y){
        if(!A.wear_hat){
            B.live-=0.002;
        }
        else{
            B.live-=0.003;
        }
    }
}
void B_Attack_l(){

    if(B.x>=A.x&&A.x>=B.x-B_F.bound_x&&B.y-40<=A.y&&B.y+B_F.bound_y-40>=A.y){
        if(!B.wear_hat){
            A.live-=0.002;
        }
        else{
            A.live-=0.003;
        }
    }
}
void B_Attack_r(){

    if(B.x<=A.x&&A.x<=B.x+B_F.bound_x&&B.y-40<=A.y&&B.y+B_F.bound_y-40>=A.y){
        if(!B.wear_hat){
            A.live-=0.002;
        }
        else{
            A.live-=0.003;
        }
    }
}
void A_Attack_MAX_l(){

    if(A.x>=B.x&&B.x>=A.x-A_M_F.bound_x&&A.y-40<=B.y&&A.y+A_M_F.bound_y-40>=B.y){
        if(!A.wear_hat){
            B.live-=0.008;
        }
        else{
            B.live-=0.012;
        }
    }
}
void A_Attack_MAX_r(){

    if(A.x<=B.x&&B.x<=A.x+A_M_F.bound_x&&A.y-40<=B.y&&A.y+A_M_F.bound_y-40>=B.y){
        if(!A.wear_hat){
            B.live-=0.008;
        }
        else{
            B.live-=0.012;
        }
    }
}
void B_Attack_MAX_r(){

    if(B.x>=A.x&&A.x>=B.x-B_M_F.bound_x&&B.y-40<=A.y&&B.y+B_M_F.bound_y-40>=A.y){
        if(!B.wear_hat){
            A.live-=0.008;
        }
        else{
            A.live-=0.012;
        }
    }
}
void B_Attack_MAX_l(){

    if(B.x<=A.x&&A.x<=B.x+B_M_F.bound_x&&B.y-40<=A.y&&B.y+B_M_F.bound_y-40>=A.y){
        if(!B.wear_hat){
            A.live-=0.008;
        }
        else{
            A.live-=0.012;
        }
    }
}

void game_destroy(){

    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_bitmap(image);
    al_destroy_bitmap(start_page_a);
    al_destroy_bitmap(start_page_b);
    al_destroy_bitmap(start_page_c);
    al_destroy_bitmap(background);
    al_destroy_bitmap(disco);
    al_destroy_bitmap(hat_about);
    al_destroy_bitmap(winpage);
    al_destroy_bitmap(angry);
    al_destroy_bitmap(A.image_l);
    al_destroy_bitmap(A.image_r);
    al_destroy_bitmap(A.image_l_max);
    al_destroy_bitmap(A.image_r_max);
    al_destroy_bitmap(B.image_l);
    al_destroy_bitmap(B.image_r);
    al_destroy_bitmap(B.image_l_max);
    al_destroy_bitmap(B.image_r_max);
    al_destroy_font(font);
    al_destroy_sample(song);
    al_destroy_sample(attack_max_sample);
    al_destroy_sample(attack_sample);
    al_destroy_sample(attack_max_sample);
    al_destroy_sample(attack_max2_sample);
    al_destroy_sample_instance(instance1);
    al_destroy_sample_instance(instance2);
    al_destroy_sample_instance(instance3);
    al_destroy_sample_instance(instance4);
}
