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
extern  char *getusrnam();
extern unsigned char Path_bn[PATHSIZE+1];	/* department full path */
extern unsigned char Pathbuf[PATHSIZE+1];
extern struct BN_LIST *BN_list;
extern int  BN_cnt;
extern char *Modekeys[];
extern char *Createkeys[];
extern char *Displaykeys[];
extern char *Printkeys[];
extern char *IOPRMsg[];
extern char *FCIPRMsg[];
extern char *MSGkeys[];
extern char Status[4];

/* -----------------------------Structures--------------------------------- */
extern struct FCIMAIN Fm;		/* structure for main Fci block */
extern struct BINFORM Bi;
extern struct OBJFN   objfn;
extern FILE *Infile;			/* data entry batch fci file */
FILE   *BATCH_infof =0;			/* BINFO_FILE	"/tmp/binfo_file" */
extern BOOL cmode;
extern BOOL camode;
extern BOOL ctmode;
extern BOOL cnmode;
static Create_sel;

/* ************************************************************************/
/*                 CREATE BATCH INFO FILE				   */
/* *********************************************************************** */
void creat_file()
{
	creat_ini();
	/* set Create_sel	*/
	get_creat_fun();
	creat_main();
}


void creat_main()
{
	int		ct,BID_ct;
	int		ct_id;
	int		bid_cnt;
	int		total_cnt[2];
	int		edit_cc;
	struct	BN_LIST *BN_ptr;
	unsigned char WKpath[PATHSIZE+1];

	if ( (BATCH_infof =fileopen(BATCH_INFO,"w+")) == 0)	/* DL VT ctrw -> w+ */
	{
		disperr("COULD NOT creat %s",BATCH_INFO);
		pr_exit(-1);
	}
	setmidwin();
	clrscr();

	BN_ptr= BN_list; 
	for (ct =0; BN_cnt > ct; ct++)
	{
		BID_ct= BN_ptr->bnbid_cnt;
		charfill(Pathbuf,PATHSIZE+1,NULL);
		strcatl(Pathbuf,Path_bn,"/",BN_ptr->bnptr_bn,NULL);

		bid_cnt = 0;
		total_cnt[0] = total_cnt[1] = 0;	/* total rec ccount	*/

		for( ct_id=0;BID_ct >ct_id;ct_id++)
		{
			charfill(WKpath,PATHSIZE+1,NULL);
			strcatl(WKpath,Pathbuf,"/.",BN_ptr->bnptr_bid[ct_id],NULL); /* DL VT . added */
			if ((Infile= fileopen(WKpath,"r") ) == NULL)
			{
				disperr("COULD NOT open fci_file(%s)",WKpath);
				continue;
			}
			edit_cc = edit_data(Create_sel, total_cnt);
			if (edit_cc == 1)
			{
				curpos(0,14);
				display("FILE= %s",WKpath);
				rollup();
				edit_out();
				bid_cnt++;
			}
			fileclose(Infile);
		}
		BN_ptr++;
	}
	resetwin();
	fileclose(BATCH_infof);
}


