#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#include "/genv/include/trandefs.h"
#include "/genv/include/printdefs.h"
#include "/genv/include/quedefs.h"
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
extern unsigned char DATA_line[80+1];
extern BOOL pmode;
extern BOOL pamode;
extern BOOL ptmode;
extern BOOL pnmode;
static int Print_sel;
struct	printdefs  printhdr;
FILE	*f_print= NULL;			/* for print file	*/
static struct BN_LIST *BN_ptr;
static unsigned char P_line[132] ;
static char *statptr;
static char 	printfile[PATHSIZE];
/* 2001/11/15 f.t add S/NO:945 -----> */
static int ETdevdesc_cnt=0;
static int ETdevdesc_wit=0;
static int ETdevdesc_witt=0;
/* <----- 2001/11/15 f.t add S/NO:945 */


/* *********************************************************************** */
/*                 PRINT BATCH INFO 					   */
/* *********************************************************************** */
void print_file()
{
	print_ini();
	/* SET Print_sel */ 
	get_print_fun();
	print_main();
	print_term();
}

/* *********************************************************************** */
/*                 INITIAL	 					   */
/* *********************************************************************** */
void print_ini()
{
	disptitle("BATCH INFORMATION *PRINT*");
	setmidwin();
	clrscr();
	resetwin();
	printer_setup();
	dispfuncv(NFKEYS,Printkeys);
	disppr("enter function key");
}


/* *********************************************************************** */
/*                 GET FUNCTION  					   */
/* *********************************************************************** */
void get_print_fun()
{
	int		key;

	Print_sel =0;
	if (pmode)
	{
		if(pamode)
			Print_sel = 1;
		else if(ptmode)
			Print_sel = 2;
		else
			Print_sel = 3;
	}
	else
	{
		for(;;)
		{
			key = gettxchar(FLWR);
			switch (~key)
			{
				case _SKEY1: {Print_sel = 1; break;}
				case _SKEY2: {Print_sel = 2; break;}
				case _SKEY3: {Print_sel = 3; break;}
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
			if (Print_sel)
				break;
		} /* end of for loop */
	}

	highnfunc(Print_sel,Printkeys[Print_sel -1]);
	disppr("");
}


/* *********************************************************************** */
/*                 PRINT  BATCH INFO MAIN				   */
/* *********************************************************************** */
print_main()
{
	int		ct_id;
	int		ct,BID_ct;
	//int		Char;
	int		dline_ct,bid_cnt;
	int		total_cnt[2];
	int		edit_cc;
	//int		w;
	int		firstflg;
	unsigned char dline[80];
	unsigned char WKpath[PATHSIZE+1];
	unsigned char DSPbn[FILENMSZ+1];

	charfill(DSPbn,FILENMSZ,' ');
	DSPbn[FILENMSZ]= 0x00;
	firstflg= 0;
	setmidwin();
	clrscr();
	curoff();
	curpos(20,5);
	display("*** IN PROCESS ,PLEASE WAIT A MINUTE ***");
	resetwin();

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
	
		dline_ct =0;
		bid_cnt =0;
		total_cnt[0] = total_cnt[1] = 0;	/* total rec count */ 	
		print_header(firstflg);
		firstflg =1;

		for( ct_id=0;BID_ct >ct_id;ct_id++)
		{
			charfill(WKpath,PATHSIZE+1,NULL);
			strcatl(WKpath,Pathbuf,"/.",BN_ptr->bnptr_bid[ct_id],NULL);	 /* DL VT '.' added to make fci file*/
			if ((Infile= fileopen(WKpath,"r") ) == NULL)
			{
				disperr("COULD NOT open fci_file(%s)",WKpath);
				continue;						/* 1987 6 4 E.Tashiro        */
			}
			/* ****** */
			/* EDIT   */
			/* ****** */
			edit_cc= edit_data(Print_sel,total_cnt);
			fileclose(Infile); /* DL VT */

			if (edit_cc == 1)
			{
				strncpy_s(P_line, sizeof(P_line), DATA_line, sizeof DATA_line);
				P_line[81]= '\n';
				P_line[82]= 0x00;
				filewrite(f_print,P_line,strlen(P_line) );
				bid_cnt++;
			}
		} /* END of all batch ids in a batch name */ 
		charfill(P_line,sizeof P_line,' ');
		if (bid_cnt)
		{
			//fmtstr(&P_line[56],"TOTAL REC=  %5d %5d\n \n",total_cnt[0],total_cnt[1]);	/* 2001/04/18 Y.shiino delS/NO:846 */
			fmtstr(&P_line[57],"TOTAL REC=  %5d %5d\n \n",total_cnt[0],total_cnt[1]);	/* 2001/04/18 Y.shiino add S/NO:846 */
			filewrite(f_print,P_line,strlen(P_line) );
			charfill(P_line,sizeof P_line,' ');
			//fmtstr(&P_line[25],"BATCH ID COUNT = %d\n",bid_cnt);	/* 2001/04/18 Y.shiino del S/NO:846 */
			fmtstr(&P_line[26],"BATCH ID COUNT = %d\n",bid_cnt);	/* 2001/04/18 Y.shiino add S/NO:846 */
			filewrite(f_print,P_line,strlen (P_line));
			charfill(P_line,sizeof P_line,' ');
		}
		else
		{
			//fmtstr(&P_line[25]," \nSELECTED BATCH ID NOTHING\n");	/* 2001/04/18 Y.shiino del S/NO:846 */
			fmtstr(&P_line[26]," \nSELECTED BATCH ID NOTHING\n");	/* 2001/04/18 Y.shiino add S/NO:846 */
			charfill(P_line,sizeof P_line,' ');
		}

		BN_ptr++;
	} /* END of all batch name */
	setmidwin();
	clrscr();
	resetwin();
	return(1);
}

