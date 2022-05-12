#pragma once
#include "TStack.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
using namespace std;


struct TNode;
class TText;

struct TMem
{
	TNode* pFirst;
	TNode* pFree;
	TNode* pLast;
};

struct TNode
{
	char str[81];
	TNode* pNext;
	TNode* pDown;
	bool garbage;

	static TMem mem;

	TNode(char* _str, TNode* _pNext, TNode* _pDown)
	{
		if (_str == nullptr)
		{
			str[0] = '\0';
		}
		else strcpy(str, _str);
		pNext = _pNext;
		pDown = _pDown;
	}
	void* operator new(size_t size)
	{
		TNode* tmp = mem.pFree;
		mem.pFree = mem.pFree->pNext;
		return tmp;
		
	}
	void operator delete(void* ptr)
	{
		TNode* tmp = mem.pFree;
		TNode* p1 = (TNode*)ptr;

		p1->pNext = tmp;
		mem.pFree = p1;
	}
	static void InitMem(size_t size)
	{
		mem.pFirst = (TNode*) new char[size * sizeof(TNode)];

		mem.pFree = mem.pFirst;
		mem.pLast = mem.pFirst + (size - 1);
		TNode* p = mem.pFirst;

		for (int i = 0; i < size - 1; i++)
		{
			p->pNext = p + 1;
			p->str[0] = 0;
			p->garbage = true;
			p += 1;
		}
		mem.pLast->pNext = nullptr;
		mem.pLast->str[0] = 0;
	}
	static void PrintFreeNodes()
	{
		TNode* p = mem.pFree;
		while (p != nullptr)
		{
			if (p->str[0] != '\0')
			{
				cout << p->str << '\n';
			}
		p = p->pNext;
		}
	}

};

class TText
{
private:
	TNode* pFirst, * pCurr;
	
	TStack <TNode*> st;

	
	int textLevel = 0;
	TNode* ReadRec(ifstream& ifs)
	{
		TNode* pTemp = nullptr, * pHead = nullptr;
		char str[81];

		while (!ifs.eof())
		{
			ifs.getline(str, 81, '\n');
			if (str[0] == '{') pTemp->pDown = ReadRec(ifs);
			else if (str[0] == '}') break;
			else if (strcmp(str, "") == 0) continue;
			else
			{
				TNode* newNode = new TNode(str, nullptr, nullptr);
				if (pHead == nullptr)
					pTemp = pHead = newNode;
				else
					pTemp->pNext = newNode;
				pTemp = newNode;
			}
		}
		return pHead;
	}
	void PrintRec(TNode* p)
	{
		if (p != nullptr)
		{
			for (int i = 0; i < textLevel; i++)
				cout << "   ";
			if (p == pCurr) cout << "|->|";
			else cout << " ";

			cout << p->str << '\n';
			textLevel++;
			PrintRec(p->pDown);
			textLevel--;
			PrintRec(p->pNext);
		}
	}
	void WriteRec(TNode* p, ostream& os)
	{
		if (p != nullptr)
		{
			os << p->str << '\n';
			if (p->pDown != nullptr)
			{
				os << "{\n";
				WriteRec(p->pDown, os);

				os << "}\n";
			}
			WriteRec(p->pNext, os);
		}
	}
	TNode* CopyNode(TNode* p)
	{
		TNode* pCopy = new TNode(p->str, nullptr, nullptr);
		if (p->pDown != nullptr)
		{
			pCopy->pDown = CopyNode(p->pDown);
		}
		if (p->pNext != nullptr)
		{
			pCopy->pNext = CopyNode(p->pNext);
		}
		return pCopy;
	}

public:

	TText(){}
	TText(TNode* p)
	{
		pFirst = p;
		pCurr = nullptr;
	}
	TText* GetCopy()
	{
		TText* res;
		res = new TText(CopyNode(pFirst));
		return res;
	}


	void InsNextLine(char* _str)
	{
		if (pCurr != nullptr)
		{
			TNode* newNode = new TNode(_str, nullptr, nullptr);
			newNode->pNext = pCurr->pNext;
			pCurr->pNext = newNode;
		}
	}
	void InsNextSection(char* _str)
	{
		if (pCurr != nullptr)
		{
			TNode* newNode = new TNode(_str, nullptr, nullptr);
			newNode->pDown = pCurr->pNext;
			pCurr->pNext = newNode;
		}
	}
	void InsDownLine(char* _str)
	{
		if (pCurr != nullptr)
		{
			TNode* newNode = new TNode(_str, nullptr, nullptr);
			newNode->pNext = pCurr->pDown;
			pCurr->pDown = newNode;
		}
	}
	void InsDownSection(char* _str)
	{
		if (pCurr != nullptr)
		{
			TNode* newNode = new TNode(_str, nullptr, nullptr);
			newNode->pDown = pCurr->pDown;
			pCurr->pDown = newNode;
		}
	}

	void GoNextLine()
	{
		if (pCurr != nullptr && pCurr->pDown != nullptr)
		{
			st.Push(pCurr);
			pCurr = pCurr->pDown;
		}
	}
	void GoDownLine()
	{
		if (pCurr != nullptr && pCurr->pNext != nullptr)
		{
			st.Push(pCurr);
			pCurr = pCurr->pNext;
		}
	
	}

	void GoBackLine()
	{
		if (!st.IsEmpty())
		{
			TNode* prevNode = st.Pop();
			pCurr = prevNode;
		}
	}
	void GoFirstLine()
	{
		st.Clear();
		pCurr = pFirst;
	}

	void CleanMem()
	{
		for (Reset(); !IsEnd(); GoNext())
		{
			pCurr->garbage = false;
		}
		TNode* p = TNode::mem.pFree;
		while (p != nullptr)
		{
			p->garbage = false;
			p = p->pNext;
		}
		for (p = TNode::mem.pFirst; p <= TNode::mem.pLast; p++)
		{
			if (p->garbage) delete p;
			p->garbage = true;
		}
	}
	void DelNext()
	{
		if (pCurr != nullptr)
		{
			TNode* pDel = pCurr->pNext;
			if (pDel != nullptr)
			{
				pCurr->pNext = pDel->pNext;
				delete pDel;
			}
		}
	}
	void DelDown()
	{
		if (pCurr != nullptr)
		{
			TNode* pDel = pCurr->pDown;
			if (pDel != nullptr)
			{
				pCurr->pDown = pDel->pNext;
				delete pDel;
			}
		}

	}

	void Reset()
	{
		st.Clear();
		if (pFirst != nullptr)
		{
			pCurr = pFirst;

			st.Push(pCurr);
			if (pCurr->pNext != nullptr) st.Push(pCurr->pNext);
			if (pCurr->pDown != nullptr) st.Push(pCurr->pDown);
		}
	}
	void GoNext()
	{
		pCurr = st.Pop();

		if (pCurr != pFirst)
		{
			if (pCurr->pNext != nullptr) st.Push(pCurr->pNext);
			if (pCurr->pDown != nullptr) st.Push(pCurr->pDown);
		}
	}
	bool IsEnd()
	{
		return st.IsEmpty();
	}
	char* GetCurrentLine()
	{
		return pCurr->str;
	}

	void Load(string fn)
	{
		ifstream ifs(fn);
		pFirst = ReadRec(ifs);
	}
	void Print()
	{
		PrintRec(pFirst);
	}
	void Save(string fn)
	{
		std::ofstream out;
		out.open(fn);
		WriteRec(pFirst, out);
	}

};

