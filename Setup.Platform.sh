# Installer by Murray Meehan 2012-01-12, murray.meehan@gmail.com


echo
echo Time to install OpenFrameworks/OpenNI/Everything we need for our project (apt-get dependencies)
echo This script is Ubuntu 11.04 and 10.04 compatable, I think. -- Murray Meehan

echo
echo Step 0: apt-get installing mandatory prerequisites: git g++ python libusb-1.0-0-dev freeglut3-dev sun-java6-jdk
sudo add-apt-repository "deb http://archive.canonical.com/ lucid partner"
sudo apt-get update
#note: libcurl4-openssl-dev  for Ubuntu 11.04 might be equivalent to libcurl-dev for Ubuntu 10.04.
sudo apt-get install git g++ python libusb-1.0-0-dev freeglut3-dev sun-java6-jdk build-essential subversion libasound2-dev cmake cmake-curses-gui libasound2-dev libpoco-dev libglut3 libglut3-dev doxygen graphviz mono-complete libboost-all-dev libcairo-dev libcurl4-openssl-dev 


echo
echo Installing everything in directory ~/499
mkdir -R ~/499/ && cd ~/499/

#git clone git://github.com/memo/msalibs.git - MSA libs are OSX compatible only :(


echo Step 2: Install openNI latest unstable, 
cd ~/499
git clone git://github.com/OpenNI/OpenNI.git
OpenNI/Platform/Linux/CreateRedist
./RedistMaker
cd ~/499/OpenNI/Platform/Linux/Redist/OpenNI-Bin-Dev-Linux-x86-v1.5.2.23
sudo ./install.sh


echo Step 2: Install NITE latest unstable, 
cd ~/499
wget http://www.openni.org/downloads/nite-bin-linux-x86-v1.5.2.21.tar.bz2 
tar jxvf nite-bin-linux-x86-v1.5.2.21.tar.bz2
cd ~/499/NITE-Bin-Dev-Linux-x86-v1.5.2.21
sudo ./install.sh

echo 
echo Step 2: Install avin2 modified SensorKinect drivers  (Linux 32-bit BINARY)
cd ~/499
git clone git://github.com/avin2/SensorKinect.git
cd ~/499/avin2-SensorKinect-faf4994/Bin
tar jxvf SensorKinect091-Bin-Linux32-v5.1.0.25.tar.bz2
cd Sensor-Bin-Linux-x86-v5.1.0.25
sudo ./install.sh

echo
echo Step 2: Install OpenFrameworks w/ Codeblocks, dependencies, examples.
echo REFERENCE: Codeblocks project template: ~/499/openFrameworks/scripts/linux/template/emptyExample_linux_fullCBP.cbp
cd ~/499
#wget http://www.openframeworks.cc/versions/preRelease_v0.07/of_preRelease_v007_linux.tar.gz && tar zxvf of_preRelease_v007_linux.tar.gz
git clone git://github.com/openframeworks/openFrameworks.git
cd ~/499/openFrameworks/scripts/linux/ubuntu
sudo ./install_codeblocks.sh
sudo ./install_dependencies.sh 
sudo ./install_codecs.sh 
cd ~/499/openFrameworks/scripts/linux/
#echo
#echo (OPTIONAL) building all examples and addonexamples found in ~/499/openFrameworks/apps/examples/ & addonsExamples/
#./buildAllExamples.sh 
#./buildAllAddonExamples.sh

echo
echo Step 2: Install ofxOpenNI w/ examples app @ ~/499/openFrameworks/apps/addonsExamples/openNI-demoAllFeatures
cd ~/499/openFrameworks/addons/ && git clone https://github.com/gameoverhack/ofxOpenNI.git
#git clone git://github.com/paulreimer/ofxMarsyas.git
cp -R ~/499/openFrameworks/addons/ofxOpenNI/examples/* ~/499/openFrameworks/apps/addonsExamples/
cd ~/of/openFrameworks/apps/addonsExamples/openNI-demoAllFeatures
#get usable Makefile, addons.make, config.make (the old ones don't work, right?)
mv makefile makefile.backup
wget http://web.uvic.ca/~meehanmw/499/ofxOpenNI/config.make
cp ../opencvExample/Makefile . #wget http://web.uvic.ca/~meehanmw/499/ofxOpenNI/Makefile
echo ofxOpenNI > ./addons.make #wget http://web.uvic.ca/~meehanmw/499/ofxOpenNI/addons.make
make # same effect as: #make Release


echo
echo Step 2d: Install Marsyas, ofxMarsyas, ofxMarsyasApps (ofxMarsyas only works on OSX)
cd ~/499 
##################3
# Option 1 - grab everything from Marsyas SVN repository
svn co https://marsyas.svn.sourceforge.net/svnroot/marsyas/trunk marsyas
cp ~/499/marsyas/src/marsyas_of/ofxMarsyas/src/* ~/499/openFrameworks/addons/ofxMarsyas/src/
cp -R ~/499/marsyas/src/apps/ofApps ~/499/openFrameworks/apps/ofxMarsyasApps
cd ~/499/marsyas/ && mkdir build && cd build
##################3
#Option 2 - grab everything from elsewhere
#wget http://sourceforge.net/projects/marsyas/files/marsyas/marsyas-0.4.5.tar.gz/download && mv download marsyas.tar.gz && tar zxvf marsyas.tar.gz
#cd ~/499/openFrameworks/apps/ && git clone git://github.com/paulreimer/ofxMarsyasApps.git # ofxMarsyas and ofxMarsyasApps are MacOSX/iPhone only b/c of MSAlib/Cinder prereq.
#cd ~/499/marsyas-0.4.5/ && mkdir build && cd build
##################3
echo Press 'c', then 'g': 'c' to configure ccmake, 'g' to generate build files
ccmake ../src/ 
make && make test && sudo make install


echo
echo Step 5: All done! 
echo Make some shortcuts in ~/of/shortcuts:
mkdir -p ~/of/shortcuts
cd ~/of/shortcuts
ln -s ~/499/openFrameworks/apps/addonsExamples/openNI-demoAllFeatures/bin openNI-demoAllFeatures-bin
ln -s ~/499/NITE-Bin-Dev-Linux-x86-v1.5.2.21/Samples/Bin/x86-Release NITE-samples
ln -s ~/499/marsyas-0.4.5/build/bin marsyas-bin
ln -s ~/499/of_preRelease_v007_linux/apps of-apps
echo Go to ~/of/shortcuts to find some example programs
cd ~/of/shortcuts
ls


