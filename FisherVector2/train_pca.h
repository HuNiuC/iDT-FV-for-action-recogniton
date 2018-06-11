#ifndef TRIAN_PCA_H_INCLUDED
#define TRIAN_PCA_H_INCLUDED

#include <sstream>
#include <iostream>
#include <fstream>
#include "pca.h"
using namespace std;
int trian_pca(string inputFile,string outputFile, int pDim,string save_projected = "0")
{

    vector<vector<double> > inputData;

    ifstream fin;
    fin.open(inputFile.c_str());
    if (!fin.is_open()) {
        cout<<"Cannot open "<<inputFile<<endl;
        return 0;
    }
    string line;

    while (getline(fin, line))  {
        vector<double> feat;
        stringstream ss;
        ss<<line;
        double val;
        while (ss>>val)
            feat.push_back(val);
        inputData.push_back(feat);
    }
    fin.close();
    cout<<"Loading complete."<<endl;
    cout<<"#pts = "<<inputData.size()<<endl;

    PCAWrapper pca;
    cout<<"Start to train PCA projection matrix..."<<endl;
    if (pca.train(inputData, pDim, outputFile))
        cout<<"Training complete."<<endl;
    else    {
        cout<<"Failed!"<<endl;
        return 0;
    }

    if (save_projected !="0")   {
        ofstream fout;
        fout.open(save_projected.c_str());
        if (!fout.is_open())    {
            cout<<"Cannot save projected file."<<endl;
            return 0;
        }
        for (int i = 0; i < inputData.size(); i++)  {
            vector<double> projected = pca.project(inputData[i]);
            fout<<projected[0];
            for (int j = 1; j < projected.size(); j++)
                fout<<" "<<projected[j];
            fout<<endl;
        }
        fout.close();
    }

    return 0;
}



#endif // TRIAN_PCA_H_INCLUDED
