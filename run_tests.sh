#! /bin/sh

# immediate exit on any failure
set -e

declare -a files=(
    "Map.cpp"
    "Map.h"
)

for f in "${files[@]}"; do
    cp ../"$f" .
done


# keyword check
if grep 'string\|double' Map.cpp
then
    echo "Map.cpp must not contain the word string or double"
    exit 2
fi

count_h=$(grep string Map.h | wc -l)
count_cpp=$(grep double Map.cpp | wc -l)
# echo $count_h $count_cpp
if [[ ! $count_h -eq 2 || ! $count_cpp -eq 0 ]]; then
    echo "Except in a typedef statement and
in the context of #include <string> in Map.h
the word string must not appear in Map.h or Map.cpp"
    exit 2
fi

global=$(grep '^\(class\|struct\)' Map.h | wc -l)
# echo $global
if [[ ! $global -eq 1 ]]; then
    echo "You must not declare any additional
struct/class outside the Map class"
    exit 2
fi

if grep '\(friend|\[\)' Map.h
then
    echo "The source files you submit for this project "
"must not contain the word friend or the character ["
    exit 2
fi


# Map   string => double
g++ -std=c++11 -Wall -Wextra \
-o testMap.out testMap.cpp Map.cpp

./testMap.out
rm testMap.out


# Map   int => string
if grep 'using[[:space:]]namespace' Map.h
then
    echo "It is not advised to have using namespace in header file"
    exit 1
fi

sed 's/^\(typedef.*\)std::string\(.*\)$/\1int\2/g' < Map.h > tmpMap.h
sed 's/^\(typedef.*\)double\(.*\)$/\1std::string\2/g' < tmpMap.h > Map.h
rm tmpMap.h

g++ -std=c++11 -Wall -Wextra \
-o testMap.out test-int2string.cpp Map.cpp

./testMap.out
rm testMap.out


# clean up
for f in "${files[@]}"; do
    rm "$f"
done

echo
echo "****** All Test Passed ******"
