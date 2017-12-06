//Déclaration des fonctions
void PPI_Init(void);
void EXPORT_Init(void);
void startRadar(void);

//Déclaration des variables
unsigned short CP_LOW_TIME;
unsigned short CP_HIGH_TIME;
unsigned char receive;

#define _capTIME_ ((double)1/18432000)*1024

void Delay(int delay)
{
	while(delay--);
}

void angleServo(void)
{
unsigned short countAngle;
int myTime;
countAngle = 0;
myTime = 149;

	while(countAngle != 90)
	{
		myTime--;
		if(myTime == 0)
		{
			//On execute le sonar
			CaptureRead(0, &CP_LOW_TIME, &CP_HIGH_TIME);
			//On écrit sur le port serie
			UartWrShort(1, CP_HIGH_TIME);
			UartWrShort(1, countAngle);
			CaptureOn(0);
			PPI_BitOut(PD,0,1); 
			Delay(0x15); 
			PPI_BitOut(PD,0,0);
			myTime = 149;
			countAngle++;
		}
	}
}

void angleReverseServo(void)
{
unsigned short countAngle;
int myTime;
countAngle = 90;
myTime = 108;

	while(countAngle != 0)
	{
		myTime--;
		if(myTime == 0)
		{
			//On execute le sonar
			CaptureRead(0, &CP_LOW_TIME, &CP_HIGH_TIME);
			//On écrit sur le port serie
			UartWrShort(1, CP_HIGH_TIME);
			UartWrShort(1, countAngle);
			CaptureOn(0);
			PPI_BitOut(PD,0,1); 
			Delay(0x15); 
			PPI_BitOut(PD,0,0);
			myTime = 108;
			countAngle--;
		}
	}
}

void commande(unsigned char data){
	if(data == 20)
	{	AlcdClear(0);
		AlcdLocate(0,6,0);
		AlcdPrint(0,"Balayage");
		AlcdLocate(0, 6, 1);
		AlcdPrint(0, "en cours");
		AlcdLocate(0, 1, 2);
		AlcdPrint(0, "Veuillez Patientez");
		receive = 0;
		startRadar();
		AlcdClear(0);
		AlcdLocate(0,5,0);
		AlcdPrint(0,"En attente");
		AlcdLocate(0, 3, 1);
		AlcdPrint(0, "d'une commande");
	}
}

void IRQ_EVENT_(void) {
unsigned char iMSG;
iMSG=GetMsg();

switch(iMSG) {
case MSG_EXINT4:
	//DebugCHAR(receive, DEC);
	commande(receive);
	break;
	}
}

void startRadar(void)
{
	Pwm3_Set(PWM_RCSERVO, 1);
	//RcServo_Regulate(1, 0x1500, 180, 0x2500);
	Pwm3_Duty(PWM_CHA, 0x600);	

	Pwm3_On(PWM_CHA);
	RcServo_Move(1,90);
	

	PPI_BitOut(PD,0,1); 
	Delay (0x15); 
	PPI_BitOut(PD,0,0);

	angleServo();	
	Pwm3_Off(PWM_CHA);

	Delay(0xfff);

	Pwm3_Duty(PWM_CHA, 0x0);
	Pwm3_On(PWM_CHA);
	RcServo_Move(1,0);

	PPI_BitOut(PD,0,1); 
	Delay (0x15); 
	PPI_BitOut(PD,0,0);

	angleReverseServo();	
	Pwm3_Off(PWM_CHA);
}
void main(void)
{
	//Initialisation Seria
	UartSetBaud(1, 9600);
	UartOn(1);

	//Gestion LCD
	UartSetBaud(0,115200);
	UartOn(0);
	AlcdInit(0);
	AlcdClear(0);
	AlcdLocate(0,5,0);
	AlcdPrint(0,"En attente");
	AlcdLocate(0, 3, 1);
	AlcdPrint(0, "d'une commande");

	//Port evenement recepteur commande
	ExintSet(4,EXINT_HEDGE);
	ExintOn(4);

	PPI_Init();
	EXPORT_Init();
	PPI_Out(PD,0xfe);
	CaptureSet(0, CAPTURE_HIGH|CAPTURE_DIV1024);
	CaptureOn(0);
	EventOn();

	while(1){
		PPI_Out(PD, PPI_In(PA));
		receive = PPI_In(PA);
	}	
}
void PPI_Init(void)
{
	PPI_SetMode(PA,0xff); 
	PPI_Out (PA,0xff);
	PPI_SetMode(PB,0xff); 
	PPI_Out (PB,0xff); 
	PPI_SetMode(PC,0xff); 
	PPI_Out (PC,0xff);
	PPI_SetMode(PD,0x00);
	PPI_Out (PD,0xff);
}

void EXPORT_Init(void)
{
PortSetMode(EXPA,0xff); 
PortSetMode(EXPB,0xff); 
PortSetMode(EXPC,0xff); 
PortSetMode(EXPD,0xff);
}
