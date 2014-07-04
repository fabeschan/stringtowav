// chanfabi luosteve

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wav_lib.h"
#include "song_generator.h"
#include <string.h>

#define NOTE_OFFSET 'A'
#define INT_OFFSET '1'
#define UNIT_LEN 7500
#define SAMPLE_RATE 22050
#define BYTES_PER_SAMPLE 2

extern int memory(const char*);
double freq[7] = {440.0, 493.883, 261.626, 293.665, 329.628, 349.228, 391.995};

typedef struct channel
{
    char *note_str; // stores a note string for a particular channel
    short int *samples; // pointer to an array of samples of the channel
    struct channel *next; // pointer to the next node in the linked list
} channel_t;

/* split_note_string -- helper function
 *  Read in a string of notes and points the note_str of each node to an
 *  allocated note string for each channel, which are separated by "|"s.
 *
 *   Arguments:
 *       note_string - the string of notes of which to read from
 *       first - the pointer to the first element of the linked list
 *
 *   Returns:
 *       The number of channels present in the string of notes.
 */
int split_note_string(const char *note_string, channel_t *first)
{
    char *temp = (char*) note_string, *cur_char = (char*) note_string;
    channel_t *cur_str = first;
    int num = 0, i, num_channels = 0;

    while (*temp)
    {
        if (*temp != '|') num++;

        else
        {
            if ((cur_str->note_str = malloc(num + 1)) == NULL) return -1;
            if ((cur_str->next = malloc(sizeof(channel_t))) == NULL) return -1;
            cur_str = cur_str->next;
            // allocate enough memory, set cur_str to it, then make a new node
            num = 0;
            num_channels++;
        }

        temp++;
    }

    if ((cur_str->note_str = malloc(num + 1)) == NULL) return -1;
    // allocate memory for the last channel

    cur_str = first;
    i = 0;
    while (*cur_char)
    // copy the note string of each channel to seperate nodes
    {
        if (*cur_char != '|')
        {
            *((cur_str->note_str) + i) = *cur_char;
            i++;
        }

        else
        {
            *(cur_str->note_str + i) = '\0';
            cur_str = cur_str->next;
            i = 0;
        }

        cur_char++;
    }

    *(cur_str->note_str + i) = '\0';
    // write the null character for the last channel

    return num_channels + 1;
}


/* assimilate_arrays -- helper function
 * Add two arrays element by element, resulting in a modified
 * main array.
 *
 * Precondition: the size of main_arr is larger than or equal to
 * the size of add_arr
 *
 * Arguments:
 * main_arr - the array to be modified with the summations of each of
 * its elements with the corresponding elements of add_arr
 * add_arr - the array whose elements are to be used to modify main_arr
 * add_len - number of elements in add_arr
 */
void assimilate_arrays(short int *main_arr, short int *add_arr, int add_len)
{
    short int *current = main_arr, *temp = add_arr;
    while (add_len)
    {
        *current++ += *temp++;
        add_len--;
    }
}


/* free_channels -- helper function
 * Frees the previously allocated memory used to store the note strings for
 * each channel, the memory used to store the samples for each channel,
 * and the memory used for the structs (elements of the linked
 * list) that contain pointers to the note strings and samples.
 *
 * Arguments:
 * first - the pointer to the first element of the linked list
 * num - the number of elements in the linked list
 */
void free_channels(channel_t *first, int num)
{
    channel_t *previous, *current = first;
    while (num)
    {
        previous = current;
        free(previous->note_str);
        free(previous->samples);
        free(previous);
        current = current->next;
        num--;
    }
}

/* See .h file for full description. */
int generate_song(const char *note_string, const char *output_file)
{
    channel_t primer, *cur_node = &primer;
    int length = 1, i, j, max_sound_bytes = 0, cur_channel_bytes,
        num_channels = split_note_string(note_string, &primer);
    const char *cur_char = cur_node->note_str;
    double octave, cur_freq;
    short int *cur_sample, *buffer;

    j = num_channels;
    while (j)
    {
        octave = 1;
        cur_channel_bytes = memory(cur_node->note_str);
        max_sound_bytes = max_sound_bytes > cur_channel_bytes ?
            max_sound_bytes : cur_channel_bytes;
        // determine the longest channel size

        cur_node->samples = malloc(cur_channel_bytes);
        if (cur_node->samples == NULL) return -1;

        cur_sample = (short int*) cur_node->samples;
        cur_char = cur_node->note_str;

        while (*cur_char)
        // generate audio samples from each note string
        {
            if (*cur_char == '<' || *cur_char == '>')
            {
                if (*cur_char == '<') octave /= 2;
                if (*cur_char == '>') octave *= 2;
            }

            else if (*cur_char <= '9' && *cur_char >= '1')
            {
                length = (int) *cur_char - INT_OFFSET + 1;
            }

            else if (*cur_char == '.')
            {
                i = UNIT_LEN * length;
                while (i)
                {
                    *cur_sample++ = 0; // write silence (samples of zeros)
                    i--;
                }

                length = 1; // reset length after using it
            }

            else if (*cur_char >= 'A' && *cur_char <= 'G')
            {
                cur_freq = octave * freq[*cur_char - NOTE_OFFSET];

                gen_sin_samples(cur_freq, SAMPLE_RATE,
                                UNIT_LEN * length, cur_sample);
                cur_sample += UNIT_LEN * length;
                // shift pointer to end of samples generated
                length = 1;
            }

            cur_char++;
        }

        cur_node = cur_node->next; // set to note string of next channel
        j--;
    }

    i = 0;
    buffer = malloc(max_sound_bytes);
    if (buffer == NULL) return -1;
    while (i < max_sound_bytes / 2)
    {
        *(buffer + i) = 0; // set buffer to all zeros
        i++;
    }

    cur_node = &primer;
    while (num_channels)
    // mix the channels into one
    {
        assimilate_arrays(buffer, cur_node->samples,
                          memory(cur_node->note_str) / 2);
        cur_node = cur_node->next;
        num_channels--;
    }

    free_channels(&primer, num_channels);

    int samples_written = write_wav_file(output_file, (char*) buffer,
            SAMPLE_RATE, max_sound_bytes / BYTES_PER_SAMPLE, BYTES_PER_SAMPLE);

    if (samples_written != max_sound_bytes / BYTES_PER_SAMPLE) return -1;

    free(buffer);

    return 0;
}
