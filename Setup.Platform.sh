# Installer by Murray Meehan 2012-01-12, murray.meehan@gmail.com
# 
# References:
# http://ofxaddons.com/repos/443
# http://www.openframeworks.cc/setup/linux-codeblocks/
# http://marsyas.info/docs/manual/marsyas-user/Specific-installation-examples.html#Specific-installation-examples
# 

echo 
echo Time to install OpenFrameworks/OpenNI/Everything we need for our project
echo This script is Ubuntu 11.04 and 10.04 compatable, I think. -- Murray Meehan

echo
echo installing everything in directory ~/of
cd ~
mkdir of
cd of

echo
echo Step 0: apt-get installing mandatory prerequisites: git g++ python libusb-1.0-0-dev freeglut3-dev sun-java6-jdk
sudo add-apt-repository "deb http://archive.canonical.com/ lucid partner"
sudo apt-get update
sudo apt-get install git g++ python libusb-1.0-0-dev freeglut3-dev sun-java6-jdk build-essential subversion libasound2-dev cmake cmake-curses-gui libasound2-dev libpoco-dev libglut3 libglut3-dev

echo
echo Step 0: apt-get installing optional prerequisites: doxygen graphviz mono-complete 
sudo apt-get install doxygen graphviz mono-complete 

echo
echo Step 1: Download all of the things
cd ~/of
#wget http://www.openframeworks.cc/versions/preRelease_v0.07/of_preRelease_v007_linux.tar.gz
#tar zxvf of_preRelease_v007_linux.tar.gz
#git clone git://github.com/OpenNI/OpenNI.git
#wget http://www.openni.org/downloads/nite-bin-linux-x86-v1.5.2.21.tar.bz2
#tar jxvf nite-bin-linux-x86-v1.5.2.21.tar.bz2
#git clone git://github.com/avin2/SensorKinect.git
#git clone https://github.com/gameoverhack/ofxOpenNI.git
#wget http://sourceforge.net/projects/marsyas/files/marsyas/marsyas-0.4.5.tar.gz/download
#mv download marsyas-0.4.5.tar.gz
#tar zxvf marsyas-0.4.5.tar.gz
#git clone git://github.com/paulreimer/ofxMarsyas.git

echo
echo Step 2: Installing all of the things
echo 2a: OpenFrameworks, Codeblocks, and some dependencies.
cd ~/of/of_preRelease_v007_linux/scripts/linux/ubuntu
sudo ./install_codeblocks.sh
sudo ./install_dependencies.sh 
sudo ./install_codecs.sh 

echo 
echo Step 2b: Install openNI latest unstable, 
cd ~/of/OpenNI/Platform/Linux/CreateRedist
./RedistMaker
cd ~/of/OpenNI/Platform/Linux/Redist/OpenNI-Bin-Dev-Linux-x86-v1.5.2.23
sudo ./install.sh

echo 
echo Step 2b: Install NITE latest unstable, 
cd ~/of/NITE-Bin-Dev-Linux-x86-v1.5.2.21
sudo ./install.sh

echo 
echo Step 2b: Install Linux 3d-bit BINARY - avin2 modified SensorKinect drivers 
cd ~/of/avin2-SensorKinect-faf4994/Bin
tar jxvf SensorKinect091-Bin-Linux32-v5.1.0.25.tar.bz2
cd Sensor-Bin-Linux-x86-v5.1.0.25
sudo ./install.sh

echo
echo Step 2c: Install ofxOpenNI
cd ~/of/of_preRelease_v007_linux/addons/ofxOpenNI 
cp -R ~/of/of_preRelease_v007_linux/addons/ofxOpenNI/examples/openNI-demoAllFeatures ~/of/of_preRelease_v007_linux/apps/addonsExamples/

echo
echo Compile ofxOpenNI app - openNI-demoAllFeatures
cd ~/of/of_preRelease_v007_linux/apps/addonsExamples/openNI-demoAllFeatures
#create usable Makefile, addons.make, config.make
mv makefile makefile.backup
cp ../opencvExample/Makefile .
echo ofxOpenNI > ./addons.make
wget http://web.uvic.ca/~meehanmw/499/config.make
make

echo
echo Make some shortcuts in ~/of/shortcuts:
mkdir -p ~/of/shortcuts
cd ~/of/shortcuts
ln -s ~/of/of_preRelease_v007_linux/apps/addonsExamples/openNI-demoAllFeatures/bin openNI-demoAllFeatures-bin
ln -s ~/of/NITE-Bin-Dev-Linux-x86-v1.5.2.21/Samples/Bin/x86-Release NITE-samples
ln -s ~/of/marsyas-0.4.5/build/bin marsyas-bin
ln -s ~/of/of_preRelease_v007_linux/apps of-apps


echo
echo Step 2d: Install Marsyas and ofxMarsyas
echo - install marsyas - stable version 0.4.5
mkdir ~/of/marsyas-0.4.5/build
cd ~/of/marsyas-0.4.5/build
echo Press 'c' to configure ccmake, then 'g' to generate build files.
ccmake ../src/ 
make
make test
sudo make install

# echo
# echo NOT WORKING - build marsyas examples, to help new programmers learn how to write for marsyas
# mkdir ~/of/marsyas-0.4.5/build/examples
# cd ~/of/marsyas-0.4.5/doc/examples


# echo
# echo NOT RUNNING THIS - install marsyas unstable SVN branch - recommended if developing parts of marsyas
#cd ~/of
#svn co https://marsyas.svn.sourceforge.net/svnroot/marsyas/trunk marsyas-svn
#cd marsyas-svn

# echo
# echo install ofxMarsyas
# mv ~/of/ofxMarsyas ~/of/of_preRelease_v007_linux/addons/ofxMarsyas
#
# echo
# echo NOT WORKING - missing ofxMarsyas samples - got some from paul via gabby
# echo WARNING: these require MSAlibs to work
# echo https://github.com/memo/msalibs
# echo https://github.com/memo/msalibs/tree/master/ofxMSAInteractiveObject
# cd ~/of/of_preRelease_v007_linux/apps/addonsExamples/
# mkdir marsyas
# cd marsyas
# wget https://www.dropbox.com/s/lzs0is96yynenjf/marNetwork.cpp
# wget https://www.dropbox.com/s/07dqur3vqaez7c9/marNetwork.h
# echo ofxMarsyas > addons.make
# cp ../openNI-demoAllFeatures/Makefile .
# make


echo
echo Step 4: Compiling examples and addonExamples
echo Note: ofxOpenNI example should have failed to compile using ./buildAllAddonExamples.sh
cd ~/of/of_preRelease_v007_linux/scripts/linux
./buildAllExamples.sh
./buildAllAddonExamples.sh

echo
echo Step 5: All done! 
echo Go to ~/of/shortcuts to find some example programs
cd ~/of/shortcuts
