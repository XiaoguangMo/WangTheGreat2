//
//  main.cpp
//  WangTheGreat2
//
//  Created by NNNO on 10/9/14.
//  Copyright (c) 2014 NNNO. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <list>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

using namespace::std;
string loc="/Users/NNNO/Desktop/WangTheGreat2/WangTheGreat2/test.txt";

int memorySize;
int slabSize;

struct treeNode{
    struct treeNode *left;
    struct treeNode *right;
    struct treeNode *father;
    int pid,size,space;
};
//treeNode root = *new treeNode;
int convertToInt(string temp);
void printTree(treeNode *node);
treeNode * CreateTree(treeNode *bTree,treeNode *father,int pid,int size,int space,int layer);
void insertNode(treeNode *father, int pid, int size);
void printTree(treeNode *node);


int main(int argc, const char * argv[]) {
    treeNode *root = new treeNode;
//    treeNode *startRoot=new treeNode;
//    startRoot->space=0;
//    startRoot->size=0;
//    root=*startRoot;
    treeNode *slabRoot = new treeNode;
    ifstream file;
    file.open(loc);
    string line;
    while (getline(file, line)) {
        if (line.length()>0) {
            for (int i=0; i<line.length(); i++) {
                if (line[i]==' ') {
                    line.erase(i,1);
                    i-=1;
                }
            }
            if (line.substr(0,11)=="memorySize(") {
                memorySize=convertToInt(line.substr(11,line.length()-13));
                cout<<"total memory size is 2^ "<<log2(memorySize)<<endl;
//                cout<<"test: insert 255: "<<log2(255)<<" qu zheng: "<<ceil(log2(255))<<endl;
            }else if(line.substr(0,9)=="slabSize(") {
                int find=0;
                for (find=0; find<line.length(); find++) {
                    if (line[find]=='B') {
                        slabSize=convertToInt(line.substr(9,find-8));
                        cout<<"total slab size is 2^ "<<log2(slabSize)<<endl;
                    }
                    if (line[find]==',') {
                        if (line[find+1]=='1'&&line[find+2]=='/') {
                            string temp=line.substr(find+3);
                            temp=temp.substr(0,temp.length()-2);
                            int temp2=atoi(temp.c_str());
                            cout<<"slab start from layer: "<<memorySize/temp2<<endl;
                            insertNode(root, -1, memorySize/temp2);
                        }
                        else{
                            cout<<"Invaliad input"<<endl;
                            exit(0);
                        }
                        
                    }
                }
                //insert slabroot
                insertNode(root, -1, 0);
            }else if(line.substr(0,6)=="alloc("){
                string temp=line.substr(6,line.length()-6);
                string temp2;
                temp=temp.substr(0,temp.length()-2);
                for (int i=0; i<temp.length(); i++) {
                    if (temp[i]==',') {
                        temp2=temp.substr(i+1);
                        temp=temp.substr(0,i);
                    }
                }
                if (convertToInt(temp2)<=slabSize&&convertToInt(temp2)>slabSize/2) {
                    cout<<convertToInt(temp2)<<" should be put into the slab"<<endl;
                    insertNode(slabRoot, atoi(temp.c_str()), convertToInt(temp2));
                }else{
                    cout<<convertToInt(temp2)<<" should be put into the buddy"<<endl;
                    insertNode(root, atoi(temp.c_str()), convertToInt(temp2));
                }
//                insertNode(&root, atoi(temp.c_str()), convertToInt(temp2));
//                cout<<"insert "<<temp<<"  "<<temp2<<endl;
            }else if(line.substr(0,7)=="dump();"){
                cout<<"this is a dump"<<endl;
//                insertNode(&root, 123, 321);
//                insertNode(&root, 123, 321);
//                insertNode(&root, 123, 321);
//                insertNode(&root, 123, 321);
//                insertNode(root, 123, 33);
//                insertNode(root, slabRoot, -1, -1);
                printTree(root);
            }else if (line.substr(0,5)=="free("){
                cout<<"this is a free"<<endl;
            }else
            {
                cout<<line<<endl;
            }
        }
    }
    file.close();
    //finish reading input file, start build binary tree
//    insertNode(NULL, root, 123, 321);
//    insertNode(root, slabRoot, -1, -1);
    
    return 0;
}

treeNode * CreateTree(treeNode *bTree,treeNode *father,int pid,int size,int space,int layer)
{
    bTree = (treeNode *)malloc(sizeof(treeNode));
    bTree->pid=pid;
    bTree->size=size;
    bTree->space=space;
    bTree->father=father;
    layer--;
    if(layer == 0)
    {
        bTree = NULL;	//置为NULL后结束
        return bTree;
    }
    //    bTree ->m_nValue = input;
    if (bTree->space>0) {
//        bTree->left = CreateTree(bTree->left);
    }
    else{
//        bTree->right = CreateTree(bTree->right);
    }
    
    return bTree;
}

// insert node into tree
void insertNode(treeNode *father, int pid, int size)
{
    
//    treeNode *newNode = new treeNode;
//    newNode->left=newNode->right=NULL;
//    newNode->pid=pid;
//    newNode->size=size;
//    newNode->father=father;
//    int whichLayer=log2(memorySize)-ceil(log2(size));
//    //    whichLayer=log2(memorySize)-whichLayer;
//    //    cout<<"which layer: "<<whichLayer<<endl;
//    if (father==NULL) {
//        cout<<"Return a null pointer."<<endl;
//        exit(0);
//    }
//    //start to deal with size
//    //    cout<<"node's size is: "<<newNode->size<<" pid is: "<<newNode->pid<<endl;
//    //    cout<<"slab size is: "<<slabSize<<endl;
//    
//    
//    //    treeNode *back=nullptr;
//    treeNode *current=father;
//    if (pid==-1) {
//        while (1) {
//            if (whichLayer>0) {
//                //                treeNode *node=new treeNode;
//                //                node=current->left;
//                //                current=node;
//                //                whichLayer--;
//            }
//        }
//    }
//    else{
//        while (current!=NULL) {
//            //        back=current;
//            if(size<father->size) {
//                //            current=current->left;
//                //            father->left=newNode;
//                //            cout<<"hello"<<endl;
//            }else{
//                //            current->right=current;
//                //            father->right=newNode;
//                current=current->right;
//            }
//        }
//    }
} // end function insertNode

void printTree(treeNode *node)      //这是中序遍历二叉树，采用了递归的方法。
{
    if(node!=NULL)
    {
        printTree(node->left);
        cout<<"pid is: "<<node->pid<<" size is: "<<node->size<<endl;
        printTree(node->right);
    }
}

int convertToInt(string temp){
    int result;
    temp=temp.substr(0,temp.length()-1);
    if (temp[temp.length()-1]=='G') {
        temp=temp.substr(0,temp.length()-1);
        result=atoi(temp.c_str());
        result=result*1024*1024;
    }else if (temp[temp.length()-1]=='T') {
        temp=temp.substr(0,temp.length()-1);
        result=atoi(temp.c_str());
        result=result*1024*1024*1024;
    }else if (temp[temp.length()-1]=='M') {
        temp=temp.substr(0,temp.length()-1);
        result=atoi(temp.c_str());
        result=result*1024;
    }else if (temp[temp.length()-1]=='K') {
        temp=temp.substr(0,temp.length()-1);
        result=atoi(temp.c_str());
        result=result;
    }
    return result;
}

