# Microsoft Visual C++ Generated NMAKE File, Format Version 2.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE ｺﾝﾌｨｸﾞﾚｰｼｮﾝが指定されていません。ﾃﾞﾌｫﾙﾄの Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "Win32 Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "binfo.mak" CFG="Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 
!ERROR 無効なｺﾝﾌｨｸﾞﾚｰｼｮﾝが指定されています。
!ENDIF 

################################################################################
# Begin Project
# PROP Target_Last_Scanned "Win32 Debug"
OUTDIR=\genv\tmpx
INTDIR=\tempfile

ALL : $(OUTDIR)/binfo.exe $(OUTDIR)/binfo.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

$(INTDIR) : 
    if not exist $(INTDIR)/nul mkdir $(INTDIR)

CPP=cl.exe
RSC=rc.exe
BSC32=bscmake.exe
BSC32_SBRS= \
	$(INTDIR)/BINFOMSG.SBR \
	$(INTDIR)/BINFO.SBR \
	$(INTDIR)/BINFOSUB.SBR \
	$(INTDIR)/BINFOMKF.SBR \
	$(INTDIR)/BINFODISP.SBR \
	$(INTDIR)/BINFOPRNT.SBR
LINK32=link.exe
DEF_FILE=
LINK32_OBJS= \
	$(INTDIR)/BINFOMSG.OBJ \
	$(INTDIR)/BINFO.OBJ \
	$(INTDIR)/BINFOSUB.OBJ \
	$(INTDIR)/BINFOMKF.OBJ \
	$(INTDIR)/BINFODISP.OBJ \
	$(INTDIR)/BINFOPRNT.OBJ
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CONSOLE"
# PROP BASE Intermediate_Dir "CONSOLE"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\genv\tmpx"
# PROP Intermediate_Dir "\tempfile"
# ADD BASE CPP /nologo /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FR /c
# ADD CPP /nologo /w /W0 /GX /Zi /YX /Od /D "WIN32" /D "NDEBUG" /D "KANA" /D "NEW" /D "OVS" /D "L0S" /D "REV5" /D "KANJI" /D "TCM" /D "MKR" /D "DECIMAL" /D "KNK" /D "REN" /D "RSH" /D "KSC" /D "TCK" /D "USEDLL" /Fr /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"binfo.bsc" 
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /NOLOGO /SUBSYSTEM:console /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib liball_dp.lib file_lib.lib dlunos.lib simcon.lib /NOLOGO /INCREMENTAL:yes /DEBUG /MACHINE:I386 /FORCE /SUBSYSTEM:windows/INCREMENTAL:no
# SUBTRACT LINK32 /PDB:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib liball_dp.lib\
 file_lib.lib dlunos.lib simcon.lib /NOLOGO /INCREMENTAL:yes\
 /PDB:$(OUTDIR)/"binfo.pdb" /DEBUG /MACHINE:I386 /FORCE\
 /OUT:$(OUTDIR)/"binfo.exe" /SUBSYSTEM:windows/INCREMENTAL:no 
CPP_PROJ=/nologo /w /W0 /GX /Zi /YX /Od /D "WIN32" /D "NDEBUG" /D "KANA" /D\
 "NEW" /D "OVS" /D "L0S" /D "REV5" /D "KANJI" /D "TCM" /D "MKR" /D "DECIMAL" /D\
 "KNK" /D "REN" /D "RSH" /D "KSC" /D "TCK" /D "USEDLL" /Fr$(INTDIR)/\
 /Fp$(OUTDIR)/"binfo.pch" /Fo$(INTDIR)/ /Fd$(OUTDIR)/"binfo.pdb" /c 
CPP_OBJS=\tempfile/

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

$(OUTDIR)/binfo.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

$(OUTDIR)/binfo.exe : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=.\BINFOMSG.C
DEP_BINFO=\
	..\INCLUDE\BINFOMSG.H

$(INTDIR)/BINFOMSG.OBJ :  $(SOURCE)  $(DEP_BINFO) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BINFO.C
DEP_BINFO_=\
	\dl\include\dlstdio.h\
	\dl\include\sys\STYPES.H\
	\dl\include\sys\FILEDESC.H\
	\dl\include\dlfdesc.h\
	\dl\include\dldirent.h\
	\dl\include\QUAD.H\
	\dl\include\SIGBLK.H\
	\genv\include\TERMDEFS.H\
	\genv\include\GENVDEFS.H\
	\genv\include\GENVFMT.H\
	\genv\include\GENVFCI.H\
	\genv\include\DEIDEFS.H\
	\genv\include\TMODES.H\
	\genv\include\ARGPATHS.H\
	\genv\include\CTYPE.H\
	\genv\include\DIR.H\
	..\INCLUDE\BINFO.H\
	\dl\include\STANDARD.H\
	\dl\include\sys\dldirent.h\
	\genv\include\SECGEN.H

