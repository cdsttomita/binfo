#include <stdio.h>
#include <string.h>
#include <dlstdio.h>
#include <sys/stypes.h>
#include <sys/filedesc.h>
#include <dlfdesc.h>
#include <quad.h>
#include <sigblk.h>
#include "/genv/include/termdefs.h"
#include "/genv/include/genvdefs.h"
#include "/genv/include/genvfmt.h"
#include "/genv/include/genvfci.h"
#include "/genv/include/deidefs.h"
#include "/genv/include/tmodes.h"
#include "/genv/include/argpaths.h"
#include "/genv/include/ctype.h"
#include "/genv/include/dir.h"
#include "../include/binfo.h"
#include "binfo_proc.h"

/* --------------------------------Globals--------------------------------- */
extern unsigned char Path_bn[PATHSIZE+1];	/* department full path */
extern unsigned char Pathbuf[PATHSIZE+1];	/* batch name full path */
extern struct BN_LIST *BN_list;
extern int  BN_cnt;
extern char *Modekeys[];
extern char *Displaykeys[];
extern char *Printkeys[];
extern char *MSGkeys[];
extern char Status[4];

/* -----------------------------Structures--------------------------------- */
extern struct FCIMAIN Fm;		/* structure for main Fci block */
extern struct BINFORM Bi;
extern struct OBJFN   objfn;
extern FILE *Infile;			/* data entry batch .file */
extern DATA_line[80+1];
extern BOOL dmode;
extern BOOL damode;
extern BOOL dtmode;
extern BOOL dnmode;
static int Disp_sel;


/* *********************************************************************** */
/*                 DISPLY BATCH INFO 					   */
/* *********************************************************************** */
void bdisp_file()
{
	disp_ini();
	/* set Disp_sel */
	get_disp_fun();
	disp_main();
}


/* *********************************************************************** */
/*                 DISPLY BATCH INFO MAIN				   */
/* *********************************************************************** */
disp_main()
{
	int		ct_id;
	int		ct, BID_ct;
	int		Char;
	int		dline_ct, bid_cnt;
	int		total_cnt[2];
	int		edit_cc;
	int		rolup;
	struct	BN_LIST *BN_ptr;
	unsigned char dline[80];
	unsigned char WKpath[PATHSIZE+1];
	unsigned char DSPbn[FILENMSZ+1];

	charfill(DSPbn,FILENMSZ,' ');
	DSPbn[FILENMSZ]= 0x00;

	BN_ptr= BN_list; 
	for (ct =0; BN_cnt > ct; ct++)
	{
		BID_ct= BN_ptr->bnbid_cnt;
		charfill(Pathbuf,PATHSIZE+1,NULL);
		strcatl(Pathbuf,Path_bn,"/",BN_ptr->bnptr_bn,NULL);
		ttyout(CURPOS,C41,L2,DATA,DSPbn,strlen(DSPbn),NULL);

		strncpy_s(dline, sizeof(dline), "JOB NAME : ", 11);
		cpy_reps(&dline[11],BN_ptr->bnptr_bn,FILENMSZ);
		add_null(&dline[11],FILENMSZ);

		ttyout(INVON,CURPOS,C31,L2,DATA,dline,strlen(dline),INVOFF,RESETATT,NULL);
	
		ttyout(VIDATT,_HIGHINTATT,_UNDRLNATT,CURPOS,C1,MIDWINTOP,DATA,MSGkeys[5],strlen(MSGkeys[5]),RESETATT,NULL);

		setwin(MIDWINTOP+1,MIDWINBOT,MIDWINLT,MIDWINRT);
		clrscr();
		dline_ct =0;
		bid_cnt =0;
		/* total rec count */ 	
		total_cnt[0] = total_cnt[1] = 0;
		rolup =OFF;
		
		for( ct_id=0;BID_ct >ct_id;ct_id++)
		{
			charfill(WKpath,PATHSIZE+1,NULL);
			strcatl(WKpath,Pathbuf,"/.",BN_ptr->bnptr_bid[ct_id],NULL);
			if ((Infile= fileopen(WKpath,"r") ) == NULL)
			{
				disperr("COULD NOT open fci_file(%s)",WKpath);
				continue;
			}
			/* ******************** */
			/* EDIT ==>> DATA_line  */
			/* ******************** */
			edit_cc= edit_data(Disp_sel,total_cnt);
			fileclose(Infile);

			if (edit_cc == 1)
			{
				if (rolup)
				{
					curpos(C1,L14);
					display(DATA_line);
					rollup();
				}
				else
				{
					curpos(C1,dline_ct);
					display(DATA_line);
				}
				bid_cnt++;
				if (++dline_ct >= 13)
				{
					disppr("Hit Release to display more,or Reset to exit");
					Char= next_scr();
					if (Char == _RESET)
					{
						resetwin();
						return(0);
					}
					rolup= ON;
					dline_ct = 0;
				}
			}
		} /* END of all batch ids in a batch name */

		if (bid_cnt)
		{
			if (rolup)
			{
				//curpos(C57,L14);	/* 2001/04/18 Y.shiino del S/NO:846 */
				curpos(C58,L14);	/* 2001/04/18 Y.shiino add S/NO:846 */
				display("TOTAL REC=  %5d %5d",total_cnt[0],total_cnt[1]);
				rollup();
				//curpos(C25,L14);	/* 2001/04/18 Y.shiino del S/NO:846 */
				curpos(C26,L14);	/* 2001/04/18 Y.shiino add S/NO:846 */
				display("BATCH ID COUNT = %d",bid_cnt);
			}
			else
			{
				//curpos(C57,dline_ct);		/* 2001/04/18 Y.shiino del S/NO:846 */
				curpos(C58,dline_ct);		/* 2001/04/18 Y.shiino del S/NO:846 */
				display("TOTAL REC=  %5d %5d",total_cnt[0],total_cnt[1]);
				//curpos(C25,dline_ct +1);	/* 2001/04/18 Y.shiino del S/NO:846 */
				curpos(C26,dline_ct +1);	/* 2001/04/18 Y.shiino del S/NO:846 */
				display("BATCH ID COUNT = %d",bid_cnt);
			}
		}
		else
		{
			if (rolup)
			{
				//curpos(C25,L13);		/* 2001/04/18 Y.shiino del S/NO:846 */
				curpos(C26,L13);		/* 2001/04/18 Y.shiino del S/NO:846 */
				display("SELECTED BATCH ID NOTHING");
				rollup();
			}
			else
			{
				//curpos(C25,dline_ct);	/* 2001/04/18 Y.shiino del S/NO:846 */
				curpos(C26,dline_ct);	/* 2001/04/18 Y.shiino del S/NO:846 */
				display("SELECTED BATCH ID NOTHING");
			}
		}

		resetwin();
		if (BN_cnt > ct +1)
			disppr("Hit Release to display next job ,or Reset to exit");
		else
			disppr("End of job name to be display,Hit Reset or Release to exit");
			Char= next_scr();
		if (Char == _RESET)
		{
			return(0);
		}
		BN_ptr++;
	} /* END of all batch name */
	return(1);
}


