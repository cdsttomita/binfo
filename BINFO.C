#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <dlstdio.h>
#include <sys/stypes.h>
#include <sys/filedesc.h>
#include <dlfdesc.h>
#include <dldirent.h>
#include <sys/stat.h>
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
/*
extern unsigned char *set_dept() ;
extern unsigned char *getenv() ;
extern char *alloc() ;
extern char *find() ;
*/
static unsigned char *set_dept();
static fname_cmp();
static num_cmp();
static addfile();

int		Allflag = 0 ;
BOOL	AllFiles = FALSE ;

typedef struct FilePtr
{
	//LPCTSTR fName;
	char	*fName;
	struct	FilePtr *next;
}; 	

FILE	*Infile = NULL ;
//int		pr_exit() ;
char	**Argv ;							/* global copy of argv  */
int		Argc ;								/* global copy of argc  */
static	int Savemodes = 0;					/* save tty modes       */
unsigned char Path_bn[PATHSIZE+1] ={ 0};	/* department full path */
unsigned char Pathbuf[PATHSIZE+1] ={ 0};	/* batch name full path */

struct	BN_LIST *BN_list;
int		BN_cnt;

extern	char *Modekeys[];
extern	char *Displaykeys[];
extern	char *Printkeys[];
extern	char *MSGkeys[];

char Status[4] =	/* job status */
{
	'X',
	'A',			/* batch active */
	'I',			/* interrupted */
	'C'				/* completed */
} ;

unsigned	char DATA_line[80+1] ;

/* -----------------------------Structures--------------------------------- */
struct FCIMAIN Fm ={ 0};	/* structure for main Fci block */
struct BINFORM Bi ={0};
struct OBJFN   objfn  ={ 0};

SIGBLK Sig;

