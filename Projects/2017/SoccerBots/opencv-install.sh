# Clear some space:
sudo apt-get purge wolfram-engine
sudo apt-get clean
sudo apt-get install localepurge

sudo apt-get update
sudo apt-get upgrade

sudo apt-get install build-essential cmake pkg-config
sudo apt-get install libjpeg-dev libtiff5-dev libjasper-dev libpng12-dev


sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev
sudo apt-get install libxvidcore-dev libx264-dev

sudo apt-get install libgtk2.0-dev

sudo apt-get install libatlas-base-dev gfortran

sudo apt-get install python2.7-dev python3-dev

# That should be it for the dependencies

# Now get the OpenCV sources:
# At present (2017-09-17), v3.3.0 is the latest.

cd /usr/local/src
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git
cd opencv
git checkout tags/3.3.0
cd ../opencv_contrib
git checkout tags/3.3.0

# Then virtualenv setup using Python pip...

# ...

# Calling cmake:

cd /usr/local/src/opencv
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D INSTALL_PYTHON_EXAMPLES=ON \
    -D OPENCV_EXTRA_MODULES_PATH=/usr/local/src/opencv_contrib/modules \
    -D BUILD_EXAMPLES=ON ..


make -j 4


cd ~/.virtualenvs/cv/lib/python2.7/site-packages/
ln -s /usr/local/lib/python2.7/site-packages/cv2.so cv2.so

exit


# Oh dear

[ 86%] Building CXX object samples/cpp/CMakeFiles/cpp-tutorial-pnp_registration.dir/tutorial_code/calib3d/real_time_pose_estimation/src/PnPProblem.cpp.o
c++: internal compiler error: Killed (program cc1plus)
Please submit a full bug report,
with preprocessed source if appropriate.
See <file:///usr/share/doc/gcc-4.9/README.Bugs> for instructions.
modules/python2/CMakeFiles/opencv_python2.dir/build.make:324: recipe for target 'modules/python2/CMakeFiles/opencv_python2.dir/__/src2/cv2.cpp.o' failed
make[2]: *** [modules/python2/CMakeFiles/opencv_python2.dir/__/src2/cv2.cpp.o] Error 4
CMakeFiles/Makefile2:20896: recipe for target 'modules/python2/CMakeFiles/opencv_python2.dir/all' failed
make[1]: *** [modules/python2/CMakeFiles/opencv_python2.dir/all] Error 2
make[1]: *** Waiting for unfinished jobs....
[ 86%] Building CXX object samples/cpp/CMakeFiles/cpp-tutorial-pnp_registration.dir/tutorial_code/calib3d/real_time_pose_estimation/src/RobustMatcher.cpp.o
[ 86%] Building CXX object samples/cpp/CMakeFiles/cpp-tutorial-pnp_registration.dir/tutorial_code/calib3d/real_time_pose_estimation/src/Utils.cpp.o
[ 86%] Building CXX object modules/stitching/CMakeFiles/opencv_perf_stitching.dir/perf/perf_stich.cpp.o
[ 86%] Linking CXX executable ../../bin/cpp-tutorial-pnp_registration
[ 86%] Built target cpp-tutorial-pnp_registration
[ 86%] Linking CXX executable ../../bin/opencv_perf_stitching
[ 86%] Built target opencv_perf_stitching
[ 86%] Linking CXX shared module ../../lib/python3/cv2.cpython-34m.so
[ 86%] Built target opencv_python3
Makefile:160: recipe for target 'all' failed
make: *** [all] Error 2

# Out of memory while compiling, maybe?

# Yep:

[26030.153193] Out of memory: Kill process 22510 (cc1plus) score 328 or sacrifice child
[26030.153206] Killed process 22510 (cc1plus) total-vm:348648kB, anon-rss:300820kB, file-rss:0kB, shmem-rss:0kB

# Tried again without -j 4, and it got there.  It was building the cv2.cpp.o object that was the big obstacle (took some minutes for that one file!).

# The build folder after compiling contains about 3.4 GB of data, so that much will need to be left free before building.

