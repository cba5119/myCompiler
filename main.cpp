#include <iostream>
#include <string.h>
#include <set>
#include <vector>
#include <map>
//#include <io.h>
#include <stdio.h>

using namespace std;


#define MYLOG(fmt, args...) printf("%s:%d(%s):" fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
#define MIN(a, b) (a < b ? a : b)


enum KeyWordType {
    CLASS, METHOD, INT, FUNCTION, BOOLEAN,
    CONSTRUCTOR, CHAR, VOID, VAR, STATIC,
    FILED, LET, DO, IF, ELSE, WHILE, RETURN,
    KEYWORD_TRUE, KEYWORD_FALSE, KEYWORD_NULL, THIS
};

enum TokenType {
    KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST
};

struct Token {
    string type;
    string value;
};

set<char> symbolSet;
map<string, KeyWordType> keyWordMap;
vector<Token> tokenVector;


int readLine(FILE* f, string& out) {
    char buf[1024] = {0};
    char* ret = fgets(buf, 1024, f);
    if (NULL != ret) {
        char* substr = strstr(buf, "\n");
        if (NULL != substr) {
            substr[0] = '\0';
        }
        out = buf;
        return strlen(buf);
    }
    else {
        return -1;
    }
}

void initMap() {
    symbolSet.insert('{');
    symbolSet.insert('}');
    symbolSet.insert('(');
    symbolSet.insert(')');
    symbolSet.insert('[');
    symbolSet.insert(']');
    symbolSet.insert('.');
    symbolSet.insert(',');
    symbolSet.insert(';');
    symbolSet.insert('+');
    symbolSet.insert('-');
    symbolSet.insert('*');
    symbolSet.insert('/');
    symbolSet.insert('&');
    symbolSet.insert('|');
    symbolSet.insert('<');
    symbolSet.insert('>');
    symbolSet.insert('=');
    symbolSet.insert('~');

    keyWordMap["class"] = CLASS;
    keyWordMap["method"] =  METHOD;
    keyWordMap["int"] = INT;
    keyWordMap["function"] = FUNCTION;
    keyWordMap["boolean"] = BOOLEAN;
    keyWordMap["constructor"] =  CONSTRUCTOR;
    keyWordMap["char"] = CHAR;
    keyWordMap["void"] = VOID;
    keyWordMap["var"] = VAR;
    keyWordMap["static"] = STATIC;
    keyWordMap["field"] =  FILED;
    keyWordMap["let"] = LET;
    keyWordMap["do"] =  DO;
    keyWordMap["if"] = IF;
    keyWordMap["else"] = ELSE;
    keyWordMap["while"] = WHILE;
    keyWordMap["return"] =       RETURN;
    keyWordMap["true"] = KEYWORD_TRUE;
    keyWordMap["false"] =    KEYWORD_FALSE;
    keyWordMap["null"] =   KEYWORD_NULL;
    keyWordMap["this"] =   THIS;
}

bool isSymbol(char c) {
    if (0 != symbolSet.count(c)) {
        return true;
    }
    return false;
}

bool isSymbol(string str) {
    if (1 != str.size()) {
        return false;
    }
    char c = str[0];

    if (0 != symbolSet.count(c)) {
        return true;
    }
    return false;
}

bool isOperator(string str) {
    if ("+" == str || "-" == str || "*" == str || "/" == str
        || "&lt;" == str || "&gt;" == str) {
        return true;
    }
    return false;
}


bool isKeyWord(string str) {
        if (0 != keyWordMap.count(str)) {
            return true;
        }
        return false;
}

bool isInt(string str) {
    if (0 == str.length()) {
        return false;
    }
    for (int i = 0; i < str.length(); i++) {
        if (str[i] < '0' || str[i] > '9') {
            return false;
        }
    }
    return true;
}


bool isBracket(string str) {
    if (str.length() == 1) {
        string bracketStr = "()[]{}";
        int index = bracketStr.find(str);
        if (-1 != index) {
            return true;
        }
    }
    return false;
}




