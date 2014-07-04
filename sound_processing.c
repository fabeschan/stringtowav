// chanfabi luosteve

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wav_lib.h"

#define DATA_OFFSET    44

/* reverse_array -- helper function
 *  Reverse the order of an array of chars two element by two element.
 *
 *   Arguments:
 *       array - the array of elements to be reversed
 *       num - the number of elements in array
 */
void reverse_array(char *array, int num)
{
    short int *start, *end, temp;
    start = (short int*) array;
    end = (short int*) &array[num - 2];

    while (start < end)
    {
        temp = *start;
        *start++ = *end;
        *end-- = temp;
    }
}

/* See .h file for full description. */
int reverse(const char *input, const char *output)
{
    FILE *input_file = fopen(input, "rb");
    unsigned sample_rate = get_sample_rate(input_file);
    unsigned short bytes_per_sample = get_bytes_per_sample(input_file);
    unsigned data_len = get_data_length(input_file);
    unsigned num_samples = data_len / bytes_per_sample;

    char *audio_samples;

    if ((audio_samples = malloc(data_len)) == NULL)
    {
        fclose(input_file);
        return -1;
    }

    read_wav_samples(input_file, audio_samples, num_samples);
    fclose(input_file);

    reverse_array(audio_samples, data_len);

    int samples_written = write_wav_file(output, audio_samples,
                   sample_rate, num_samples, bytes_per_sample);

    if (samples_written != num_samples) return -1;
    // check for discrepancies between number of samples to be
    // written and the number of samples actually written

    free(audio_samples);
    return 0;
}

/* sum_arrays -- helper function
 *  Add two arrays together element by element.
 *
 *   Arguments:
 *       longer - the array with more elements
 *       long_num - the number of elements in longer
 *       shorter - the array with the same number of or less elements
 *                 than longer
 *       short_num - the number of elements in shorter
 *       summation - the resultant array from the summation of longer
 *                   and shorter
 */
void sum_arrays(short int *longer, unsigned long_num, short int *shorter,
                unsigned short_num, short int *summation)
    {
        short int *temp = summation;
        while (long_num)
        // copy all elements from the longer array to the resultant array
        {
            *temp++ = *longer++;
            long_num--;
        }

        while (short_num)
        // add each element from the shorter array to that of the resultant
        {
            *summation++ += *shorter++;
            short_num--;
        }
    }

/* See .h file for full description. */
int mix(const char *input1, const char *input2, const char *output)
{
    FILE *input_file1 = fopen(input1, "rb"), *input_file2 = fopen(input2, "rb");
    unsigned data_len1 = get_data_length(input_file1);
    unsigned data_len2 = get_data_length(input_file2);
    unsigned data_len = data_len1 > data_len2 ? data_len1:data_len2;
    // assign the longer of the two input data_lens to data_len
    unsigned sample_rate = get_sample_rate(input_file1);
    unsigned short bytes_per_sample = get_bytes_per_sample(input_file1);
    unsigned num_samples = data_len / bytes_per_sample;
    unsigned num_samples1 = data_len1 / bytes_per_sample;
    unsigned num_samples2 = data_len2 / bytes_per_sample;

    char *wav1 = malloc(data_len1);
    if (wav1 == NULL) return -1;
    char *wav2 = malloc(data_len2);
    if (wav2 == NULL) return -1;
    char *wavout = malloc(data_len);
    if (wavout == NULL) return -1;

    read_wav_samples(input_file1, wav1, num_samples1);
    read_wav_samples(input_file2, wav2, num_samples2);

    fclose(input_file1);
    fclose(input_file2);

    short int *wav_1 = (short int*) wav1;
    short int *wav_2 = (short int*) wav2;
    short int *wav_out = (short int*) wavout;

    if (data_len1 > data_len2)
    {
        sum_arrays(wav_1, num_samples1, wav_2, num_samples2, wav_out);
    }

    else sum_arrays(wav_2, num_samples2, wav_1, num_samples1, wav_out);

    free(wav1);
    free(wav2);

    int samples_written = write_wav_file(output, wavout, sample_rate,
                                         num_samples, bytes_per_sample);
    if (samples_written != num_samples) return -1;
    // check for discrepancies between number of samples to be
    // written and the number of samples actually written

    free(wavout);
    return 0;
}

/* See .h file for full description. */
int change_volume(const char *input, double factor, const char *output)
{
    FILE *input_file = fopen(input, "rb");
    unsigned sample_rate = get_sample_rate(input_file);
    unsigned short bytes_per_sample = get_bytes_per_sample(input_file);
    unsigned data_len = get_data_length(input_file);
    unsigned num_samples = data_len / bytes_per_sample;

    char *buffer = malloc(data_len);
    if (buffer == NULL) return -1;

    read_wav_samples(input_file, buffer, num_samples);
    fclose(input_file);

    short int *current = (short int*) buffer;
    int i = num_samples;
    while (i)
    {
        *current = (short int) *current * factor;
        current++;
        i--;
    }

    write_wav_file(output, buffer, sample_rate, num_samples, bytes_per_sample);

    free(buffer);
    return 0;
}
