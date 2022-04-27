#include "sem.h"
void printError(string reason) {
	string temp;
	temp += reason;
	temp += '\n';
	ofstream output(OUTerrorLL1);
	output << temp;
	cout << temp;
}
void checkParam(TreeNode * t)//检查参数类型和个数
{
	if (t == NULL)
		return;

	// 注释掉的这段是以前的旧代码 当时没有考虑到函数参数可以直接填常量
	/*
	if (t->name == "Variable")
	{
		bstr[bindex] = t->child[0]->name;
		//cout<< t->child[0]->name<<endl;
		bindex++;
	}
	*/

	if (t->name == "Factor") {
		if (t->child[0]->child[0] != NULL) {
			bstr[bindex] = t->child[0]->child[0]->name;
			bindex++;
		} else {
			if (t->child[0]->tk->wd.tok == INTC) {
				//cout << t->child[0]->tk->wd.tok << endl;
				bstr[bindex] = "integerT0712"; //这里偷了个懒 如果发现这个参数是int常量 就把bstr里的值赋成integerT0712
				//integerT0712 本身没有什么意义 就是一个不太容易与变量名重复的值罢了 
				bindex++;
			}

		}
	}
	for (int i = 0; i < 10; i++) {
		checkParam(t->child[i]);
	}
}

void typeSaveTable(TreeNode* t, SymbolTable* st) {
	if (t == NULL)
		return;
	//cout << t->name << endl;
	if (t->name == "TypeDecList") {
		//cout << t->childIndex;
		string name = t->child[0]->child[0]->name;
		token* tk = t->child[0]->child[0]->tk;

		string tempstr = t->child[2]->child[0]->child[0]->name;

		string type;
		if (tempstr == "integer" || tempstr == "char")
			type = t->child[2]->child[0]->child[0]->tk->wd.str;
		else
			type = t->child[2]->child[0]->child[0]->child[0]->tk->wd.str;


		string kind = "typeKind";

		//cout << name;
		st->addRecord(name, kind, type, tk);
	}

	for (int i = 0; i < 10; i++) {
		typeSaveTable(t->child[i], st);
	}
}

void varSaveTable(TreeNode* t, SymbolTable* st) {
	if (t == NULL)
		return;

	if (t->name == "VarDecList") {
		string type;
		if (t->child[0]->child[0]->name == "BaseType") {
			type = t->child[0]->child[0]->child[0]->tk->wd.str;
		} else {
			type = t->child[0]->child[0]->tk->wd.str;
		}

		TreeNode* p = t->child[1];
		while (p->name == "VarIDList") {
			//cout << t->childIndex;
			string name = p->child[0]->name;
			token* tk = p->child[0]->tk;
			string kind = "varKind ";

			st->addRecord(name, kind, type, tk);

			if (p->child[1]->child[1] == NULL)
				break;
			else
				p = p->child[1]->child[1];
		}
	}

	for (int i = 0; i < 10; i++) {
		varSaveTable(t->child[i], st);
	}
}