void analysisProgram(string str, vector<string> &tokens) {
    cout<<"length = "<<str.length()<<endl;
    int preIndex = 0,  index = 0;;
    for (int i = 0; i < str.length(); i++) {
        int index = 0;

        while ((i <= str.length() - 1) && (' ' == str[i] || '\t' == str[i])) {
            i++;
        }

        if (i == str.length()) {
            break;
        }

        if(isSymbol(str[i]) && '"' != str[i]) {
            string token;

            token += "<symbol>";
            if ('<' == str[i]) {
                token += "&lt;";
            }
            else if ('>' == str[i]) {
                token += "&gt;";
            }
            else if ('&' == str[i]) {
                token += "&amp;";
            }
            else {
              token += str.substr(i, 1);
            }
            token += "</symbol>";

            tokens.push_back(token);

            continue;
        }

        preIndex = i;

        if ('"' == str[i]) {
            i++;
            while ((i <= str.length() - 1) && ('"' != str[i])) {
                i++;
            }
            string token;
            token += "<stringConstant>";
            token += str.substr(preIndex + 1, i - preIndex - 1);
            token += "</stringConstant>";
            cout<<"push 4 "<<str.substr(preIndex + 1, i - preIndex - 1)<<endl;
            tokens.push_back(token);

            continue;
        }

        while ((i <= str.length() - 1) && (' ' != str[i] && '\t' != str[i] && !isSymbol(str[i]))) {
            i++;
        }

        if (i == str.length()) {
            break;
        }

        if(isSymbol(str[i])) {
            string token;
            string tmp = str.substr(preIndex, i - preIndex);
            if (isKeyWord(tmp)) {
                token += "<keyword>";
                token += tmp;
                token += "</keyword>";
            }
            else if (isInt(tmp)) {
                token += "<integerConstant>";
                token += tmp;
                token += "</integerConstant>";
            }
            else {
                token += "<identifier>";
                token += tmp;
                token += "</identifier>";
            }

            tokens.push_back(token);
            cout<<"push 2 "<<token<<endl;
            i--;
            continue;
        }

        index = i;

        string token;
        string tmp = str.substr(preIndex, index - preIndex);
        if (isKeyWord(tmp)) {
            token += "<keyword>";
            token += tmp;
            token += "</keyword>";
        }
        else if (isInt(tmp)) {
            token += "<integerConstant>";
            token += tmp;
            token += "</integerConstant>";
        }
        else {
            token += "<identifier>";
            token += tmp;
            token += "</identifier>";
        }

        tokens.push_back(token);
        cout<<"push 3 "<<token<<endl;
    }
}

/*
void dir(string path)
{
    long hFile = 0;
    struct _finddata_t fileInfo;
    string pathName, exdName;

    if ((hFile = _findfirst(path.c_str(), &fileInfo)) == -1) {
        return;
    }
    do {
        cout << fileInfo.name << (fileInfo.attrib&_A_SUBDIR? "[folder]":"[file]") << endl;
    } while (_findnext(hFile, &fileInfo) == 0);
    _findclose(hFile);
    return;
}
*/


bool dealTheXml(string str, string& key, string& value) {
    int angleBracketIndex1 = str.find("<");
    int angleBracketIndex2 = str.find(">");

    if (-1 == angleBracketIndex1 || -1 == angleBracketIndex2) {
        return false;
    }

    key = str.substr(angleBracketIndex1 + 1, angleBracketIndex2 - angleBracketIndex1 - 1);

    str = str.substr(angleBracketIndex2 + 1, str.length());

    angleBracketIndex1 = str.find("<");

    if (-1 == angleBracketIndex1) {
        return false;
    }

    value = str.substr(0, angleBracketIndex1);
    return true;
}


string createXml(Token token) {
    return "<" + token.type + ">" + token.value + "</" + token.type + ">\n";
}

int findFirstToken(int start, int end, string str) {
    int ret = -1;

    for (int i = start; i <= end; i++) {
        if (str == tokenVector[i].value) {
            ret = i;
            break;
        }
    }

    return ret;
}

bool isMacth(string s1, string s2) {
    if (("(" == s1 && ")" == s2)
       || ("[" == s1 && "]" == s2)
       || ("{" == s1 && "}" == s2)) {
        return true;
    }
    return false;
}


int findMacthIndex(int start, int end) {
    int ret = -1;
    MYLOG("start %d   end %d\n", start, end);
    vector<string> bracketStack;

    bracketStack.push_back(tokenVector[start].value);

    for (int i = start + 1; i <= end; i++) {
        if (isBracket(tokenVector[i].value)) {
            string str = bracketStack.back();
            if (isMacth(str, tokenVector[i].value)) {
                bracketStack.pop_back();
            }
            else {
                bracketStack.push_back(tokenVector[i].value);
            }
        }

        if (bracketStack.empty()) {
            ret = i;
            break;
        }
    }

    return ret;
}


int dealWithExpression(int start, int end, string& str);
int dealWithStatements(int start, int end, string& str);
int dealWithStatement(int start, int end, string& str);

