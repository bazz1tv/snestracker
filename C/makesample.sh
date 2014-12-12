#!/bin/bash

WAVname=""
Basename=""
TargetSampleRate=32000
Interpolation=
LoopPoint=
LoopFlag=""
FilterFlag=""
WrapFlag=""
ResampleFlag=""
FilterChoice=""

Wrap () 
{
	printf 'Do you want to disable wrapping (y/n): '
	read DisableWrap_yn
	if [ "$DisableWrap_yn" = "y" ] ; then
		WrapFlag="-w"
	fi
}

Resample () 
{
	printf 'Would you like to resample? (y/n): '
	read sample_yn

	if [ "$sample_yn" = "y" ] ; then
		ResampleFlag="-s"
		printf "\nWhat's the Target Sample Rate?\n"
		printf "Default 32000 (Hz): "
		read TargetSampleRate
		if [ -z $TargetSampleRate ] ; then
			TargetSampleRate=32000
		fi
		printf "\nWhat kind of Interpolation do you want?\n"
		echo '1) None'
		echo '2) Linear'
		echo '3) Sine'
		echo '4) Cubic'
		read Interpolation
	
		if [ "$Interpolation" = "1" ] ; then
			ResampleFlag="-sn"
		elif [ "$Interpolation" = "2" ] ; then
			ResampleFlag="-sl"
		elif [ "$Interpolation" = "3" ] ; then
			ResampleFlag="-ss"
		elif [ "$Interpolation" = "4" ] ; then
			ResampleFlag="-sc"
		fi
	
		ResampleFlag="${ResampleFlag}$TargetSampleRate"
	fi
}

Loop () 
{
	printf 'Would you like to Loop (y/n): '
	read Loop_yn
	if [ "$Loop_yn" = "y" ] ; then
		LoopFlag="-l"
		echo ''
		printf "Do you know the Loop point? (y/n): "
		read know_loop_point_yn
		if [ "$know_loop_point_yn" = "y" ] ; then
			echo ''
			printf "Enter Loop point: "
			read LoopPoint
			LoopFlag="$LoopFlag$LoopPoint"
		fi
	fi
}

Filters ()
{
	printf "Now time for Filters"
	echo ''
	echo 'What Filters do you want to use? (0123)'
	printf ": "
	read FilterChoice
	if [ -z $FilterChoice ] ; then
		FilterFlag="-f0123"
	else
		FilterFlag="-f$FilterChoice"
	fi
}

GetBase ()
{
	filename=$(basename "$WAVname")
	#extension="${filename##*.}"
	Basename="${filename%.*}"
}

echo 'Welcome'

echo " Let's get Started getting your samples to SNES! "

echo ''
printf 'Type WAV filename: '
read WAVname
GetBase
#echo "$Basename.brr"
BRRname="$Basename.brr"
echo "Great. We will create a $BRRname file for you"

echo ''
Resample

echo ''
Loop

echo ''
Filters

echo ''
Wrap
#Target Call
#Variably shape the call

echo "java -cp BRRTools/bin/ brr.BRREncoder $WAVname $BRRname $LoopFlag $WrapFlag $FilterFlag $ResampleFlag"
java -cp BRRTools/bin/ brr.BRREncoder $WAVname $BRRname $LoopFlag $WrapFlag $FilterFlag $ResampleFlag


