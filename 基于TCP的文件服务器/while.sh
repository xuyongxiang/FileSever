#!/bin/bash
arr=("/home/ubuntu/fir-dir/" "/home/ubuntu/dir-dir/")
i=0
while  [  $i -lt ${#arr[*]} ]
do
	if [ -e ${arr[$i]} ]
	then
		
	    echo "是否需要清空${arr[$i]}目录:【Y/N】"
		read choose
		case $choose in
			"y"|"Y"|"Yes"|"yes"|"YES")
				rm -r ${arr[$i]}/*
				echo "目录${arr[$i]}已经清空"
				;;
			"n"|"N"|"No"|"no"|"NO")
				echo "目录${arr[$i]}没有清空"
				;;
				*)
				echo "输入有误"
				;;
		esac
	else
		mkdir ${arr[$i]}
		echo "新建了${arr[$i]}目录"
	fi
	((i++))
done

echo "input a path:"
read path
if [ -e $path ]
then
	arr1=(`ls $path`)
	j=0
	file=0
	dir=0
	while [ $j -lt ${#arr1[*]} ]
	do
		if [[ -f $path/${arr1[$j]} ]] && [[ -r $path/${arr1[$j]}  ]]
		then
            cp $path/${arr1[$j]} ${arr[0]} -r
			((file++))
		fi

        if [[ -d $path/${arr1[$j]} ]] && [[ -r $path/${arr1[$j]}  ]]
		then
            cp $path/${arr1[$j]} ${arr[1]} -r
			((dir++))
		fi
		((j++))
	done
	echo "拷贝了$file份文件"
	echo "拷贝了$dir份目录"
else
	echo "输入的路径不存在"
fi