/***************************************************************************/
/*                 EDIT FCI FILE ==> batch info file 			   */
/* *********************************************************************** */
edit_out()
{
	int		i,j;
	int		rdl;
	int		totins;			/* get total inserts */
	int		totdel;			/* get total deletes */
	unsigned char fmtwk[9];
	unsigned char timebuf[9];
	unsigned char *pnmwk;

	/* make sure we read the beginning */
	fileseek( Infile, (long)0 ) ;

	/* read into fci struct */
	if ( (rdl = fileread(Infile,&Fm,FCINSIZE) ) <=0)
	{
		disperr("COULD NOT READ %s",Pathbuf);
		return(-1);
	}
	
	charfill(&Bi,sizeof (Bi),' ');
	strncpy_s(Bi.drtdir, sizeof(Bi.drtdir), Fm.fc_drtdir, FILENMSZ);
	strncpy_s(Bi.dname, sizeof(Bi.dname), Fm.fc_dname, FILENMSZ);
	strncpy_s(Bi.dbtch, sizeof(Bi.dbtch), Fm.fc_dbtch, FILENMSZ);
	strncpy_s(Bi.frtdir, sizeof(Bi.frtdir), Fm.fc_frtdir, FILENMSZ);
	strncpy_s(Bi.fname, sizeof(Bi.fname), Fm.fc_fname, FILENMSZ);

	for( i=0 ; i < MAXMODES ; i++ )
	{
		Bi.stat[i] = Status[Fm.fc_stat[i]] ;
		Bi.dopen[i] = Fm.fc_dopen[i] ;
	}

	for( i=0 ; i < MAXIO ; i++ )
	{
		fmtstr(fmtwk,"%02d",Fm.fc_ioiter[i]);
		strncpy_s(Bi.ioiter[i], sizeof(Bi.ioiter[i]), fmtwk, 2);

		if( Fm.fc_iouid[i] > 0 )
		{
			fmtstr( fmtwk, "%03d", Fm.fc_iouid[i] ) ;
			strncpy_s(Bi.iouid[i], sizeof(Bi.iouid[i]), fmtwk, 3);
			if( ( pnmwk = getusrnam( Fm.fc_iouid[i] ) ) != (char *)ERRCODE )
			{
				strncpy_s(Bi.iouname[i], sizeof(Bi.iouname[i]), pnmwk, NMSZ);
			}
			else
			{
				charfill(Bi.iouname[i],NMSZ,'*');
			}
		}
		else
		{
			charfill(Bi.iouid[i],3,'*');
			charfill(Bi.iouname[i],NMSZ,'*');
		}
		Bi.iostatus[i] = Fm.fc_iostatus[i];
	}

	for (i=0; i< MAXMODES;i++)
	{
		fmtstr(fmtwk,"%05d",Fm.fc_rcntt[i]);
		strncpy_s(Bi.rcntt[i], sizeof(Bi.rcntt[i]), fmtwk, 5);

		fmtstr(fmtwk,"%05d",Fm.fc_dcntt[i]);
		strncpy_s(Bi.dcntt[i], sizeof(Bi.dcntt[i]), fmtwk, 5);

		fmtstr(fmtwk,"%08d",Fm.fc_kscntt[i]);
		strncpy_s(Bi.kscntt[i], sizeof(Bi.kscntt[i]), fmtwk, 8);

		fmtstr(fmtwk,"%08d",Fm.fc_kscnttf[i]);
		strncpy_s(Bi.kscnttf[i], sizeof(Bi.kscnttf[i]), fmtwk, 8);

		fmtstr(fmtwk,"%05d",Fm.fc_ercntt[i]);
		strncpy_s(Bi.ercntt[i], sizeof(Bi.ercntt[i]), fmtwk, 5);

		fmtstr(fmtwk,"%08d",Fm.fc_ekscntt[i]);
		strncpy_s(Bi.ekscntt[i],sizeof(Bi.ekscntt[i]) , fmtwk, 8);

		/* error k.s.count of function key */
		fmtstr(fmtwk,"%08d",Fm.fc_ekscnttf[i]);
		strncpy_s(Bi.ekscnttf[i], sizeof(Bi.ekscnttf[i]), fmtwk, 8);

		/* total flag count */
		fmtstr(fmtwk,"%05d",Fm.fc_flgcntt[i]);
		strncpy_s(Bi.flgcntt[i], sizeof(Bi.flgcntt[i]), fmtwk, 5);

		if (Fm.fc_fstime[i] == 0)
			charfill(Bi.fstime[i],24,'*');
		else
			timetostr(Fm.fc_fstime[i],Bi.fstime[i]);

		if (Fm.fc_strtime[i] == 0)
			charfill(Bi.strtime[i],24,'*');
		else
			timetostr(Fm.fc_strtime[i],Bi.strtime[i]);

		if (Fm.fc_lstime[i] == 0)
			charfill(Bi.lstime[i],24,'*');
		else
			timetostr(Fm.fc_lstime[i],Bi.lstime[i]);

		if (elapsed_time(i,timebuf) )   /* calculate total elapsed time */
			strncpy_s(Bi.eltimet[i], sizeof(Bi.eltimet[i]), timebuf, 8);
		else
			charfill(Bi.eltimet[i],8,'*');
	}

	/* # inserts, # deletes */
	totins = Fm.fc_mins[0] + Fm.fc_mins[1];	/* total inserts */
	totdel = Fm.fc_mdel[0] + Fm.fc_mdel[1];	/* total deletes */
	fmtstr(Bi.mins,"%05d",totins);
	fmtstr(Bi.mdel,"%05d",totdel);

	totins = Fm.fc_mdins[0] + Fm.fc_mdins[1];	/* total doc inserts */
	totdel = Fm.fc_mddel[0] + Fm.fc_mddel[1];	/* total doc deletes */
	fmtstr(Bi.mdins,"%05d",totins);
	fmtstr(Bi.mddel,"%05d",totdel);

	for (i=0; i< MAXMODES;i++)
	{
		for (j=0 ;j < MAXUSER;j++)
		{
			if (Fm.fc_muid[j][i] > 0)
		   {
			   fmtstr(fmtwk,"%03d",Fm.fc_muid[j][i]);
			   strncpy_s(Bi.muid[j][i], sizeof(Bi.muid[j][i]), fmtwk, 3);
			   if ( (pnmwk=getusrnam(Fm.fc_muid[j][i]) ) != (char *)ERRCODE)
				{
					strncpy_s(Bi.muidname[j][i], sizeof(Bi.muidname[j][i]), pnmwk, NMSZ);
				}
			     else
				{
					charfill(Bi.muidname[j][i],NMSZ,'*');
				}
			}
			else
			{
				charfill(Bi.muid[j][i],3,'*');
				charfill(Bi.muidname[j][i],NMSZ,'*');
			}
		}
	}

	Bi.sysfile = Fm.fc_sysfile;

	for (i=0; i< MAXMODES;i++)
	{
		fmtstr(fmtwk,"%05d",Fm.fc_iter[i]);
		strncpy_s(Bi.iter[i], sizeof(Bi.iter[i]), fmtwk, 5);
	}

	for (i=0; i< MAXIO;i++)
	{
		fmtstr(fmtwk,"%05d",Fm.fc_iositer[i]);
		strncpy_s(Bi.iositer[i], sizeof(Bi.iositer[i]), fmtwk, 5);

		fmtstr(fmtwk,"%05d",Fm.fc_iorcnt[i]);
		strncpy_s(Bi.iorcnt[i], sizeof(Bi.iorcnt[i]), fmtwk, 5);

		fmtstr(fmtwk,"%05d",Fm.fc_iodcnt[i]);
		strncpy_s(Bi.iodcnt[i], sizeof(Bi.iodcnt[i]), fmtwk, 5);

		if ( Fm.fc_iostime[i] == 0)
			charfill(Bi.iostime[i],24,'*');
		else
			timetostr(Fm.fc_iostime[i],Bi.iostime[i]);

		if (Fm.fc_ioetime[i] == 0)
			charfill(Bi.ioetime[i],24,'*');
		else
			timetostr(Fm.fc_ioetime[i],Bi.ioetime[i]);

		fmtstr(fmtwk,"%05d",Fm.fc_iopen[i]);
		strncpy_s(Bi.iopen[i], sizeof(Bi.iopen[i]), fmtwk, 5);
	}

	Bi.endmark= ' ';
	filewrite(BATCH_infof,&Bi,sizeof (Bi));
	filewrite(BATCH_infof,"\n",1);
	return( 0 ) ;
}

