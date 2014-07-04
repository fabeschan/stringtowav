// chanfabi luosteve

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "wav_lib.h"


/* The ratio of a circle's circumference to its diameter -- redefined to avoid
 * the overhead of the call to acos() each time it is used.
 */
#define PI                       3.141592653589793

/* Maximum numerical value for a sample. */
#define MAX_SAMPLE_VALUE         0x7FFF

/* The offsets and sizes (in bytes) of the various header fields in a WAV file
 * with fixed-size FORMAT section, in order.
 */

#define RIFF_TEXT_OFFSET         0
#define RIFF_TEXT_SIZE           4

#define RIFF_LENGTH_OFFSET       (RIFF_TEXT_OFFSET + RIFF_TEXT_SIZE)
#define RIFF_LENGTH_SIZE         4

#define WAVE_TEXT_OFFSET         (RIFF_LENGTH_OFFSET + RIFF_LENGTH_SIZE)
#define WAVE_TEXT_SIZE           4

#define FORMAT_TEXT_OFFSET       (WAVE_TEXT_OFFSET + WAVE_TEXT_SIZE)
#define FORMAT_TEXT_SIZE         4

#define FORMAT_LENGTH_OFFSET     (FORMAT_TEXT_OFFSET + FORMAT_TEXT_SIZE)
#define FORMAT_LENGTH_SIZE       4

#define PADDING_OFFSET           (FORMAT_LENGTH_OFFSET + FORMAT_LENGTH_SIZE)
#define PADDING_SIZE             2

#define CHANNELS_OFFSET          (PADDING_OFFSET + PADDING_SIZE)
#define CHANNELS_SIZE            2

#define SAMPLE_RATE_OFFSET       (CHANNELS_OFFSET + CHANNELS_SIZE)
#define SAMPLE_RATE_SIZE         4

#define BYTES_PER_SECOND_OFFSET  (SAMPLE_RATE_OFFSET + SAMPLE_RATE_SIZE)
#define BYTES_PER_SECOND_SIZE    4

#define BYTES_PER_SAMPLE_OFFSET  (BYTES_PER_SECOND_OFFSET + BYTES_PER_SECOND_SIZE)
#define BYTES_PER_SAMPLE_SIZE    2

#define BITS_PER_SAMPLE_OFFSET   (BYTES_PER_SAMPLE_OFFSET + BYTES_PER_SAMPLE_SIZE)
#define BITS_PER_SAMPLE_SIZE     2

#define DATA_TEXT_OFFSET         (BITS_PER_SAMPLE_OFFSET + BITS_PER_SAMPLE_SIZE)
#define DATA_TEXT_SIZE           4

#define DATA_LENGTH_OFFSET       (DATA_TEXT_OFFSET + DATA_TEXT_SIZE)
#define DATA_LENGTH_SIZE         4

#define DATA_OFFSET              (DATA_LENGTH_OFFSET + DATA_LENGTH_SIZE)
// no DATA_SIZE constant, since it is not fixed

#define UNIT_LEN 7500
#define INT_OFFSET '1'

/* memory -- helper function
 *   Calculate the memory needed for writing in the samples.
 *   Precondition:
 *       string contains only a combination of letters "A" to "G",
 *       ">", "<", numbers "1" to "9", and ".".
 *
 *   Arguments:
 *       string - a string containing notes and symbols
 *
 *   Returns:
 *       the number of bytes required to store the samples created through
 *       interpretation of string
 */
int memory(const char *string)
{
    int num_bytes = 0, length = 1;
    char *cur_char = (char*) string;

    while (*cur_char)
    {
        if (*cur_char >= 'A' && *cur_char <= 'G')
        {
            num_bytes += UNIT_LEN * 2;
        }
        else if (*cur_char == '.')
        {
            num_bytes += UNIT_LEN * 2;
        }
        else if (*cur_char >= '1' && *cur_char <= '9')
        {
            length = (int) (*cur_char - INT_OFFSET);
            num_bytes += UNIT_LEN * 2 * length;
        }

        cur_char++;
    }

    return num_bytes;
}


