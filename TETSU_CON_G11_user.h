#include <iodefine.h>
#include 	"r_cg_macrodriver.h"
#include 	"r_cg_cgc.h"
#include 	"r_cg_port.h"
#include 	"r_cg_tau.h"
#include 	"r_cg_it8bit.h"
#include 	"r_cg_tmkb.h"
#include 	"r_cg_wdt.h"
#include 	"r_cg_sau.h"
#include 	"r_cg_adc.h"
#include	"r_cg_dac.h"
#include 	"r_cg_pga.h"
#include 	"r_cg_comp.h"

/* port definision */
#define	LED_ACC_PORT		P2_bit.no0		/* rev2 */
#define LED_BRK_PORT		P2_bit.no1		/* rev2 */
#define LED_ACC			0
#define LED_BRK			1
#define LED_DIRECTION		2
#define IN2_PORT		P3_bit.no1
#define IN1_PORT		P3_bit.no0
#define STBY_PORT		P5_bit.no5
#define CONTROL_SOUND		P5_bit.no4
#define PWMA_PORT		P5_bit.no6
#define ADCH_M_CON_OUT		3			/* ANI3 */
#define ADCH_OCP_SHUNT		2			/* ANI2 */	
#define LED_DIRECTION_PORT	P0_bit.no0		/* rev2 */
#define LIMIT_SW_PORT		P13_bit.no7
#define DIRECTION_SW_PORT	P3_bit.no3
#define DIRECTION_DEFAULT	1
#define	LED_ON			0
#define LED_OFF			1
#define SW_ON			0
#define SW_OFF			1
#define SOUND_ON		1
#define SOUND_OFF		0
#define SOUND_LIMIT		100			/* 1ms * 100 = 100ms */
#define INTERVAL_COUNT		100			/* 1ms * 100 = 100ms */
#define SW_CHATTERING_COUNT 	50			/* 1ms * 50 = 50ms */
#define STEP_COUNT 		16
#define BLINK_ON		1
#define BLINK_OFF		0
#define BLINK_FAST		2
#define BLINK_PERIOD		500			/* 1ms * 500 = 500ms */
#define BLINK_PERIOD_FAST	100
#define EM_OFF			0
#define EM_ALART		1
#define LIGHT_ON		1
#define LIGHT_OFF		0
#define SPEED_LIMIT_PERCENT	100.0			/* 100% */
#define JUMP_START_PERCENT	20.0			/* 20% */
#define ALLWAYS_LIGHT_ON_PERCENT 10.0			/* 10% */

/* for rev2 */
#define SPEED_LIMIT_VALUE	((unsigned int )( _01DF_TMKB_CYCLE00_VALUE * SPEED_LIMIT_PERCENT / 100.0 ))
#define JUMP_START_VALUE	((unsigned int )( _01DF_TMKB_CYCLE00_VALUE * JUMP_START_PERCENT / 100.0 ))
#define ALLWAYS_LIGHT_ON_VALUE  ((unsigned int )( _01DF_TMKB_CYCLE00_VALUE * ALLWAYS_LIGHT_ON_PERCENT / 100.0 ))


#define wait10us()		{wait1us(); wait1us(); wait1us(); wait1us(); wait1us(); wait1us(); wait1us(); wait1us(); wait1us(); wait1us();}
#define wait100us()		{wait10us(); wait10us(); wait10us(); wait10us(); wait10us(); wait10us(); wait10us(); wait10us(); wait10us(); wait10us();}


/* function definision */
void	tetsu_con_g11_main ( void );
void	wait1us( void );
void	advalue_get( unsigned int * , unsigned int * );
void	handle_step_set ( unsigned int * );
int	speed_set ( int , unsigned char  );
void	tmkb_set( int, unsigned char );
unsigned int	tmkb_value_get ( unsigned char );


/* structure definision */
struct led_param {
	unsigned int	blink_count;
	unsigned char	dimlevel;
	unsigned char	blink_flag:2;
	unsigned char	on_off:1;
	unsigned char	blinking:1;
};