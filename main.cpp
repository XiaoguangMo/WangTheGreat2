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
int slabProportion;

struct treeNode{
//    struct treeNode *left;
//    struct treeNode *right;
//    struct treeNode *father;
    int pid,size,space;
};
//treeNode *root=new treeNode;
int convertToInt(string temp);
int convertToTimesOfTwo(int temp);
void printTree(treeNode *node);
treeNode * CreateTree(treeNode *bTree,treeNode *father,int pid,int size,int space,int layer);
void insertNode(treeNode *father, int pid, int size);
void printTree(treeNode *node);


int main(int argc, const char * argv[]) {
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
//                cout<<"total memory size is 2^ "<<log2(memorySize)<<endl;
//                cout<<"test: insert 255: "<<log2(255)<<" qu zheng: "<<ceil(log2(255))<<endl;
            }else if(line.substr(0,9)=="slabSize(") {
                int find=0;
                for (find=0; find<line.length(); find++) {
                    if (line[find]=='B') {
                        slabSize=convertToInt(line.substr(9,find-8));
//                        cout<<"total slab size is 2^ "<<log2(slabSize)<<endl;
                    }
                    if (line[find]==',') {
                        if (line[find+1]=='1'&&line[find+2]=='/') {
                            string temp=line.substr(find+3);
                            temp=temp.substr(0,temp.length()-2);
                            int temp2=atoi(temp.c_str());
                            slabProportion=temp2;
//                            cout<<"slab start from layer: "<<log2(memorySize/temp2)<<" temp2 is: "<<slabProportion<<endl;
                        }
                        else{
                            cout<<"Invaliad input"<<endl;
                            exit(0);
                        }
                        
                    }
                }
            }
        }
    }
    file.close();
    ifstream file2;
    file2.open(loc);
    string line2;
    struct treeNode{
        int pid,size;
    }treeNode[memorySize/slabSize*2];
    for (int i=0; i<memorySize/slabSize*2; i++) {
        treeNode[i].pid=0;
        treeNode[i].size=0;
    }
    treeNode[slabProportion-1].pid=-1;
    while (getline(file2, line2)) {
        if (line2.length()>0) {
            for (int i=0; i<line2.length(); i++) {
                if (line2[i]==' ') {
                    line2.erase(i,1);
                    i-=1;
                }
            }
            if(line2.substr(0,6)=="alloc("){
                string temp=line2.substr(6,line2.length()-6);
                string temp2;
                temp=temp.substr(0,temp.length()-2);
                for (int i=0; i<temp.length(); i++) {
                    if (temp[i]==',') {
                        temp2=temp.substr(i+1);
                        temp=temp.substr(0,i);
                    }
                }
                if (convertToInt(temp2)<=slabSize&&convertToInt(temp2)>slabSize/2) {
//                    cout<<convertToInt(temp2)<<" should be put into the slab"<<endl;
                    int a=pow(2,ceil(log2(convertToInt(temp2))));
//                    cout<<log2(memorySize/slabSize/2)<<endl;
//                    cout<<memorySize/slabSize*2<<" and "<<memorySize/a-1<<endl;
                    int flag=0;
                    for (int i=memorySize/a-1;i<memorySize/a-1+memorySize/a/slabProportion ; i++) {
                        if (treeNode[i].pid==0&&flag==0) {
                            flag=1;
                            temp2=temp2.substr(0,temp2.length()-2);
//                            cout<<"pid="<<temp<<" size="<<temp2<<endl;
                            treeNode[i].pid=atoi(temp.c_str());
                            treeNode[i].size=convertToInt(temp2);
//                            cout<<"pid="<<treeNode[i].pid<<" size="<<treeNode[i].size<<endl;
                        }
                    }
                    if (flag==0) {
                        //start compation
                        
                    }
//                    cout<<a<<endl;
                }else{
//                    cout<<convertToInt(temp2)<<" should be put into the buddy"<<endl;
                    if (convertToInt(temp2)>slabSize) {
                        int a=pow(2,ceil(log2(convertToInt(temp2))));
                        //                    cout<<log2(memorySize/slabSize/2)<<endl;
                        //                    cout<<memorySize/slabSize*2<<" and "<<memorySize/a-1<<endl;
                        int flag=0;
//                        cout<<"test"<<memorySize/a+memorySize/a/slabProportion<<"test"<<memorySize/a+memorySize/a-2<<endl;
                        for (int i=memorySize/a+memorySize/a/slabProportion;i<memorySize/a+memorySize/a-2; i++) {

                            if (treeNode[i].pid==0&&flag==0) {
                                flag=1;
                                temp2=temp2.substr(0,temp2.length()-2);
                                //                            cout<<"pid="<<temp<<" size="<<temp2<<endl;
                                
                                treeNode[i].pid=atoi(temp.c_str());
                                treeNode[i].size=convertToInt(temp2);
                                //                            cout<<"pid="<<treeNode[i].pid<<" size="<<treeNode[i].size<<endl;
                            }
                        }
                        //                    cout<<a<<endl;
                    }else{
                        //zui nan chu li
                        
                    }
                }
                //                cout<<"insert "<<temp<<"  "<<temp2<<endl;
            }else if (line2.substr(0,5)=="free("){
                string temp=line2.substr(5,line2.length()-7);
//                cout<<"free!!!!!!"<<temp<<endl;
                for (int i=0; i<memorySize/slabSize*2; i++) {
                    if (treeNode[i].pid==atoi(temp.c_str())) {
                        treeNode[i].pid=0;
                        treeNode[i].size=0;
                    }
                }
            }else if (line2.substr(0,8)=="realloc("){
                string temp=line2.substr(8,line2.length()-10);
                string temp2;
                for (int i=0; i<temp.length(); i++) {
                    if (temp[i]==',') {
                        temp2=temp.substr(i+1);
                        temp=temp.substr(0,i);
                    }
                }
                for (int i=0; i<memorySize/slabSize*2; i++) {
                    if (treeNode[i].pid==atoi(temp.c_str())) {
                        treeNode[i].pid=0;
                        treeNode[i].size=0;
                    }
                }
                if (convertToInt(temp2)<=slabSize&&convertToInt(temp2)>slabSize/2) {
                    //                    cout<<convertToInt(temp2)<<" should be put into the slab"<<endl;
                    int a=pow(2,ceil(log2(convertToInt(temp2))));
                    //                    cout<<log2(memorySize/slabSize/2)<<endl;
                    //                    cout<<memorySize/slabSize*2<<" and "<<memorySize/a-1<<endl;
                    int flag=0;
                    for (int i=memorySize/a-1;i<memorySize/a-1+memorySize/a/slabProportion ; i++) {
                        if (treeNode[i].pid==0&&flag==0) {
                            flag=1;
                            temp2=temp2.substr(0,temp2.length()-2);
                            //                            cout<<"pid="<<temp<<" size="<<temp2<<endl;
                            
                            treeNode[i].pid=atoi(temp.c_str());
                            treeNode[i].size=convertToInt(temp2);
                            //                            cout<<"pid="<<treeNode[i].pid<<" size="<<treeNode[i].size<<endl;
                        }
                    }
                    //                    cout<<a<<endl;
                }else{
                    cout<<convertToInt(temp2)<<" should be put into the buddy"<<endl;
                }
//                                cout<<"realloc!!!!!!"<<temp<<"second part "<<temp2<<endl;
            }else if(line2.substr(0,7)=="dump();"){
//                cout<<"this is a dump"<<endl;
                cout<<"slab:"<<endl;
                for (int i=; <#condition#>; <#increment#>) {
                    for (<#initialization#>; <#condition#>; <#increment#>) {
                        <#statements#>
                    }
                }
                for (int i=0; i<memorySize/slabSize*2; i++) {
                    if (treeNode[i].pid!=0) {
                        cout<<"pid="<<treeNode[i].pid<<" size="<<treeNode[i].size<<endl;
                    }
                    
                }
                cout<<"buddy"<<endl;
                
            }
        else
            {
//                cout<<line2<<endl;
            }
        }
    }
    file2.close();
    //finish reading input file, start build binary tree
    return 0;
}

