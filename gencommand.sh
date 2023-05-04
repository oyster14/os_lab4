for i in `seq 0 9`; do
    echo "\e[1;42m N ${i} \e[0m"
    ./iosched -sN ./lab4_assign/input${i} > my_out/my_out_${i}_N
    diff ./lab4_assign/out_${i}_N ./my_out/my_out_${i}_N
    echo "\e[1;42m S ${i} \e[0m"
    ./iosched -sS ./lab4_assign/input${i} > my_out/my_out_${i}_S
    diff ./lab4_assign/out_${i}_S ./my_out/my_out_${i}_S
done