#!/bin/bash

function userInformation {
    echo Userul curent: $USER
    echo Calea home: $HOME
    echo Calea curenta: pwd
    echo Tipul de shell utilizat: $SHELL
}

function comupterInformation {
    echo Sistem de operare: `uname -o`
    #echo  `uname -r`
    echo Nume calculator: `uname -n`
    echo Numar identificare kernel: `uname -v`
}

function listLogs {
    
    for file in $1/*
    do
        if [ -d $file ];then
            listLogs $file
        elif [[ $file == *.log* && $file != *.gz ]];then
            echo $file                      
        fi
    done
}

function listLogsAndArchives {  #this recursive function will also print .gz files
    
    for file in $1/*
    do
        if [ -d $file ];then
            listLogsAndArchives $file
        elif [[ $file == *.log* ]];then
            echo $file                      
        fi
    done
}

function cautareText {       #this function will take the full path of a file after listing them
    if [[ $1 == *.gz ]];then
        echo $2
        gunzip -c $1 | grep -a $2   # -a for processing a binary file as if it were text
    elif [[ $1 == *log* ]];then
        echo $2
        grep $2 $1
    else
        echo Nu este fisier de log
    fi
}

while [[ $i != 0 ]];
do 
    echo 1.Afisare informatii user.
    echo 2.Numărul şi numele utilizatorilor login-aţi în sistem.
    echo 3.Afişarea tuturor shell-urilor, curent instalate în sistem.
    echo 4.Afişarea pentru toţi utilizatorii din sistem a numelui de login, numelui definit şi a directorului de home.
    echo 5.Afişare tip sistem de operare, nume calculator, număr de identificare kernel.
    echo 6.Afişare tip procesor şi frecvenţa de rulare.
    echo 7.Afişare memorie totală şi memorie liberă.
    echo 8.Afişare şi căutare în log-uri:
    echo 0.Exit
    echo Dati optiunea
    read i
    case $i in
    1)  userInformation;;
    2)  echo `who -q`;;     #all login names and number of users logged on
    3)  echo `cat /etc/shells`;;
    4)  awk -F ':' '{ print $1,$6 }' /etc/passwd;;      #-F sets a field separator :  and gives us fields 1=user 6=homeDir
    5)  comupterInformation;;
    6)  echo `grep "model name" /proc/cpuinfo | uniq | awk -F ':' '{ print $2 }' | awk '{ print $3,$6 }'`;;   # NR-row number
    7)  echo `grep MemTotal: /proc/meminfo`
        echo `grep MemFree: /proc/meminfo`;;
    #sub menu for a) and b) exercise 8   
    8) while [[ $option != 0 ]];
        do
            echo
            echo 1.Se vor afişa fişierele de log,doar cele curente
            echo 2. Se va selecta un fişier de log şi se va căuta un text citit de la tastatură.Căutarea se va realiza şi în fişierele arhivate ale logului respectiv
            echo 0.Inapoi
            read option
            case $option in
            1) listLogs /var/log;;
            2) listLogsAndArchives /var/log       #display logs first
               echo Dati fisier log
               read fisier
               echo Dati textul cautat
               read text  
               echo $text
               #cautareText $fisier $text;;
               if [[ $fisier == *.gz ]];then    
                    gunzip -c $fisier | grep -a "$text"   # -a for processing a binary file as if it were text
                elif [[ $fisier == *log* ]];then
                    grep "$text" $fisier
                else
                    echo Nu este fisier de log
                fi
            esac
            echo
        done
    esac
    echo 
done