#include	<TETSU_CON_G11_user.h>


volatile unsigned char		Timer_tick;
volatile unsigned char		Direction_sw;
volatile unsigned char		Limit_sw;
volatile unsigned char		Start_sound;
volatile struct led_param 	Led_port[3];
volatile unsigned char		Allways_light_on_flag;

const int	ACC_BRK_LEVEL[STEP_COUNT] = 
					{ 4, 4, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, -2, -4, -6 };					


void	tetsu_con_g11_main ( void )
{

	volatile unsigned char 		direction = DIRECTION_DEFAULT,
					emergency_flag = EM_OFF;
	volatile int			speed = 0,
					speed_temp = 0;
	volatile unsigned int		master_handle,
					current_value;
	unsigned int 			i;
	
	
	
	unsigned int			border_handle[STEP_COUNT] = {	130*4,
									135*4,
									140*4,
									145*4,
									150*4,
									155*4,
									160*4,
									165*4,
									170*4,
									180*4,
									190*4,
									200*4,
									210*4,
									220*4,
									230*4,
									240*4
										};
	
	
	Timer_tick = 0;	
	Direction_sw = SW_OFF;
	Limit_sw = SW_OFF;
	Start_sound = SW_OFF;
	LED_DIRECTION_PORT = LED_OFF;
	LED_ACC_PORT = LED_OFF;
	LED_BRK_PORT = LED_OFF;
	Allways_light_on_flag = LIGHT_ON;
	
	
	for( i =  0; i < 3 ; i++ )
	{
		Led_port[i].on_off = LED_OFF;
		Led_port[i].blink_flag = BLINK_OFF;
		Led_port[i].dimlevel = 0;
		Led_port[i].blink_count = 0;
		Led_port[i].blinking = 0;
	}

	
	R_IT8Bit0_Channel0_Start();		// start main interval timer 
	EI();
	R_PGA_Start();
	wait10us();
	R_DAC0_Start();
	R_COMP0_Start();
	wait10us();
	R_TMR_KB0_Start();
	C0OE = 1;				// must need to set 1 by examination
	STBY_PORT = 1;


	if ( DIRECTION_SW_PORT == SW_ON ){
		/* change the flag */
		Allways_light_on_flag = LIGHT_OFF;
	}

	handle_step_set ( border_handle );


	while( 1 )
	{

		
		if ( Timer_tick == 1 )
		{
			

			Timer_tick = 0;
			advalue_get( &current_value, &master_handle );
			/* åüèo */
			for ( i = 0 ; i < STEP_COUNT-1 ; i++ )
			{
				if( master_handle < border_handle[i] )
				{
					break;
				}
			}
			speed = ACC_BRK_LEVEL[i];
			if ( speed > 0 )
			{
				Led_port[LED_ACC].on_off = LED_ON;
				Led_port[LED_BRK].on_off = LED_OFF;
			}else{
				if ( speed < 0 ){
					Led_port[LED_ACC].on_off = LED_OFF;
					Led_port[LED_BRK].on_off = LED_ON;
				}else{
					Led_port[LED_ACC].on_off = LED_OFF;
					Led_port[LED_BRK].on_off = LED_OFF;
				}
			}

			if( emergency_flag == EM_OFF ){
			
				if (  speed_set ( speed, direction ) < 0 )
				{
					/* blink dir */
					direction = !direction;
					Led_port[LED_DIRECTION].on_off = direction;
					Led_port[LED_DIRECTION].blink_flag = BLINK_FAST;
					emergency_flag = EM_ALART;
				}else{
					/* stop blink */
					Led_port[LED_DIRECTION].blink_flag = BLINK_OFF;
				}
			}
			
			
		}
		
		if ( TKBPAFLG0 != 0 )
		{
			speed_set ( 0, direction );
			/* blink all */
			Led_port[LED_DIRECTION].blink_flag = BLINK_FAST;
			Led_port[LED_ACC].blink_flag = BLINK_FAST;
			Led_port[LED_BRK].blink_flag = BLINK_FAST;
			emergency_flag = EM_ALART;
			
		}	

		if ( Direction_sw == SW_ON )
		{
			Direction_sw = SW_OFF;
			if( emergency_flag == EM_OFF )
			{
				direction = !direction;
				Led_port[LED_DIRECTION].on_off = direction;
			}else{
				if( TKBPAFLG0 != 0 && speed <= 0 )
				{
					tmkb_set( 0, direction );
					wait100us();
					TKBPAHFT0 = 0b00000011;
					Led_port[LED_DIRECTION].blink_flag = BLINK_OFF;
					Led_port[LED_ACC].blink_flag = BLINK_OFF;
					Led_port[LED_BRK].blink_flag = BLINK_OFF;
					emergency_flag = EM_OFF;		
				}else{
					if ( speed <= 0 )
						emergency_flag = EM_OFF;
				}
			}
		}
				
		if ( Limit_sw == SW_ON )
		{
			Limit_sw = SW_OFF;
			Start_sound = SOUND_ON;
		}
		
		WDTE = 0xAC;
		__halt();
		
	}
	
}