/* same_sign -- helper function
 *  Return 1 iff x and y are both positive or both negative.
 *
 *  Arguments:
 *      x, y - values to compare
 *
 *  Returns:
 *      1 if x and y have the same sign;
 *      0 otherwise (including when one (or both) of x, y is (are) equal to 0)
 */
int same_sign(short x, short y)
{
    return (x < 0 && y < 0) || (x > 0 && y > 0);
}


/* See .h file for full description.
 *  DO NOT MODIFY THIS FUNCTION IN ANY WAY. YOU MUST INCLUDE AND USE THIS
 *  FUNCTION IN YOUR SOLUTION. SEE PROJECT HANDOUT FOR DETAILS.
 */
void gen_sin_samples(double freq, double sample_rate, unsigned num_samples,
                     short *sample_buffer)
{
    // Number of samples required for .05 seconds of silence.
    const unsigned SILENCE_SAMPLES = (unsigned) (sample_rate * .05);
    unsigned i;

    // Fill in the requested number of pure sin wave samples, with volume 1/10th
    // of maximum.
    for (i = 0; i < num_samples; ++i)
        sample_buffer[i] = (short) ( MAX_SAMPLE_VALUE / 10.0
                                      * sin(2 * PI * freq * i / sample_rate) );
        // Note that (2 * PI * freq * i / sample_rate)
        //   = 2 * PI * freq * (num_samples / sample_rate) * (i / num_samples)
        // and (num_samples / sample_rate) = number of seconds.

    // Check if there is enough time for the silence.
    if (num_samples <= SILENCE_SAMPLES)  return;
    // Figure out when the sign of the samples changes during the silence.  This
    // indicates when the samples are closest to 0, so starting the silence then
    // will lead to a cleaner cut-off.
    for (i = num_samples - SILENCE_SAMPLES;
         i < num_samples && same_sign(sample_buffer[i-1], sample_buffer[i]);
         ++i) ;
    // Replace the remaining samples with silence.
    while (i < num_samples)  sample_buffer[i++] = 0;
}


/* read_bytes_from_offset -- helper function
 *  Read num_bytes from input, starting at offset and return the value read as
 *  an unsigned int.
 *
 *  Precondition: num_bytes <= sizeof(unsigned)
 *
 *  Arguments:
 *      input - the file to read data from
 *      offset - the byte offset to start reading from input
 *      num_bytes - the total number of bytes requested
 *
 *  Returns:
 *      the bit pattern contained in the bytes:
 *          offset, offset + 1, ..., offset + num_bytes
 *      of input, stored in an unsigned value;
 *      0 if a read error occurs
 */
unsigned read_bytes_from_offset(FILE *input, int offset, int num_bytes)
{
    unsigned read_buffer = 0;
    if (fseek(input, offset, SEEK_SET) == -1)  return 0;
    if (fread(&read_buffer, num_bytes, 1, input) != 1)  return 0;
    return read_buffer;
}


/* See .h file for full description. */
unsigned short get_num_channels(FILE *wav_file)
{
    return (unsigned short) read_bytes_from_offset(wav_file,
            CHANNELS_OFFSET, CHANNELS_SIZE);
}


/* See .h file for full description. */
unsigned get_sample_rate(FILE *wav_file)
{
    return (unsigned) read_bytes_from_offset(wav_file,
            SAMPLE_RATE_OFFSET, SAMPLE_RATE_SIZE);
}


/* See .h file for full description. */
unsigned get_bytes_per_second(FILE *wav_file)
{
    return (unsigned) read_bytes_from_offset(wav_file,
            BYTES_PER_SECOND_OFFSET, BYTES_PER_SECOND_SIZE);
}


/* See .h file for full description. */
unsigned short get_bytes_per_sample(FILE *wav_file)
{
    return (unsigned short) read_bytes_from_offset(wav_file,
            BYTES_PER_SAMPLE_OFFSET, BYTES_PER_SAMPLE_SIZE);
}


/* See .h file for full description. */
unsigned short get_bits_per_sample(FILE *wav_file)
{
    return (unsigned short) read_bytes_from_offset(wav_file,
            BITS_PER_SAMPLE_OFFSET, BITS_PER_SAMPLE_SIZE);
}


