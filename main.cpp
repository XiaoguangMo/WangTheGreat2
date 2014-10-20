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
//string loc="/Users/NNNO/Desktop/WangTheGreat2/WangTheGreat2/test.txt";
string loc;

int memorySize;
int slabSize;
int slabProportion;

struct treeNode{
    int pid,size,space;
};
int convertToInt(string temp);
int convertToTimesOfTwo(int temp);
void printTree(treeNode *node);

int main(int argc, const char * argv[]) {
    cout<<"This program is written by Xiaoguang Mo, Oct 19 2014\nUsing array to implement. Please never try to understand the complicated logic\nCompaction not completed.\nInput the file location to start:"<<endl;
    cin>>loc;
    ifstream file;
    file.open(loc.c_str());
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
            }else if(line.substr(0,9)=="slabSize(") {
                int find=0;
                for (find=0; find<line.length(); find++) {
                    if (line[find]=='B') {
                        slabSize=convertToInt(line.substr(9,find-8));
                    }
                    if (line[find]==',') {
                        if (line[find+1]=='1'&&line[find+2]=='/') {
                            string temp=line.substr(find+3);
                            temp=temp.substr(0,temp.length()-2);
                            int temp2=atoi(temp.c_str());
                            slabProportion=temp2;
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
    file2.open(loc.c_str());
    string line2;
    struct treeNode{
        int pid,size,space;
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
                //check for size
                if (convertToInt(temp2)<=0) {
                    cout<<"The size to be alloc is illegal:"<<temp2<<endl;
                    exit(0);
                }
                //check for pid
                int exitFlag=0;
                for (int i=0; i<memorySize/slabSize*2; i++) {
                    if (treeNode[i].pid==atoi(temp.c_str())) {
                        cout<<"Inputed pid is already exist!"<<endl;
                        exitFlag=1;
                    }
                }
                if (exitFlag==1) {
                    continue;
                }
                //start alloc
                if (convertToInt(temp2)<=slabSize&&convertToInt(temp2)>slabSize/2) {
                    int a=pow(2,ceil(log2(convertToInt(temp2))));
                    int flag=0;
                    int flagFather=0;
                    int flagChild=0;
                    for (int i=memorySize/a-1;i<memorySize/a-1+memorySize/a/slabProportion ; i++) {
                        if (treeNode[i].pid==0&&flag==0) {
                            int layer=floor(log2(i));
                            while (1) {
//                                cout<<"hello"<<layer<<endl;
                                layer=(layer-1)/2;
//                                cout<<layer<<endl;
                                if (treeNode[layer].pid!=0) {
                                    flagFather=1;
//                                    cout<<"flagfather"<<endl;
                                }
                                if (layer==0) {
                                    break;
                                }
                                
                            }
//                            cout<<"flagfather is"<<flagFather<<endl;
                            for (int i=0; i<memorySize/slabSize*2; i++) {
                                int layer2=floor(log2(i));
//                                cout<<layer2<<endl;
                                layer2=(layer2-1)/2;
                                if (treeNode[i].pid!=0) {
                                    while (1) {
                                        layer2=(layer-1)/2;
                                        //                                cout<<layer<<endl;
                                        if (layer2==1) {
                                            flagChild=1;
                                            //                                    cout<<"flagfather"<<endl;
                                        }
                                        if (layer2==0) {
                                            break;
                                        }
                                        
                                    }
                                }
                                
                            }
                            flag=1;
                            temp2=temp2.substr(0,temp2.length()-2);
                            
                            
                            //start compation
                            
                            if (flagChild==0&&flagFather==0) {
                                //                            cout<<"pid="<<temp<<" size="<<temp2<<endl;
                                treeNode[i].pid=atoi(temp.c_str());
                                treeNode[i].size=convertToInt(temp2);
                                //                            cout<<"pid="<<treeNode[i].pid<<" size="<<treeNode[i].size<<endl;
                            }
                            
                        }
                    }
                    if (flag==0) {
                        //start compation
                        
                    }
//                    cout<<a<<endl;
                }else{
                    //buddy buddy buddy buddybuddybuddybuddy
//                    cout<<convertToInt(temp2)<<" should be put into the buddy"<<endl;
                    if (convertToInt(temp2)>slabSize) {
                        int a=pow(2,ceil(log2(convertToInt(temp2))));
                        //                    cout<<log2(memorySize/slabSize/2)<<endl;
                        //                    cout<<memorySize/slabSize*2<<" and "<<memorySize/a-1<<endl;
                        int flag=0;
//                        cout<<"test"<<memorySize/a+memorySize/a/slabProportion<<"test"<<memorySize/a+memorySize/a-2<<endl;
                        int flagFather=0,flagChild=0;
                        //38 mei jin zhe li
                        for (int i=memorySize/a+memorySize/a/slabProportion;i<=memorySize/a+memorySize/a-2; i++) {
//                            if (temp=="44") {
//                                cout<<i<<endl;
//                            }
//                            cout<<i<<endl;
//                            cout<<a<<endl;
//                            if (i==5) {
//                                cout<<"hello"<<endl;
//                            }
                            flagFather=0;
                            flagChild=0;
                            if (treeNode[i].pid==0&&flag==0) {
                                int layer=i;
                                while (1){
//                                                                    cout<<"hello"<<layer<<endl;
                                    layer=(layer-1)/2;
                                    //                                cout<<layer<<endl;
                                    if (treeNode[layer].pid!=0) {
                                        flagFather=1;
                                        //                                    cout<<"flagfather"<<endl;
                                    }
                                    if (layer==0) {
                                        break;
                                    }
                                    
                                }
                                //                            cout<<"flagfather is"<<flagFather<<endl;
                                //                            int layer2=i+1;
                                for (int j=0; j<memorySize/slabSize*2; j++) {
                                    int layer2=j;
//                                    cout<<layer2<<endl;
//                                    layer2=(layer2-1)/2;
//                                    if (temp=="34") {
//                                        cout<<temp<<endl;
//                                        cout<<treeNode[11].pid<<endl;
//                                    }
                                    if (treeNode[5].pid!=0) {
//                                        cout<<"hello"<<endl;
                                    }
                                    if (treeNode[j].pid!=0) {
                                        
                                        while (1) {
                                            layer2=(layer2-1)/2;
                                            //                                cout<<layer<<endl;
//                                            cout<<treeNode[5].pid<<endl;
//                                            if (treeNode[layer2].pid==31) {
//                                                cout<<"hello"<<endl;
//                                            }
//                                            if (i==5) {
//                                                cout<<"layer2:"<<layer2<<" i="<<i<<endl;
//
//                                            }
                                            if (layer2==i) {
//                                                cout<<"hello"<<endl;
                                                flagChild=1;
                                                //                                    cout<<"flagfather"<<endl;
                                            }
                                            if (layer2==0) {
                                                break;
                                            }
                                            
                                        }
                                    }
                                    
                                }
                                temp2=temp2.substr(0,temp2.length()-2);
//                                cout<<"hello"<<flagChild<<flagFather<<endl;
                                //                            cout<<"pid="<<temp<<" size="<<temp2<<endl;
//                                cout<<"temp:"<<temp<<flagFather<<flagChild<<endl;
                                if (flagChild==0&&flagFather==0) {
                                    //                            cout<<"pid="<<temp<<" size="<<temp2<<endl;
                                    flag=1;

                                    treeNode[i].pid=atoi(temp.c_str());
//                                    cout<<temp.c_str()<<endl;
                                    treeNode[i].size=convertToInt(temp2);
                                }
                                
                                //                            cout<<"pid="<<treeNode[i].pid<<" size="<<treeNode[i].size<<endl;
                            }
                        }
                        if (flag==0) {
//                            cout<<temp<<"fail to alloc"<<endl;
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
                int freeFlag=0;
                for (int i=0; i<memorySize/slabSize*2; i++) {
                    if (treeNode[i].pid==atoi(temp.c_str())) {
                        treeNode[i].pid=0;
                        treeNode[i].size=0;
                        freeFlag=1;
                    }
                }
                if (freeFlag==0) {
                    cout<<"Can't find a process with pid:"<<temp<<endl;
                    continue;
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
                //free
                int freeFlag=0;
                for (int i=0; i<memorySize/slabSize*2; i++) {
                    if (treeNode[i].pid==atoi(temp.c_str())) {
                        treeNode[i].pid=0;
                        treeNode[i].size=0;
                        freeFlag=1;
                    }
                }
                if (freeFlag==0) {
                    cout<<"Can't find a process with pid:"<<temp<<endl;
                    continue;
                }
                //alloc
                if (convertToInt(temp2)<=0) {
                    cout<<"The size to be alloc is illegal:"<<temp2<<endl;
                    exit(0);
                }
                //check for pid
                int exitFlag=0;
                for (int i=0; i<memorySize/slabSize*2; i++) {
                    if (treeNode[i].pid==atoi(temp.c_str())) {
                        cout<<"Inputed pid is already exist!"<<endl;
                        exitFlag=1;
                    }
                }
                if (exitFlag==1) {
                    continue;
                }
                //start alloc
                if (convertToInt(temp2)<=slabSize&&convertToInt(temp2)>slabSize/2) {
                    int a=pow(2,ceil(log2(convertToInt(temp2))));
                    int flag=0;
                    int flagFather=0;
                    int flagChild=0;
                    for (int i=memorySize/a-1;i<memorySize/a-1+memorySize/a/slabProportion ; i++) {
                        if (treeNode[i].pid==0&&flag==0) {
                            int layer=floor(log2(i));
                            while (1) {
                                //                                cout<<"hello"<<layer<<endl;
                                layer=(layer-1)/2;
                                //                                cout<<layer<<endl;
                                if (treeNode[layer].pid!=0) {
                                    flagFather=1;
                                    //                                    cout<<"flagfather"<<endl;
                                }
                                if (layer==0) {
                                    break;
                                }
                                
                            }
                            //                            cout<<"flagfather is"<<flagFather<<endl;
                            for (int i=0; i<memorySize/slabSize*2; i++) {
                                int layer2=floor(log2(i));
                                //                                cout<<layer2<<endl;
                                layer2=(layer2-1)/2;
                                if (treeNode[i].pid!=0) {
                                    while (1) {
                                        layer2=(layer-1)/2;
                                        //                                cout<<layer<<endl;
                                        if (layer2==1) {
                                            flagChild=1;
                                            //                                    cout<<"flagfather"<<endl;
                                        }
                                        if (layer2==0) {
                                            break;
                                        }
                                        
                                    }
                                }
                                
                            }
                            flag=1;
                            temp2=temp2.substr(0,temp2.length()-2);
                            
                            
                            //start compation
                            
                            if (flagChild==0&&flagFather==0) {
                                //                            cout<<"pid="<<temp<<" size="<<temp2<<endl;
                                treeNode[i].pid=atoi(temp.c_str());
                                treeNode[i].size=convertToInt(temp2);
                                //                            cout<<"pid="<<treeNode[i].pid<<" size="<<treeNode[i].size<<endl;
                            }
                            
                        }
                    }
                    if (flag==0) {
                        //start compation
                        
                    }
                    //                    cout<<a<<endl;
                }else{
                    //buddy buddy buddy buddybuddybuddybuddy
                    //                    cout<<convertToInt(temp2)<<" should be put into the buddy"<<endl;
                    if (convertToInt(temp2)>slabSize) {
                        int a=pow(2,ceil(log2(convertToInt(temp2))));
                        //                    cout<<log2(memorySize/slabSize/2)<<endl;
                        //                    cout<<memorySize/slabSize*2<<" and "<<memorySize/a-1<<endl;
                        int flag=0;
                        //                        cout<<"test"<<memorySize/a+memorySize/a/slabProportion<<"test"<<memorySize/a+memorySize/a-2<<endl;
                        int flagFather=0,flagChild=0;
                        //38 mei jin zhe li
                        for (int i=memorySize/a+memorySize/a/slabProportion;i<=memorySize/a+memorySize/a-2; i++) {
                            //                            if (temp=="44") {
                            //                                cout<<i<<endl;
                            //                            }
                            //                            cout<<i<<endl;
                            //                            cout<<a<<endl;
                            //                            if (i==5) {
                            //                                cout<<"hello"<<endl;
                            //                            }
                            flagFather=0;
                            flagChild=0;
                            if (treeNode[i].pid==0&&flag==0) {
                                int layer=i;
                                while (1){
                                    //                                                                    cout<<"hello"<<layer<<endl;
                                    layer=(layer-1)/2;
                                    //                                cout<<layer<<endl;
                                    if (treeNode[layer].pid!=0) {
                                        flagFather=1;
                                        //                                    cout<<"flagfather"<<endl;
                                    }
                                    if (layer==0) {
                                        break;
                                    }
                                    
                                }
                                //                            cout<<"flagfather is"<<flagFather<<endl;
                                //                            int layer2=i+1;
                                for (int j=0; j<memorySize/slabSize*2; j++) {
                                    int layer2=j;
                                    //                                    cout<<layer2<<endl;
                                    //                                    layer2=(layer2-1)/2;
                                    //                                    if (temp=="34") {
                                    //                                        cout<<temp<<endl;
                                    //                                        cout<<treeNode[11].pid<<endl;
                                    //                                    }
                                    if (treeNode[5].pid!=0) {
                                        //                                        cout<<"hello"<<endl;
                                    }
                                    if (treeNode[j].pid!=0) {
                                        
                                        while (1) {
                                            layer2=(layer2-1)/2;
                                            //                                cout<<layer<<endl;
                                            //                                            cout<<treeNode[5].pid<<endl;
                                            //                                            if (treeNode[layer2].pid==31) {
                                            //                                                cout<<"hello"<<endl;
                                            //                                            }
                                            //                                            if (i==5) {
                                            //                                                cout<<"layer2:"<<layer2<<" i="<<i<<endl;
                                            //
                                            //                                            }
                                            if (layer2==i) {
                                                //                                                cout<<"hello"<<endl;
                                                flagChild=1;
                                                //                                    cout<<"flagfather"<<endl;
                                            }
                                            if (layer2==0) {
                                                break;
                                            }
                                            
                                        }
                                    }
                                    
                                }
                                temp2=temp2.substr(0,temp2.length()-2);
                                //                                cout<<"hello"<<flagChild<<flagFather<<endl;
                                //                            cout<<"pid="<<temp<<" size="<<temp2<<endl;
                                //                                cout<<"temp:"<<temp<<flagFather<<flagChild<<endl;
                                if (flagChild==0&&flagFather==0) {
                                    //                            cout<<"pid="<<temp<<" size="<<temp2<<endl;
                                    flag=1;
                                    
                                    treeNode[i].pid=atoi(temp.c_str());
                                    //                                    cout<<temp.c_str()<<endl;
                                    treeNode[i].size=convertToInt(temp2);
                                }
                                
                                //                            cout<<"pid="<<treeNode[i].pid<<" size="<<treeNode[i].size<<endl;
                            }
                        }
                        if (flag==0) {
                            //                            cout<<temp<<"fail to alloc"<<endl;
                        }
                        //                    cout<<a<<endl;
                    }else{
                        //zui nan chu li
                        
                    }
                }
            }else if(line2.substr(0,7)=="dump();"){
//                cout<<treeNode[6].pid<<endl;
//                cout<<treeNode[8].pid<<endl;
//                cout<<treeNode[9].pid<<endl;
//                cout<<treeNode[10].pid<<endl;
//                cout<<treeNode[11].pid<<endl;
//                cout<<treeNode[12].pid<<endl;
//                cout<<treeNode[13].pid<<endl;
//                cout<<treeNode[14].pid<<endl;
                for (int i=0; i<memorySize/slabSize*2; i++) {
                    if (treeNode[i].pid!=0) {
                        int layer=i;
                        while (1){
                            layer=(layer-1)/2;
                            treeNode[layer].space=1;
                            if (layer==0) {
                                break;
                            }
                            
                        }
                    }
                }
                
                cout<<"slab:"<<endl;
                for (int i=log2(memorySize/slabSize); i>log2(slabProportion); i--) {
                    int flag=0;
                    int flagIntoSlab=0;
                    int flag3=0;
                    char temp[i];
                    for (int j=(pow(2, i))-1; j<=(pow(2, i)-1+pow(2, i+1)/slabProportion); j++) {
                        if ((treeNode[j].pid!=0&&treeNode[j].pid!=-1)) {
                            flag=1;
                            //                            if (treeNode[j].pid==44) {
                            //                                cout<<"found 44"<<endl;
                            //                            }
                            if (j>flagIntoSlab) {
                                flagIntoSlab=j;
                            }
                            for (int k=0;k<i;k++) {
                                temp[k]='1';
                            }
                            int temp2=j+1-pow(2, floor(log2(j+1)));
                            for (int k=i-1; k>=0; k--) {
                                //                                int temp2=j+1-pow(2, floor(log2(j+1)));
                                //                                cout<<temp2<<endl;
                                //                                int temp3=pow(2, k+1);
                                
                                if (temp2%2==0) {
                                    temp[k]='0';
                                }
                                temp2=temp2/2;
                                //                                cout<<temp2<<endl;
                            }
                            for (int l=0; l<i; l++) {
                                cout<<temp[l];
                            }
                            cout<<" "<<treeNode[j].pid<<endl;
                        }else if ((treeNode[j].pid==0&&treeNode[(j-1)/2].space==1)&&flag3==0)
                        {
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
                            flag3=1;
                        }
                    }
                    
                    
                    
                    
                    
//                    int flag=0;
//                    char temp[i];
//                    for (int j=(pow(2, i))-1; j<=(pow(2, i)-1+pow(2, i+1)/slabProportion); j++) {
//                        if (treeNode[j].pid!=0&&treeNode[j].pid!=-1) {
//                            flag=1;
//                            for (int k=0;k<i;k++) {
//                                temp[k]='0';
//                            }
//                            
//                            for (int k=i-1; k>=0; k--) {
//                                int temp2=j+2-pow(2, floor(log2(j+1)));
//                                int temp3=pow(2, k+1);
//                                if (temp2%temp3==0) {
//                                    temp[i-1-k]='1';
//                                }
//                            }
//                            
//                            for (int l=0; l<i; l++) {
//                                cout<<temp[l];
//                            }
//                            cout<<" "<<treeNode[j].pid<<endl;
//                        }
//                    }
//                    if (flag==0) {
//                        for (int r=0; r<i; r++) {
//                            if (r==i-1) {
//                                temp[r]='1';
//                            }
//                            else{
//                                temp[r]='0';
//                            }
//                            cout<<temp[r];
//                        }
//                        cout<<" free"<<endl;
//                    }
                }
                int flagIntoSlab=0;
                cout<<"buddy:"<<endl;
                for (int i=log2(memorySize/slabSize); i>0; i--) {
                    
                    int flag=0;
                    char temp[i];
                    for (int j=(pow(2, i)-1+pow(2, i+1)/slabProportion); j<=pow(2, i+1)-2; j++) {
                        if (treeNode[j].pid!=0&&treeNode[j].pid!=-1) {
                            flag=1;
//                            if (treeNode[j].pid==44) {
//                                cout<<"found 44"<<endl;
//                            }
                            if (j>flagIntoSlab) {
                                flagIntoSlab=j;
                            }
                            for (int k=0;k<i;k++) {
                                temp[k]='1';
                            }
                            int temp2=j+1-pow(2, floor(log2(j+1)));
                            for (int k=i-1; k>=0; k--) {
//                                int temp2=j+1-pow(2, floor(log2(j+1)));
//                                cout<<temp2<<endl;
//                                int temp3=pow(2, k+1);
                                
                                if (temp2%2==0) {
                                    temp[k]='0';
                                }
                                temp2=temp2/2;
//                                cout<<temp2<<endl;
                            }
                            for (int l=0; l<i; l++) {
                                cout<<temp[l];
                            }
                            cout<<" "<<treeNode[j].pid<<endl;
                        }
                    }
                    if (((flag==0&&pow(2, i)<flagIntoSlab)||(flag==0&&pow(2, i)-1<slabProportion))&&(treeNode[(i-1)/2].space==1&&(i-1)/2!=0)) {
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
