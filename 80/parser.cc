/*
 * Copyright (C) Rida Bazzi, 2020
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */
 #include <string>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "parser.h"

using namespace std;

void Parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
}

// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevand
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// Parsing

// This function is simply to illustrate the GetToken() function
// you will not need it for your project and you can delete it
// the function also illustrates the use of peek()
void Parser::ConsumeAllInput()
{
    Token token;
    int i = 1;
    
    token = lexer.peek(i);
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        i = i+1;
        token = lexer.peek(i);
        token.Print();
    }
   
    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }

}

//This is where we are going to do the parsing

//This is the epsilonTransition checker to see if the token has an epsilon transition
vector<Parser::regNode*> Parser::append(vector<regNode*> base, vector<regNode*> added)
{
	vector<regNode*> v1;
	for(int i = 0; i<base.size();i++)
	{
		v1.push_back(base[i]);
	}
	for(int i = 0; i<added.size();i++)
	{
		v1.push_back(added[i]);
	}
		return v1;
}

int Parser::findLongestString(string input, REG* reg)
{
	int counter = 0;
	int longestString = 0;
	vector<regNode*> v1,v2,v3;
	v1.push_back(reg->start);
	for( char c : input)
	{
		counter++;
//		cout << "We are searching for all " << c << " transitions in this general one\n";
//		printRegNodeVector(v1);
		for(int i = 0; i<v1.size();i++)
		{
//			cout << "We are searching for all " << c << " transitions in this specific one\n";
//			cout << printRegNode(v1[i]) << endl;
			v2 = consumeSingleChar(c, v1[i]);
//			cout << "The transitions are \n";
//			printRegNodeVector(v2);
			v3 = unique(v3,v2);
//			cout << "The running tally for this general should be \n";
//			printRegNodeVector(v3);
		}
//		cout << "The new Reachable nodes are \n";
//		printRegNodeVector(v3);
		if(isNodeInVector(reg->accept, v3))
		{
			longestString = counter;
//			cout << "AYE OUR NEW LONGEST STRING IS " << longestString << endl;
			//break or something here and return it;
		}
		v1 = v3;
		v3.clear();
	
		
	}
	return longestString;
}

string Parser::breakUpString(string input)
{
		return "";
}

bool Parser::isNodeInVector(regNode* node,vector<regNode*> vector)
{
	for(int i = 0; i<vector.size(); i++)
	{
		if(node == vector[i])
		{
			return true;
		}
	}
	return false;
}

bool Parser::isStringInReg(string input, REG* reg)
{
	bool in = false;
	vector<regNode*> finalV;
	finalV = consumeFullString(input, reg);
//	printRegNodeVector(finalV);
	for(int i = 0; i<finalV.size();i++)
	{
		if(reg->accept == finalV[i])
		{
			in = true;
		}
	}
	if(!in)
	{
		
	}
	return in;
}
vector<Parser::regNode*> Parser::unique(vector<regNode*> tally, vector<regNode*> newNode) //adds the unique elements of node 2 to node1
{
	vector<regNode*> returnV;
	if(tally.empty() && newNode.empty())
	{
//		cout << "we have an empty tally AND empty new\n";
	}
	else if(tally.empty() && !newNode.empty())
	{
//		cout << "we have an empty tally \n";
		returnV = newNode;
	}
	else if(!(tally.empty()) && newNode.empty())
	{
//		cout << "we have an empty new\n";
		returnV = tally;
	}
	else
	{
		returnV = tally;
		bool in = false;
//		cout << "Both tally and new contain stuff \n" << endl;
		for(int i = 0; i<newNode.size(); i++)
		{
			if(isNodeInVector(newNode[i], tally))
			{
			}
			else
			{
				returnV.push_back(newNode[i]);
			}
		}	
	}
	return returnV;
}

void Parser::printRegNodeVector(vector<regNode*> node)
{
	
	if(node.size() == 0)
	{
		cout << "Theres nothing in this vector" << endl;
	}
	else
	{
		for(int i = 0; i < node.size(); i++)
		{
			cout << "First: " << node[i]->firstLabel << "\t Second: " << node[i]->secondLabel << "\t Address: " << node[i] << endl;
		}
		cout << endl;
	}
}