void procSaveTable(TreeNode* t, SymbolTable* st, SymbolTable* nextst) {
	if (t == NULL)
		return;

	if (t->name == "ProcDeclaration") {
		nextst = new SymbolTable();//新的表
		string name = t->child[1]->child[0]->name;
		token* tk = t->child[1]->child[0]->tk;
		string kind = "procKind";
		string type = t->child[0]->tk->wd.str;

		st->addRecord(name, kind, type, tk, nextst);

		TreeNode* temp = t->child[3];
		if (temp->child[0] == NULL) {
			varSaveTable(t->child[6], nextst);
			return;
		}

		if (temp->child[0]->child[0]->child[0]->child[0]->name == "BaseType") {
			type = temp->child[0]->child[0]->child[0]->child[0]->child[0]->tk->wd.str;
			name = temp->child[0]->child[0]->child[1]->child[0]->name;
			tk = temp->child[0]->child[0]->child[1]->child[0]->tk;
			kind = "varKind ";
			nextst->addRecord(name, kind, type, tk);
			nextst->paramcount++;

			temp = temp->child[0]->child[1];//parammore
		} else {
			type = temp->child[0]->child[0]->child[0]->child[0]->tk->wd.str;
			name = temp->child[0]->child[0]->child[1]->child[0]->name;
			tk = temp->child[0]->child[0]->child[1]->child[0]->tk;
			kind = "varKind ";
			nextst->addRecord(name, kind, type, tk);
			nextst->paramcount++;

			temp = temp->child[0]->child[1]; //paramMore
		}

		while (temp->child[1]) {
			temp = temp->child[1];//param dec list
			if (temp->child[0]->child[0]->child[0]->name == "BaseType") {
				type = temp->child[0]->child[0]->child[0]->child[0]->tk->wd.str;
				name = temp->child[0]->child[1]->child[0]->name;
				tk = temp->child[0]->child[1]->child[0]->tk;
				kind = "varKind ";
				nextst->addRecord(name, kind, type, tk);
				nextst->paramcount++;

				temp = temp->child[1];
			} else {
				type = temp->child[0]->child[0]->child[0]->tk->wd.str;
				name = temp->child[0]->child[1]->child[0]->name;
				tk = temp->child[0]->child[1]->child[0]->tk;
				kind = "varKind ";
				nextst->addRecord(name, kind, type, tk);
				nextst->paramcount++;

				temp = temp->child[1];
			}
		}

		varSaveTable(t->child[6], nextst);
	}

	for (int i = 0; i < 10; i++) {
		procSaveTable(t->child[i], st, nextst);
	}
}
//
void checkProgramBody(TreeNode* t, SymbolTable* st) {
	if (t == NULL)
		return;
	if (t->child[0] != NULL && t->child[0]->tk != NULL)
		if (t->child[0]->tk->wd.tok == ID) {
			for (int i = 0; i < st->index; i++) {
				// 遍历过程中 如果定位到了函数 接下来就开始检查参数个数和类型
				if (t->child[0]->name == st->table[i]->name && st->table[i]->kind == "procKind") {
					//cout << t->child[0]->name << " " << st->table[i]->next->paramcount << endl;
					//Token* pt = t->child[0]->tk;
					//int pi = t->child[0]->tk->index;

					string astr[16]; //astr 存的是正确情况下的参数类型列表 bstr是实际的参数类型列表
					int aindex = 0;

					// astr的初始化很简单 因为已经有符号表了所以记录一下就行
					for (int j = 0; j < st->table[i]->next->paramcount; j++) {
						astr[aindex] = (st->table[i]->next)->table[j]->type;
						//cout << astr[aindex] << endl;
						aindex++;
					}

					checkParam(t);

					//cout << aindex << "  " << bindex << endl;
					if (aindex != bindex) // 如果参数个数不相等 那就直接报错
					{
						string temp = "Line ";
						temp += to_string(t->child[0]->tk->line);
						temp += " ";
						temp += "\"";
						temp += t->child[0]->name;
						temp += "\" ";
						temp += "ParamNum Error!\n";
						printError(temp);
					} else // 参数类型相等 看看类型是不是按顺序匹配上了
					{
						for (int i = 0; i < aindex; i++) {
							string tempa = astr[i];
							string tempb = "default";
							if (bstr[i] == "integerT0712")//如果是int常量 就直接把类型写成integer
							{
								tempb = "integer";
							} else // 不是常量
							{
								for (int j = 0; j < st->index; j++) {
									if (bstr[i] == st->table[j]->name) {
										tempb = st->table[j]->type;
										break;
									}
								}
							}
							if (tempa != tempb) //不匹配 报类型错误
							{
								string temp = "Line ";
								temp += to_string(t->child[0]->tk->line);
								temp += " ";
								temp += "\"";
								temp += t->child[0]->name;
								temp += "\" ";
								temp += "ParamType Error!\n";
								printError(temp);
							}
						}
					}

					bindex = 0;

				}
			}
		}

	//检查过程名是不是过程标识符 其实完全可以跟上面合并在一起 但是我怕出错就单独写了 
	for (int i = 0; i < INDEX; i++) {
		if (tokenList[i].wd.tok == LPAREN) {
			string temp = tokenList[i - 1].wd.str;
			int flag = 0;
			for (int j = 0; j < st->index; j++) {
				if (temp == st->table[j]->name && st->table[j]->type == "procedure") {
					flag = 1;
				}
			}
			if (flag == 0 && temp != "read" && temp != "write") {
				string temp = "Line ";
				temp += to_string(tokenList[i - 1].line);
				temp += " ";
				temp += "\"";
				temp += tokenList[i - 1].wd.str;
				temp += "\" ";
				temp += "ProcNotFound Error!\n";
				printError(temp);
			}
		}
	}

	for (int i = 0; i < 10; i++) {
		checkProgramBody(t->child[i], st);
	}
}

