for i in `seq 10 12`; do
    echo "\e[1;42m N ${i} \e[0m"
    ./lab4_assign/iosched -sN ./lab4_assign/input${i} > my_out/prof_out_${i}_N
    ./iosched -sN ./lab4_assign/input${i} > my_out/my_out_${i}_N
    diff ./my_out/prof_out_${i}_N ./my_out/my_out_${i}_N
    echo "\e[1;42m S ${i} \e[0m"
    ./lab4_assign/iosched -sS ./lab4_assign/input${i} > my_out/prof_out_${i}_S
    ./iosched -sS ./lab4_assign/input${i} > my_out/my_out_${i}_S
    diff ./my_out/prof_out_${i}_S ./my_out/my_out_${i}_S
    echo "\e[1;42m L ${i} \e[0m"
    ./lab4_assign/iosched -sL ./lab4_assign/input${i} > my_out/prof_out_${i}_L
    ./iosched -sL ./lab4_assign/input${i} > my_out/my_out_${i}_L
    diff ./my_out/prof_out_${i}_L ./my_out/my_out_${i}_L
    echo "\e[1;42m C ${i} \e[0m"
    ./lab4_assign/iosched -sC ./lab4_assign/input${i} > my_out/prof_out_${i}_C
    ./iosched -sC ./lab4_assign/input${i} > my_out/my_out_${i}_C
    diff ./my_out/prof_out_${i}_C ./my_out/my_out_${i}_C
    echo "\e[1;42m F ${i} \e[0m"
    ./lab4_assign/iosched -sF ./lab4_assign/input${i} > my_out/prof_out_${i}_F
    ./iosched -sF ./lab4_assign/input${i} > my_out/my_out_${i}_F
    diff ./my_out/prof_out_${i}_F ./my_out/my_out_${i}_F
done