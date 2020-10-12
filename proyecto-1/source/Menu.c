#include "Menu.h"

// Function that makes the bounce of the letters in the tittle screen
void pat_bounce(PATTERN *pat)
{
    if(pat->tt <= 0)    // timer's run out: play pattern
    {
        pat->fvy += GRAV;
        pat->fy += pat->fvy;

        // touched floor: bounce
        if(pat->fy > POS0)
        {
            // damp if we still have enough speed
            // otherwise kill movement
            if(pat->fvy > DAMP)
            {
                pat->fy= 2*POS0-pat->fy;
                pat->fvy= DAMP-pat->fvy;
            }
            else
            {
                pat->fy= POS0;
                pat->fvy= 0;
            }
        }
    }
    else    // still in waiting period
        pat->tt--;
}
// Function that sets the tittle screen of the game
void title_init(PATTERN* pats, OBJ_ATTR *oe){
    txt_init_obj(&oam_mem[0], 0xF200, CLR_YELLOW, 0xEE);
    obj_puts2(120-12*HWLEN/2, 8, tittle, 0xF200, oe);
    int ii =0;
    for(ii=0; ii<HWLEN; ii++)
    {
        // init patterns
        pats[ii].state=0;
        pats[ii].tt= 3*ii+1;
        pats[ii].fy= -12<<8;
        pats[ii].fvy= 0;
        
        // init sprite position
        oe[ii].attr0 &= ~ATTR0_Y_MASK;
        oe[ii].attr0 |= 160;
    }

    tte_write("#{P:60, 110} Presione Start");

}
// Function that sets the instructions screen
void print_instructions(OBJ_ATTR *oe){
    oam_copy(oe, 0, 12);
	tte_write("#{es}");
	tte_write("#{P:5, 30} Utilice las flechas \n para moverse");
	tte_write("#{P:5, 60} Obtenga 3 monedas \n para ganar el juego");
	tte_write("#{P:5, 90} Suerte");
	tte_write("#{P:5, 120} Presione A para continuar");
}
// Function that sets the end game screen
void final_screen(OBJ_ATTR *oam, int currentScore, u32 sprites_amount)
{
    char totalScore[100]; 
    oam_copy(oam, 0, sprites_amount);
	tte_write("#{es}");
	tte_write("#{P:65, 40} Fin del Juego");
    snprintf(totalScore, 100, "#{P:70, 80} Su puntaje es:%d", currentScore);
	tte_write(totalScore);
	tte_write("#{P:0, 120} Presione Start para continuar");
}

