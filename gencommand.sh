for i in `seq 0 9`; do
    echo "\e[1;42m N ${i} \e[0m"
    ./iosched -sN ./lab4_assign/input${i} > my_out/my_out_${i}_N
    diff ./lab4_assign/out_${i}_N ./my_out/my_out_${i}_N
    echo "\e[1;42m S ${i} \e[0m"
    ./iosched -sS ./lab4_assign/input${i} > my_out/my_out_${i}_S
    diff ./lab4_assign/out_${i}_S ./my_out/my_out_${i}_S
    echo "\e[1;42m L ${i} \e[0m"
    ./iosched -sL ./lab4_assign/input${i} > my_out/my_out_${i}_L
    diff ./lab4_assign/out_${i}_L ./my_out/my_out_${i}_L
    echo "\e[1;42m C ${i} \e[0m"
    ./iosched -sC ./lab4_assign/input${i} > my_out/my_out_${i}_C
    diff ./lab4_assign/out_${i}_C ./my_out/my_out_${i}_C
    echo "\e[1;42m F ${i} \e[0m"
    ./iosched -sF ./lab4_assign/input${i} > my_out/my_out_${i}_F
    diff ./lab4_assign/out_${i}_F ./my_out/my_out_${i}_F
    
done