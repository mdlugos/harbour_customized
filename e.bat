@echo off
code.cmd %~dp0..\p\p.code-workspace %*
codium.cmd %~dp0..\p\p.code-workspace %*
"%ProgramFiles%\Microsoft VS Code\bin\code.cmd" %~dp0..\p\p.code-workspace %*
"%ProgramFiles%\VSCodium\bin\codium.cmd" %~dp0..\p\p.code-workspace %*
"%LOCALAPPDATA%\Programs\Microsoft VS Code\bin\codium.cmd" %~dp0..\p\p.code-workspace %*
%~dp0..\codium\bin\codium.cmd %~dp0..\p\p.code-workspace %*