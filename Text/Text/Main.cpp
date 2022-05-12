#define _CRT_SECURE_NO_WARNINGS
#define HOME  71
#define DOWN  80
#define NEXT  77
#define UP	  72
#define ESC	  27
#define INS	  82
#define DEL	  83
#define ENTER 13

#include <iostream>
#include <conio.h>
#include "TText.h"
#include "Control.h"


TMem TNode::mem;

int main()
{
	TNode::InitMem(100);

	TText tx;

	tx.Load("..\\input.txt");
	tx.GoFirstLine();

	char control;

	do {
		cout << ">, v, ^, Home, Ins, Del, Enter, Esc\n";

		tx.Print();
		cout << '\n';

		control = _getch();
		if (control == 0xE0) control = _getch();
		if (control == ESC)
		{
			cout << '\n';

			tx.Save("..\\output.txt");
			cout << "\nResult saved to res.txt.\n\n";
			break;
		}

		if (control != ENTER) control = _getch();

		switch (control)
		{
		case ENTER: break;
		case HOME: tx.GoFirstLine(); break;
		case DOWN: tx.GoDownLine(); break;
		case NEXT: tx.GoNextLine(); break;
		case UP: tx.GoBackLine();break;
		case INS:
		{
			char str[81];
			cout << "1 - Next line; 2 - Next section; 3 - Down line; 4 - Down section; 5 - Cancel" << endl;
			char k = _getch();
			if (k == '5') break;
			cout << "Enter line: ";
			cin >> str;
			switch (k)
			{
			case '1': tx.InsNextLine(str); break;
			case '2': tx.InsNextSection(str); break;
			case '3': tx.InsDownLine(str); break;
			case '4': tx.InsDownSection(str); break;
			}
			break;
		}
		case DEL:
			cout << "1 - Next line; 2 - Down line; 3 - Cancel" << endl;
			char a = _getch();
			if (a == '3') break;
			switch (a)
			{
			case '1': tx.DelNext(); break;
			case '2': tx.DelDown(); break;
			case '3': break;
			}
			break;
		}
		
		clrscr();
	} while (control != ESC);

	cout << "-----------------------------------------\n";
	cout << "Free memory :\n";
	TNode::PrintFreeNodes();
	cout << '\n';
	cout << "-----------------------------------------\n";
	tx.CleanMem();

	cout << "Free memory after garbage collection :\n";
	TNode::PrintFreeNodes();
	cout << "-----------------------------------------\n";
	cout << "-----------------------------------------\n";
	cout << "\nCopy test:\n";
	
	TText* ctx = tx.GetCopy();
	cout << "Original text: \n";
	tx.Print();
	cout << "\nCopyed text: \n";
	ctx->Print();
}