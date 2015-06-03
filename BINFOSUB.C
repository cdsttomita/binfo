#include <stdio.h>
#include <string.h>
#include <sys/stypes.h>
#include <sys/filedesc.h>
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
extern char *getusrnam();
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


/***************************************************************************/
/*                 EDIT FCI FILE ==> DATA_line	 			   */
/* *********************************************************************** */
edit_data( int Select, int rec_cnt[2] )
{
	unsigned char edit_wk[8];
	unsigned char *pnmwk;
	int		rdl;
	int		uid;
	int		SEL_sw,sel_ch1,sel_ch2;
	int		sel_ch3;
#ifdef TFA
	int		sel_ch4;
	char	save1;
	char	save2;
#endif TFA

	/* make sure we read the beginning */
	fileseek( Infile, (long)0 ) ;

	/* read into fci struct */
	if ( (rdl = fileread(Infile,&Fm,FCINSIZE) ) <=0)
	{
		disperr("COULD NOT READ %s",Pathbuf);
		return(-1);
	}

	if (Select == 1)
		;
	else
	{
		/*----- added disk status for Tokyo Facility -----*/
#ifdef TFA
		SEL_sw = (  (sel_ch1 = Fm.fc_iostatus[TAPE])  || (sel_ch2 = Fm.fc_iostatus[COMM]) ||
					(sel_ch3 = Fm.fc_iostatus[FLPPY]) || (sel_ch4 = Fm.fc_iostatus[DISK] ));
#else TFA
		/*----- added floppy status -----*/
		SEL_sw = (  (sel_ch1 = Fm.fc_iostatus[TAPE]) || (sel_ch2 = Fm.fc_iostatus[COMM]) ||
					(sel_ch3 = Fm.fc_iostatus[FLPPY]) );
#endif TFA

		if( SEL_sw && (Select == 2)  )
			;
		else if( (!SEL_sw) && (Select == 3) )
			;
		else
			return(0);
	}
	charfill( DATA_line, sizeof(DATA_line), ' ' ) ;
	DATA_line[81] = 0x00 ;

	//edit_cpy_bno(&DATA_line[0],Fm.fc_dbtch,5,FILENMSZ);	/* 2001/04/18 Y.shiino del S/NO:846 */
	edit_cpy_bno(&DATA_line[0],Fm.fc_dbtch,6,FILENMSZ);		/* 2001/04/18 Y.shiino add S/NO:846 */
	/* ------------------------------------------ set floppy status ---------- */
	//DATA_line[5]= DATA_line[7]= DATA_line[34]= DATA_line[36]=DATA_line[63]=DATA_line[67]= '|';/* S/NO:846 */
	DATA_line[6]= DATA_line[8]= DATA_line[35]= DATA_line[37]=DATA_line[64]=DATA_line[68]= '|';	/* S/NO:846 */
	//DATA_line[6] = Status[Fm.fc_stat[0]];			/* S/NO:846 */
	DATA_line[7] = Status[Fm.fc_stat[0]];			/* S/NO:846 */
	/* ------------------------------------------ set floppy status ---------- */
	//DATA_line[35]= Status[Fm.fc_stat[1]];			/* S/NO:846 */
	DATA_line[36]= Status[Fm.fc_stat[1]];			/* S/NO:846 */

	uid = Fm.fc_muid[0][0];
	if ( (uid != 0)  && (uid != -1 ) && ( (pnmwk=getusrnam(uid) ) != ( char * )ERRCODE) )
		//cpy_reps(&DATA_line[8],pnmwk,NMSZ);		/* S/NO:846 */
		cpy_reps(&DATA_line[9],pnmwk,NMSZ);			/* S/NO:846 */
	else
		//cpy_reps(&DATA_line[8],"........",NMSZ);	/* S/NO:846 */
		cpy_reps(&DATA_line[9],"........",NMSZ);	/* S/NO:846 */

	uid = Fm.fc_muid[1][0];
	if ( (uid != 0)  && (uid != -1 ) && ( (pnmwk=getusrnam(uid) ) != ( char *)ERRCODE) )
		//cpy_reps(&DATA_line[17],pnmwk,NMSZ);		/* S/NO:846 */
		cpy_reps(&DATA_line[18],pnmwk,NMSZ);		/* S/NO:846 */
	else
		//cpy_reps(&DATA_line[17],"........",NMSZ);	/* S/NO:846 */
		cpy_reps(&DATA_line[18],"........",NMSZ);	/* S/NO:846 */

	uid = Fm.fc_muid[2][0];
	if ( (uid != 0)  && (uid != -1 ) && ( (pnmwk=getusrnam(uid) ) !=( char *) ERRCODE) )
		//cpy_reps(&DATA_line[26],pnmwk,NMSZ);		/* S/NO:846 */
		cpy_reps(&DATA_line[27],pnmwk,NMSZ);		/* S/NO:846 */
	else
		//cpy_reps(&DATA_line[26],"........",NMSZ);	/* S/NO:846 */
		cpy_reps(&DATA_line[27],"........",NMSZ);	/* S/NO:846 */

	/* ---------------------------------------- set floppy status ----------- */
	uid = Fm.fc_muid[0][1];
	if ( (uid != 0)  && (uid != -1 ) && ( (pnmwk=getusrnam(uid) ) != ( char *)ERRCODE) )
		//cpy_reps(&DATA_line[37],pnmwk,NMSZ);		/* S/NO:846 */
		cpy_reps(&DATA_line[38],pnmwk,NMSZ);		/* S/NO:846 */
	else
		//cpy_reps(&DATA_line[37],"........",NMSZ);	/* S/NO:846 */
		cpy_reps(&DATA_line[38],"........",NMSZ);	/* S/NO:846 */

	uid = Fm.fc_muid[1][1];
	if ( (uid != 0)  && (uid != -1 ) && ( (pnmwk=getusrnam(uid) ) != ( char *)ERRCODE) )
		//cpy_reps(&DATA_line[46],pnmwk,NMSZ);		/* S/NO:846 */
		cpy_reps(&DATA_line[47],pnmwk,NMSZ);		/* S/NO:846 */
	else
		//cpy_reps(&DATA_line[46],"........",NMSZ);	/* S/NO:846 */
		cpy_reps(&DATA_line[47],"........",NMSZ);	/* S/NO:846 */

	uid = Fm.fc_muid[2][1];
	if ( (uid != 0)  && (uid != -1 ) && ( (pnmwk=getusrnam(uid) ) !=( char *) ERRCODE) )
		//cpy_reps(&DATA_line[55],pnmwk,NMSZ);		/* S/NO:846 */
		cpy_reps(&DATA_line[56],pnmwk,NMSZ);		/* S/NO:846 */
	else
		//cpy_reps(&DATA_line[55],"........",NMSZ);	/* S/NO:846 */
		cpy_reps(&DATA_line[56],"........",NMSZ);	/* S/NO:846 */

	/* -------------------------------------- set floppy status ----------- */
	//DATA_line[64]= Fm.fc_iostatus[TAPE];	/* S/NO:846 */
	DATA_line[65]= Fm.fc_iostatus[TAPE];	/* S/NO:846 */
	//if (DATA_line[64] == NULL)			/* S/NO:846 */
	if (DATA_line[65] == 0x00)				/* S/NO:846 */
		//DATA_line[64] = 'X';				/* S/NO:846 */
		DATA_line[65] = 'X';				/* S/NO:846 */

	//DATA_line[65]= Fm.fc_iostatus[COMM];	/* S/NO:846 */
	DATA_line[66]= Fm.fc_iostatus[COMM];	/* S/NO:846 */
	//if (DATA_line[65] == NULL)			/* S/NO:846 */
	if (DATA_line[66] == 0x00)				/* S/NO:846 */
		//DATA_line[65] = 'X';				/* S/NO:846 */
		DATA_line[66] = 'X';				/* S/NO:846 */

#ifdef TFA
	/* ------------------------------- added disk status for Tokyo Facility -- */
	save1 = Fm.fc_iostatus[FLPPY];
	save2 = Fm.fc_iostatus[DISK];
	//if ((save1 != NULL) && (save2 != NULL))	   	DATA_line[66] = 'C';	/* S/NO:846 */
	if ((save1 != NULL) && (save2 != NULL))	   		DATA_line[67] = 'C';	/* S/NO:846 */
	//else if ((save1 != NULL) && (save2 == NULL)) 	DATA_line[66] = 'F';	/* S/NO:846 */	
	else if ((save1 != NULL) && (save2 == NULL)) 	DATA_line[67] = 'F';	/* S/NO:846 */
	//else if ((save1 == NULL) && (save2 != NULL)) 	DATA_line[66] = 'D';	/* S/NO:846 */
	else if ((save1 == NULL) && (save2 != NULL)) 	DATA_line[67] = 'D';	/* S/NO:846 */
	//else									   		DATA_line[66] = 'X';	/* S/NO:846 */
	else									   		DATA_line[67] = 'X';	/* S/NO:846 */
#else
	//DATA_line[66] = Fm.fc_iostatus[FLPPY];	/* S/NO:846 */
	DATA_line[67] = Fm.fc_iostatus[FLPPY];		/* S/NO:846 */
	//if (DATA_line[66] == NULL)				/* S/NO:846 */
	if (DATA_line[67] == 0x00)					/* S/NO:846 */
		//DATA_line[66] = 'X';					/* S/NO:846 */
		DATA_line[67] = 'X';					/* S/NO:846 */
#endif TFA


	fmtstr(edit_wk,"%5d",Fm.fc_rcntt[0]);
	//cpy_reps(&DATA_line[68],edit_wk,5);	/* S/NO:846 */
	cpy_reps(&DATA_line[69],edit_wk,5);		/* S/NO:846 */
	rec_cnt[0] += Fm.fc_rcntt[0];

	fmtstr(edit_wk,"%5d",Fm.fc_rcntt[1]);
	//cpy_reps(&DATA_line[74],edit_wk,5);	/* S/NO:846 */
	cpy_reps(&DATA_line[75],edit_wk,5);		/* S/NO:846 */
	rec_cnt[1] += Fm.fc_rcntt[1];

	if (Fm.fc_rcntt[0] == 0)
		//DATA_line[72] = '0';				/* S/NO:846 */
		DATA_line[73] = '0';				/* S/NO:846 */
	if (Fm.fc_rcntt[1] == 0)
		//DATA_line[78] = '0';				/* S/NO:846 */
		DATA_line[79] = '0';				/* S/NO:846 */
	return(1);
}