/* *********************************************************************** */
/*                 next_scr	 					   */
/* *********************************************************************** */
next_scr()
{
	int key;

	for(;;)
	{
		key = gettxchar(FLWR);
		switch (~key)
		{
			case _REL:
			case _RESET:
			{
				break;
			}
			case _END:
			{
				pr_exit(0);
			}
			default:
				continue;
		}
		break;
	}
	disppr("");
	return (~key);
}


/* *********************************************************************** */
/*                 INITIAL	 					   */
/* *********************************************************************** */
void disp_ini()
{
	disptitle("BATCH INFORMATION *DISPLAY*");
	setmidwin();
	clrscr();
	curoff();
	resetwin();
	dispfuncv(NFKEYS,Displaykeys);
}

/* *********************************************************************** */
/*                 GET FUNCTION  					   */
/* *********************************************************************** */
void get_disp_fun()
{
	int key;

	Disp_sel =0;
	if (dmode)
	{
		if (damode)  		Disp_sel =1;
		else if (dtmode)  	Disp_sel =2;
		else 		  	Disp_sel =3;
	}
	else
	{
		for(;;)
		{
			disppr("enter function key");
			key = gettxchar(FLWR);
			switch (~key)
			{
				case _SKEY1: {Disp_sel = 1; break;}
				case _SKEY2: {Disp_sel = 2; break;}
				case _SKEY3: {Disp_sel = 3; break;}
				case _SKEY4: { continue;}
				case _SKEY5: { continue;}
				case _SKEY6: { continue;}
				case _SKEY7: { continue;}
				case _SKEY8: { continue;}
	
				case _REL:
				case _RESET:
				{
					continue;
				}
				case _END:
				{
					pr_exit(0);
				}
				default:
					continue;
			} /* END OF switch */
			if (Disp_sel)
				break;
		} /* end of for loop */
	}

	highnfunc(Disp_sel,Displaykeys[Disp_sel -1]);
}
