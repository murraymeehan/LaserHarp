# Installer by Murray Meehan 2012-01-12, murray.meehan@gmail.com


echo
echo Time to install OpenFrameworks/OpenNI/Everything we need for our project (apt-get dependencies)
echo This script is Ubuntu 11.04 and 10.04 compatable, I think. -- Murray Meehan

echo
echo Step 0: installing mandatory prerequisites: git g++ python libusb-1.0-0-dev freeglut3-dev sun-java6-jdk

# Change this line to the correct one for your version of Ubuntu: natty=11.04, lucid=10.04?
sudo add-apt-repository "deb http://archive.canonical.com/ natty partner"
sudo apt-get update
#note: libcurl4-openssl-dev  for Ubuntu 11.04 might be equivalent to libcurl-dev for Ubuntu 10.04.
sudo apt-get install git g++ python libusb-1.0-0-dev freeglut3-dev build-essential subversion libasound2-dev cmake cmake-curses-gui libasound2-dev libpoco-dev libglut3 libglut3-dev doxygen graphviz mono-complete libboost-all-dev libcairo-dev libcurl4-openssl-dev 
#note: sun-java6-sdk was removed from the ubuntu repositories. that's inconvenient.
sudo apt-get install sun-java6-jdk 
#So when this line does not work, you have to install from the Sun website. *grumble grumble grumble*. http://www.oracle.com/technetwork/java/javasebusiness/downloads/java-archive-downloads-javase6-419409.html#jdk-6u30-oth-JPR

# instructions to setup java:
# 1. Follow these instrctions: https://help.ubuntu.com/community/Java#Manual_method
# 2. add this line to the file "~/.bashrc":
# 		export PATH=$PATH:/usr/lib/jvm/jdk1.6.0_30/bin

echo
echo Installing everything in directory ~/499
mkdir -R ~/499/ && cd ~/499/

#git clone --depth 1 git://github.com/memo/msalibs.git - MSA libs are OSX compatible only :(


echo Step 2: Install openNI latest unstable, 
cd ~/499
git clone --depth 1 git://github.com/OpenNI/OpenNI.git
cd OpenNI/Platform/Linux/CreateRedist
sh ./RedistMaker
cd ../Redist/OpenNI-Bin-Dev-Linux-x64-v1.5.2.23
#cd ../Redist/OpenNI-Bin-Dev-Linux-x86-v1.5.2.23 # even if this gives error messages, it still might work. Check ../Bin/ to see if something was built, and try running the sample program "NiViewer" after you install the primesense rules file.
sudo ./install.sh

# This is a good time to make sure that the 'NiViewer' program works.
# but I can't, cause the usb rules.d file isn't installed. grumble grumble. 

echo Step 2: Install NITE latest unstable, 
cd ~/499
wget http://www.openni.org/downloads/nite-bin-linux-x86-v1.5.2.21.tar.bz2 
tar jxvf nite-bin-linux-x86-v1.5.2.21.tar.bz2
cd ~/499/NITE-Bin-Dev-Linux-x86-v1.5.2.21
sudo ./install.sh

echo 
echo Step 2: Install avin2 modified SensorKinect drivers  (Linux 32-bit BINARY)
cd ~/499
git clone --depth 1 git://github.com/avin2/SensorKinect.git
cd SensorKinect/Bin
# pick the appropriate installer bin, and install it. In my case, that means:
tar jxvf SensorKinect091-Bin-Linux64-v5.1.0.25.tar.bz2
cd Sensor-Bin-Linux-x64-v5.1.0.25
sudo ./install.sh


echo
echo Step 2: Install OpenFrameworks w/ Codeblocks, dependencies, examples.
echo REFERENCE: Codeblocks project template: ~/499/openFrameworks/scripts/linux/template/emptyExample_linux_fullCBP.cbp
cd ~/499
#wget http://www.openframeworks.cc/versions/preRelease_v0.07/of_preRelease_v007_linux.tar.gz && tar zxvf of_preRelease_v007_linux.tar.gz
git clone --depth 1 git://github.com/openframeworks/openFrameworks.git
cd ~/499/openFrameworks/scripts/linux/ubuntu
sudo ./install_codeblocks.sh
sudo ./install_dependencies.sh 
sudo ./install_codecs.sh 

#echo
#echo (OPTIONAL) building all examples and addonexamples found in ~/499/openFrameworks/apps/examples/ & addonsExamples/
#cd ..
#./buildAllExamples.sh 
#./buildAllAddonExamples.sh

echo
echo Step 2: Install ofxOpenNI w/ examples app @ ~/499/openFrameworks/apps/addonsExamples/openNI-demoAllFeatures
cd ~/499/openFrameworks/addons/ && git clone --depth 1 https://github.com/gameoverhack/ofxOpenNI.git
#git clone --depth 1 git://github.com/murraymeehan/ofxMarsyas.git
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
#cd ~/499/openFrameworks/apps/ && git clone --depth 1 git://github.com/paulreimer/ofxMarsyasApps.git # ofxMarsyas and ofxMarsyasApps are MacOSX/iPhone only b/c of MSAlib/Cinder prereq.
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