/* ---------------------------  calculate elapsed time ------------------- */
elapsed_time( int mode, char *timebuf )
{
	//int		tottime ;			/* store total time */
	int		hrs ;
	int		min ;
	int		sec ;

	if ( Fm.fc_eltimet[mode] == 0)
		return(0);
	else
	{
		hrs = Fm.fc_eltimet[mode] / 3600;
		sec = Fm.fc_eltimet[mode] % 3600;
		min = sec / 60;
		sec %= 60;
		fmtstr(timebuf,"%d:%02d:%02d",hrs,min,sec);
	}
	return(1);
}


/* *********************************************************************** */
/*                 INITIAL	 					   */
/* *********************************************************************** */
void creat_ini()
{
	disptitle("BATCH INFORMATION *CREATION*");
	setmidwin();
	clrscr();
	curoff();
	resetwin();
	dispfuncv(NFKEYS,Createkeys);
}


/* *********************************************************************** */
/*                 GET FUNCTION  					   */
/* *********************************************************************** */
void get_creat_fun()
{
	int		key;

	Create_sel =0;
	if (cmode)
	{
		if(camode)
			Create_sel = 1;
		else if(ctmode)
			Create_sel = 2;
		else
			Create_sel = 3;
	}
	else
	{
		for(;;)
		{
			disppr("enter function key");
			key = gettxchar(FLWR);
			switch (~key)
			{
				case _SKEY1: {Create_sel = 1; break;}
				case _SKEY2: {Create_sel = 2; break;}
				case _SKEY3: {Create_sel = 3; break;}
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
			}

			if (Create_sel)
				break;
		}
	}
	highnfunc(Create_sel,Createkeys[Create_sel -1]);
}
