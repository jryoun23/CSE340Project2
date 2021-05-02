/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"
#include <vector>
#include <string>
using namespace std;

class Parser {
  public:
  	
  	struct regNode
  	{
  		regNode* first = NULL;
  		regNode* second = NULL;
  		char firstLabel = '!';
  		char secondLabel = '!';
  		
	};
  
  	
  	struct REG
  	{
  		regNode * start;
  		regNode * accept;
	};
		
  	struct regToken
  	{
  		string name = "";
  		REG * reg = NULL;
	};
	  
    void ConsumeAllInput();
    void parse_input();
    

  private:
    LexicalAnalyzer lexer;
    void parse_tokens_section();
    void parse_token_list();
    void parse_token();
    REG* parse_expr();
    void syntax_error();
    Token expect(TokenType expected_type);
    vector<regToken> regList;
    //here we are going to create two new functoins
    //one is going to be the main check if we have an epsilon, and the other is just going to return the nodes if there are any
    bool checkEpsilon(regNode* regNode);
    bool epsilonTransition(REG* reg);
    vector<regNode*> epsilonTransitions(regNode* node);
    //here is where we are going to start tokenizing the input stirngs
    vector<regNode*> charTransitions(char label, regNode * node); 
    vector<string> inputList;
   	void printRegNodeVector(vector<regNode*> node);
   	string printRegNode(regNode* regNode);
   	vector<regNode*> consumeSingleChar(char input, regNode* node);		//returns teh vector consuming only one char
   	vector<regNode*> consumeFullString(string input, REG* reg);	//returns the vector from going through the whole string
   	vector<regNode*> unique(vector<regNode*> node1,vector<regNode*> node2 );
   	vector<regNode*> append(vector<regNode*> base,vector<regNode*> added );
   	bool isStringInReg(string in, REG* reg);
   	bool isNodeInVector(regNode* node, vector<regNode*> vector);
   	string longestString = "";
   	int findLongestString(string input, REG* reg);
    string breakUpString(string Input);
};

#endif