//treeNode * CreateTree(treeNode *bTree,treeNode *father,int pid,int size,int space,int layer)
//{
//    cout<<"found root's left"<<endl;
//    father->left=bTree;
//    bTree=father->left;
//    bTree = (treeNode *)malloc(sizeof(treeNode));
//    bTree->pid=pid;
//    bTree->size=size;
//    bTree->space=space;
//    bTree->father=father;
//    bTree->left=NULL;
//    bTree->right=NULL;
//    layer--;
//    if(layer == 0)
//    {
//        bTree = NULL;	//置为NULL后结束
//        return bTree;
//    }
//    //    bTree ->m_nValue = input;
//    if (bTree->space>0) {
////        bTree->left = CreateTree(bTree->left);
//    }
//    else{
////        bTree->right = CreateTree(bTree->right);
//    }
//    
//    return bTree;
//}
//
//// insert node into tree
//void insertNode(treeNode *father, int pid, int size)
//{
//    cout<<"insert called"<<endl;
//    treeNode *node;
//    node=CreateTree(node, father, pid, size, 123, 123);
////    treeNode *newNode = new treeNode;
////    newNode->left=newNode->right=NULL;
////    newNode->pid=pid;
////    newNode->size=size;
////    newNode->father=father;
////    int whichLayer=log2(memorySize)-ceil(log2(size));
////    //    whichLayer=log2(memorySize)-whichLayer;
////    //    cout<<"which layer: "<<whichLayer<<endl;
////    if (father==NULL) {
////        cout<<"Return a null pointer."<<endl;
////        exit(0);
////    }
////    //start to deal with size
////    //    cout<<"node's size is: "<<newNode->size<<" pid is: "<<newNode->pid<<endl;
////    //    cout<<"slab size is: "<<slabSize<<endl;
////    
////    
////    //    treeNode *back=nullptr;
////    treeNode *current=father;
////    if (pid==-1) {
////        while (1) {
////            if (whichLayer>0) {
////                //                treeNode *node=new treeNode;
////                //                node=current->left;
////                //                current=node;
////                //                whichLayer--;
////            }
////        }
////    }
////    else{
////        while (current!=NULL) {
////            //        back=current;
////            if(size<father->size) {
////                //            current=current->left;
////                //            father->left=newNode;
////                //            cout<<"hello"<<endl;
////            }else{
////                //            current->right=current;
////                //            father->right=newNode;
////                current=current->right;
////            }
////        }
////    }
//} // end function insertNode
//
//void printTree(treeNode *node)      //这是中序遍历二叉树，采用了递归的方法。
//{
//    if(node!=NULL)
//    {
//        cout<<"pid is: "<<node->pid<<" size is: "<<node->size<<endl;
//        printTree(node->left);
//        printTree(node->right);
//    }
//}

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

int convertToTimesOfTwo(int temp){
    return ceil(log2(temp));
}
