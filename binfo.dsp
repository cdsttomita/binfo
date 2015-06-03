# Microsoft Developer Studio Project File - Name="binfo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=binfo - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "binfo.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "binfo.mak" CFG="binfo - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "binfo - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE "binfo - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "binfo - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CONSOLE"
# PROP BASE Intermediate_Dir "CONSOLE"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\genv\tmpx"
# PROP Intermediate_Dir "\tempfile"
# PROP Ignore_Export_Lib 0
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FR /YX /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "KANA" /D "NEW" /D "OVS" /D "L0S" /D "REV5" /D "KANJI" /D "TCM" /D "MKR" /D "KNK" /D "REN" /D "RSH" /D "KSC" /D "TCK" /D "USEDLL" /Fr /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 mpr.lib liball_dp.lib file_lib.lib dlunos.lib simcon.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /debug /machine:I386 /nodefaultlib:"LIBC"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "binfo - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "binfo___Win32_Release"
# PROP BASE Intermediate_Dir "binfo___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\genv\tmpx"
# PROP Intermediate_Dir "\tempfile"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "KANA" /D "NEW" /D "OVS" /D "L0S" /D "REV5" /D "KANJI" /D "TCM" /D "MKR" /D "KNK" /D "REN" /D "RSH" /D "KSC" /D "TCK" /D "USEDLL" /Fr /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "KANA" /D "NEW" /D "OVS" /D "L0S" /D "REV5" /D "KANJI" /D "TCM" /D "MKR" /D "KNK" /D "REN" /D "RSH" /D "KSC" /D "TCK" /D "USEDLL" /Fr /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 liball_dp.lib file_lib.lib dlunos.lib simcon.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /incremental:yes /debug /machine:I386 /nodefaultlib:"LIBC"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 liball_dp.lib file_lib.lib dlunos.lib simcon.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /debug /machine:I386 /nodefaultlib:"LIBC"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "binfo - Win32 Debug"
# Name "binfo - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\BINFO.C
# End Source File
# Begin Source File

SOURCE=.\binfo.rc
# End Source File
# Begin Source File

SOURCE=.\BINFODISP.C
# End Source File
# Begin Source File

SOURCE=.\BINFOMKF.C
# End Source File
# Begin Source File

SOURCE=.\BINFOMSG.C
# End Source File
# Begin Source File

SOURCE=.\BINFOPRNT.C
# End Source File
# Begin Source File

SOURCE=.\BINFOSUB.C
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\binfo_proc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
