#include <iostream>
#include "getFiles.h"
#include "SampleFea.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "gzload.h"
#include <fstream>
using namespace std;


int main()
{
    //list
    string list_path = "ID_list.txt";
    string base_path = "/media/SeSaMe_NAS/data/NTUfeature/";
    ifstream in_path(list_path.c_str());
//    string view_path = "/media/SeSaMe_NAS/data/Lixinhui/MCAD_FV3/para_data/SampleFea_Cam04_PTZ04_PTZ06.txt";
    string view_path = "/media/SeSaMe_NAS/data/NTUfeature/SampleFeatures_NTU.txt";
    string ID;
    string temp_store = "temp_store.txt";
    string line;
    if(in_path)
    {
        while(getline(in_path,ID))
        {
        //path to store the features
            string fea_path = base_path + ID ;
        //the features file list of the fea_path
            string list_name = base_path + ID +".txt";

            getFiles(fea_path.c_str(),list_name);
            ifstream in_g(list_name.c_str());

            if (in_g)
            {
                while(getline(in_g,line))
                {
                    string cam = line.substr(4,4);
                    if(cam =="C002" ||cam =="C003")
                    {
                        string video_fea_path = fea_path+"/"+line;
                    	string out_put;
                    	ofstream out_temp(temp_store.c_str());
                    	cout << video_fea_path << endl;
                    	gzLoad(video_fea_path,out_put);
                    	out_temp <<out_put<<endl;
                    	out_temp.close();
                    	SampleFea(temp_store,40,view_path);
                    	cout << video_fea_path << " has been finished !!!!" <<endl;
                    	remove(temp_store.c_str());
                    }
                }
            }
            in_g.close();
            remove (list_name.c_str());
            cout << "Hello world!" << endl;
            cout << ID << " is  OK "<< endl;

        }
    }
    return 0;
}