void	advalue_get( unsigned int *current, unsigned int *handle )
{

			R_ADC_Set_OperationOn();
			wait1us();
			R_ADC_Start();
			ADS = 0x03;	/* Handle */
			ADIF = 0;
			while ( !ADIF );
			R_ADC_Get_Result((uint16_t *)handle);
			ADS = 0x17;	/* PGA */
			ADIF = 0;
			while ( !ADIF );
			R_ADC_Get_Result((uint16_t *)current);
			R_ADC_Set_OperationOff();
			R_ADC_Stop();

			return;	
}



void	handle_step_set ( unsigned int *border_handle )
{
	
	volatile unsigned int max_handle_value, min_handle_value, stepsize_handle_value, range_handle_value, i;
	volatile unsigned int dummy, temp;
	max_handle_value = 0;
	min_handle_value = 0xffff;
	
	/* direction led blink on */
	
	Led_port[LED_DIRECTION].blink_flag = BLINK_ON;
	
	
	i = 3;
	while ( i != 0 )
	{
		advalue_get( &dummy, &temp );	
		
		if ( temp > max_handle_value )
		{
			max_handle_value = temp;
		}
		if( temp < min_handle_value )
		{
			min_handle_value = temp;
		}
		
		if( Limit_sw == SW_ON )
		{
			Limit_sw = SW_OFF;
			i--;
		}
		WDTE = 0xAC;
	}

	range_handle_value = max_handle_value - min_handle_value;
	stepsize_handle_value = range_handle_value / STEP_COUNT;

	temp = min_handle_value;
	for ( i = 0 ; i < STEP_COUNT ; i++ )
	{
		temp += stepsize_handle_value;
		border_handle[i] = temp;
	}

	
	temp = min_handle_value;	
	while ( temp < border_handle[STEP_COUNT-2] )
	{
		advalue_get( &dummy, &temp );	
		
		WDTE = 0xAC;
	}
	
	/* blink off */
	Led_port[LED_DIRECTION].blink_flag = BLINK_OFF;
	
	return;
	
}

int	speed_set ( int speed_value, unsigned char direction_flag )
{
	int speed_temp;
	int	base_value;
	static unsigned char past_direction_flag = DIRECTION_DEFAULT;

	if ( Allways_light_on_flag == LIGHT_ON )
		base_value = ALLWAYS_LIGHT_ON_VALUE;
	else
		base_value = 0;

	/* Timer value should be in 0x0000 - 0x7fff */
	speed_temp = tmkb_value_get( direction_flag );

	if ( speed_temp < base_value )
		speed_temp = base_value;
	
	/* need to change operation if speed_temp ( Timer ) is zero */
	if ( speed_temp == base_value )
	{
		if( speed_value < 0 )
		{
			speed_value = base_value;
		}else{
			speed_value += speed_temp;
			if( speed_value < JUMP_START_VALUE )
				speed_value = JUMP_START_VALUE;
		}		
	}else{

		if ( ( speed_value + speed_temp ) < base_value )
			speed_value = base_value;
		else
			speed_value += speed_temp;
	
	}
	
	if ( speed_value > SPEED_LIMIT_VALUE )
		speed_value = SPEED_LIMIT_VALUE;

	if ( past_direction_flag != direction_flag )
	{
		if(  speed_value == base_value )		//base_valueÇÕÇ±ÇÃèÍçáç≈í·å¿ÇÃílÇà”ñ°Ç∑ÇÈÅBAllwaysLightON_flag=1Ç≈Ç†ÇÍÇŒ0Ç≈ÇÕÇ»Ç¢ÇÃÇ≈Ç±ÇÃî‰ärÇ…Ç»ÇÈ
		{
			past_direction_flag = direction_flag;
		}else{
			tmkb_set( base_value, direction_flag );
			return -1;
		}
	}

	tmkb_set( speed_value, direction_flag );

	return speed_value;

	
}


void	tmkb_set( int speed_value, unsigned char direction_flag )
{

	if ( direction_flag )
	{
		TKBCR01 = ( unsigned int ) speed_value;
		TKBCR02 = 0;
		TKBCR03 = 0;
		TKBRDT0 = 1;
	}else{
		TKBCR01 = 0;
		TKBCR02 = 0;
		TKBCR03 = ( unsigned int ) speed_value;
		TKBRDT0 = 1;
	}
	
	return;	
	
}


unsigned int	tmkb_value_get ( unsigned char direction_flag )
{

	if ( direction_flag )
	{
		return ( int ) TKBCR01;
	}else{
		return ( int ) TKBCR03;
	}
	
}


void	wait1us( void )
{

	/* 1us @ 24 MHz */

	/* call 3clock */
	
	
	/* nop 1clock * 15 clock */
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();

	/* ret 6clock */
	
	/* total 24clock */
	
}