int dealWithCallFunction(int start, int end, string& str) {

}


int dealWithExpressionList(int start, int end, string& str) {
    MYLOG("start[%d] = \"%s\" end[%d] = \"%s\" \n", start, tokenVector[start].value.c_str(), end, tokenVector[end].value.c_str());
    int cur = start;

    str += "<expressionList>\n";
    while (cur <= end) {
        int index = cur;
        while(index < end) {
            if ("," == tokenVector[index].value) {
                break;
            }
            else {
                index++;
            }
        }

        if (index < end) {
            dealWithExpression(cur, index - 1, str);
            str += createXml(tokenVector[index]);
            cur = index + 1;
        }
        else {
            dealWithExpression(cur, index, str);
            break;
        }

    }
    str += "</expressionList>\n";

    return cur - start + 1;
}

int dealWithTerm(int start, int end, string& str) {
    MYLOG("start[%d] = \"%s\" end[%d] = \"%s\" \n", start, tokenVector[start].value.c_str(), end, tokenVector[end].value.c_str());
    int cur = start;

    str += "<term>\n";

    if ("integerConstant" == tokenVector[start].type
        || "stringConstant" == tokenVector[start].type) {
        str += createXml(tokenVector[start]);

        cur = cur + 1;
    }
    else if ("identifier" == tokenVector[cur].type){
        str += createXml(tokenVector[cur]);
        cur = cur + 1;
        if (end - start + 1 >= 2) {
            if ("." == tokenVector[cur].value) {
                str += createXml(tokenVector[cur]);
                str += createXml(tokenVector[cur + 1]);

                str += createXml(tokenVector[cur + 2]);
                cur += dealWithExpressionList(cur + 3, end - 1, str);
                str += createXml(tokenVector[end]);
            }
            else if ("[" == tokenVector[cur].value) {
                str += createXml(tokenVector[cur]);
                cur += dealWithExpression(cur + 1, end - 1, str);
                str += createXml(tokenVector[end]);
            }
        }
    }

    str += "</term>\n";
    return cur - start + 1;
}



int dealWithExpression(int start, int end, string& str) {
    int cur = start;
    MYLOG("start[%d] = \"%s\" end[%d] = \"%s\" \n", start, tokenVector[start].value.c_str(), end, tokenVector[end].value.c_str());

    str += "<expression>\n";

    while(cur <= end) {
        int index = cur;

        while(index < end) {
            if (isOperator(tokenVector[index].value)) {
                break;
            }
            else {
                index++;
            }
        }
        MYLOG("index = \"%s\" \n", tokenVector[index].value.c_str());
        if (index < end) {
            cur += dealWithTerm(cur, index - 1, str);

            str += createXml(tokenVector[index]);
        }
        else {
            cur += dealWithTerm(cur, index, str);
            break;
        }
    }

    str += "</expression>\n";
    return cur - start + 1;
}

int dealWithParamterList(int start, int end, string& str) {
    MYLOG("start[%d] = \"%s\" end[%d] = \"%s\" \n", start, tokenVector[start].value.c_str(), end, tokenVector[end].value.c_str());
    MYLOG("start = \"%s\" end = \"%s\" \n", tokenVector[start].value.c_str(), tokenVector[end].value.c_str());
    int cur = start;
    if (start < end && "(" == tokenVector[start].value && ")" == tokenVector[end].value) {

        str += createXml(tokenVector[start]);
        str += "<parameterList>\n";

        for (int i = start + 1; i < end; i++) {

        }

        str += "<parameterList>\n";
        str += createXml(tokenVector[end]);
    }

    return end - start + 1;
}

int dealWithLet(int start, int end, string& str) {
    MYLOG("start[%d] = \"%s\" end[%d] = \"%s\" \n", start, tokenVector[start].value.c_str(), end, tokenVector[end].value.c_str());
    int cur = start;

    str += "<letStatement>\n";

    if (end - start + 1 >= 5 && "let" == tokenVector[start].value && ";" == tokenVector[end].value) {
        str += createXml(tokenVector[start+0]);

        int index = findFirstToken(start+1, end, "=");

        if (-1 != index) {
            MYLOG("index = %s index = %d start = %d\n", tokenVector[index].value.c_str(), index, start);
            if (index - start + 1 > 3) {
                str += createXml(tokenVector[start + 1]);
                if ("[" == tokenVector[start + 2].value && "]" == tokenVector[index - 1].value) {
                    str += createXml(tokenVector[start+2]);
                    dealWithExpression(start+3, index - 2, str);
                    str += createXml(tokenVector[index-1]);
                }
            }
            else {
                str += createXml(tokenVector[start + 1]);
            }

            str += createXml(tokenVector[index]);

            dealWithExpression(index + 1, end - 1, str);

            str += createXml(tokenVector[end]);
        }
    }

    str += "</letStatement>\n";

    return end - start + 1;
}


