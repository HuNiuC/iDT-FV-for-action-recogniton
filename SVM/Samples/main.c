#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctime>
using namespace std;
string data_path = "/media/SeSaMe_NAS/data/Lixinhui/NTU_FV1/SVM/fold1/Cam02_Cam03_train_data.txt";
string save_path = "/media/SeSaMe_NAS/data/Lixinhui/NTU_FV1/SVM/fold1/train_Sample_data.txt";
int main ()
{
   ifstream in_f(data_path.c_str());
   ofstream out_f(save_path.c_str());
   string line;
   srand((unsigned)time(NULL)); //初始化随机数种子
   while(getline(in_f,line))
   {
       if (rand()/(double)RAND_MAX < 0.15)
       {
          out_f << line<<endl;
       }
   }
   in_f.close();
   out_f.close();
   cout << "OK !!!"<< endl;
}