/* *********************************************************************** */
/*                 PRINT  header					   */
/* *********************************************************************** */
void print_header(int firstflg)
{
	long  wn= 0;
	static unsigned char *FORMpos2 = {"\f \n \n"};
	static unsigned char *FORMpos1 = {" \n \n"};
	static char  *SELmsg[] =
	{
		"",
		"(ALL BATCHES)",
		"(TRANSFERED BATCHES)",
		"(NOT TRANSFERED BATCHES)",
	};

	if (firstflg == 0)
		 wn= filewrite(f_print,FORMpos1,strlen(FORMpos1));
	 else
		 wn= filewrite(f_print,FORMpos2,strlen(FORMpos2));

	charfill(P_line,sizeof P_line,' ');
	strncpy_s(&P_line[5], sizeof(P_line), "*** BATCH INFORMATION LIST ***", 30);
	strncpy_s(&P_line[36], sizeof(P_line), SELmsg[Print_sel], strlen(SELmsg[Print_sel]));
	strncpy_s(&P_line[89], sizeof(P_line), "DATE:", 5);
	strncpy_s(&P_line[95], sizeof(P_line), timestr(), 24);
	P_line[120]= '\n';
	P_line[121]= 0x00;
	wn= filewrite(f_print,P_line,strlen(P_line)); 

	charfill(P_line,sizeof P_line,' ');
	cpy_reps(&P_line[5],"DEPARTMENT:",11);
	cpy_repsx(&P_line[16],objfn.dept,FILENMSZ);
	wn= filewrite(f_print,P_line,strlen(P_line) );

	charfill(P_line,sizeof P_line,' ');
	cpy_reps (&P_line[5],"  JOB NAME:",11);
	cpy_repsx(&P_line[16],BN_ptr->bnptr_bn,FILENMSZ);
	wn= filewrite(f_print,P_line,strlen(P_line) );

	charfill(P_line,80,'_');
	P_line[81]= '\n';
	P_line[82]= 0x00;
	wn= filewrite(f_print,P_line,strlen(P_line) );

	strncpy_s(P_line, sizeof(P_line), MSGkeys[5], strlen(MSGkeys[5]));
	P_line[81]= '\n';
	P_line[82]= 0x00;
	wn= filewrite(f_print,P_line,strlen(P_line));

	charfill(P_line,80,'-');
	P_line[81]= '\n';
	P_line[82]= 0x00;
	wn= filewrite(f_print,P_line,strlen(P_line));
}


/* *********************************************************************** */
/*                 PRINT  terminate 					   */
/* *********************************************************************** */
void print_term()
{
	fileclose(f_print);
	sysqueue(printfile,&printhdr,statptr);
}


/*************************************************************************
	print file set up
**************************************************************************/
printer_setup()
{
	long	time;
	char    time_str[30];
	int		tvec[7];
	char	asc_time[6 + 1];
	char	*array[9];

	time = gettime();
	timetostr(time, time_str); 
	cvtime(time, tvec);
	fmtstr(asc_time, "%02d%02d%02d", tvec[4], tvec[5], tvec[6]); 
	strcatl(printfile, QTMPLIST,".BINFO", asc_time, NULL);
	if ((f_print = fileopen(printfile,"r+"))== NULL)
		 f_print = fileopen(printfile, "w+");
	initprtdefs(&printhdr);
	getpnames(array);

	//printhdr.pr_lines = 66;			/* lines per page *//* 2001/11/15 f.t del S/NO:945 */
	printhdr.pr_lines = ETdevdesc_cnt;	/* lines per page *//* 2001/11/15 f.t add S/NO:945 */

	strcpy_s(printhdr.pr_orig_name, sizeof(printhdr.pr_orig_name), "binfo list");
	strcpy_s(printhdr.pr_queue_name, sizeof(printhdr.pr_queue_name), array[0]);
	return(0);
}


/* 2001/11/15 f.t add S/NO:945 -----> */
void getplines()
{
	char buf[4096],*array[10];
	FILE *wfp;
	int n;

	if((wfp = fileopen("/etc/devdesc.txt","r")) == NULL)
	{
		disperr("Can't open '/dev/devdesc.txt'");
	}
	else
	{
		while((n = fgetline(wfp,buf,128))>0)
		{
			if(buf[0] == ' ' || buf[0] == '*' || buf[0] == '}' || buf[0] == '{')
				continue;
			if(!strncmp("print",&buf[0],5))
			{
				breakline(buf,':',array,10);
				ETdevdesc_cnt=atoi(array[7]);									/* 1998.07.25 f.t yyyy*/
				ETdevdesc_wit=atoi(array[4]);									/* 1999/10/20 f.t S/NO:358 */
				if(ETdevdesc_wit<1 || ETdevdesc_wit>=130) ETdevdesc_wit=130;	/* 1999/10/20 f.t S/NO:358 */
				ETdevdesc_witt=ETdevdesc_wit/10;								/* 1999/10/20 f.t S/NO:358 */
				break;
			}
		}
		fileclose(wfp);
	}
}
/* <----- 2001/11/15 f.t add S/NO:945 */