/* See .h file for full description. */
unsigned get_data_length(FILE *wav_file)
{
    return (unsigned) read_bytes_from_offset(wav_file,
            DATA_LENGTH_OFFSET, DATA_LENGTH_SIZE);
}


/* See .h file for full description. */
int read_wav_samples(FILE *wav_file, char audio_buffer[], unsigned num_samples)
{
    unsigned short bytes_per_sample = get_bytes_per_sample(wav_file);

    // seek to the data section of wav_file
    if (fseek(wav_file, DATA_OFFSET, SEEK_SET) == -1)  return 0;

    // read the samples
    return fread(audio_buffer, bytes_per_sample, num_samples, wav_file);
}


/* See .h file for full description. */
int write_wav_file(const char *file_name, const char *audio_data,
                   unsigned sample_rate, unsigned num_samples,
                   unsigned short bytes_per_sample)
{
    int samples_written = 0;
    FILE *wav_file = fopen(file_name, "wb");

    // ensure the file is ready for writing
    if (wav_file == NULL)  return 0;

    // compute all the header information we require -- must be stored in
    // variables to use fwrite, except for the hard-coded strings
    unsigned riff_header_length = num_samples * bytes_per_sample
                                  + DATA_OFFSET - WAVE_TEXT_OFFSET;
    unsigned format_header_length = 0x10; // fixed-size FORMAT
    unsigned short padding = 0x01;
    unsigned short channels = 0x01;
    // sample_rate is a parameter
    unsigned bytes_per_second = sample_rate * bytes_per_sample;
    // bytes_per_sample is a parameter
    unsigned short bits_per_sample = bytes_per_sample * 8;
    unsigned data_length = num_samples * bytes_per_sample;

    // write RIFF chunk
    if (fwrite("RIFF", 1, RIFF_TEXT_SIZE, wav_file) != RIFF_TEXT_SIZE)
                { fclose(wav_file); return 0; }
    if (fwrite(&riff_header_length, RIFF_LENGTH_SIZE, 1, wav_file) != 1)
                { fclose(wav_file); return 0; }
    if (fwrite("WAVE", 1, WAVE_TEXT_SIZE, wav_file) != WAVE_TEXT_SIZE)
                { fclose(wav_file); return 0; }

    // write FORMAT chunk
    if (fwrite("fmt ", 1, FORMAT_TEXT_SIZE, wav_file) != FORMAT_TEXT_SIZE)
                { fclose(wav_file); return 0; }
    if (fwrite(&format_header_length, FORMAT_LENGTH_SIZE, 1, wav_file) != 1)
                { fclose(wav_file); return 0; }
    if (fwrite(&padding, PADDING_SIZE, 1, wav_file) != 1)
                { fclose(wav_file); return 0; }
    if (fwrite(&channels, CHANNELS_SIZE, 1, wav_file) != 1)
                { fclose(wav_file); return 0; }
    if (fwrite(&sample_rate, SAMPLE_RATE_SIZE, 1, wav_file) != 1)
                { fclose(wav_file); return 0; }
    if (fwrite(&bytes_per_second, BYTES_PER_SECOND_SIZE, 1, wav_file) != 1)
                { fclose(wav_file); return 0; }
    if (fwrite(&bytes_per_sample, BYTES_PER_SAMPLE_SIZE, 1, wav_file) != 1)
                { fclose(wav_file); return 0; }
    if (fwrite(&bits_per_sample, BITS_PER_SAMPLE_SIZE, 1, wav_file) != 1)
                { fclose(wav_file); return 0; }

    // write DATA chunk
    if (fwrite("data", 1, DATA_TEXT_SIZE, wav_file) != DATA_TEXT_SIZE)
                { fclose(wav_file); return 0; }
    if (fwrite(&data_length, DATA_LENGTH_SIZE, 1, wav_file) != 1)
                { fclose(wav_file); return 0; }

    // write the audio data
    samples_written =
        fwrite(audio_data, bytes_per_sample, num_samples, wav_file);

    fclose(wav_file);

    return samples_written;
} // write_wav_file()
