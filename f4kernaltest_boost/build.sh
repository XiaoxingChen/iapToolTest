cd $(cd "$(dirname "$0")"; pwd)
cd build
cmake .. -DBOOST_INCLUDEDIR=/usr/local/include/ -DBOOST_LIBRARYDIR=/usr/local/lib/
make
