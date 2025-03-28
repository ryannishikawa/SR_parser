//****************************************************************************************************************************
//Program name: "syntax_analyzer".  This program is a small, bottom-up, shift-reduce parser. Copyright (C)           *                                     *
//2025                 *
//This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License  *
//version 3 as published by the Free Software Foundation.                 *
//This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied         *
//warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.     *
//A copy of the GNU General Public License v3 is available here:  <https://www.gnu.org/licenses/>.                           *
//****************************************************************************************************************************


//=======1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1=========2=========3**
//
//Author information
//  Author names: Ryan Nishikawa, Celine Truong, Natalie Garcia
//  Author emails: ryannishikawa48@csu.fullerton.edu, 
//  Course ID: CPSC335
//
//Program information
//  Program name: target_str
//  Due date+time: March 30, 2025 11:59 PM
//  Date of last update: March 25, 2025
//  Programming language(s): C++
//  Files in this program: r.sh, syntax_analyzer.cpp
//  
//  OS of the computer where the program was developed: Ubuntu 22.04.3 LTS
//  OS of the computer where the program was tested: Ubuntu 22.04.3 LTS
//  Status: WIP
//
//References for this program
//   lectures
//
//Purpose
//  to read in the token stream as it is produced by the lexical analyzer, parse the token 
//  stream by following the production rules of the grammar defining the language, and output a 
//  syntax tree
//
//This file
//   File name: syntax_analyzer.cpp
//   Language: C++
//   Compile: g++ -o syntax_analyzer syntax_analyzer.cpp
//   Run(with 10 test cases from input.txt): ./r.sh
//      if running for the first time, do "chmod +x r.sh" to give the script permission to execute
//
//References for this file
//   https://www.geeksforgeeks.org/setw-function-in-cpp-with-examples/
//
//=======1=========2=========3=========4=========5=========6=========7=========8=========9=========0=========1=========2**
//
//
//
//
//===== Begin code area ================================================================================================
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cctype>
#include <stack>

using namespace std;

