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
                    int a=pow(2,ceil(log2(convertToInt(temp2))));
                    int flag=0;
                    for (int i=memorySize/a-1;i<memorySize/a-1+memorySize/a/slabProportion ; i++) {
                        if (treeNode[i].pid==0&&flag==0) {
                            flag=1;
                            temp2=temp2.substr(0,temp2.length()-2);
                            
                            treeNode[i].pid=atoi(temp.c_str());
                            treeNode[i].size=convertToInt(temp2);
                        }
                    }
                }else{
                    cout<<convertToInt(temp2)<<" should be put into the buddy"<<endl;
                }
            }else if(line2.substr(0,7)=="dump();"){
                cout<<"slab:"<<endl;
                for (int i=log2(memorySize/slabSize); i>log2(slabProportion); i--) {
                    int flag=0;
                    char temp[i];
                    for (int j=(pow(2, i))-1; j<(pow(2, i)-1+pow(2, i+1)/slabProportion); j++) {
                        if (treeNode[j].pid!=0&&treeNode[j].pid!=-1) {
                            flag=1;
                            for (int k=0;k<i;k++) {
                                temp[k]='0';
                            }
                            for (int k=i-1; k>=0; k--) {
                                int temp2=j+2-pow(2, floor(log2(j+1)));
                                int temp3=pow(2, k+1);
                                if (temp2%temp3==0) {
                                    temp[i-1-k]='1';
                                }
                            }
                            for (int l=0; l<i; l++) {
                                cout<<temp[l];
                            }
                            cout<<" "<<treeNode[j].pid<<endl;
                        }
                    }
                    if (flag==0) {
                        for (int r=0; r<i; r++) {
                            if (r==i-1) {
                                temp[r]='1';
                            }
                            else{
                                temp[r]='0';
                            }
                            cout<<temp[r];
                        }
                        cout<<" free"<<endl;
                    }
                }
                int flagIntoSlab=0;
                cout<<"buddy"<<endl;
                for (int i=log2(memorySize/slabSize); i>0; i--) {
                    
                    int flag=0;
                    char temp[i];
                    for (int j=(pow(2, i)-1+pow(2, i+1)/slabProportion); j<pow(2, i+1)-2; j++) {
                        if (treeNode[j].pid!=0&&treeNode[j].pid!=-1) {
                            flag=1;
                            if (j>flagIntoSlab) {
                                flagIntoSlab=j;
                            }
                            for (int k=0;k<i;k++) {
                                temp[k]='0';
                            }
                            for (int k=i-1; k>=0; k--) {
                                int temp2=j+2-pow(2, floor(log2(j+1)));
                                int temp3=pow(2, k+1);
                                if (temp2%temp3==0) {
                                    temp[i-1-k]='1';
                                }
                            }
                            for (int l=0; l<i; l++) {
                                cout<<temp[l];
                            }
                            cout<<" "<<treeNode[j].pid<<endl;
                        }
                    }
                    if ((flag==0&&pow(2, i)<flagIntoSlab)||(flag==0&&pow(2, i)-1<slabProportion)) {
                        for (int r=0; r<i; r++) {
                            if (r==i-1) {
                                temp[r]='1';
                            }
                            else{
                                temp[r]='0';
                            }
                            cout<<temp[r];
                        }
                        cout<<" free"<<endl;
                    }
                }
            }
        else
            {
//                cout<<line2<<endl;
            }
        }
    }
    file2.close();
    return 0;
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

int convertToTimesOfTwo(int temp){
    return ceil(log2(temp));
}