/* ******************************************************************** */
/*            edit _cpy_bno						*/
/* ******************************************************************** */
void edit_cpy_bno( unsigned char *linea, unsigned char *bnoa, int la, int lb )
{
	unsigned char *ba= linea +la -1;
	unsigned char *bb= bnoa  +lb -1;
	int ia,ib;

	charfill(linea,la,' ');
	for (ib= lb ; (ib)  && (*bb-- != ')') ; ib--)
		;
	for (ia = la; (ib) && (ia) && (*bb != '(') ; ia--,ib--)
		*ba-- = *bb--;
}


/* ******************************************************************** */
/*            cpy_resp							*/
/* ******************************************************************** */
void cpy_reps( unsigned char *d, unsigned char *s, int l )
{
	int		i ;

	for( i=0 ;l > i ; i++, d++, s++ )
	{
		if (*s ==0x00)
			*d = ' ';
		else
			*d = *s;
	}
}


/* ******************************************************************** */
/*            cpy_respx							*/
/* ******************************************************************** */
void cpy_repsx( unsigned char *d, unsigned char *s, int l )
{
	//int		i ;

	while( *s )
		*d++ = *s++ ;

	*d++ = '\n';
	*d++ = 0x00;
}


/* ******************************************************************** */
/*            add_null 							*/
/* ******************************************************************** */
void add_null( unsigned char *s, int l )
{
	unsigned char *p = s +l -1;

	while (*p == ' ')
		p--;

	p++;
	*p = 0x00;
}


/* ******************************************************************** */
/*            charfill							*/
/* ******************************************************************** */
void charfill( unsigned char *buff, int length, char ch)
{
	while (length-- > 0)
		*buff++ = ch;
}


/* ******************************************************************** */
/*            repl_null							*/
/* ******************************************************************** */
void repl_null( unsigned char *str, int l )
{
	unsigned char *p = str +l -1;
	
	while ( (l--) && (*p == ' ') )
		*p-- = 0x00;
}


/* ------------------routine to null terminate a field------------------ */
null_term( char *buf, int size )
{
	int		len;

	len = splen(buf,size);
	buf[len] = '\0';
	return(len);
}


/*...........................................................................
*      FUNCTION:QTOAS (quad to ascii with sign)
*...........................................................................*/
qtoas( short *num, char *buf )
{
	static	short qc[QVAL] ={ 0};
	short	qn[QVAL];
	int		sign=0;

	if (qcmp(num,qc) <0)
	{
		sign = 1;
		qneg2(num,qn);
		qtoa(qn,buf);
	}
	else
	{
		qtoa(num,buf);
	}
	if (sign)
	{
		strcat_s(buf,sizeof(buf),"-");
	}
	return( 0 ) ; /* ??? */
}
