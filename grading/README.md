Extract all files from this archive into a folder called "grading" and run the commands there. Ensure that you copy  your executable to the "grading" folder as well.

In order to run the reference outputs, please use the following commands:

chmod +777 preprocess.sh gradeit.sh
module load gcc-9.2
make
./gradeit.sh <output directory> <name of executable> <reference output directory>

For example, the fourth command can be:
./gradeit.sh myoutputs nyush refoutputs

Please note that these cases are NOT exhaustive. The test cases for final grading will be different than the ones provided and will not be shared. Commands like `job` and `fg` are not tested extensively in these test cases, but they will be for the final grading. 