void checkAssignAndDeclaration(TreeNode* t)// 这个函数的功能比较复杂 首先是检查赋值号左边的变量类型 还顺便看是否有未声明标识符
{
	// 需要对着LL1的树一点一点看
	// 我就不写注释了太麻烦了
	TreeNode* p0 = t->child[2]->child[0];
	TreeNode* p1 = t->child[3];
	int i0 = p0->tk->index;
	int i1 = p1->tk->index;
	for (int i = i0; i <= i1; i++) {
		if (tokenList[i].wd.tok == ASSIGN) {
			string tempstr = tokenList[i - 1].wd.str;
			int flag = 0;
			for (int j = 0; j < smbltable->index; j++) {
				if (tempstr == smbltable->table[j]->name && smbltable->table[j]->kind == "varKind ") {
					flag = 1;
				}
				if (tempstr == "]")
					flag = 1;
			}
			if (flag == 0) {
				string temp = "Line ";
				temp += to_string(tokenList[i - 1].line);
				temp += " ";
				temp += "\"";
				temp += tempstr;
				temp += "\" ";
				temp += "NotVarKind Error!\n";
				printError(temp);
			}
		}
		if (tokenList[i].wd.tok == ID) {
			string tempstr = tokenList[i].wd.str;
			int flag = 0;
			for (int j = 0; j < smbltable->index; j++) {
				if (tempstr == smbltable->table[j]->name && smbltable->table[j]->kind != "typeKind ") {
					flag = 1;
				}


			}
			if (flag == 0) {
				string temp = "Line ";
				temp += to_string(tokenList[i].line);
				temp += " ";
				temp += "\"";
				temp += tempstr;
				temp += "\" ";
				temp += "NoDeclaration Error!\n";
				printError(temp);
			}
		}
	}
}
void checkAssignLeftRight(TreeNode * t)// 检查赋值符号左右的类型的
{
	TreeNode * p0 = t->child[2]->child[0];
	TreeNode * p1 = t->child[3];
	int i0 = p0->tk->index;
	int i1 = p1->tk->index;
	for (int i = i0; i <= i1; i++)
	{
		if (tokenList[i].wd.tok == ASSIGN)
		{
			string tempstr = tokenList[i - 1].wd.str;
			int flag = 0;
			for (int j = 0; j < smbltable->index; j++)
			{
				if (tempstr == smbltable->table[j]->name && smbltable->table[j]->type == "integer")
				{
					flag = 1;
					break;
				}
				if (tempstr == "]")
					flag = 1;
			}
			if (flag == 0)
			{
				string temp = "Line ";
				temp += to_string(tokenList[i].line);
				temp += " ";
				temp += "\"";
				temp += ":=";
				temp += "\" ";
				temp += "NotSameType Error!\n";
				printError(temp);
			}

			for (int j = 0; j < smbltable->index; j++)
			{
				if (tempstr == smbltable->table[j]->name && smbltable->table[j]->type == "integer")
				{
					for (int k = i + 1; tokenList[k].wd.str != ";"; k++)
					{
						if (tokenList[k].wd.tok == LPAREN);
						else if (tokenList[k].wd.tok == RPAREN);
						else if (tokenList[k].wd.tok == INTC);
						else if (tokenList[k].wd.tok == PLUS);
						else if (tokenList[k].wd.tok == MINUS);
						else if (tokenList[k].wd.tok == TIMES);
						else if (tokenList[k].wd.tok == OVER);
						else if (tokenList[k].wd.tok == ID)
						{
							int flag = 0;
							for (int l = 0; l < smbltable->index; l++)
							{
								if (tokenList[k].wd.str == smbltable->table[l]->name && smbltable->table[l]->type == "integer")
									flag = 1;
								if (tempstr == "]")
									flag = 1;
							}
							if (flag == 0)
							{
								string temp = "Line ";
								temp += to_string(tokenList[i].line);
								temp += " ";
								temp += "\"";
								temp += ":=";
								temp += "\" ";
								temp += "NotSameType Error!\n";
								printError(temp);
							}
						}

					}
				}
			}

		}
	}

	int procedure[16];
	int begin[16];
	int end[16];
	int pindex = 0;
	for (int i = 0; i < i0; i++)
	{
		if (tokenList[i].wd.str == "procedure")
		{
			procedure[pindex] = tokenList[i].index;
			for (int j = i; j < i0; j++)
			{
				if (tokenList[j].wd.str == "begin")
				{
					begin[pindex] = tokenList[j].index;
					for (int k = j; k < i0; k++)
					{
						if (tokenList[k].wd.str == "end")
						{
							end[pindex] = tokenList[k].index;
							pindex++;
							break;
						}
					}
					break;
				}
			}
		}
	}

	for (int i = 0; i < pindex; i++)
	{
		//cout << procedure[i] << " "
			//<< begin[i] << " "
			//<< end[i] << " " << endl;
		string procname = tokenList[procedure[i] + 1].wd.str;
		//cout << procname << endl;
		SymbolTable* p = NULL;
		for (int j = 0; j < smbltable->index; j++)
		{
			if (smbltable->table[j]->name == procname)
			{
				// cout << smbltable->table[j]->name << endl;
				p = smbltable->table[j]->next;
			}
		}

		for (int j = begin[i]; j < end[i]; j++)
		{

			if (tokenList[j].wd.tok == ASSIGN)
			{
				string tempstr = tokenList[j - 1].wd.str;
				int flag = 0;
				for (int k = 0; k < p->index; k++)
				{
					if (tempstr == p->table[k]->name && p->table[k]->type == "integer")
					{
						flag = 1;
						break;
					}
				}
				if (flag == 0)
				{
					string temp = "Line ";
					temp += to_string(tokenList[j].line);
					temp += " ";
					temp += "\"";
					temp += ":=";
					temp += "\" ";
					temp += "NotSameType Error!\n";
					printError(temp);
				}
				for (int k = 0; k < p->index; k++)
				{
					if (tempstr == p->table[k]->name && p->table[k]->type == "integer")
					{
						for (int l = j + 1; tokenList[l].wd.str != ";"; l++)
						{
							if (tokenList[l].wd.tok == LPAREN);
							else if (tokenList[l].wd.tok == RPAREN);
							else if (tokenList[l].wd.tok == INTC);
							else if (tokenList[l].wd.tok == PLUS);
							else if (tokenList[l].wd.tok == MINUS);
							else if (tokenList[l].wd.tok == TIMES);
							else if (tokenList[l].wd.tok == OVER);
							else if (tokenList[l].wd.tok == ID)
							{
								int flag = 0;
								for (int m = 0; m < p->index; l++)
								{
									if (tokenList[l].wd.str == p->table[m]->name && p->table[m]->type == "integer")
										flag = 1;
								}
								if (flag == 0)
								{
									string temp = "Line ";
									temp += to_string(tokenList[j].line);
									temp += " ";
									temp += "\"";
									temp += ":=";
									temp += "\" ";
									temp += "NotSameType Error!\n";
									printError(temp);
								}
							}

						}
					}
				}

			}


		}
	}
}

