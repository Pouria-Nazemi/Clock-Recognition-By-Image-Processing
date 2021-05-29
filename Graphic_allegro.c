#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <bmpio.h>

int hour_angle(int h);
int min_sec_angle(int m);
int changehrs(int m,int a);

int must_init(bool test, const char *description){
    if(test){
    	return 0;
	} 
    printf("couldn't initialize %s\n", description);
    exit(1);
}
int digital_clock(int hour_result[]){
	hour_result[3]=10;
	hour_result[6]=10;
	char name[100];
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    ALLEGRO_TIMER* timer = al_create_timer(1);
    must_init(timer, "timer");
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");
    ALLEGRO_DISPLAY* disp = al_create_display(900, 400);
    must_init(disp, "display");
    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");
    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* back = al_load_bitmap("Digital\\back2.png"); //back
    ALLEGRO_BITMAP *icon1;
    ALLEGRO_BITMAP* clock = al_load_bitmap("d10.png");
    must_init(back, "Back");
	    
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    
    bool redraw = true;
    
    ALLEGRO_EVENT event;
    al_install_keyboard();
    al_set_window_title(disp, "Clock Distinguish");
    icon1 = al_load_bitmap("Digital\\d10.png");
    al_set_display_icon(disp,icon1);
    al_start_timer(timer);
    
	int i,fasele_x,fasele_y=271,y=258;
    al_wait_for_event(queue, &event);
        switch(event.type){
           /*case ALLEGRO_EVENT_TIMER:
                redraw = true;*/
            case ALLEGRO_EVENT_KEY_DOWN:
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                return 0;
    	}
    if(redraw && al_is_event_queue_empty(queue)){
		al_clear_to_color(al_map_rgb(240, 240, 240));
		al_draw_bitmap(back,0, 0, 0);
		for(i=1;i<=8;i++){
			sprintf(name,"Digital\\d%d.png",hour_result[i]);
			printf("%s",name);
			ALLEGRO_BITMAP* clock = al_load_bitmap(name);
			if(i==3 || i==6){ //2 noghte
				fasele_x =100 +( (i-1)*90 ) +35; // fasele amoodi
				fasele_y = 258; //fasele ofoghi
			}
			else{
				fasele_x = 100 +( (i-1)*90 );// fasele amoodi
				fasele_y = 216;//fasele ofoghi
			}
			al_draw_bitmap(clock,fasele_x, fasele_y, 0);
        } 	
        al_flip_display();
        al_wait_for_event(queue, &event);
        switch(event.type){
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
            return 0;
    	}
        sleep(7);
  	}
		al_destroy_bitmap(back);
	    al_destroy_bitmap(clock);
	    al_destroy_bitmap(icon1);
	    al_destroy_font(font);
	    al_destroy_display(disp);
	    al_destroy_timer(timer);
	    al_destroy_event_queue(queue);					
}

void analog_clock(int hour_result[]){
	getchar();//daryaft enter ezafe karbar
	printf("\nPress Enter to display Analog Clock\n");
	getchar();
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_audio(), "audio");
	must_init(al_init_acodec_addon(), "audio codecs");
	must_init(al_reserve_samples(16), "reserve samples");

    ALLEGRO_TIMER* timer = al_create_timer(1);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    ALLEGRO_DISPLAY* disp = al_create_display(1100, 700);
    must_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* clock = al_load_bitmap("analog_clock\\clock2.png");
    ALLEGRO_BITMAP* back = al_load_bitmap("analog_clock\\back1.png");
    ALLEGRO_BITMAP* morning = al_load_bitmap("analog_clock\\morning.png");
    ALLEGRO_BITMAP* noon = al_load_bitmap("analog_clock\\noon.png");
    ALLEGRO_BITMAP* afternoon = al_load_bitmap("analog_clock\\afternoon.png");
    ALLEGRO_BITMAP* night = al_load_bitmap("analog_clock\\night.png");
    must_init(clock, "clock");
    must_init(back, "Back");
    must_init(morning, "morning");
    must_init(noon, "noon");
    must_init(afternoon, "afternoon");
    must_init(night, "night");
    
    ALLEGRO_SAMPLE* sound = al_load_sample("analog_clock\\sound.wav");
	must_init(sound, "sound");
    
    must_init(al_init_primitives_addon(), "primitives");
    ALLEGRO_BITMAP *icon1;
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
	
    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    al_install_keyboard();
    al_set_window_title(disp, "My Analog Clock");
    icon1 = al_load_bitmap("analog_clock\\clock2.png");
    al_set_display_icon(disp,icon1);
    al_start_timer(timer);
	int hour,minute,second,h,m,s,j,k;
	
	hour=hour_result[1]*10 + hour_result[2];   
	minute=hour_result[4]*10 + hour_result[5];  // time
	second=hour_result[7]*10 + hour_result[8];


	h=hour_angle(hour);
	m=min_sec_angle(minute);
	s=min_sec_angle(second);
	h=changehrs(minute,h); // harkat aghrabe saat shomar dar time 1 saat
	int i,exit = 1,shart=0,x=249,y=289,r=95;
	while(exit){
		//sleep(2);
		al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
        al_wait_for_event(queue, &event);

        switch(event.type){
           case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
    	}
        if(done){
            break;
        }
	
        if(redraw && al_is_event_queue_empty(queue)){
			for(i=h;i<=360 && shart==0 ;i+=6){//hour
				for(j=m;j<=360 && shart==0 ;j+=6){//minute
					if(j==360){
						hour++;
					}
					for(k=s;k<=360;k+=6){//second
						al_wait_for_event(queue, &event);
						if(event.type ==ALLEGRO_EVENT_DISPLAY_CLOSE){
							done = true;
	                		shart = 1;
	                		redraw = false;
	                		exit = 0;
	                		break;
						}
						al_clear_to_color(al_map_rgb(240, 240, 240));
						al_draw_bitmap(back,0, 0, 0);
			            al_draw_bitmap(clock,x-195 ,y-195, 0);//clock background
			            
					    if(hour>= 5 && hour<=11){
							al_draw_bitmap(morning,560, 76, 0); //window_view_morning
						}
						else if(hour>11 && hour<=16){
							al_draw_bitmap(noon,560, 76, 0);//window_view_noon
						}
						else if(hour>16 && hour<=19){
							//printf("%d",hour);
							al_draw_bitmap(afternoon,560, 76, 0);//window_view_afternoon
						}
						else if(hour>19 && hour<24){
							al_draw_bitmap(night,560, 76, 0);//window_view_night
						}
						else if(hour>=0 && hour<=4 ){
							al_draw_bitmap(night,560, 76, 0);//window_view_night
						}
						//al_draw_circle(x, y, r, al_map_rgb(255, 255, 255), 5);//clock circle
			            al_draw_line(x,y, x + (r-10)*sin((k-1)*3.1415/180), y - (r-10)*cos((k)*3.1415/180),al_map_rgb(255, 0, 0), 3);//second
			            al_draw_line(x,y, x + (r-20)*sin((j-1)*3.1415/180), y - (r-20)*cos((j)*3.1415/180),al_map_rgb(0, 152, 70),5);//minute
						al_draw_line(x,y, x + (r-50)*sin((i-1)*3.1415/180), y - (r-50)*cos((i)*3.1415/180),al_map_rgb(57, 49, 143), 7);//hour
			            al_draw_filled_circle(x, y, 3, al_map_rgb(0, 0, 0));//center circle

			            if(i==360)
							h=0;
						if(j==360){
							m=0;
						}
						if(k==360){
							s=6;
						}
			            al_flip_display();
			
			            redraw = false;
					}
				}
			}
    	}
	}
	al_destroy_sample(sound);
	al_destroy_bitmap(back);
    al_destroy_bitmap(clock);
    al_destroy_bitmap(morning);
    al_destroy_bitmap(noon);
    al_destroy_bitmap(afternoon);
    al_destroy_bitmap(night);
    al_destroy_bitmap(icon1);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}
