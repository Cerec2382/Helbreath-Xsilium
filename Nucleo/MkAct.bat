@Echo Off
Echo ---------------------------------------
Echo      Firestorms quick acct tool
Echo   For generating HB accts on the fly
Echo ---------------------------------------
Echo.
Echo Syntax: MkAct (username) (password) (server dir)
Echo Example: MkAct test 123 C:\HBServer\ (last \ required.)
Echo If the above syntax is NOT what you used, quit now.
Pause

Echo.
Echo ---------------------------------------
Echo             Making Account
Echo ---------------------------------------
Echo.
Echo Creating new Account file in the directory:
Echo %3
Echo If this is not your HB server directory OR
Echo your directory does not end in \ please quit now.
Pause

if exist %3Account\%1.txt goto error
else goto write

:write
Echo [NAME] >> %3Account\%1.txt
Echo Account-name     = %1 >> %3Account\%1.txt
Echo [PASSWORD]
Echo Account-password = %2  >> %3Account\%1.txt
Echo [Account-STATUS] >> %3Account\%1.txt
Echo Account-valid-time = 0 >> %3Account\%1.txt
Echo Account-valid-date = 2003 4 5 >> %3Account\%1.txt
Echo Account-change-password = 2003 3 29 >> %3Account\%1.txt
Echo [ETC] >> %3Account\%1.txt
Echo Account-Gender   = Male >> %3Account\%1.txt
Echo Account-Age      = xx >> %3Account\%1.txt
Echo Account-Birth-Year = xxxx >> %3Account\%1.txt
Echo Account-Birth-Month = xx >> %3Account\%1.txt
Echo Account-Birth-Day   = xx >> %3Account\%1.txt
Echo Account-RealName = NA >> %3Account\%1.txt
Echo Account-SSN      = ECTS >> %3Account\%1.txt
Echo Account-Email    = aaa >> %3Account\%1.txt
Echo Account-Quiz     = NONE >> %3Account\%1.txt
Echo Account-Answer   = NONE >> %3Account\%1.txt
Echo Account-created-by = >> %3Account\%1.txt
Echo [CHARACTERS] >> %3Account\%1.txt

Echo.
Echo ---------------------------------------
Echo             Account Made
Echo ---------------------------------------
goto EOF

:Error
Echo.
Echo ---------------------------------------
Echo                  ERROR                   
Echo ---------------------------------------
Echo.
Echo There is already an Account named: %1
Echo If you do NOT wish to overwrite this Account
Echo Press ctrl+c right now, however if you
Echo intend to overwrite the Account, press enter.
Pause
del %3Account\%1.txt
goto write

:EOF