struct GFCNTS Gfcounts ={ 0};
#define POS(P) (((short)(( struct OBJFN *)0l)->P))
struct FASB Fnfasb[] =
{
/*	  	         c  s 	m e c     p f       a   c p h   c p o a a 
	h  v   l   o  c  a  i u u x o e m r i v a a u g h r e t o a f x x 
	o  e   e   u  o  s  g p s i n n o o l e l c t e k m l a n g f p o
	r  r   n   t  l  e  n   t t t d d c l r d c o n d t p b t e s g f*/

/*00*/ {C13,L1,FILENMSZ,POS(dept),	/* depart field */
		      0,_AL,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*01*/ {C13,L2,FILENMSZ,POS(batchname),/* batch name field */
		      0,_AL,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
/*02*/ {C13,L3,/*PATHSIZE DL VT */45,POS(batchid),	/* batch id field */
		      0,_AL,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0}
};

/* option */
char *options = "ca,ct,cn,da,dt,dn,pa,pt,pn";

BOOL mkfmode =0;
BOOL camode =0;
BOOL ctmode =0;
BOOL cnmode =0;
BOOL damode =0;
BOOL dtmode =0;
BOOL dnmode =0;
BOOL pamode =0;
BOOL pnmode =0;
BOOL ptmode =0;
BOOL cmode =0;
BOOL dmode =0;
BOOL pmode =0;
BOOL mode =0;

struct BIDCLIST
{
	char bid_type;
	char *bid_min;
	char *bid_max;
};

struct	BIDCLIST *Bidclist;
unsigned char Bidcline[80];
char	*stdalon;

/* -------------------------file write err handler------------------------- */
void handle_err( char *name, int arg1, int arg2 )
{
	/* file write error, exiting fcimaint */
	dispmsg( 27, MSGkeys[4] ) ;
	pr_exit( EX_SYSERR ) ;
}

/****************************************************************************/
main( int argc, char **argv )
{
	unsigned char *strfn;

	/* handle file write errors */
	handle( "file_write_err",&Sig, handle_err, 0 ) ;

	onsuspend( pr_exit ) ;
	onintr( pr_exit ) ;
	startutil() ;

	Argc = --argc;
	Argv = ++argv;

	/* save the tty modes we got in here with */
	Savemodes = tmodes( TMODEGET ) ;
	tmodes( TMODESET1 ) ;

	charfill( objfn, sizeof(objfn), NULL ) ;
	charfill( Pathbuf, sizeof(Pathbuf), NULL ) ;

	if (getallargs(&argc,&argv,options,
				&camode,&ctmode,&cnmode,
				&damode,&dtmode,&dnmode,
				&pamode,&ptmode,&pnmode) < 0)
	{
		dispmsg( 599 ) ;
		pr_exit( EX_BADARGS ) ;
	}

	cmode = (camode || ctmode ||cnmode);
	dmode = (damode || dtmode ||dnmode);
	pmode = (pamode || ptmode ||pnmode);
	mode  = (cmode  || dmode  ||pmode );

	if (getfiles(&Argc,&Argv,options) >0)
	{
		strfn = Argv[0];
		strfn = set_dept(strfn,objfn.dept);
		strcpy (objfn.batchname,strfn);
		if (Argv[1] && Argv[1][0]!='-')
		{
			strncpy(objfn.batchid, Argv[1], PATHSIZE);
		}
		else
		{
			strcpy(objfn.batchid,  "*");
		}
	}
	else
	{
		if ( (getenv("DEPT")) == 0)
		{
			disperr("DEPT not declared");
			pr_exit(EX_BADARGS);
		}
		strcpy(objfn.dept,getenv("DEPT"));
		strcpy(objfn.batchname,"*");
		strcpy(objfn.batchid,  "*");
	}

	do{
		get_do_func();
	} while (!mode);

	pr_exit(EX_OK);
}


void pr_exit( int rtncc)
{
	endutil(rtncc,TRUE,ESCR_NONE);
}


/* --------------- init stuff, clear screen, display fkeys ---------------- */
/***************************************************************************/
/*                 set department					   */
/***************************************************************************/
static unsigned char *set_dept( unsigned char *str, unsigned char *dept )
{
	unsigned char matchch;
	unsigned char *deptptr;

	*dept= 0x00;
	if (*str == '/' || *str == '{' )
	{
		matchch = (*str == '/') ? '/' :'}';
		str++;
		while (*str && *str != matchch)
			*dept++ = *str++;
		*dept = 0x00;
		if (*str != matchch)
			return(NULL);
		/* str== next string addr */
		str++;
	}
	else
	{
		if (( deptptr = getenv("DEPT")) == NULL)
		{
			disperr("DEPT not declared");
			pr_exit(-1);
		}
		else
			strcpy(dept,deptptr);
	}
	return(str);
}


/***************************************************************************/
/*                 INIT SCREEN Command line  & Function keys               */
/* *********************************************************************** */
void init_scr()
{
	setmidwin();			/* set and clear middle screen */
	clrscr();
	resetwin();

	settopwin();
	clrscr();
	curpos(C49,L1);
	display ("DATE:");
	curpos(C55,L1);
	display(timestr());
	resetwin();

	disptitle("** BATCH INFORMATION **");

	curpos(C1,L1);
	display(MSGkeys[1]);
	curpos(C1,L2);
	display(MSGkeys[2]);
	curpos(C1,L3);
	display(MSGkeys[3]);
}

/***************************************************************************/
/*                 MAIN PROCESS GET FUNCTION && GO 		           */
/* *********************************************************************** */
void get_do_func()
{
	static int SKEYNO;
	static int Goflg =0;

	init_scr();
	disp_parm();
	if( mode )
	{
		if( cmode )
			SKEYNO = 1 ;
		else if( dmode )
			SKEYNO = 2 ;
		else
			SKEYNO = 3 ;
	}
	else
	{
		while( !(SKEYNO= get_func()) )
			;
	}

	highnfunc(SKEYNO,Modekeys[SKEYNO -1]);

	if (check_dir() ==0)
		return;

	mk_blist();
	switch (SKEYNO)
	{
		case 1:
		{
			creat_file();
			free_blist();
			break;
		}
		case 2:
		{
			bdisp_file();
			free_blist();
			break;
		}
		case 3:
		{
			print_file();
			free_blist();
			break;
		}
		default:
		{
			break;
		}
	}
	dispnfunc(SKEYNO,Modekeys[SKEYNO -1]);
}

/***************************************************************************/
/*                 DISP PARM	 					   */
/* *********************************************************************** */
void disp_parm()
{
	clrfield( &Fnfasb[0] ) ;
	clrfield( &Fnfasb[1] ) ;

	dispfield( &Fnfasb[0], objfn.dept, FILENMSZ ) ;
	dispfield( &Fnfasb[1], objfn.batchname, FILENMSZ ) ;
	dispfield( &Fnfasb[2], objfn.batchid, 68 ) ;
	
	/* display starting function keys */
	dispfuncv( NFKEYS, Modekeys ) ;
}

/***************************************************************************/
/*                 CHECK DIRECTORY 					   */
/* *********************************************************************** */
check_dir()
{
	char *homeptr, home[50];

	repl_null( objfn.dept, FILENMSZ ) ;
	repl_null( objfn.batchname, FILENMSZ ) ;
	repl_null( objfn.batchid, PATHSIZE ) ;

	if( ( homeptr = getenv("GHOME") ) != NULL )
		strcpy( home, homeptr ) ;
	else 
		strcpy( home, "/" ) ;

 	strcatl( Path_bn, "/", (home[0] == '/' ? home+1 : home), "/", objfn.dept, "/", "de/batches", NULL ) ;

	strcpy(Pathbuf,Path_bn);

	if( objfn.batchname[0] != '*' )
		strcatl( Pathbuf, Path_bn, "/", objfn.batchname, NULL ) ;

	if( ( is_dir( Pathbuf ) ) != 1 )
	{
		disperr( "%s IS NOT DIRECTORY", Pathbuf ) ;
		/* error return */
		return( 0 ) ;
	}

	if( set_check_id( objfn.batchid ) != YES )
	{
		disperr( "CHECK BATCH ID %s..", objfn.batchid ) ;
		return( NO ) ;
	}
	return( 1 ) ;
}

/***************************************************************************/
/*                 GET FUNCTION 					   */
/* *********************************************************************** */
get_func()
{
	static int fieldnum =0;
	static int fieldmode; 
	static int dummy = 0;
	static int AFUN =0;
	static int SKEYno;
 
	Gfcounts.gf_start = Gfcounts.gf_lastc = Gfcounts.gf_cursor = 0;
	Gfcounts.gf_kscnt = Gfcounts.gf_funcnt = Gfcounts.gf_errcnt = &dummy;
	Gfcounts.gf_stat = 0x00;

	fieldmode = MMODE;
	fieldnum  = 0;
	settopwin();
	SKEYno = 0;

	disppr("enter function key");
	while (	!SKEYno)
	{
		AFUN= getrec(Fnfasb,&Gfcounts,&objfn,&fieldnum,fieldmode);
		fieldmode |= RESTART;

		if (Gfcounts.gf_stat) 
		{
			if (fieldnum >= 2)
				fieldnum = 0;
			else if (fieldnum == 0)
			{
				if (objfn.dept[0] == '*')
				{
					disperr("CAN NOT SPECIFY * IN DEPARTMENT");
					continue;
				}
				fieldnum++;
			}
			else fieldnum++;
			continue;
		}
		else
		{
			switch (~AFUN)
			{
				case _SKEY1:
				{
					SKEYno = 1;
					break;
				}
				case _SKEY2:
				{
					SKEYno = 2;
					break;
				}
				case _SKEY3:
				{
					SKEYno = 3;
					break;
				}
				case _SKEY4:
				{
					break;
				}
				case _SKEY5:
				{
					break;
				}
				case _SKEY6:
				{
					break;
				}
				case _SKEY7:
				{
					break;
				}
				case _SKEY8:
				{
					break;
				}
				case _END:
				{
					resetwin();
					pr_exit(0);
					exit(0);
				}
				case _REL:
				{
					fieldnum =0;
					continue;
				}
				default:
				{
					bell();
					continue;
				}
			}
		}
	}/* END of while */

	resetwin() ;
	return( SKEYno ) ;
}

/***************************************************************************/
/*                 MAKE BATCH NAME LIST 				   */
/* *********************************************************************** */
mk_blist()
{
	int		ct, ct_id, BID_ct ;
	struct	BN_LIST *BN_ptr;
	unsigned char WKpath[PATHSIZE+1];

	curoff();
	if (objfn.batchname[0] == '*')
	{
		stdalon = getenv("STDALONE");
		if (stdalon && (*stdalon == 'Y'))
			BN_cnt = dir_listd(Path_bn,&BN_list); 
		else
			BN_cnt = dir_listserv(Path_bn,&BN_list);

		if (BN_cnt == 0)
		{
			dispmsg("%s IS EMPTY DIRECTORY",Path_bn);
			return(0);
		}
		if ( BN_cnt < 0)
		{
			dispmsg("%s IS BAD DIRECTORYd",Path_bn);
			return(0);
		}
	}
	else
	{
		BN_cnt = 1;
		mk_listd(objfn.batchname,&BN_list);
	}

	setmidwin();
	BN_ptr= BN_list; 
	for (ct =0; BN_cnt > ct; ct++)
	{
		BID_ct= BN_ptr->bnbid_cnt;
		charfill(Pathbuf,PATHSIZE+1,NULL);
		strcatl(Pathbuf,Path_bn,"/",BN_ptr->bnptr_bn,NULL);
		for( ct_id=0;BID_ct >ct_id;ct_id++)
		{
			charfill(WKpath,PATHSIZE+1,NULL);
			strcatl(WKpath,Pathbuf,"/.",BN_ptr->bnptr_bid[ct_id],NULL);	 /* DL VT .added */
			curpos(0,14);
			display("(%02d) BATCHNAME= %8s,(%02d) BATCH ID= %4s",ct,BN_ptr->bnptr_bn,ct_id,BN_ptr->bnptr_bid[ct_id]);
			rollup();
		}
		BN_ptr++;
	}
	resetwin();
	return( 0 ) ; /* ??? */
}


dir_listserv( unsigned char *dirname, struct BN_LIST **bndir )
{
	struct	dirent *dirlib; 
	struct	BN_LIST  *BN_ptr;
	//struct	stat stbf;
	DIR		*dirp;
	int		j;	/* 86.08.04 M.kbys add register	*/
	FILE	*fp;
	int		tabno;
	int		tablesize;
	//int		fcount;
	FILEDESC	fdesc;

	dirlib = (struct dirent *)malloc(sizeof(struct dirent)); /* DL VT */
	/* 2000/06/15 Y.shiino S/NO:508 -----> */
	//dirlib->d_name[0] = (char *)malloc(31);
	dirlib->d_name = (char *)malloc(31);
	/* <----- 2000/06/15 Y.shiino S/NO:508 */

	/* open directory */
	dirp = fopendir( dirname ) ;
	if( dirp == NULL )
	{
		*bndir = NULL ;
		return( -1 ) ;
	}

	/* test for directory */
	if( ( fp = fileopen( dirname, "r" ) ) == NULL )
		return( -1 ) ;

	filestat( fp, &fdesc ) ;
	fileclose( fp ) ;
	tabno = ( fdesc.f_size / sizeof(struct dirent) ) ;
	tablesize = tabno * sizeof(struct BN_LIST) + sizeof (struct BN_LIST) ;
	*bndir= (struct BN_LIST *)alloc( tablesize ) ;

	/* read dir entries */
	j = 0 ;
	BN_ptr = *bndir ;

	/* read dir entries */
	while (freaddir(dirp,dirlib) >= 0)
	{
		/* good entry add to table */
		if (dirlib->d_ino == -1 )
			continue;
		if ( ( strcmp(dirlib->d_name,".") == 0) || ( strcmp(dirlib->d_name,"..") == 0))
			continue;
       	j++;

		charfill(Pathbuf,FILENMSZ,NULL);
		strcatl(Pathbuf,Path_bn,"/",dirlib->d_name,NULL);
		BN_ptr->bnbid_cnt = DL_dirlistserv(Pathbuf,&BN_ptr->bnptr_bid);
		
		BN_ptr->bnptr_bn = alloc(FILENMSZ);
		strncpy(BN_ptr->bnptr_bn,dirlib->d_name,FILENMSZ);
		BN_ptr++;
	}
	BN_ptr = NULL;

	/* 2000/06/15 Y.shiino S/NO:508 -----> */
	//free(dirlib->d_name[0]);
	free(dirlib->d_name);
	/* <----- 2000/06/15 Y.shiino S/NO:508 */

	free(dirlib);	

	(void)fclosedir( dirp ) ;
	/* return count filled */
	return( j ) ;
}


void mk_listd( char *bn, struct BN_LIST **bndir)
{
	struct BN_LIST *BN_ptr;
	BN_ptr= *bndir = (struct BN_LIST *)alloc(sizeof (struct BN_LIST) );

	BN_ptr->bnptr_bn = alloc(FILENMSZ);

	strncpy(BN_ptr->bnptr_bn,bn,FILENMSZ);
	charfill(Pathbuf,FILENMSZ,NULL);
	strcatl(Pathbuf,Path_bn,"/",bn,NULL);

	stdalon = getenv("STDALONE");
	if (stdalon && (*stdalon == 'Y'))
		BN_ptr->bnbid_cnt = dir_list(Pathbuf,&BN_ptr->bnptr_bid);
	else
		BN_ptr->bnbid_cnt = DL_dirlistserv(Pathbuf,&BN_ptr->bnptr_bid);

	BN_ptr++;
}


/**************************************************************************/
/*                 FREE BATCH NAME LIST 				  */
/* ********************************************************************** */
void free_blist()
{
	struct	BN_LIST *BN_ptr ;
	int		ct;
	//int		ct_id;
	//int		BID_ct ;

	BN_ptr = BN_list ;
	for( ct=0 ; BN_cnt > ct ; ct++, BN_ptr++ )
	{
		free(BN_ptr->bnptr_bn);
	}
	free(BN_list);
}


int fileSort(const void * p1, const void * p2)
{
	if(strlen(*(char **)p1) < strlen(*(char **)p2))	return(-1);	/* CTC.FT.1998.09.01 G5_TR_000075 */
	if(strlen(*(char **)p1) > strlen(*(char **)p2)) return(1);	/* CTC.FT.1998.09.01 G5_TR_000075 */
	return ( strcmp( *(char **)p1, *(char **)p2 ) );
}		


dir_listd( unsigned char *dirname, struct BN_LIST **bndir )
{
	struct	FilePtr *start, *end, *temp;  
	struct	BN_LIST  *BN_ptr;
	LPCTSTR	ptr;
	HANDLE	han;
	//int		k;
	int		j=0;
	int		count=0;
	WIN32_FIND_DATA fD;

	start = temp = end = NULL;					

	ptr = malloc( strlen( dirname ) +5 );
	strcpy( (char *)ptr, (char *)dirname );
	if( ptr[strlen(ptr)-1] != '/' )
		strcat( (char *)ptr, "/" );
	strcat( (char *)ptr, "*.*" );

	if( ( han = FindFirstFile( ptr, &fD ) ) != INVALID_HANDLE_VALUE )
	{
		do
		{
			if( !strcmp(fD.cFileName,".") || !strcmp(fD.cFileName, ".."))
				continue;
			if( AllFiles || fD.cFileName[0] != '.' )
			{
				temp = ( struct FilePtr * )malloc( sizeof( struct FilePtr ) );
				temp->next = NULL;
				if( start == NULL )
				{
					start = temp;
					end = temp;
				}
				else 
				{
					end->next = temp;
					end = temp;
				}
				temp->fName = (char *)malloc( strlen( fD.cFileName )+1 );
				strcpy( (char *)temp->fName, fD.cFileName );	
	  			count++;
			}
		} 
		while( FindNextFile( han, &fD ) );
		FindClose( han );
	}
	else
	{
		return( -1 );
	}

	free((char *)ptr);
//	*bndir = malloc( count * sizeof( void * ) + sizeof( void * ) ); /*mod:2011.05.11 */
	*bndir = (struct BN_LIST *)malloc( count * sizeof(struct BN_LIST)+ sizeof( void * ) );
	
	/* read dir entries */
	j = 0 ;
	BN_ptr = *bndir;
	temp = start;

	while( temp != NULL )
	{	
		j++;

		charfill(Pathbuf,FILENMSZ,NULL);
		strcatl(Pathbuf,Path_bn,"/",temp->fName ,NULL);
		stdalon = getenv("STDALONE");
		if (stdalon && (*stdalon == 'Y'))
			BN_ptr->bnbid_cnt = dir_list(Pathbuf,&BN_ptr->bnptr_bid);
		else
			BN_ptr->bnbid_cnt = DL_dirlistserv(Pathbuf,&BN_ptr->bnptr_bid);

		BN_ptr->bnptr_bn = (char *) malloc(FILENMSZ);
		strncpy(BN_ptr->bnptr_bn,temp->fName ,FILENMSZ);
		BN_ptr++;
		temp = temp->next;
	}

	BN_ptr = NULL;
	temp = start;

	while (temp != NULL)
	{
		free((char *)temp->fName);
		end = temp;
		temp=temp->next;
		free(end);
	}
	
	return( count ) ;
}


dir_list( char *dirname, char ***files )
{
//	struct	FilePtr *start, *end, *temp ;		/* SN0548 Y.shiino 2011/01/06 */
	struct	FilePtr *start, *end ;				/* SN0548 Y.shiino 2011/01/06 */
	static struct	FilePtr *temp=0x00 ;		/* SN0548 Y.shiino 2011/01/06 */
	LPCTSTR	ptr ;
	HANDLE	han ;
	int		k, count=0 ;
	WIN32_FIND_DATA fD ;

	/* SN0548 Y.shiino 2011/01/06 -----> */
/*CNO	while (temp != 0x00)
	{
		free(temp->fName);
		end = temp;
		temp=temp->next;
		free(end);
	}
	/* <----- SN0548 Y.shiino 2011/01/06 */

	start = temp = end = NULL ;
	ptr = malloc( strlen( dirname ) +5 );
	strcpy( (char *)ptr, dirname );

	if( ptr[strlen(ptr)-1] != '/' )
		strcat( (char *)ptr, "/" );
	strcat( (char *)ptr, "*.*" );

	if( ( han = FindFirstFile( ptr, &fD ) ) != INVALID_HANDLE_VALUE )
	{
		do
		{
//			if( !strcmp(fD.cFileName,".") || !strcmp(fD.cFileName, ".."))								// SN0548 Y.shiino 2011/01/06
			if( !strcmp(fD.cFileName,".") || !strcmp(fD.cFileName, "..") || fD.cFileName[0] == '.' )	// SN0548 Y.shiino 2011/01/06
				continue;
//			if( AllFiles || fD.cFileName[0] != '.' )													// SN0548 Y.shiino 2011/01/06
			if( bid_check( fD.cFileName ) )																// SN0548 Y.shiino 2011/01/06
			{
				temp = ( struct FilePtr * )malloc( sizeof( struct FilePtr ) );
				temp->next = NULL;
				if( start == NULL )
				{
					start = temp;
					end = temp;
				}
				else 
				{
					end->next = temp;
					end = temp;
				}
				temp->fName = (char *)malloc( strlen( fD.cFileName )+1 );
				strcpy( (char *)temp->fName, fD.cFileName );
	  			count++;
			}
		} 
		while( FindNextFile( han, &fD ) );
		FindClose( han );
	}
	else
	{
		return( -1 );
	}

	free((char *)ptr);
	//*files = (char *)malloc( count * sizeof( void * ) + sizeof( void * ) );
	*files = (char **)malloc( count * sizeof( void * ) + sizeof( void * ) );
	temp = start;
	k = 0;

	while( temp != NULL )
	{	
		files[0][k] = temp->fName;
		temp = temp->next;
		k++;
	}
	temp = start;
	/* 2001/04/18 Y.shiino del S/NO:864 ----->
	while (temp != NULL)
	{
		free(temp->fName);
		end = temp;
		temp=temp->next;
		free(end);
	}
	<----- 2001/04/18 Y.shiino del S/NO:864 */
		
	files[0][k] = NULL;
	qsort( *files, count, sizeof( char* ), fileSort ); // DL 01.09.1995
	return( count ) ;
}


/****************************************************************************/
/* free_dirlist -- frees storage previously allocated by a call to dirlist. */
/* arguments : array of structures to free                                  */
/* size of array                                                            */
/* return : N/A                                                             */
/* ************************************************************************ */
void free_dirlist( unsigned char *files )
{
	int j;

	if (files == NULL)
	{
		return;
	}

	/* free until null or array end	*/
	for( j=0 ; files[j] != 0x00 ; j++ )
	{
		free( (char *)files[j] ) ;
		files[j] = 0x00 ;
	}
	free(files);
}

/*************************************************************************/ 
/* 	ck directory is really a directory				 */
/* ********************************************************************* */ 
is_dir( char *path )
{
	FILEDESC	fdesc;

	if (status(path,&fdesc) < 0)
	{
		/* bad dir */
		return( 0 ) ;
	}

	/* return 1 if good */
	return((fdesc.f_flags & FISADIR) == FISADIR);
}


/* -------------------------------------------------------------------- */
static fname_cmp( unsigned char *s1, unsigned char *s2 )
{
	unsigned char fname1[FILENMSZ+1];
	unsigned char fname2[FILENMSZ+1];

	cpy_lsp_str(fname1,s1,FILENMSZ+1);
	cpy_lsp_str(fname2,s2,FILENMSZ+1);

	return( strcmp( fname1, fname2 ) ) ;
}


/* -------------------------------------------------------------------- */
void cpy_lsp_str( unsigned char *fname, unsigned char *str, int l )
{
	unsigned char *fp = fname+l-1 ;
	unsigned char *sp = str+strlen(str)-1 ;
	int		i = l-1 ;
	int		j = strlen(str) ;

	*fp-- = 0x00 ;
	while(i--)
		*fp-- = ' ' ;

	while (j-- && *sp-- == ' ')	;

	sp++;
	fp= fname +strlen(fname) -1;
	while ( (j--) && (*sp != '.') )
		*fp-- = *sp--;
}


/* -------------------------------------------------------------------- */
static num_cmp( unsigned char *s1, unsigned char *s2 )
{
	//unsigned char	buf1[FILENMSZ+1], buf2[FILENMSZ+1];
	int		len1, len2;

	/* treat numerics as less than all other ascii */
	if (!isdigit(*s2))
	{
		return(strcmp(s1, s2));
	}

	/* strip off leading zeroes */
	while (*s1 && *s1 == '0')
		s1++;

	while (*s2 && *s2 == '0')
		s2++;

	/* get lengths of strings */
	len1 = strlen(s1);
	len2 = strlen(s2);

	/* if lengths are different, can determine order by length */
	/* if they are the same, strcmp will handle it */
	if (len1 != len2)
	{
		return(len1 - len2);
	}
	else
	{
		return(strcmp(s1, s2));
	}
}


bid_check( unsigned char *bid )
{
	struct BIDCLIST *bidclist = Bidclist;
	int min,max;

	if (!Bidclist)
	{
		return(YES);
	}
	while (bidclist->bid_type)
	{
		min = hexcmp(bidclist->bid_min, bid);
		max = hexcmp(bidclist->bid_max, bid);

		if (min<=0 && max>=0) return(YES);
		bidclist++;
	}
	return(NO);
}


hexcmp( unsigned char *s1, unsigned char *s2 )
{
	int l1 = strlen(s1);
	int l2 = strlen(s2);

	if (l1==l2)
	{
		return(strcmp(s1,s2));
	}
	else if(l1>l2)
		return(1);
	else
		return(-1);
}


set_check_id( unsigned char *adr )
{
	int		len ;
	unsigned char *pch ;
	int		cnt, cnt1 ;
	unsigned char *bidary[20] ;
	struct	BIDCLIST *bidclist ;
	unsigned char *bidc = Bidcline ;

	Bidclist = bidclist = NULL ;
	if( !( len = strlen( adr ) ) )
	{
		return(NO);
	}
	pch = Bidcline;
	while( len-- )
	{
		if( *adr!=' ' && *adr!='(' && *adr!=')' )
		{
			*pch++ = *adr;
		}
		adr ++ ;
	}
	*pch = '\0' ;
	if( *Bidcline == '*' )
	{
		return( YES ) ;
	}
	breakline( bidc, ',', bidary, 20 ) ;
	cnt = 0 ;

	while( *bidary[cnt] )
	{
		cnt ++ ;
	}
	if( !cnt )
	{
		return( NO ) ;
	}

	Bidclist = bidclist = (struct BIDCLIST *)alloc( (cnt+1) * sizeof(struct BIDCLIST) ) ;
	charfill( Bidclist, (cnt+1) * sizeof (struct BIDCLIST), '\0' ) ;
	for( cnt1=0 ; cnt1 < cnt ; cnt1++, bidclist++ )
	{
		if( ( pch = find( '-', bidary[cnt1] ) ) )
		{
			*pch++ = '\0' ;
			bidclist->bid_min = bidary[cnt1] ;
			bidclist->bid_max = pch ;
			bidclist->bid_type= 'm' ;
		}
		else
		{
			bidclist->bid_min = bidary[cnt1] ;
			bidclist->bid_max = bidary[cnt1] ;
			bidclist->bid_type= 's' ;
		}
	}

	return(YES);
}


DL_dirlistserv(unsigned char *dirname, unsigned char ***files)
{
	struct	dirent *dirlib;
	//struct	stat stbf;
	DIR		*dirp;
	int 	j;
	FILE	*fp;
	int		tablesize;
	//int		fcount;
	FILEDESC	fdesc;

	dirlib = (struct dirent *)malloc(sizeof(struct dirent)); /* DL VT */

	/* 2000/06/15 Y.shiino S/NO:508 -----> */
	//dirlib->d_name[0] = (char *)malloc(31);
	dirlib->d_name = (char *)malloc(31);
	/* <----- 2000/06/15 Y.shiino S/NO:508 */

	/* open directory */
	dirp = fopendir(dirname); 
	if (dirp == NULL)
	{
		*files = NULL;
		return(-1);
	}
	/* test for directory */
	if( ( fp = fileopen( dirname, "r" ) ) == NULL )
		return( -1 ) ;

	filestat(fp,&fdesc);
	fileclose(fp);
	tablesize = (fdesc.f_size/sizeof(struct dirent))*sizeof(unsigned char *)+sizeof(unsigned char *);
	//*files = alloc(tablesize);
	*files = (unsigned char **)alloc(tablesize);

	/* read dir entries */
	j = 0 ;
	while (freaddir(dirp,dirlib) >= 0)
	{	/* good entry add to table */
		if( dirlib->d_ino == -1 || (!Allflag && dirlib->d_name[0] == '.') )
		{
			continue;
		}
		if ( ( strcmp(dirlib->d_name,".") == 0) || ( strcmp(dirlib->d_name,"..") == 0))
			continue;
		if (bid_check(dirlib->d_name))
		{
       		if (addfile(*files, j, dirlib) < 0)
			{
				fclosedir(dirp);
				return(-1);
			}
			j++;
		}
	}
	/* 2000/06/15 Y.shiino S/NO:508 -----> */
	//free(dirlib->d_name[0]);
	free(dirlib->d_name);
	/* <----- 2000/06/15 Y.shiino S/NO:508 */
	free(dirlib);	

	(void)fclosedir(dirp);

	/* NULL terminate table */
	(*files)[j] = NULL ;
	qsort( *files, j, sizeof( char* ), fileSort );  /* CTC.FT.1998.09.01 G5_TR_000075 */
	/* return count filled */
	return( j ) ;
}


static addfile(unsigned char **filesadd, int j, struct dirent *dir)
{
	int		k ;

	/* Start at end of table and find place to 		*/
	/* insert new file entry.  (j is next slot to fill) 	*/
	for (k = j; k > 0; k--)
	{
		if (fname_cmp(dir->d_name, filesadd[k-1]) >= 0)
		{
			/* found place to insert */
			break;
		}
		else
		{
			/* move entry up one */
			filesadd[k] = filesadd[k-1];
		}
	}

	/* get storage for entry */
	filesadd[k] = (unsigned char *)alloc(strlen(dir->d_name)+1);
	if (filesadd[k] == NULL)
	{
		return(-1);
	}

	/* fill entry */
	strcpy(filesadd[k], dir->d_name);

	/* normal return */
	return(0);
}
