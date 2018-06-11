#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <fstream>
#include "feature.h"
#include "SelectSample.h"
#include "gmm.h"
#include "train_pca.h"
#include "compute_FV.h"
#include "gzload.h"
#include "combine.h"
#include "getFiles.h"
using namespace std;



// This code is run for the step 1 2 3 4
// main1  for downsample  the raw data
// main2 to condect PCA for the randomly damples
// main3 to generate the codebook
// main4 to transfer the data(with .gz format) to the txt format using FisherVector codebook
 


//this is the base_path,change it everytime your change the view file
string base_path = "/media/SeSaMe_NAS/data/NTUfeature/";

// to randomly select samples
int main1()
{
    string sample_file = "/media/SeSaMe_NAS/data/NTUfeature/SampleFeatures_NTU.txt";
    string store_path =  "/media/SeSaMe_NAS/data/NTUfeature/RandomFeaC002C003View";
    int sample_num = 256000;
    SelectSample(sample_file,store_path,sample_num,1);
    cout << " has finished !!!" << endl;
}

// the following code is to conduct PCA for the randomly samples
int main2()
{
    string inputData (base_path + "RandomFeaC002C003View.hof");
    string outputFile (base_path+ "data/hof_pca.mat");
    string after_pca_hof (base_path+ "data/after_pca_hof");
    int Dam = 54;
    trian_pca(inputData,outputFile, Dam,after_pca_hof);

    string inputData_hog(base_path + "RandomFeaC002C003View.hog");
    string outputFile_hog(base_path+ "data/hog_pca.mat");
    string after_pca_hog(base_path+ "data/after_pca_hog");
    int Dam_hog = 48;
    trian_pca(inputData_hog,outputFile_hog, Dam_hog,after_pca_hog);

    string inputData_traj (base_path + "RandomFeaC002C003View.traj");
    string outputFile_traj(base_path+ "data/traj_pca.mat");
    string after_pca_traj (base_path+ "data/after_pca_traj");
    int Dam_traj = 15;
    trian_pca(inputData_traj,outputFile_traj, Dam_traj,after_pca_traj);

    string inputData_mbhx (base_path + "RandomFeaC002C003View.mbhx");
    string outputFile_mbhx (base_path+ "data/mbhx_pca.mat");
    string after_pca_mbhx (base_path+ "data/after_pca_mbhx");
    int Dam_mbhx = 48;
    trian_pca(inputData_mbhx,outputFile_mbhx, Dam_mbhx,after_pca_mbhx);

    string inputData_mbhy (base_path + "RandomFeaC002C003View.mbhy");
    string outputFile_mbhy(base_path+ "data/mbhy_pca.mat");
    string after_pca_mbhy (base_path+ "data/after_pca_mbhy");
    int Dam_mbhy = 48;
    trian_pca(inputData_mbhy,outputFile_mbhy, Dam_mbhy,after_pca_mbhy);


    return 0;

}

// the following is to generate the codebook
int main()
{
// to randomly select sample points for descripter
    string inputData(base_path + "data/after_pca_hof");
    int numClusters = 256;
    string outputCodeBook(base_path + "data/hof.gmm");

    GMMWrapper gmm;
    cout<<"Start to train GMM..."<<endl;
    if (!gmm.train(inputData, numClusters, outputCodeBook)) {
        cout<<"Training failed."<<endl;
        return 0;
    }
    cout<<"Training complete."<<endl;

    string inputData_hog (base_path + "data/after_pca_hog");
    string outputCodeBook_hog (base_path + "data/hog.gmm");

    GMMWrapper gmm_hog;
    cout<<"Start to train GMM..."<<endl;
    if (!gmm_hog.train(inputData_hog, numClusters, outputCodeBook_hog)) {
        cout<<"Training failed."<<endl;
        return 0;
    }
    cout<<"Training complete."<<endl;

    string inputData_traj (base_path + "data/after_pca_traj");
    string outputCodeBook_traj(base_path + "data/traj.gmm");

    GMMWrapper gmm_traj;
    cout<<"Start to train GMM..."<<endl;
    if (!gmm_traj.train(inputData_traj, numClusters, outputCodeBook_traj)) {
        cout<<"Training failed."<<endl;
        return 0;
    }
    cout<<"Training complete."<<endl;

    string inputData_mbhx (base_path + "data/after_pca_mbhx");
    string outputCodeBook_mbhx (base_path + "data/mbhx.gmm");

    GMMWrapper gmm_mbhx;
    cout<<"Start to train GMM..."<<endl;
    if (!gmm_mbhx.train(inputData_mbhx, numClusters, outputCodeBook_mbhx)) {
        cout<<"Training failed."<<endl;
        return 0;
    }
    cout<<"Training complete."<<endl;



    string inputData_mbhy  (base_path + "data/after_pca_mbhy");
    string outputCodeBook_mbhy(base_path + "data/mbhy.gmm");
    GMMWrapper gmm_mbhy;
    cout<<"Start to train GMM..."<<endl;
    if (!gmm_mbhy.train(inputData_mbhy, numClusters, outputCodeBook_mbhy)) {
        cout<<"Training failed."<<endl;
        return 0;
    }
    cout<<"Training complete."<<endl;

    cout << "Hello world!" << endl;
    return 0;
}


//the following code is to encode
int main4()
{
    string base_path = "/media/SeSaMe_NAS/data/NTU_RGBD/";
    string output_path = "/media/SeSaMe_NAS/data/Lixinhui/NTU_FV1/data_all/";
    string input_list = "list.txt";
    string ID,line;
    ifstream in_f(input_list.c_str());
    if (in_f)
    {
        while(getline(in_f,ID))
        {
            string fea_path(base_path + ID);
            string list_name = base_path + ID +".txt";

            getFiles(fea_path.c_str(),list_name);
            ifstream in_g(list_name.c_str());

            if (in_g)
            {
                while(getline(in_g,line))
                {
                    string input_gz(fea_path+"/"+line);
                    string temp_store ="./tmp/store.txt";
                    ofstream out_temp(temp_store.c_str());
                    string out_put;
                    gzLoad(input_gz,out_put);
                    out_temp <<out_put<<endl;
                    out_temp.close();
                    string input_idt(temp_store) ;
//                    string output_fv_file = output_path + ID + "/" + line;
                    string output_fv_file (output_path + ID + "/" + line);
                    string pca_list(output_path + "data/pca.lst");
                    string gmm_list (output_path + "data/codebook.lst");
                    string combine_file(output_fv_file + "_fv.txt");
                    compute_FV( input_idt, output_fv_file,  pca_list,  gmm_list);
                    combine( output_fv_file,combine_file) ;
                    remove (temp_store.c_str());
                    cout << output_fv_file  << "  has finished !!! " << endl;
                }
            }
        }
    }



}
