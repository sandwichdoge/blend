#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/sod/sod.h"

//create 4 blended instances of 2 images
//compile:
//gcc ../lib/sod/sod.c blend.c -lm -Ofast -march=native -Wall -std=c99 -o blend
//more info: https://sod.pixlab.io/api.html

char *file_get_extension(char *str)
{
  char *ext = strrchr(str, '.');
  return ext;
}

char *filename_without_ext(char *str)
{
    char *ret = malloc(strlen(str));
    char *ext = strrchr(str, '.');
    if (ext) {
        int pos = ext - str;
        memcpy(ret, str, pos);
        ret[pos] = '\0';
    }
    else strcpy(ret, str);
    return ret;
}

int blend_images(char *fpath1, char *fpath2, char *output_mask, int output_count)
{
	char *output_ext = file_get_extension(output_mask);
	char *output_without_ext = filename_without_ext(output_mask);
	sod_img img_in = sod_img_load_from_file(fpath1, 3); /*0=full color channels */
  sod_img img_in2 = sod_img_load_from_file(fpath2, 3); /*3 channels for compatibility*/
	if (img_in.data == 0 || img_in2.data == 0) {
		/*invalid path, unsupported format, memory failure, etc.*/
		printf("cannot load input images..exiting\n");
		return -1;
	}

	float alpha_step = (float)(1.0) / (float)(output_count + 1);  //last step will be bg pic
	char buf[128];  //to format output filename
	char index[24];  //to format output filename

	for (int i = 1; i <= output_count; i++) {
		/*blend 2 images
		nothing fancy, just read all pixels to array and average out their RGB channels based on alpha weight*/
		sod_img ret = sod_blend_image(img_in2, img_in, alpha_step * i);
		/*format output filename*/
		sprintf(index, "%d", i);
		strcpy(buf, output_without_ext);
		strcat(buf, index);
		strcat(buf, output_ext);
		/*write result to output file*/
		if (strcmp(output_ext, ".png") == 0) sod_img_save_as_png(ret, buf);
		else if (strcmp(output_ext, ".jpg") == 0 || 
			strcmp(output_ext, ".jpeg") == 0) sod_img_save_as_jpeg(ret, buf, 100);		
		/*clean up*/
		sod_free_image(ret);
	}

	/*clean up*/
	sod_free_image(img_in);
  sod_free_image(img_in2);
	free(output_without_ext);
	return 0;
}

void show_help(char *prog_name)
{
	printf("blend 2 images\n"
	"usage  : %s <input1> <input2> <n_output_files> <output_mask>\n"
	"example: %s img1.png img2.png 3 output.png\n", prog_name, prog_name);
	return;
}

int main(int argc, char *argv[])
{
	char *input1 = argv[1];
  char *input2 = argv[2];
	char *output_count_r = argv[3];
	char *output = argc > 4 ? argv[4] : "out.png";  // no arg4 -> default output to out.png

	if (argc < 2 || strcmp(input1, "--help") == 0 || strcmp(input1, "-h") == 0) show_help(argv[0]);

	int output_count;  //number of output instances
	sscanf(output_count_r, "%d", &output_count);

	blend_images(input1, input2, output, output_count);

	
	return 0;
}