//parse table
const pair<string, int> SR_table[17][11] = {
 {{"id", -1}, {"+", -1}, {"-", -1}, {"*", -1}, {"/", -1}, {"(", -1}, {")", -1}, {"$", -1}, {"E", -1}, {"T", -1}, {"F", -1}}, //symbols 
 {{"S", 5},   {"E", 0},  {"E", 0},  {"E", 0},  {"E", 0},  {"S", 4},  {"E", 0},  {"E", 0},  {"G", 1},  {"G", 2},  {"G", 3}},  //state 0 
 {{"E", 0},   {"S", 6},  {"S", 7},  {"E", 0},  {"E", 0},  {"S", 4},  {"E", 0},  {"A", -1}, {"E", 0},  {"E", 0},  {"E", 0}},  //state 1 
 {{"E", 0},   {"R", 3},  {"R", 3},  {"S", 8},  {"S", 9},  {"E", 0},  {"R", 3},  {"R", 3},  {"E", 0},  {"E", 0},  {"E", 0}},  //state 2 
 {{"E", 0},   {"R", 6},  {"R", 6},  {"R", 6},  {"R", 6},  {"E", 0},  {"R", 6},  {"R", 6},  {"E", 0},  {"E", 0},  {"E", 0}},  //state 3 
 {{"S", 5},   {"E", 0},  {"E", 0},  {"E", 0},  {"E", 0},  {"S", 4},  {"E", 0},  {"E", 0},  {"G", 10}, {"G", 2},  {"G", 3}},  //state 4  
 {{"E", 0},   {"R", 8},  {"R", 8},  {"R", 8},  {"R", 8},  {"E", 0},  {"R", 8},  {"R", 8},  {"E", 0},  {"E", 0},  {"E", 0}},  //state 5 
 {{"S", 5},   {"E", 0},  {"E", 0},  {"E", 0},  {"E", 0},  {"S", 4},  {"E", 0},  {"E", 0},  {"E", 0},  {"G", 11}, {"G", 3}},  //state 6 
 {{"S", 5},   {"E", 0},  {"E", 0},  {"E", 0},  {"E", 0},  {"S", 4},  {"E", 0},  {"E", 0},  {"E", 0},  {"G", 12}, {"G", 3}},  //state 7 
 {{"S", 5},   {"E", 0},  {"E", 0},  {"E", 0},  {"E", 0},  {"S", 4},  {"E", 0},  {"E", 0},  {"E", 0},  {"E", 0},  {"G", 13}}, //state 8 
 {{"S", 5},   {"E", 0},  {"E", 0},  {"E", 0},  {"E", 0},  {"S", 4},  {"E", 0},  {"E", 0},  {"E", 0},  {"E", 0},  {"G", 14}}, //state 9 
 {{"E", 0},   {"S", 6},  {"S", 7},  {"E", 0},  {"E", 0},  {"E", 0},  {"S", 15}, {"E", 0},  {"E", 0},  {"E", 0},  {"E", 0}},  //state 10 
 {{"E", 0},   {"R", 1},  {"R", 1},  {"S", 8},  {"S", 9},  {"E", 0},  {"R", 1},  {"R", 1},  {"E", 0},  {"E", 0},  {"E", 0}},  //state 11
 {{"E", 0},   {"R", 2},  {"R", 2},  {"S", 8},  {"S", 9},  {"E", 0},  {"R", 2},  {"R", 2},  {"E", 0},  {"E", 0},  {"E", 0}},  //state 12
 {{"E", 0},   {"R", 4},  {"R", 4},  {"R", 4},  {"R", 4},  {"E", 0},  {"R", 4},  {"R", 4},  {"E", 0},  {"E", 0},  {"E", 0}},  //state 13
 {{"E", 0},   {"R", 5},  {"R", 5},  {"R", 5},  {"R", 5},  {"E", 0},  {"R", 5},  {"R", 5},  {"E", 0},  {"E", 0},  {"E", 0}},  //state 14
 {{"E", 0},   {"R", 7},  {"R", 7},  {"R", 7},  {"R", 7},  {"E", 0},  {"R", 7},  {"R", 7},  {"E", 0},  {"E", 0},  {"E", 0}}}; //state 15

//list of grammar rules
const pair<string, string> rules[8] = {
    {"E", "E + T"},  //1
    {"E", "E - T"},  //2
    {"E", "T"},      //3
    {"T", "T * F"},  //4
    {"T", "T / F"},  //5
    {"T", "F"},      //6
    {"F", "( E )"},  //7
    {"F", "id"}      //8
};

// list of symbols including multi-character operators
const vector<string> symbols = {
    "(", ")", "[", "]", "{", "}", ".", "+", "-", "*", "/", "%", "<", ">", "=", ";", ",",
    "++", "--", "<=", ">=", "==", "&&", "||", "!", "&", "|"
};

//list of error statements
const vector<string> errors = {
    "Invalid input",
    "Invalid input"
};

// function to check if a character is a symbol
bool isSymbol(char c) {
    string symbolChars = "()+-*/";
    return symbolChars.find(c) != string::npos;
}

//func to print out a stack
template <typename T>
void printStack(stack<T> s) {
    while (!s.empty()) {
        cout << s.top() << " ";
        s.pop();
    }
};

//func to parse token stream
stack<string> parse(const string& tokens) {
    stack<string> s;
    string now = "";
    s.push("$");
    for (int i = tokens.size(); i > 0; i--) {
        now += tokens[i];
        if (isSymbol(tokens[i])) { s.push(now); now.clear(); }
        else if (now == "id") { s.push(now); now.clear(); }
    }

    return s;
}

int main() {
    string tokens;
    cout << "\nInput token stream: ";
    cin >> tokens;
    
    cout << "\n\n" << "|" << setw(20) << left << "Stack" << "|" << setw(20) << left << "Input" << "|" << setw(20) << left << "Action\n";

    

    return 0;
}