int dealWithWhile(int start, int end, string& str) {
    MYLOG("start[%d] = \"%s\" end[%d] = \"%s\" \n", start, tokenVector[start].value.c_str(), end, tokenVector[end].value.c_str());
    int cur = start;
    str += "<whileStatement>\n";

    str += createXml(tokenVector[cur]);
    cur = cur + 1;

    int leftBracketIndex = findFirstToken(cur, end, "(");
    int rightBracketIndex = findFirstToken(cur, end, ")");

    if (-1 != leftBracketIndex && -1 != rightBracketIndex) {
        str += createXml(tokenVector[leftBracketIndex]);

        cur += dealWithExpression(leftBracketIndex + 1, rightBracketIndex - 1, str);

        str += createXml(tokenVector[rightBracketIndex]);

        int leftBraceIndex = findFirstToken(rightBracketIndex+1, end, "{");
        int rightBraceIndex = findMacthIndex(leftBraceIndex, end);
        MYLOG("left %d  right %d\n", leftBraceIndex, rightBraceIndex);
        if (-1 != leftBracketIndex && -1 != rightBracketIndex) {
            str += createXml(tokenVector[leftBraceIndex]);

            dealWithStatements(leftBraceIndex + 1, rightBraceIndex - 1, str);

            str += createXml(tokenVector[rightBraceIndex]);
        }
    }

    str += "</whileStatement>\n";
    return end - start + 1;
}


int dealWithVarDec(int start, int end, string& str) {
    MYLOG("start[%d] = \"%s\" end[%d] = \"%s\" \n", start, tokenVector[start].value.c_str(), end, tokenVector[end].value.c_str());
    str += "<varDec>\n";

    if (end - start + 1 >= 4) {
        for (int i = start; i <= end; i++) {
            str += createXml(tokenVector[i]);
        }
    }
    str += "</varDec>\n";
    return end - start + 1;
}

int dealWithStatement(int start, int end, string& str) {
    MYLOG("start = \"%s\" end = \"%s\" \n", tokenVector[start].value.c_str(), tokenVector[end].value.c_str());
    int cur = start;

   if ("let" == tokenVector[cur].value) {
       cur += dealWithLet(cur, end, str);
   }
   else if ("while" == tokenVector[cur].value) {
       cur += dealWithWhile(cur, end, str);
   }

    return end - start + 1;
}

int dealWithStatements(int start, int end, string& str) {
    MYLOG("start[%d] = \"%s\" end[%d] = \"%s\" \n", start, tokenVector[start].value.c_str(), end, tokenVector[end].value.c_str());
    int cur = start;

    str += "<statements>\n";

    while (cur < end) {
        int semicolonIndex = findFirstToken(cur, end, ";");
        int leftBrace = findFirstToken(cur, end, "{");
        MYLOG("leftBrace %d = %s\n", leftBrace, tokenVector[leftBrace].value.c_str());
        int rightBrace = -1;

        if (-1 != leftBrace) {
            rightBrace = findMacthIndex(leftBrace, end);
            MYLOG("rightBrace %d = %s\n", rightBrace, tokenVector[rightBrace].value.c_str());
        }

        int index = end;
        if (-1 != semicolonIndex && -1 == rightBrace) {
            index = semicolonIndex;
        }
        else if (-1 == semicolonIndex && -1 != rightBrace) {
            index = rightBrace;
        }
        else if (-1 != semicolonIndex && -1 != rightBrace) {
            if (semicolonIndex < leftBrace) {
                index = semicolonIndex;
            }
            else {
               index = rightBrace;
            }
        }
        MYLOG("cur = \"%s\" index = \"%s\" \n", tokenVector[cur].value.c_str(), tokenVector[index].value.c_str());
        cur += dealWithStatement(cur, index, str);
    }

    str += "</statements>\n";

    return end - start + 1;
}