$(INTDIR)/BINFO.OBJ :  $(SOURCE)  $(DEP_BINFO_) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BINFOSUB.C
DEP_BINFOS=\
	\dl\include\sys\STYPES.H\
	\dl\include\sys\FILEDESC.H\
	\dl\include\QUAD.H\
	\dl\include\SIGBLK.H\
	\genv\include\TERMDEFS.H\
	\genv\include\GENVDEFS.H\
	\genv\include\GENVFMT.H\
	\genv\include\GENVFCI.H\
	\genv\include\DEIDEFS.H\
	\genv\include\TMODES.H\
	\genv\include\ARGPATHS.H\
	\genv\include\CTYPE.H\
	\genv\include\DIR.H\
	..\INCLUDE\BINFO.H\
	\genv\include\SECGEN.H

$(INTDIR)/BINFOSUB.OBJ :  $(SOURCE)  $(DEP_BINFOS) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BINFOMKF.C
DEP_BINFOM=\
	\dl\include\dlstdio.h\
	\dl\include\sys\STYPES.H\
	\dl\include\sys\FILEDESC.H\
	\dl\include\dlfdesc.h\
	\dl\include\QUAD.H\
	\dl\include\SIGBLK.H\
	\genv\include\TERMDEFS.H\
	\genv\include\GENVDEFS.H\
	\genv\include\GENVFMT.H\
	\genv\include\GENVFCI.H\
	\genv\include\DEIDEFS.H\
	\genv\include\TMODES.H\
	\genv\include\ARGPATHS.H\
	\genv\include\CTYPE.H\
	\genv\include\DIR.H\
	..\INCLUDE\BINFO.H\
	\dl\include\STANDARD.H\
	\genv\include\SECGEN.H

$(INTDIR)/BINFOMKF.OBJ :  $(SOURCE)  $(DEP_BINFOM) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BINFODISP.C
DEP_BINFOD=\
	\dl\include\dlstdio.h\
	\dl\include\sys\STYPES.H\
	\dl\include\sys\FILEDESC.H\
	\dl\include\dlfdesc.h\
	\dl\include\QUAD.H\
	\dl\include\SIGBLK.H\
	\genv\include\TERMDEFS.H\
	\genv\include\GENVDEFS.H\
	\genv\include\GENVFMT.H\
	\genv\include\GENVFCI.H\
	\genv\include\DEIDEFS.H\
	\genv\include\TMODES.H\
	\genv\include\ARGPATHS.H\
	\genv\include\CTYPE.H\
	\genv\include\DIR.H\
	..\INCLUDE\BINFO.H\
	\dl\include\STANDARD.H\
	\genv\include\SECGEN.H

$(INTDIR)/BINFODISP.OBJ :  $(SOURCE)  $(DEP_BINFOD) $(INTDIR)

# End Source File
################################################################################
# Begin Source File

SOURCE=.\BINFOPRNT.C
DEP_BINFOP=\
	\dl\include\dlstdio.h\
	\dl\include\sys\STYPES.H\
	\dl\include\sys\FILEDESC.H\
	\dl\include\dlfdesc.h\
	\dl\include\QUAD.H\
	\dl\include\SIGBLK.H\
	\genv\include\TERMDEFS.H\
	\genv\include\GENVDEFS.H\
	\genv\include\GENVFMT.H\
	\genv\include\GENVFCI.H\
	\genv\include\DEIDEFS.H\
	\genv\include\TMODES.H\
	\genv\include\ARGPATHS.H\
	\genv\include\CTYPE.H\
	\genv\include\DIR.H\
	..\INCLUDE\BINFO.H\
	\genv\include\TRANDEFS.H\
	\genv\include\printdefs.h\
	\genv\include\QUEDEFS.H\
	\dl\include\STANDARD.H\
	\genv\include\SECGEN.H\
	\genv\include\LANGUAGE.H

$(INTDIR)/BINFOPRNT.OBJ :  $(SOURCE)  $(DEP_BINFOP) $(INTDIR)

# End Source File
# End Group
# End Project
################################################################################
