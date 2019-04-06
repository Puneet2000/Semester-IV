ratios=(0.2 0.4 0.8 1.2 1.6 2 4 8 12 16 20)
filename="inp-params.txt"
X=2
for ratio in ${ratios[@]}
do
	echo "For $ratio"
	> "$filename"
	echo "100 10 10 10 10 $ratio $X" > $filename 
	./mex && ./sem
done