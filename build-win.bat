@echo off
echo Compiling C++ project...
echo.

g++ -g main.cpp InfixToPostfix.cpp PostfixToAST.cpp AST_NODE.cpp -o project.exe

if %errorlevel% equ 0 (
    echo.
    echo Compilation successful!
    echo Running the executable...
    echo --------------------------
    project.exe
) else (
    echo.
    echo Compilation FAILED. Check the errors above.
)

echo.
pause