string Parser::printRegNode(regNode* reg)
{
	string out = "";
	if(reg == NULL)
	{
		out +=  "This is null\n" ;
	}
	else
	{
		out += "First: ";
		out += reg->firstLabel;
		out +=  "\t Second: ";
		out +=  reg->secondLabel;
		out += "\n";
	}
	return out;
}

//this is the recursive function called upon the node to determine if it is a 
bool Parser::checkEpsilon(regNode* regNode) //true is we do have an epsilon token and false is we do not
{
	bool result = 0,result1 = 0, result2 = 0;
	if(regNode->first == NULL && regNode->second == NULL)
	{
		//here we have the base case where we reach the end and we have an epsilon transition... oh no!
//		cout << "case1\n";
		result = true;
	}
	else if(regNode->firstLabel != '_' && regNode->secondLabel != '_')
	{
//		cout << "case2\n";
		result = false;
	}
	else
	{
		
		if(regNode->secondLabel == '_' )
		{
			result2 = checkEpsilon(regNode->second);
		}
		if(regNode->firstLabel == '_' )
		{
			result1 = checkEpsilon(regNode->first);
		}
		result = (result1 || result2);
	}
	return result;
}

//This is going to be the workhorse for the epsilontransition identifier; EDIT: SIKE LOL THIS WAS USELESS

bool Parser::epsilonTransition(REG* reg)
{
	bool returnValue = 0;
	auto initV = epsilonTransitions(reg->start);
	for (int i = 0; i < initV.size(); i++)
	{
		if(initV[i] == reg->accept)
		{
			returnValue = 1;
		}
	}
	return returnValue;
}

vector<Parser::regNode*> Parser::epsilonTransitions(regNode* node) //re-writing this so it will be itereratively
{
	bool changed = true;
	vector<regNode*> v1, mostRecent,newMostRecent;
	int counter = 0;
	v1.push_back(node);							//this one is going to be the collection
	mostRecent.push_back(node);					//this one is the most recent set
	while(changed)
	{
//		printRegNodeVector(mostRecent);
		changed = false;
		for(int i =0; i< mostRecent.size();i++)
		{
//			cout << printRegNode(mostRecent[i]);
			if(mostRecent[i]->firstLabel == '_' && mostRecent[i]->secondLabel == '_')
			{
//				cout << "we're in the doublg underscore \n";
				v1.push_back(mostRecent[i]->first);	
				v1.push_back(mostRecent[i]->second);
				newMostRecent.push_back(mostRecent[i]->first);
				newMostRecent.push_back(mostRecent[i]->second);
				changed = true;
			}
			else if(mostRecent[i]->firstLabel != '_' && mostRecent[i]->secondLabel != '_')
			{
				//changed is what it was!
//				cout << "we're in no change \n";
			}
			else if(mostRecent[i]->firstLabel == '_' && mostRecent[i]->secondLabel != '_')
			{
				v1.push_back(mostRecent[i]->first);
				newMostRecent.push_back(mostRecent[i]->first);	
				changed = true;
//				cout << "we're in first undersccore change \n";
			}
			else if(mostRecent[i]->firstLabel != '_' && mostRecent[i]->secondLabel == '_')
			{
				v1.push_back(mostRecent[i]->second);
				newMostRecent.push_back(mostRecent[i]->second);
				changed = true;
//				cout << "we're in second undersccore change \n";
			}
		}
//		cout << endl;
		mostRecent = newMostRecent;
		newMostRecent.clear();	
	}
//	printRegNodeVector(v1);	
	return v1;
}

 vector<Parser::regNode*> Parser::charTransitions(char label, regNode* node)  //this is going to return a node pointer vector to all of the reachable nodes in node by char label;
 {
 	vector<regNode*> v1;
 	if(node->firstLabel == label && node->secondLabel == label)
 	{
 		v1.push_back(node->first);
 		v1.push_back(node->second);
	}
	else if(node->firstLabel == label && node->secondLabel != label)
	{
		v1.push_back(node->first);
	}
	else if(node->firstLabel != label && node->secondLabel == label)
	{
		v1.push_back(node->second);
	}
	else
	{
		//theres NO transitions from this node;
	}

	return v1;
 }