void checkRange()//检查array越界的
{
	string type[16];
	int min[16];
	int max[16];
	int iarr = 0;

	for (int i = 0; i < INDEX; i++)
	{
		if (tokenList[i].wd.tok == UNDERANGE)
		{
			min[iarr] = atoi(tokenList[i - 1].wd.str.c_str());
			max[iarr] = atoi(tokenList[i + 1].wd.str.c_str());
			type[iarr] = tokenList[i - 5].wd.str;
			//			cout << min[iarr] << endl;
			//			cout << max[iarr] << endl;
			iarr++;
		}
	}

	for (int i = 0; i < INDEX; i++)
	{
		if (tokenList[i].wd.str == "[" && tokenList[i + 1].wd.tok == INTC && tokenList[i + 2].wd.str == "]")
		{
			string temptype;
			int flag = 0;
			for (int j = 0; j < smbltable->index; j++)
			{
				if (smbltable->table[j]->name == tokenList[i - 1].wd.str && smbltable->table[j]->kind == "varKind ")
				{
					temptype = smbltable->table[j]->type;
					flag = 1;
					break;
				}
			}
			if (flag == 0)
			{
				string temp = "Line ";
				temp += to_string(tokenList[i].line);
				temp += " ";
				temp += "\"";
				temp += tokenList[i - 1].wd.str;
				temp += "\" ";
				temp += "NotVar Error!\n";
				printError(temp);
			}
			//cout << tokenList[i + 1].wd.str << endl;
			for (int j = 0; j < iarr; j++)
			{

				if (type[j] == temptype)
				{
					if (atoi(tokenList[i + 1].wd.str.c_str()) <= max[j] && atoi(tokenList[i + 1].wd.str.c_str()) >= min[j]);
					else
					{
						string temp = "Line ";
						temp += to_string(tokenList[i + 1].line);
						temp += " ";
						temp += "\"";
						temp += tokenList[i + 1].wd.str;
						temp += "\" ";
						temp += "OutRange Error!\n";
						printError(temp);
					}
				}
			}
		}
	}

}
/****************************************************************/
/****************************************************************/

void semanticAnalysis(TreeNode *treeroot,char* out)
{
	TreeNode* t = treeroot;

	t = t->child[1];
	//cout << t->name;
	// 现在t是DeclarePart
	for (int i = 0; t->child[i] != NULL; i++)
	{
		TreeNode* temp = t->child[i];
		if (temp->name == "TypeDec")
			typeSaveTable(temp, smbltable);
		else if (temp->name == "VarDec")
			varSaveTable(temp, smbltable);
		else if (temp->name == "ProcDec")
		{
			SymbolTable * p = NULL;
			procSaveTable(temp, smbltable, p);
		}
	}

	TreeNode* t1 = treeroot;

	t1 = t1->child[2];
	checkProgramBody(t1, smbltable); //检查参数类型

	checkAssignAndDeclaration(treeroot);  //检查声明

	checkAssignLeftRight(treeroot);		//检查赋值
	
	checkRange();

	smbltable->printTable();
	ofstream output(out);
	output << outputstr;
}