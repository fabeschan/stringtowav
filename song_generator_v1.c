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

double freq[7] = {440.0, 493.883, 261.626, 293.665, 329.628, 349.228, 391.995};
extern int memory(const char*);

/* See .h file for full description. */
int generate_song(const char *note_string, const char *output_file)
{
    int length = 1, i, channel_bytes = memory(note_string);
    char *audio_samples = malloc(channel_bytes);
    if (audio_samples == NULL) return -1;

    char *cur_char;
    cur_char = (char*) note_string;

    double octave = 1, cur_freq;
    short int *cur_sample = (short int*) audio_samples;

    while (*cur_char)
    {
        if (*cur_char == '<' || *cur_char == '>')
        {
            if (*cur_char == '<') octave /= 2;
            if (*cur_char == '>') octave *= 2;
        }

        else if (*cur_char <= '9' && *cur_char >= '1')
        {
            length = (int) *cur_char - INT_OFFSET + 1;
            // convert to int from ASCII
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
            // translate to int from ASCII, with 'A' as 0

            gen_sin_samples(cur_freq, SAMPLE_RATE,
                            UNIT_LEN * length, cur_sample);

            cur_sample += UNIT_LEN * length;
            // shift to the end of samples generated
            length = 1; // reset length after using it
        }

        cur_char++;
    }

    int samples_written = write_wav_file(output_file, audio_samples, SAMPLE_RATE,
                    channel_bytes / BYTES_PER_SAMPLE, BYTES_PER_SAMPLE);

    if (samples_written != channel_bytes / BYTES_PER_SAMPLE) return -1;
    // check for discrepancies between number of samples to be
    // written and the number of samples actually written

    return 0;
}