vector<Parser::regNode*> Parser::consumeSingleChar(char in, regNode* node) //IT WORKS LETS GO
{
	vector<regNode*> v1,v2,ep,c;
	ep = epsilonTransitions(node);
	for(int i = 0; i<ep.size();i++)
	{
		if(ep[i]->firstLabel == in && ep[i]->secondLabel == in)
		{
			c.push_back(ep[i]->first);
			c.push_back(ep[i]->second);
		}
		else if(ep[i]->firstLabel != in && ep[i]->secondLabel != in)
		{
			//add nothing to the c
		}
		else if(ep[i]->firstLabel == in && ep[i]->secondLabel != in)
		{
			c.push_back(ep[i]->first);
		}
		else if(ep[i]->firstLabel != in && ep[i]->secondLabel == in)
		{
			c.push_back(ep[i]->second);
		}
	}
	for(int i = 0; i<c.size();i++)
	{
		ep = epsilonTransitions(c[i]);
//		cout << "epsilon transitions for " + printRegNode(c[i]) << endl;
//		printRegNodeVector(ep);
		for(int j = 0;j<ep.size();j++)
		{
			v1.push_back(ep[j]); // if we put it in c in here, w eget an infinite loop becuase c keeeeeeeeeps getting bigger;
		}
		
	}
	for(int i  = 0; i<v1.size(); i++)
	{
		bool in = false;
		for(int j = 0; j<c.size(); j++)
		{
			if(v1[i] == c[j])
			{
				in = true;
			}
			else
			{
			}
		}
		if(in)
		{
			//dont add
		}
		else
		{
//			cout << "adding " + printRegNode(v1[i]) + "to v2\n";
			v2.push_back(v1[i]);
		}
		
	}
//	printRegNodeVector(v2);
	for(int i = 0; i<v2.size(); i++)
	{
		c.push_back(v2[i]);
	}
	return c;
}

vector<Parser::regNode*> Parser::consumeFullString(string input, REG* reg) //RETURNS ALL OF THE FINAL STATES OF AN INPUT BASED ON A REG;
{
	vector<regNode*> v1,v2,v3;
	v1.push_back(reg->start);
	for( char c : input)
	{
		
//		cout << "We are searching for all " << c << " transitions in this general one\n";
//		printRegNodeVector(v1);
		for(int i = 0; i<v1.size();i++)
		{
//			cout << "We are searching for all " << c << " transitions in this specific one\n";
//			cout << printRegNode(v1[i]) << endl;
			v2 = consumeSingleChar(c, v1[i]);
//			cout << "The transitions are \n";
//			printRegNodeVector(v2);
			v3 = unique(v3,v2);
//			cout << "The running tally for this general should be \n";
//			printRegNodeVector(v3);
		}
		v1 = v3;
		v3.clear();
	
		
	}
	return v1;
}
//input -> tokens_section INPUT_TEXT
void Parser::parse_input()
{

	parse_tokens_section();
	Token t = lexer.peek(1);
	if(t.token_type != INPUT_TEXT)
	{
		syntax_error();
	}
	int errorCounter = 0;
	string out = "EPSILON IS NOOOOOT A TOKEN !!! "; //20 21 22 23 24 
	for(int i=0; i < regList.size(); i++)
	{
//  	cout << regList[i].name << "\t" << regList[i].reg << "\t" << regList[i].reg->start->first->firstLabel << endl;
//		cout << "Name: " << regList[i].name << "\t acceptAddress: " << regList[i].reg->accept << endl;
		bool epCheck = epsilonTransition(regList[i].reg);
		if (epCheck)
		{
			errorCounter++;
			out += regList[i].name + " ";
		}
   	}
   	if(errorCounter!= 0)
   	{
   		cout << out;
   		exit(1);
	}
	t = expect(INPUT_TEXT);
	string input = t.lexeme;
	string t1 = "";
	for(auto c : input)							//this creates the input list 
	{
		if(c == '"')
		{
			if(t1.empty())		//this is the first quotation mark
			{
				//go next
			}
			else				//this is the mlast quotation
			{		
				inputList.push_back(t1);
				t1 = "";
			}
		}
		else if(c == ' ')
		{
			if(t1.empty())		//this is the first space mark
			{
				//go next
			}
			else				//this is the mlast quotation
			{		
				inputList.push_back(t1);
				t1 = "";
			}
		}
		else					//this is where we add the chars to t1
		{
			t1+= c;
		}
	}	
	//now we have out INPUT LIST && the regList saved! YAY
	vector<regNode*> test;
	bool testBool;
	bool error;
	int longestString = 0;
	
	for(int i = 0; i<inputList.size(); i++)
	{
		int regNum = 0;
		int longestString = 0;
		int newLongestString = 0;
		int totalLength = 0;
		string in = inputList[i]; 
		int inputLength = inputList[i].length();
//		cout << inputLength << endl;
		while(totalLength != inputLength)
		{
			for(int j = 0; j<regList.size(); j++)
			{
				newLongestString = findLongestString(in, regList[j].reg);
				if(newLongestString > longestString)
				{
//					cout << longestString << " " << newLongestString;
					longestString = newLongestString;
					regNum = j;
				}
			}
//			cout << longestString;
			if(longestString == 0)
			{
				cout << "ERROR";
				exit(1);
			}
			else if(longestString != in.length())
			{
				 cout << regList[regNum].name + " , " + "\"" + in.substr(0, longestString) + "\"" << endl;	
				 in = in.substr(longestString, in.length()-longestString);
				 totalLength += longestString;
				 longestString = 0;
			}
			else if(longestString == in.length())
			{
				cout << regList[regNum].name + " , " + "\"" + in.substr(0, longestString) + "\"" << endl;
				totalLength += longestString;
				longestString = 0;	
			}
			
		}
//		cout << endl;
		
		
		
	}
		
	
// cout << regList[j].name + " , " + "\"" + testString + "\"" << endl;
}


