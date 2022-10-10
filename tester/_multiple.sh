clang++ -g -fsanitize=address tester_main.cpp

echo "MULTIPLE TEST" > multiple_results.txt

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

for i in {10219..1000000}
do
	./a.out ft $i 1> ft.txt 2> kk.txt
	if [ $? -eq 0 ]; then
		# echo $i":" OK >> multiple_results.txt
		echo -e $i":" "${GREEN}OK${NC}"
	else
		echo $i":" KO >> multiple_results.txt
		echo -e $i":" "${RED}FAIL${NC}"
	fi
done