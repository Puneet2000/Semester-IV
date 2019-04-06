ratios=(3 5 10 15 20)
filename="inp-params.txt"
for ratio in ${ratios[@]}
do
	echo "For $ratio"
	> "$filename"
	echo "$ratio 10 1 2" > $filename 
	./a.out
done