//tokens section -> token_list HASH
void Parser::parse_tokens_section()
{
	parse_token_list();
	expect(HASH);
	Token t = lexer.peek(1);
	if(t.token_type!= INPUT_TEXT)
	{
		syntax_error();
	}
}


//token list -> token
//token list -> token COMMA token_list
void Parser::parse_token_list()
{
	parse_token();
	Token t = lexer.peek(1);
	if(t.token_type == COMMA)
	{
		expect(COMMA);
		parse_token_list();
	}
	else if(t.token_type != HASH)
	{
		syntax_error();
	}
}

void Parser::parse_token()
{
	//This si where we are going to load and store our tokens
	Token t = expect(ID);
	regToken r1;
	REG *newReg = new REG;
	newReg = parse_expr();
	r1.name = t.lexeme;
	r1.reg = newReg;
	regList.push_back(r1);
	
	//Here, we are going to see if we can find epsilon anywhere
}

Parser::REG* Parser::parse_expr()
{
	REG* returnReg;
	//This is where the magic happens
	Token t = lexer.peek(1);
	if(t.token_type == CHAR)				//case char
	{
		Token t = expect(CHAR); 
//		cout << t.lexeme;						//This is where we create the base REG
		char name = t.lexeme[0];				//name is the name of the char in the thing
		struct REG *aReg = new REG;
		struct regNode *ptrS = new regNode;				//create the two nodes s(second) and f(first)
		struct regNode *ptrF = new regNode;
							
		
		ptrF->firstLabel = name;				//assign the name of the first node to the first name 
		ptrF->first = ptrS;						//assign where the pointer points to the next node
		
		aReg->start = ptrF;						//assign the start node to the new one
		aReg->accept = ptrS;
		returnReg = aReg;
	}
	else if (t.token_type == LPAREN)		//case LPAREN
	{
		expect(LPAREN);
		struct REG *R1 = new REG;
		R1 = parse_expr();
		expect(RPAREN);
		t = lexer.peek(1);
		if(t.token_type == DOT)
		{
			expect(DOT);
			expect(LPAREN);
			struct REG *R2 = new REG;		//grab the second reg
			R2 = parse_expr();				
			REG *newReg = new REG;			//create a new reg
			R1->accept->first = R2->start;	//link teh 1st and second reg
			R1->accept->firstLabel= '_';
			
			newReg->start = R1->start;		//add the new start and accept states to the reg.
			newReg->accept= R2->accept;
			
//			cout << newReg->start->firstLabel << endl << newReg->start->first->firstLabel << newReg->start->first->first -> firstLabel;

			expect(RPAREN);
			returnReg = newReg;					//return the reg
		}
		else if (t.token_type == OR)
		{
			 // creating the new reg
			expect(OR);
			expect(LPAREN);
			struct REG *R2 = new REG;		//grab the second reg
			R2 = parse_expr();
			//so here we have R1 and R2.
			//we are going to create a new REG that has a new start and accept
			//we are going to make the start first and second have Underscore transitions to R1 and R2's start nodes respectively
			// we are also going to have underscore transitions from the accept states of R1 and R2, to the new accept
			//then we are going to return the newReg.
			
//			//here we are adding the new transitions to the start node of the newReg above.
			struct REG *newReg = new REG;
			
			//as it turns out, i need to amke a new start and end node as well as a new reg... I should've known that.
			struct regNode *newStart = new regNode;				//create the two nodes s(second) and f(first)
			struct regNode *newAccept = new regNode;
			newReg->start = newStart;
			newReg->accept = newAccept;
			
			newReg->start->first = R1->start;
			newReg->start->firstLabel = '_';
			newReg->start->second = R2->start;
			newReg->start->secondLabel = '_';
			
			//here we are adding the transitions from the old accept states to the new one
			R2->accept->first = newReg->accept;
			R2->accept->firstLabel = '_';
			R1->accept->first = newReg->accept;
			R1->accept->firstLabel = '_';
			
			expect(RPAREN);
			
//			cout << newReg->start->firstLabel << newReg->start->first->firstLabel << newReg->start->first->first->firstLabel << endl;
//			cout << newReg->start->secondLabel << newReg->start->second->firstLabel << newReg->start->second->first->firstLabel << endl;
			returnReg = newReg;
		}
		else if (t.token_type == STAR)
		{
			//ok so here, we have r1 and thats all we need;
			//essentially, we need to make a new reg with a new start and accept.
			//then we need to make the new start state point to the new accept state and the R1 start via epsilon transitions
			//then we need to make the old R1 accept point to the old R1 start;
			//then, as the last step, we need to make a new epsilon transition from the old accept, to the new accept.
			//this just creates a new reg with a new start and accept
			struct REG *newReg = new REG;
			struct regNode *newStart = new regNode;				
			struct regNode *newAccept = new regNode;
			newReg->start = newStart;
			newReg->accept = newAccept;
			//now we need to create epsilon transitions from the start state to the accept state and first state of R1;
			newReg->start->first = R1->start;		//start ofR1
			newReg->start->firstLabel= '_';
			newReg->start->second = newReg->accept;	//new accept;
			newReg->start->secondLabel = '_';
			
			//now we are going to make a new epsilon transition from the old R1 accept to the old R1 start;
			R1->accept->second = R1->start;
			R1->accept->secondLabel = '_';
			//now we are going to make a transition from the old R1 accept to the new reg accept
			R1->accept->first = newReg->accept;
			R1->accept->firstLabel = '_';
			
			expect(STAR);
//			cout << newReg->start->firstLabel <<newReg->start->first->firstLabel<<newReg->start->first->first->firstLabel<<newReg->start->first->first->first->firstLabel<<newReg->start->first->first->first->first->firstLabel;
			
			//FIRST IS ACCEPT* SECOND IS START*

			returnReg = newReg;
		}
		else 
		{
			syntax_error();
		}
		
	}
	else if (t.token_type == UNDERSCORE)	//case UNDERSCORE
	{
		Token t = expect(UNDERSCORE); 
//		cout << t.lexeme;						//This is where we create the base REG
		char name = '_';				//name is the name of the char in the thing
		struct REG *aReg = new REG;
		struct regNode *ptrS = new regNode;				//create the two nodes s(second) and f(first)
		struct regNode *ptrF = new regNode;
							
		
		ptrF->firstLabel = name;				//assign the name of the first node to the first name 
		ptrF->first = ptrS;						//assign where the pointer points to the next node
		
		aReg->start = ptrF;						//assign the start node to the new one
		aReg->accept = ptrS;
		returnReg = aReg;
	}
	return returnReg;
}


int main()
{
	// note: the parser class has a lexer object instantiated in it. You should not be declaring
    // a separate lexer object. You can access the lexer object in the parser functions as shown in the
    // example method Parser::ConsumeAllInput
    // If you declare another lexer object, lexical analysis will not work correctly
    Parser parser;
	parser.parse_input();
//	parser.ConsumeAllInput();
	
}
