# blend
Blend 2 images into multiple instances of different alpha weight.

Example use of sod library.


![alt text](https://raw.githubusercontent.com/sandwichdoge/blend/master/Images/BlendEx.png)

# compile
gcc ../lib/sod/sod.c blend.c -lm -Ofast -march=native -Wall -std=c99 -o blend

# usage
<program_name> <input1> <input2> <n_output_files> <output_mask>
or
<program_name> -h|--help
