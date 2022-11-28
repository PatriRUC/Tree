#include <bits/stdc++.h>
#include<windows.h>
#include<cstdio> 
using namespace std;


// 标签处理工作 
string Tag[][3] = { // tag 列表，头有两种，尾有一种

    {"<html", "<html>", "</html>" }, {"<head", "<head>", "</head>"}, {"<title", "<title>", "</title>"}, 
    {"<body", "<body>", "</body>" }, {"<div", "<div>", "</div>"},    {"<ul", "<ul>", "</ul>"}, 
    {"<li", "<li>", "</li>" },      {"<a", "<a>", "</a>" },          {"<h1", "<h1>", "</h1>"}, 
    {"<h2", "<h2>", "</h2>" },      {"<h3", "<h3>", "</h3>" },       {"<p", "<p>", "</p>"},
    {"<script", "<script>", "</script>" }, {"<span", "<span>", "</span>"}, {"<h4", "<h4>", "</h4>" },
    {"<td", "<td>", "</td>"}, {"<table", "<table>", "</table>"}, {"<tr", "<tr>", "</tr>"},
    {"<tobdy", "<tbody>", "</tbody>"}, {"<footer", "<footer>", "</footer>"}, {"<header", "<header>", "</header>"},
    {"<nav", "<nav>", "</nav>"}, {"<button", "<button>", "</button>"}, {"<em", "<em>", "</em>"},
    {"<strong", "<strong>", "</strong>"}, {"<i", "<i>", "</i>"}

};

int size = sizeof(Tag) / sizeof(Tag[0]);



bool IsRightTag(string word) // 若为闭合头标签
{
    for(int i = 0; i < size; i++){
        if(word == Tag[i][0] || word == Tag[i][1]){
            return true;
        }
    }
    return false;
}


bool IsLeftTag(string word) // 若为闭合尾标签
{
    for(int i = 0; i < size; i++){
        if(word == Tag[i][2]) return true;
    }
    return false;
}

// 标签处理工作

int FindStr(string s, string t) // 字符串匹配
{
    int i = 0, j = 0;
    int s_len = s.length(), t_len = t.length();
    while(i < s_len && j < t_len){
        if(s[i] == t[j]){
            i++;
            j++;
        }
        else{
            i = i-j+1;
            j = 0;
        }
    }
    if(j == t_len) return i - t_len; 
    else return -1;
    // 找到返回非负数，没找到返回-1
}


typedef struct TreeNode // 树结构
{
	int index;
	string data = ""; // 空串
	TreeNode* parent;
	TreeNode* firstchild;
	TreeNode* sibling;
}Node;


Node* CreateTree() // 创建一颗新树
{
	Node* root = new Node;
	root->index = 0;
	root->parent = NULL;
	root->firstchild = NULL;
	root->sibling = NULL;
	return root;
}


Node* CreateNode() // 创建新节点
{
	Node* newnode = new Node;
	newnode->parent = NULL;
	newnode->firstchild = NULL;
	newnode->sibling = NULL;
	return newnode;
}


void WholeTree(Node* root) // 打印整个树，注意每次都必须传根进来
{
    if(!root) return;
    cout << root->data << "（序号为: " << root->index << "）" << endl;
    WholeTree(root->firstchild);
    WholeTree(root->sibling);
}


void GetTreeNode(Node* root, int index, Node* &Now) // 返还指定序号的节点，注意每次都必须传根进来
{
    if(!root) return;
    else{
        if(root->index == index){
            Now = root;
            return;
        }
        else{
            GetTreeNode(root->firstchild, index, Now);
            GetTreeNode(root->sibling, index, Now); 
        }
    }
}


string GetTag(Node* root) // 获取某节点的标签
{
    string s = root->data;
    string tag;
    int len = s.length();
    for(int i = 0; i < len; i++){
        if(s[i] == ' '){ // 第一个空格前的肯定是标签
            if(s[i - 1] == '>') i -= 1;
            tag = s.substr(1, i - 1);
            break;
        }
    }
    return tag;
}


