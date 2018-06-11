# iDT-FV-for-action-recogniton
iDT(mproved dense trajectories ) and Fisher Vector algorithm implement with C++ 


IDT + FisherVector编码

1. IDT 特征提取
IDT 的官方代码在这里   https://lear.inrialpes.fr/people/wang/dense_trajectories
依赖的工具包是OpenCV2.4 和 ffmpeg.
然后编译产生 DenseTrack 可执行文件
然后就可以用这个可执行文件去提取自己的数据库视频特征啦。
注意的是产生的数据还是挺占存储空间的，所以最好压缩下。

举例：
我用的是NTU_RGB数据库，RGB模态的video.此数据库下的文件格式为：
NTU_RGBD/nturgb+d_rgb_S001/videoname.avi

使用Linux下的shell 脚本：myIDT.sh 文件

Folder_A是我存放视频数据的路径
Folder_B是产生的iDT特征的路径（保证存在）

#!/bin/sh  
#============ get the file name ===========  

Folder_A="/media/SeSaMe_NAS/data/NTU_RGBD/nturgb+d_rgb_S001"
Folder_B="/media/SeSaMe_NAS/data/NTUfeature/nturgb+d_rgb_S001"

for file_a in ${Folder_A}/*; do  
    temp_file=`basename $file_a`  
    echo $temp_file
    echo $file_a
    feature_file=$Folder_B/${temp_file%%.*}
    echo $feature_file
    ./DenseTrack $file_a |gzip > $feature_file.features.gz
done 

以上将每个视频IDT特征保存成 .gz的压缩格式，因为这样更省存储空间。

2. IDT 格式转换(从每个视频都保存为.gz格式到随机采样后整个数据库保存为.txt格式)

因为FisherVector要应用GMM(高斯混合模型)生成CodeBook如果把所有的数据都用到，那聚类任务时间空间的占用都太大了，所以要对数据进行随机采样。GMM的聚类中心论文里设置为K = 256 ，选用一共256000个样本。我是分为两步随机采样，一是每个视频采样40个点，生成SampleFeatures_NTU.txt 文件.（程序见文件夹FisherVector1） 然后GMM前再进行随机采样，生成256000个点。(程序见文件夹FisherVector2)

FisherVector1文件夹里的文件说明：
1. libz.a 是读取.gz需要的，为了方便我就把这个库函数文件Copy过来了
2. gzload 是读取.gz文件的函数
3. getFiles 是读取某路径下的全部文件
4. SampleFea.h 是随机选取样本点
5. main.cpp 是主程序，里面34行是设置个临时文件的名字，因为gzload会产生临时文件。 if(cam =="C002" ||cam =="C003") 是由于我做多视角，要判断视角属性，一般数据库用不到这一步，可以删除。

改好路径，然后
1 cd ./build
2 $ cmake ..
3 $ make
the executable file is in the ./build


3. FisherVector 正式开始
原理参考论文和这篇博客 https://blog.csdn.net/wzmsltw/article/details/53023363
原始代码参考 https://github.com/chensun11/dtfv

回归一下IDT算法：

    HOG特征:HOG特征计算的是灰度图像梯度的直方图。直方图的bin数目取为8。故HOG特征的长度为96（2*2*3*8）。
    HOF特征:HOF计算的是光流（包括方向和幅度信息）的直方图。直方图的bin数目取为8+1，前8个bin于HOG相同，额外的一个bin用于统计光流幅度小于某个阈值的像素。故HOF的特征长度为108（2*2*3*9）。
    MBH特征:MBH计算的是光流图像梯度的直方图，也可以理解为在光流图像上计算的HOG特征。由于光流图像包括x方向和y方向，故分别计算MBHx和MBHy。MBH总的特征长度为192（2*96）。


Fisher Vector先用大量特征训练码书，再用码书对特征进行编码。在iDT中使用的Fisher Vector的各个参数为：

    用于训练的特征长度：trajectory+HOF+HOG+MBH=30+96+108+192=426维
    用于训练的特征个数：从训练集中随机采样了256000个
    PCA降维比例：2，即维度除以2，降维后特征长度为213。先降维，后编码
    Fisher Vector中高斯聚类的个数K：K=256

故编码后得到的特征维数为2KD个，即109056维。在编码后iDT同样也使用了SVM进行分类。在实际的实验中，推荐使用liblinear，速度比较快。

这真是一年前写的代码(俗称一脸青涩的代码) ～

main.cpp 里一共有四个mainXX函数

1. 随机采样256000个样本点，并且将数据 hof/hog/mbhx/mbhy特征分开存放

2. 将iDT特征进行PCA降维，维度为原来的一半，存放在data文件夹下

3. 进行GMM聚类， 聚类中心保存为.gmm文件

4.  用码本对原始iDT特征进行编码

此处需要手动建立两个文件codebook.lst文件 /pca.lst 文件

每行为之前生成的.mat/.gmm的路径。注意特征的顺序不要有变动...

比如我的codebook.lst文件内容
/media/SeSaMe_NAS/data/NTUfeature//data/traj.gmm
/media/SeSaMe_NAS/data/NTUfeature/data/hog.gmm
/media/SeSaMe_NAS/data/NTUfeature/data/hof.gmm
/media/SeSaMe_NAS/data/NTUfeature/data/mbhx.gmm
/media/SeSaMe_NAS/data/NTUfeature/data/mbhy.gmm

pca.lst文件内容：
/media/SeSaMe_NAS/data/NTUfeature/data/traj_pca.mat
/media/SeSaMe_NAS/data/NTUfeature/data/hog_pca.mat
/media/SeSaMe_NAS/data/NTUfeature/data/hof_pca.mat
/media/SeSaMe_NAS/data/NTUfeature/data/mbhx_pca.mat
/media/SeSaMe_NAS/data/NTUfeature/data/mbhy_pca.mat


这个过程无比漫长，对于很大的数据库来说.....

生成了每个视频特征保存为 .txt的格式


5. SVM分类

首先，要生成SVM格式的训练和测试数据

介绍一下 libSVM的数据格式
Label 1:value 2:value ….

Label：是类别的标识
Value：就是要训练的数据，从分类的角度来说就是特征值，数据之间用空格隔开

我需要的实验是以视角划分训练集和测试集的，其他数据集可以修改划分方式

然后，就可以用 libsvm

链接在这里

https://www.csie.ntu.edu.tw/~cjlin/libsvm/

进行训练和预测了。


以上是我根据回忆写的，可能有部分数据放置位置不对，需调整。 一些程序很冗余，每次改动都得重新编译。可以改为接收参数的形式。

回想一年前的自己，真的是用战术上的勤奋，去掩盖战略上的懒惰。 
慢慢意识到，我是个没有天分的程序员...
Fighting ...




