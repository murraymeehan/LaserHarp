# Generate an x-y-z position logfile suitable for octave/matlab/excel analysis and plotting
./LaserHarp | grep , | grep -v Hand > logfile
echo data=[ > logfile.m
cat logfile1.m >> logfile.m
echo ]; >> logfile.m
#rm logfile1.m
echo To use the logfile, run 'octave' and type 'logfile' to read data into variable 'data'.