int hour_angle(int h){
	int x;
	switch(h){
		case 0:
		case 24: x=0;
		break;
		case 1:
		case 13: x=30;
		break;
		case 2:
		case 14: x=60;
		break;
		case 3:
		case 15: x=90;
		break;
		case 4:
		case 16: x=120;
		break;
		case 5:
		case 17: x=150;
		break;
		case 6:
		case 18: x=180;
		break;
		case 7:
		case 19: x=210;
		break;
		case 8:
		case 20: x=240;
		break;
		case 9:
		case 21: x=270;
		break;
		case 10:
		case 22: x=300;
		break;
		case 11:
		case 23: x=330;
		break;
		case 12: x=360;
		break;
	}
	return(x);
}

int min_sec_angle(int m){
	int x;
	if(m%5==0){
		switch(m){
			case 0: x=0;
			break;
			case 5: x=30;
			break;
			case 10: x=60;
			break;
			case 15: x=90;
			break;
			case 20: x=120;
			break;
			case 25: x=150;
			break;
			case 30: x=180;
			break;
			case 35: x=210;
			break;
			case 40: x=240;
			break;
			case 45: x=270;
			break;
			case 50: x=300;
			break;
			case 55: x=330;
			break;
			case 60: x=360;
			break;
		}
	}
	else{
		if(m>0&&m<15){
			switch(m){
				case 1: x=6;
				break;
				case 2: x=12;
				break;
				case 3: x=18;
				break;
				case 4: x=24;
				break;
				case 6: x=36;
				break;
				case 7: x=42;
				break;
				case 8: x=48;
				break;
				case 9: x=54;
				break;
				case 11: x=66;
				break;
				case 12: x=72;
				break;
				case 13: x=78;
				break;
				case 14: x=84;
				break;
			}
		}
		if(m>15&&m<30){
			switch(m){
				case 16: x=96;
				break;
				case 17: x=102;
				break;
				case 18: x=108;
				break;
				case 19: x=114;
				break;
				case 21: x=126;
				break;
				case 22: x=132;
				break;
				case 23: x=138;
				break;
				case 24: x=144;
				break;
				case 26: x=156;
				break;
				case 27: x=162;
				break;
				case 28: x=168;
				break;
				case 29: x=174;
				break;
			}
		}
		if(m>30&&m<45){
			switch(m){
				case 31: x=186;
				break;
				case 32: x=192;
				break;
				case 33: x=198;
				break;
				case 34: x=204;
				break;
				case 36: x=216;
				break;
				case 37: x=222;
				break;
				case 38: x=228;
				break;
				case 39: x=234;
				break;
				case 41: x=246;
				break;
				case 42: x=252;
				break;
				case 43: x=258;
				break;
				case 44: x=264;
				break;
			}
		}
	
		if(m>45&&m<60){
			switch(m){
				case 46: x=276;
				break;
				case 47: x=282;
				break;
				case 48: x=288;
				break;
				case 49: x=294;
				break;
				case 51: x=306;
				break;
				case 52: x=312;
				break;
				case 53: x=318;
				break;
				case 54: x=324;
				break;
				case 56: x=330;
				break;
				case 57: x=342;
				break;
				case 58: x=348;
				break;
				case 59: x=354;
				break;
			}
		}
	
	}
	return(x);
}

int changehrs(int m,int a){
	if(m>15&&m<=30)
		a+=12;
	if(m>30&&m<=45)
		a+=18;
	if(m>45&&m<60)
		a+=24;
	return (a);
}
