echo "Compiling C++ project..."
echo

# Compile the project
g++ -g main.cpp InfixToPostfix.cpp PostfixToAST.cpp AST_NODE.cpp -o project

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo
    echo "Compilation successful!"
    echo "Running the executable..."
    echo "--------------------------"
    ./project
else
    echo
    echo "Compilation FAILED. Check the errors above."
fi

echo
read -p "Press Enter to continue..."