outdir=$1
total=31
mkdir -p $1
FILE=$2
if [[ ! -f "$FILE" ]]
	then 
		echo "The executable you mentioned does not exist. Please check the name and try again."
		exit 1
fi
for i in $( eval echo {1..$total})
do 
	./preprocess.sh
	FILE=inputs/input${i}
	if [[ ! -f "$FILE" ]]
		then 
			echo "Input $i is missing, please fix before proceeding"
			exit 1
	fi
	$(cat < inputs/input${i} | ./$2 > $1/output${i} 2> $1/erroroutput${i})
done
printf "$2 run sucessfully on all inputs\n\nGrading against reference outputs:\n" 

count=0
errorlog=""
result=""
erroresult=""
for i in $( eval echo {1..$total})
	do 
		FILE=$3/refoutput${i}
		FILE2=$3/referroroutput${i}
		if [[ ! -f "$FILE" || ! -f "$FILE2" ]]
			then
				echo -n "0 "
				errorlog+="Reference output for input $i is missing.\n\n"
				continue
		fi
		result=""
		erroresult=""
		output=$(cat < $1/output${i})
		refoutput=$(cat < $3/refoutput${i})
		erroroutput=$(cat < $1/erroroutput${i})
		referroroutput=$(cat < $3/referroroutput${i})
		result=$(diff $3/refoutput${i} $1/output${i})
		errorresult=$(diff $3/referroroutput${i} $1/erroroutput${i})
		if [ "${result}" == "" ] && [ "${errorresult}" == "" ]
			then	
				echo -n "1 "
				count=$((count+1))
				continue
		else
			echo -n "0 "
			errorlog+="Error in input $i\n"
			if [ "${result}" != "" ]
				then
					errorlog+="Your output:\n$output\nExpected output:\n$refoutput\nDifference: $result\n\n"
			fi
			if [ "${errorresult}" != "" ]
				then
					errorlog+="Your output:\n$erroroutput\nExpected output:\n$referroroutput\nDifference: $errorresult\n\n"
			fi
		fi
	done
printf "\n\nResult $count out of $total\n\n"
if [ "${count}" != "${total}" ]
then
	printf "Error Log:\n$errorlog"
fi
echo "Please note that these cases are NOT exhaustive. The test cases for final grading will be different than the ones provided and will not be shared. Commands like job and fg are not tested extensively in these test cases, but they will be for the final grading."
