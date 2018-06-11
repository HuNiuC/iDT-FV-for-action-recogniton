#ifndef COMBINE_H_INCLUDED
#define COMBINE_H_INCLUDED

int combine(string input_name,string output_name)
{
    string Tr_name,Hof_name,Hog_name,MBHx_name,MBHy_name;
    Tr_name = input_name +".traj.fv.txt";
    Hof_name = input_name +".hof.fv.txt";
    Hog_name = input_name +".hog.fv.txt";
    MBHx_name = input_name +".mbhx.fv.txt";
    MBHy_name = input_name +".mbhy.fv.txt";

    ifstream in_tr(Tr_name.c_str());
    ifstream in_hof(Hof_name.c_str());
    ifstream in_hog(Hog_name.c_str());
    ifstream in_mbhx(MBHx_name.c_str());
    ifstream in_mbhy(MBHx_name.c_str());

    ofstream out_f(output_name.c_str());
    string line_tr,line_hof,line_hog,line_mbhx,line_mbhy;
    stringstream ss;
    vector<double> feat;
    double val;
    while (getline(in_tr, line_tr))  {
        ss<<line_tr;
        while (ss>>val)
            feat.push_back(val);
        ss.clear();
        ss.str("");
    }
    in_tr.close();

        while (getline(in_hog, line_hog))  {
        ss<<line_hog;
        while (ss>>val)
            feat.push_back(val);
        ss.clear();
        ss.str("");
    }
    in_hog.close();

        while (getline(in_hof, line_hof))  {
        ss<<line_hof;
        while (ss>>val)
            feat.push_back(val);
        ss.clear();
        ss.str("");
    }
    in_hof.close();



        while (getline(in_mbhx, line_mbhx))  {
        ss<<line_mbhx;
        while (ss>>val)
            feat.push_back(val);
        ss.clear();
        ss.str("");
    }
    in_mbhx.close();


        while (getline(in_mbhy, line_mbhy))  {
        ss<<line_mbhy;
        while (ss>>val)
            feat.push_back(val);
        ss.clear();
        ss.str("");
    }
    in_mbhy.close();

    int total_fea = feat.size();
    cout << total_fea<< endl;
    for (int i=0;i<total_fea;i++)
    {
        out_f<<feat[i]<<" ";
    }
    out_f<<endl;
    out_f.close();
    return 0;

}


#endif // COMBINE_H_INCLUDED
