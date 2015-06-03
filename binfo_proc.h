#include <dldirent.h>

/* BINFO.C */
void	handle_err();
void	pr_exit();
void	init_scr();
void	get_do_func();
void	disp_parm();
int		check_dir();
int		get_func();
int		mk_blist();
int		dir_listserv();
void	mk_listd();
void	free_blist();
int		fileSort();
int		dir_listd();
int		dir_list();
void	free_dirlist();
int		is_dir();
void	cpy_lsp_str();
int		bid_check();
int		hexcmp();
int		set_check_id();
int		DL_dirlistserv();

/* BINFODISP.C */
void	bdisp_file();
int		disp_main();
int		next_scr();
void	disp_ini();
void	get_disp_fun();

/* BINFOMKF.C */
void	creat_file();
void	creat_main();
int		edit_out();
int		elapsed_time();
void	creat_ini();
void	get_creat_fun();

/* BINFOMSG.C */

/* BINFOPRNT.C */
void	print_file();
void	print_ini();
void	get_print_fun();
int		print_main();
void	print_header();
void	print_term();
int		printer_setup();
void	getplines();

/* BINFOSUB.C */
int		edit_data();
void	edit_cpy_bno();
void	cpy_reps();
void	cpy_repsx();
void	add_null();
void	charfill();
void	repl_null();
int		null_term();
int		qtoas();


/* ‚»‚Ì‘¼ */
DIR *fopendir();
FILE *fileopen();
char *alloc();
unsigned char *find();
char *timestr();
int dispmsg();
int handle();
int onsuspend();
int onintr();
int startutil();
int	tmodes();
int	getfiles();
int	disperr();
int	endutil();
int	ttyout();
int	display();
int getallargs();
int disptitle();
int highnfunc();
int dispnfunc();
int clrfield();
int dispfield();
int clrfield();
int dispfuncv();
int strcatl();
int disppr();
int getrec();
int filestat();
int fileclose();
int freaddir();
int fclosedir();
int status();
int breakline();
int fileseek();
int fileread();
int fmtstr();
int splen();
int qneg2();
int timetostr();
int filewrite();
int gettxchar();
int sysqueue();
int gettime();
int cvtime();
int initprtdefs();
int getpnames();
int fgetline();