int dealWithFunction(int start, int end, string& str) {
    MYLOG("start[%d] = \"%s\" end[%d] = \"%s\" \n", start, tokenVector[start].value.c_str(), end, tokenVector[end].value.c_str());
    int cur = start;

    if (end - start + 1 >= 7) {

       str += createXml(tokenVector[start]);
       str += createXml(tokenVector[start+1]);
       str += createXml(tokenVector[start+2]);

       cur = cur + 3;

       int rightBracketIndex = findFirstToken(cur, end, ")");

       cur += dealWithParamterList(cur, rightBracketIndex, str);

       str += "<subroutineBody>\n";

       str += createXml(tokenVector[cur]);

       cur = cur + 1;

       while (cur < end) {
           if ("var" == tokenVector[cur].value) {
               int semicolonIndex = findFirstToken(cur, end, ";");
               if (-1 != semicolonIndex) {
                   cur += dealWithVarDec(cur, semicolonIndex, str);
               }
           }
           else {
               break;
           }
       }

       while(cur < end) {
           cur += dealWithStatements(cur, end - 1, str);
       }

       str += "</subroutineBody>\n";

       str += createXml(tokenVector[end]);
    }

    return cur - start + 1;
}


void dealWithClass(int start, int end, string& str) {
    MYLOG("start[%d] = \"%s\" end[%d] = \"%s\" \n", start, tokenVector[start].value.c_str(), end, tokenVector[end].value.c_str());
    str += "<class>\n";
    if (end - start + 1 >= 4) {
        if ("{" == tokenVector[start+2].value && "}" == tokenVector[end].value) {
            str += "<keyword>"+tokenVector[start+0].value+"</keyword>\n";
            str += "<identifier>"+tokenVector[start+1].value+"</identifier>\n";
            str += "<symbol>"+tokenVector[start+2].value+"</symbol>\n";

            int i = start + 3;

            while(i < end) {
                if ("function" == tokenVector[i].value) {
                    str += "<subroutineDec>\n";
                    i += dealWithFunction(i, end - 1, str);
                    str += "</subroutineDec>\n";
                }
            }

            str += "<symbol>"+tokenVector[end].value+"</symbol>\n";
        }
    }
    str += "</class>\n";
}

void dealWithProgram(string& str) {
    string keyword = tokenVector[0].value;

    if ("class" == keyword) {
        dealWithClass(0, tokenVector.size() - 1,  str);
    }

}

int main(int argc, char *argv[])
{
    string filename;

    if (argc ==  2) {
        filename = argv[1];
    }
    else {
        filename = "my.jack";
    }

    FILE* f = fopen(filename.c_str(), "r");
    if (NULL == f) {
       cout << "open error!" << endl;
       return -1;
    }

    initMap();

    string str;
    vector<string> tokens;

    while(readLine(f, str) != -1) {
        //delete the comment
        int commentIndex = str.find("//");
        if (-1 != commentIndex) {
            str = str.substr(0, commentIndex);
        }

        int starIndex1 = str.find("/*");
        int starIndex2 = str.find("*/");

        if (-1 != starIndex1) {
           str = str.substr(starIndex2 + 2, str.length());
        }

        if ("" != str) {
           cout<<"read = "<<str<<endl;
           analysisProgram(str, tokens);
        }

    }


    FILE* outputFile = fopen("my.xml", "w+");
    if (NULL == outputFile) {
       cout << "create output error!" << endl;
       return -1;
    }


    fwrite("<tokens>\n", 1, strlen("<tokens>\n"), outputFile);
    for (int i = 0; i < tokens.size(); i++) {
        //cout<<tokens[i]<<endl;
        fwrite(tokens[i].c_str(), 1, tokens[i].length(), outputFile);
        fwrite("\n", 1, 1, outputFile);
    }
    fwrite("</tokens>\n", 1, strlen("<tokens>\n"), outputFile);



    fseek(outputFile, 0, SEEK_SET);

    int i = 0;
    while(readLine(outputFile, str) != -1) {
        string key, value;
        if(dealTheXml(str, key, value)) {
            cout<<"k, v, index = "<<key<<"  "<<value<<"  "<<i++<<endl;
            Token token;
            token.type = key;
            token.value = value;
            tokenVector.push_back(token);
        }
    }

    cout<<"write end"<<endl;


    string parseStr;

    dealWithProgram(parseStr);


 //   cout<<parseStr<<endl;

    cout<<"deal end"<<endl;

    FILE* parseFile = fopen("myP.xml", "w+");
    if (NULL == parseFile) {
       cout << "create parseFile error!" << endl;
       return -1;
    }

    fwrite(parseStr.c_str(), 1, parseStr.length(), parseFile);


    return 0;
}
