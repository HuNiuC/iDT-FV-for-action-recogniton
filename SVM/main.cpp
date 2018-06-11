#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string>
#include <fstream>
#include "getFiles.h"
#include <sstream>
#include <vector>
using namespace std;

string data_path = "/media/SeSaMe_NAS/data/Lixinhui/NTU_FV1/data_all";
string save_path = "/media/SeSaMe_NAS/data/Lixinhui/NTU_FV1/SVM/fold1/";
string list_name = "ID_list1.txt";
string train_data(save_path+"Cam02_Cam03_train_data.txt");
string test_data (save_path+"Cam01_test_data.txt");
string test_label(save_path+"Cam01_test_label.txt");
int main()
{
       getFiles(data_path.c_str(),list_name);
       ifstream in_g(list_name.c_str());
       ofstream o_train(train_data.c_str(),std::ofstream::app);
       ofstream o_test(test_data.c_str(),std::ofstream::app);
       ofstream o_label(test_label.c_str(),std::ofstream::app);
       string line;
       int label =0;
       if (in_g)
       {
           while(getline(in_g,line))
           {
               string  view_number = line.substr(4,4);
               string  action_number = line.substr(18,2);

               string input_fv(data_path+"/"+line);
               cout << input_fv << endl;
               ifstream in_f2(input_fv.c_str());
               string line2;
               stringstream ss,str2int;
               vector<double> feat;
               double val;
               if (view_number =="C002"||view_number =="C003")
               {
                   getline(in_f2,line2);
                   ss<<line2;
                   while(ss>>val)
                       feat.push_back(val);
                   int total_fea = feat.size();
                   str2int<<action_number;
                   str2int>>label;
                   o_train << label <<"\t";
                   for (int i=0;i<total_fea;i++)
                   {
                     o_train<<i+1<<":"<<feat[i]<<"\t";
                   }
                   o_train<<endl;
                   ss.clear();
              }
              else if (view_number =="C001")
               {
                   //if ID_number is in the probe_set ,put it in the test .txt
                       getline(in_f2,line2);
                       ss<<line2;
                       while(ss>>val)
                           feat.push_back(val);
                       int total_fea = feat.size();
                       str2int<<action_number;
                       str2int>>label;
                        o_test << label <<"\t";
                        o_label << label << endl;
                       for (int i=0;i<total_fea;i++)
                           {
                                o_test<<i+1<<":"<<feat[i]<<"\t";
                           }
                       o_test<<endl;
                       ss.clear();
                }
    
              else
                   cout << ".....file_name error!!!!" << endl;


}
}
       cout << "OK !!"<< endl;
       o_train.close();
       o_test.close();
       o_label.close();
       return 0;
}