string GetClass(Node* root) // 获取某节点的（全部！）class，若无class则返回空串
{
    string s = root->data;
    string sign = "class="; // 有class的标志
    int start = FindStr(s, sign); // 获取子串的起始位置
    if(start == -1) return ""; // 若未找到，返回空串
    // <div> class="abc" </div>
    start += 7; // 来到前一个引号后的位置
    int len = 1;
    while(1){
        if(s[start + len] == '"') break;
        len++;
    }
    string res = s.substr(start, len);
    return res;
}


void DisplayNode(Node* root) // 展示某节点详细信息，即展示其标签、父亲和孩子列表
{
    cout << "第 " << root->index << " 个节点, tag = " << GetTag(root) << ", class = " << GetClass(root) << endl;
    cout << "\n该节点的正文为: " << root->data << endl;

    cout << "\n该节点的父亲为: " << endl;
    if(!root->parent) cout << "无! " << endl;
    else cout << GetTag(root->parent) << " 序号为: " << root->parent->index << endl;

    cout << "\n该节点的孩子为: " << endl;
    Node* p = root->firstchild;
    if(!p) cout << "无! " << endl;
    else{
        while(p){
            cout << GetTag(p) << " 序号为: " << p->index << endl;
            p = p->sibling;
        }
    }
}


void InnerText(Node* root)
{
    if(!root) return;
    // 遍历字符串中的每一个字符，遇到空格，分析前一个字符串，若没有非文本标志就输出，然后分析空格后面的字符串 
    bool w = false;
    string s = root->data;
    int len = s.length();
    int i = 0;
    int j = 0;
    while(j < len - 1){ // 最后是个空格
        if(s[j] == ' '){
            string t = s.substr(i, j - i); // 前一段字符串
            if(FindStr(t, "=") == -1 &&  FindStr(t, "<") == -1 && FindStr(t, ">") == -1){ // 若三个标志都没找到
                w = true;
                cout << t << ' '; // 则输出
            }
            i = j + 1; // 每遇到一个空格，i 都要换位
        }
        j++;
    }

    if(w == true) cout << '\n'; // 若输出过东西，换个行
    InnerText(root->firstchild);
    InnerText(root->sibling);    
}


void OuterText(Node* root) // 其实就是遍历输出
{
    if(!root) return;
    cout << root->data << endl;
    OuterText(root->firstchild);
    OuterText(root->sibling);
}


void GetHref(Node* root) // 输出链接
{
    if(GetTag(root) != "a"){
        cout << "这个不是A标签哦! " << endl;
        return;
    }
    string s = root->data;
    string sign = "href="; 
    int start = FindStr(s, sign); // 获取子串的起始位置
    start += 6; // 来到前一个引号后的位置
    int len = 1;
    while(1){
        if(s[start + len] == '"') break;
        len++;
    }
    string res = s.substr(start, len);
    cout << "该标签的href = " << res << endl;
    return;
}


string GetId(Node* root) // 返回 id
{
    string s = root->data;
    string sign = "id="; 
    int start = FindStr(s, sign); // 获取子串的起始位置
    if(start == -1) return "";
    start += 4; // 来到前一个引号后的位置
    int len = 1;
    while(1){
        if(s[start + len] == '"') break;
        len++;
    }
    string res = s.substr(start, len);
    return res;
}


void Display_By_Tag(Node* root, string mytag, bool &find) // 展示某节点后代中某tag的节点
{
    if(!root) return; 
    string tag = GetTag(root);
    if(tag == mytag){
        find = true;
        cout << root->data << " 序号为: " << root->index << endl;
    }
    Display_By_Tag(root->firstchild, mytag, find);
    Display_By_Tag(root->sibling, mytag, find);
}


void Display_By_Class(Node* root, string myclass, bool &find) // 展示某节点后代中某class的节点
{
    if(!root) return;
    string Class = GetClass(root);
    if(FindStr(Class, myclass) != -1){
        find = true;
        cout << root->data << " 序号为: " << root->index << endl;
    }
    Display_By_Class(root->firstchild, myclass, find);
    Display_By_Class(root->sibling, myclass, find);
}


string text[100000];

