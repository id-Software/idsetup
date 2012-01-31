#include <stdlib.h>
#include <conio.h>
#include "setup.h"

#ifdef DEBUG
pup_t	far *puplist[]=
{
	&askpres,
	&cmodem,
	&consel,
	&control,
	&cserial,
	&cwarp,
	&gusirqer,
	&idcard,
	&idjoysel,
	&idkeysel,
	&idmain2,
	&idmousel,
	&irqerr,
	&macros,
	&mcard,
	&midiport,
	&modemchs,
	&modsave,
	&mousentr,
	&mouspres,
	&netplay,
	&netplay2,
	&netsave,
	&netserr,
	&netwk2,
	&numdig,
	&phonelst,
	&quitwin,
	&sbdma,
	&sbirq,
	&sbport,
	&sersave,
	&show,
	&sockerr,
	&title,
	NULL
};

void ShowAllPups(void)
{
	int	i;

	i = 0;
	while(puplist[i])
	{
		DrawPup(puplist[i++]);
		getch();
	}
}
#endif
