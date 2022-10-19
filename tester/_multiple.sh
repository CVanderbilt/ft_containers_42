clang++ -g -fsanitize=address *.cpp -o multiple.out

# echo "MULTIPLE TEST" > multiple_results.txt

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

for i in {22311..1000000}
do
	./multiple.out ft $i > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		# echo $i":" OK >> multiple_results.txt
		echo -e $i":" "${GREEN}OK${NC}"
	else
		echo $i":" KO >> multiple_results.txt
		echo -e $i":" "${RED}FAIL${NC}"
	fi
done