int main()
{	
    int x;
	// x = MessageBox(GetForegroundWindow(),"user's manual","user's manual",1);
    while(1){
        int write;
        cout << "您是否要录入HTML文本,是请打1,不是请打0: ";
        cin >> write;
        if(write == 0){
            cout << "\n选择器服务结束。更多功能, 敬请期待! " << endl;
            break;
        }

        else{

            // 1、将 html文本按照 tag 切分存好 
            Node* root = CreateTree();
            fstream f;
            f.open("C:\\Users\\HONOR\\Desktop\\news.txt", ios::in);
            string s;
            int counts = 0;
	        while(f>>s){
                string seg;
                while(s.length() != 0){
                    string temp = s;
                    for(int i = 0; i < s.length()-1; i++){
                        if(s[i+1] == '<'){
                            seg = s.substr(0, i+1);
                            text[counts] = seg;
                            counts++;
                            s = s.substr(i+1);
                            break;                         
                        }
                        else if(s[i] == '>'){
                            seg = s.substr(0, i+1);
                            text[counts] = seg;
                            counts++;
                            s = s.substr(i+1);
                            break;
                        }
                    }
                    if(s == temp){ // 若未作改动
                        text[counts] = s;
                        counts++;
                        break;
                    }
                }
                
            }

	        f.close();
            // 1、

            // 2、以下将 html 存于树中

            int num = 0; // 节点的数量
            Node* pa = root; // 当前最新的父亲
			for(int i = 0; i < counts; i++){ // 先按照标签存了。在某个爸的一辈：
                
				string s = text[i];
                if(IsRightTag(s)){ // 若是头标签
                    num++;
                    Node* newnode = CreateNode();
                    newnode->index = num;
                    newnode->data = newnode->data + s + ' '; 
                    newnode->parent = pa; // 他先认了爸
                    if(!pa->firstchild){
                        pa->firstchild = newnode; // 若爸还没儿，他是第一个
                    }
                    else{
                        Node* p = pa->firstchild;
                        while(p->sibling){ // 若爸已有儿，他是哥的弟
                            p = p->sibling;
                        }
                        p->sibling = newnode;
                    }
                    pa = newnode; // 然后他马上当爸
                }

                else if(IsLeftTag(s)){ // 若是尾标签 
                    pa->data = pa->data + s + ' '; // 加入文本中
                    pa = pa->parent; // 归还爸爸称谓
                }

                else{
                    pa->data = pa->data + s + ' '; // 若啥都不是，加入文本中
                }
                
			}
            // 将 html 存进了树中

            // 3、用户发出请求

            int request; // 接下来我有一个小小的请求
            Node* Now = root; // 现在处理的节点的指针（很重要！），初始为根
			cout << "初始化成功，您当前在树根! " << endl;

            while(1){

                cout << "\n您要进行什么操作?" << endl; // 请求表
                cout << "打印完整html----------请打1" << endl;
                cout << "进入某个节点----------请打2" << endl;
                cout << "查看所在节点信息-------请打3" << endl;
                cout << "ID Selector----------请打4" << endl;
                cout << "Class Selector-------请打5" << endl;
                cout << "Tag Selector---------请打6" << endl;
                cout << "Blank Selector-------请打7" << endl;
                cout << "获取当前innerText-----请打8" << endl;
                cout << "获取当前outerText-----请打9" << endl;
                cout << "获取当前href----------请打10" << endl;
                cout << "返回初始状态----------请打11" << endl;
                cout << "前面插入元素----------请打12" << endl;
                cout << "后面插入元素----------请打13" << endl;
                cout << "无子节点元素----------请打14" << endl;
                cout << "寻找某种属性----------请打15" << endl;
                cout << "反选标签元素----------请打16" << endl;
                cout << "退出当前的树----------请打17" << endl;
                cout << "我要: ";
                cin >> request;

                if(request == 1){ // 遍历打印整棵树
                    WholeTree(root);
                }

                else if(request == 2){ // 进入一个节点中
                    while(1){
                        cout << "请输入您要进入哪一个节点: " << endl;
                        int index;
                        cin >> index;
                        if(index > num || index < 0){ // 若序号不合法
                            cout << "序号不合法，请重试！" << endl;
                        }
                        else{
                            GetTreeNode(root, index, Now);
                            cout << "已进入该节点!" << endl;
                            break;
                        }
                    }
                }

                else if(request == 3){ // 看节点简要信息
                    DisplayNode(Now);
                }

                else if(request == 4){ // id 选择器
                    Node* Idlist[100];
                    int Idnum = 0;
                    string MyId;
                    cout << "请输入您要找的id: " ;
                    cin >> MyId;
                    Node *n;
                    for(int i = 0; i < num; i++){
                        GetTreeNode(root, i, n);
                        if(GetId(n) == MyId){
                            Idlist[Idnum] = n;
                            Idnum++;
                        }
                    }
                    if(Idnum == 0) cout << "未找到您要的id! " << endl;
                    else{
                        for(int i = 0; i < Idnum; i++){
                            cout << "第 " << i + 1 << " 个 " << Idlist[i]->data << "序号为 " << Idlist[i]->index << endl;
                        }
                    } 
                }

                // class 选择器 ###################################################################
                else if(request == 5){

                    int subreq;
                    cout << "您想实现哪种选取: " << endl;
                    cout << "选择某些class-----请打1" << endl;
                    cout << "选择后代class-----请打2" << endl;
                    cout << "您的选择是: ";
                    cin >> subreq;

                    if(subreq == 1){
                        Node* clist[100];
                        int num_clist = 0;
                        int n; // class 的种类
                        cout << "请输入您要选取同时含有几种class的节点: ";
                        cin >> n;
                        cout << "\n请输入您要选择的class, 用空格分隔: " << endl;
                        string class_list[n];
                        for(int i = 0; i < n; i++){
                            cin >> class_list[i];
                        }
                        Node* tn;
                        for(int i = 0; i < num; i++){
                            bool all = true;
                            GetTreeNode(root, i, tn);
                            string Class = GetClass(tn); // 对每一个节点的class，只有全包括class表中的才合格
                            for(int j = 0; j < n; j++){
                                if(FindStr(Class, class_list[j]) == -1){ // 若找不到
                                    all = false;
                                    break;
                                }
                            }
                            if(all){
                                clist[num_clist] = tn;
                                num_clist++;
                            }
                        } // 
                        if(num_clist == 0) cout << "没有符合要求的元素! " << endl;
                        else{
                            for(int i = 0; i < num_clist; i++){
                                cout << "第 " << i + 1 << " 个 " << clist[i]->data << "序号为: " << clist[i]->index << endl;
                            }
                            int a;
                            cout << "请输入您要返还第几个节点: ";
                            while(1){
                                cin >> a;
                                if(a > num_clist) cout << "超出范围, 请重试! " << endl;
                                else{
                                    Now = clist[a-1];
                                    cout << "已进入该节点! " << endl;
                                    break;
                                }
                            }      
                        }              
                    }
                    
                    else if(subreq == 2){
                        string class1, class2;
                        cout << "您将选择某种class1元素的后代中, 所有的class2元素! " << endl;
                        cout << "请输入class1: ";
                        cin >> class1;
                        cout << "\n请输入class2: ";
                        cin >> class2;
                        Node *tn;
                        bool find = false;
                        for(int i = 0; i < num; i++){
                            GetTreeNode(root, i, tn);
                            string s = GetClass(tn);
                            if(FindStr(s, class1) != -1){ // 若其class符合条件，找它后代符合条件的class
                                Display_By_Class(tn->firstchild, class2, find);
                                
                            }
                        }
                        if(!find) cout << "没找到符合要求的节点! " << endl;
                        else{
                            cout << "您要返还序号为几的节点: "; // 返回方法2
                            int index;
                            cin >> index;
                            GetTreeNode(root, index, Now);
                            cout << "返回节点成功! " << endl;
                        }
                    }
                }

                // Tag 选择器 #####################################################################

                else if(request == 6){
                    Node* list[100]; // 返还的节点列表，不涉及后代的两个功能都用它
                    bool find = false; // 是否有结果的标志，涉及后代的四个功能都用它
                    int list_num = 0; // 返还的节点数量
                    int subreq;
                    cout << "您要实现哪种选取: " << endl;
                    cout << "选择某些元素-----请打1" << endl;
                    cout << "按class选择元素--请打2" << endl;
                    cout << "选择包含元素-----请打3" << endl;
                    cout << "选择父子元素-----请打4" << endl;
                    cout << "选择前后元素-----请打5" << endl;
                    cout << "选择后续元素-----请打6" << endl;
                    cout << "您要实现的功能为: ";
                    cin >> subreq;

                    // 功能 1：tag
                    if(subreq == 1){
                        int n; // 标签的种类
                        cout << "\n请输入您要选择多少种标签: ";
                        cin >> n;
                        cout << "\n请输入您要选择的标签, 用空格分隔: " << endl;
                        string tag_list[n];
                        for(int i = 0; i < n; i++){
                            cin >> tag_list[i];
                        }
                        for(int i = 0; i < n; i++){ // 对每一个标签，遍历树，若某节点对应该标签，将该节点加入
                            string ts = tag_list[i];
                            bool tagfind = false;
                            for(int j = 1; j <= num; j++){
                                Node* tn;
                                GetTreeNode(root, j, tn);
                                if(ts == GetTag(tn)){ // 若标签匹配
                                    tagfind = true;
                                    list[list_num] = tn;
                                    list_num++;
                                }
                            }
                            if(tagfind == false) cout << "找不到 " << ts << " 标签对应的节点! " << endl; 
                        } // 获得返回节点的列表。

                        if(list_num > 0){
                            cout << "所有符合条件的节点为: " << endl;
                            for(int i  = 0; i < list_num; i++){
                                cout << "第 " << i+1 << " 个 " << list[i]->data << ", 序号为 " << list[i]->index << endl; 
                            }
                            int a;
                            cout << "请输入您要返还第几个节点: ";
                            while(1){
                                cin >> a;
                                if(a > list_num) cout << "超出范围, 请重试! " << endl;
                                else{
                                    Now = list[a-1];
                                    cout << "已进入该节点! " << endl;
                                    break;
                                }
                            } 
                        }
                    } // 只有这个返回格式不同 !

                    // 功能 2：class
                    else if(subreq == 2){
                        string MyTag;
                        string MyClass;
                        cout << "\n将为您返还指定 class 的某 tag 节点! " << endl;
                        cout << "请输入您要选择的tag: ";
                        cin >> MyTag;
                        cout << "\n请输入您要选择的class : ";
                        getline(cin, MyClass);
                        getline(cin, MyClass);
                        for(int j = 1; j <= num; j++){ // 遍历每一个节点
                            Node* tn;
                            GetTreeNode(root, j, tn);
                            string tag = GetTag(tn);
                            string Class = GetClass(tn);
                            if(tag == MyTag && FindStr(Class, MyClass) != -1){ // 若两个条件都满足
                                list[list_num] = tn;
                                list_num++;
                            } 
                        }

                        if(list_num == 0) cout << "找不到符合条件的节点的说……" << endl;
                        else{
                            cout << "所有符合条件的节点为: " << endl;
                            for(int i  = 0; i < list_num; i++){
                                cout << "第 " << i+1 << "个 " << list[i]->data << "序号为" << list[i]->index << endl; 
                            }
                            int a;
                            cout << "请输入您要返还第几个节点: ";
                            while(1){
                                cin >> a;
                                if(a > list_num) cout << "超出范围, 请重试! " << endl;
                                else{
                                    Now = list[a-1];
                                    cout << "已进入该节点! " << endl;
                                    break;
                                }
                            }                            
                        }
                    }

                    else if(subreq == 3){ // 后代
                        string tag1, tag2;
                        cout << "您将选择tag1标签元素后代中,所有tag2元素! " << endl;
                        cout << "请输入tag1: ";
                        cin >> tag1;
                        cout << "\n请输入tag2: ";
                        cin >> tag2;
                        Node* tn;
                        for(int i = 0; i < num; i++){
                            GetTreeNode(root, i, tn);
                            string s = GetTag(tn);
                            if(s == tag1){
                                Display_By_Tag(tn->firstchild, tag2, find);
                            }
                        }
                        if(!find) cout << "没找到符合要求的节点! " << endl;
                        else{
                            cout << "您要返还序号为几的节点: ";
                            int index;
                            cin >> index;
                            GetTreeNode(root, index, Now);
                            cout << "返还节点成功! " << endl;
                        }           
                    }

                    else if(subreq == 4){ // 父子
                        string tag1, tag2;
                        cout << "您将选择tag1标签元素儿子中,所有tag2元素! " << endl;
                        cout << "请输入tag1: ";
                        cin >> tag1;
                        cout << "\n请输入tag2: ";
                        cin >> tag2;
                        Node* tn; // 第一层
                        for(int i = 0; i < num; i++){
                            GetTreeNode(root, i, tn);
                            string s1 = GetTag(tn);
                            if(s1 == tag1){
                                Node* child = tn->firstchild;
                                while(child){
                                    string s2 = GetTag(child); // 得到了它的儿子的标签
                                    if(s2 == tag2){
                                        find = true;
                                        cout << child->data << " 序号为: " << child->index << endl; 
                                    }
                                    child = child->sibling;
                                }
                            }
                        }
                        if(!find) cout << "没找到符合要求的节点! " << endl;
                        else{
                            cout << "您要返还序号为几的节点: ";
                            int index;
                            cin >> index;
                            GetTreeNode(root, index, Now);
                            cout << "返还节点成功! " << endl;
                        }
                        
                    }

                    else if(subreq == 5){ // 紧跟，从兄弟中找！
                        string tag1, tag2;
                        cout << "您将选择紧跟tag1标签元素的首个tag2标签元素! " << endl;
                        cout << "请输入tag1: ";
                        cin >> tag1;
                        cout << "\n请输入tag2: ";
                        cin >> tag2;
                        Node* tn;
                        for(int i = 0; i < num; i++){
                            GetTreeNode(root, i, tn);
                            string s1 = GetTag(tn);
                            if(s1 == tag1){
                                Node* sib = tn->sibling;
                                while(sib){
                                    string s2 = GetTag(sib);
                                    if(s2 == tag2){
                                        find = true;
                                        cout << sib->data << "序号为: " << sib->index << endl;
                                        break; // 找到一个符合条件的兄弟就不再找下一个了
                                    }
                                    sib = sib->sibling;
                                }
                            }
                        }
                        if(!find) cout << "没找到符合要求的节点! " << endl;
                        else{
                            cout << "您要返还序号为几的节点: ";
                            int index;
                            cin >> index;
                            GetTreeNode(root, index, Now);
                            cout << "返还节点成功! " << endl;                           
                        }
                    }

                    else if(subreq == 6){ // 后面，也从兄弟中找，所有满足条件的兄弟都是! 
                        string tag1, tag2;
                        cout << "您将选择在tag1标签元素后的tag2标签元素! " << endl;
                        cout << "请输入tag1: ";
                        cin >> tag1;
                        cout << "\n请输入tag2: ";
                        cin >> tag2;
                        Node* tn;        
                        for(int i = 0; i < num; i++){
                            GetTreeNode(root, i, tn);
                            string s1 = GetTag(tn);
                            if(s1 == tag1){
                                Node* sib = tn->sibling;
                                while(sib){
                                    string s2 = GetTag(sib);
                                    if(s2 == tag2){
                                        find = true; 
                                        cout << sib->data << "序号为: " << sib->index << endl;
                                        // 不 break，继续找即可
                                    }
                                    sib = sib->sibling;
                                }
                            }
                        }
                        if(!find) cout << "没找到符合要求的节点! " << endl;
                        else{
                            cout << "您要返还序号为几的节点: ";
                            int index;
                            cin >> index;
                            GetTreeNode(root, index, Now);
                            cout << "返还节点成功! " << endl;                           
                        }
                    }
                    
                }

                // Tag 选择器 ######################################################################

                else if(request == 8){ // 输出内部文本
                    Node* p = Now;
                    p->firstchild = NULL;
                    p->sibling = NULL;
                    InnerText(p);
                    InnerText(Now->firstchild);
                }
                else if(request == 9){ // 输出外部文本
                    cout << Now->data << endl;
                    OuterText(Now->firstchild);
                }
                else if(request == 10){ // 输出所含链接
                    GetHref(Now);
                }
                else if(request == 11){ // 回到初始状态
                    Now = root;
                    cout << "您已回到根目录！" << endl;
                }
                else if(request == 12 || request == 13){ // 增加元素
                    string target;
                    cout << "请输入您要在哪个标签元素前/后增加新元素: ";
                    cin >> target;
                    Node* i = CreateNode(); // 创建要插入的新节点
                    num++;
                    i->index = num;
                    string text;
                    cout << "\n请输入您的元素的内容: " << endl;
                    getline(cin, text);
                    getline(cin, text);
                    i->data = text;
                    Node* tn;
                    for(int j = 0; j < num - 1; j++){
                        GetTreeNode(root, j, tn);
                        string tag = GetTag(tn);
                        if(tag == target){
                            Node* ptn = tn->parent; // tn之父
                            i->parent = ptn; // 同父母
                            if(request == 12){ // 插在前面, tn可能是大孩也可能不是                               
                                Node* ptnf = ptn->firstchild; // 其父之大儿
                                while(ptnf){
                                    if(ptnf == tn){ // 想要进入这个if，只能tn是大儿
                                        ptn->firstchild = i;
                                        i->sibling = tn;
                                        break;
                                    }
                                    else if(ptnf->sibling == tn){ // 若找到了tn的哥
                                        ptnf->sibling = i;
                                        i->sibling = tn;
                                        break;
                                    }
                                    ptnf = ptnf->sibling;
                                }
                            }
                            if(request == 13){ // 插在后面
                                i->sibling = tn->sibling;
                                tn->sibling = i;
                            }
                        }

                    }
                    cout << "操作成功! " << endl;
                }

                else if(request == 14){ // 无子节点
                    bool find = false;
                    string target;
                    cout << "请输入您要找的无子元素的标签: ";
                    cin >> target;
                    Node* tn;
                    for(int i = 0; i < num; i++){
                        GetTreeNode(root, i, tn);
                        string tag = GetTag(tn);
                        if(tag == target && !tn->firstchild){
                            find = true;
                            cout << tn->data << " 序号为: " << tn->index << endl;
                        }
                    }
                    if(!find) cout << "没有符合要求的节点! " << endl;
                    else{
                        int index;
                        cout << "请输入您要返回的节点序号: " ;
                        cin >> index;
                        GetTreeNode(root, index, Now);
                        cout << "返还节点成功! " << endl;
                    }

                }

                else if(request == 15){ // 寻找某种属性
                    bool find = false;
                    string atr;
                    cout << "请输入您要找的属性: " << endl;
                    cin >> atr;
                    atr = atr + "=\"";
                    Node* tn;
                    for(int i = 0; i < num; i++){
                        GetTreeNode(root, i, tn);
                        string text = tn->data;
                        if(FindStr(text, atr) != -1){
                            find = true;
                            cout << tn->data << "序号为: " << tn->index << endl;
                        }
                    }
                    if(!find) cout << "没有符合要求的节点! " << endl;
                    else{
                        int index;
                        cout << "请输入您要返回的节点序号: " ;
                        cin >> index;
                        GetTreeNode(root, index, Now);
                        cout << "返还节点成功! " << endl;
                    }
                }

                else if(request == 16){ // 反选标签
                    bool find = false;
                    string target;
                    cout << "请输入您不想找的元素的标签: ";
                    cin >> target;
                    Node* tn;
                    for(int i = 0; i < num; i++){
                        GetTreeNode(root, i, tn);
                        string tag = GetTag(tn);
                        if(tag != target){
                            find = true;
                            cout << tn->data << " 序号为: " << tn->index << endl;
                        }
                    }
                    if(!find) cout << "没有符合要求的节点! " << endl;
                    else{
                        int index;
                        cout << "请输入您要返回的节点序号: " ;
                        cin >> index;
                        GetTreeNode(root, index, Now);
                        cout << "返还节点成功! " << endl;
                    }

                }

                else if(request == 17){ // 退出当前文本
                    break;
                }

                else{
                    cout << "请输入1~17之间的整数! " << endl;
                }
            }	 
        }
    }
    return 0;
}
