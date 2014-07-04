// chanfabi luosteve

#ifndef WAV_LIB_H
#define WAV_LIB_H


/* wav_lib.h
 *  A very basic library for reading and writing WAV (.wav) files.
 *
 *  Functions:
 *      gen_sin_samples - fill a buffer with samples of a sin wave
 *      write_wav_file - write a comlete WAV file
 *      read_wav_samples - read samples from a WAV file
 *      get_num_channels - read the number of channels in a WAV file
 *      get_sample_rate - read the sample rate of a WAV file
 *      get_bytes_per_second - read the number of bytes in 1 second of a WAV
 *      file
 *      get_bytes_per_sample - read the number of bytes in 1 sample of a WAV
 *      file
 *      get_bits_per_sample - read the number of bits in 1 sample of a WAV file
 *      get_data_length - read the number of bytes required to store all the
 *              samples of a WAV file
 */

#include <stdio.h> // required for (FILE *) in definitions below


/* gen_sin_samples
 *  Fill a buffer with samples of a pure sin wave at a given sampling rate.
 *
 *  DO NOT MODIFY THIS FUNCTION IN ANY WAY. YOU MUST INCLUDE AND USE THIS
 *  FUNCTION IN YOUR SOLUTION. SEE PROJECT HANDOUT FOR DETAILS.
 *
 *  Precondition:
 *      sample_buffer is large enough to hold num_samples double values.
 *      num_samples > 0.05/sample_rate (requesting > 0.05 seconds of samples)
 *
 *  Arguments:
 *      freq - the frequency (in Hz) of the sin wave to sample
 *      sample_rate - the sample rate (in Hz) at which to sample the wave
 *      num_samples - the number of samples requested
 *      sample_buffer - a pointer to an array in which to store the samples
 */
void gen_sin_samples(double freq, double sample_rate, unsigned num_samples,
                     short *sample_buffer);


/* write_wav_file
 *  Write single channel WAV format audio data to a file with the specified
 *  file_name.
 *
 *  Arguments:
 *      file_name - the name of the file to write audio data to
 *      audio_data - the audio samples to write
 *      sample_rate - the sample rate (in Hz) of the data
 *      num_samples - the number of samples stored in audio_data
 *      bytes_per_sample - the number of bytes in each sample in audio_data
 *
 *  Returns:
 *      the number of samples written to the WAV file
 */
int write_wav_file(const char *file_name, const char *audio_data,
                   unsigned sample_rate, unsigned num_samples,
                   unsigned short bytes_per_sample);


/* read_wav_samples
 *  Read the specified number of samples from wav_file into audio_buffer.
 *
 *  Arguments:
 *      wav_file - the WAV file to read samples from
 *      audio_buffer - an array to read the samples into
 *      num_samples - the number of samples to read
 *
 *  Returns:
 *      the number of samples read from wav_file
 */
int read_wav_samples(FILE *wav_file, char audio_buffer[], unsigned num_samples);


/* get_num_channels
 *  Return the number of channels in the supplied WAV file.
 *
 *  Arguments:
 *      wav_file - a pointer to a valid WAV file
 *
 *  Returns:
 *      the number of channels in wav_file;
 *      0 if a read error occured
 */
unsigned short get_num_channels(FILE *wav_file);


/* get_sample_rate
 *  Return the sample rate of the supplied WAV file.
 *
 *  Arguments:
 *      wav_file - a pointer to a valid WAV file
 *
 *  Returns:
 *      the sample rate of wav_file;
 *      0 if a read error occured
 */
unsigned get_sample_rate(FILE *wav_file);


/* get_bytes_per_second
 *  Return the number of bytes of data required to play one second of the
 *  supplied WAV file.
 *
 *  Arguments:
 *      wav_file - a pointer to a valid WAV file
 *
 *  Returns:
 *      the number of bytes required to play 1 second of wav_file;
 *      0 if a read error occured
 */
unsigned get_bytes_per_second(FILE *wav_file);


/* bytes_per_sample
 *  Return the number of bytes in one sample of the supplied WAV file.
 *
 *  Arguments:
 *      wav_file - a pointer to a valid WAV file
 *
 *  Returns:
 *      the number of bytes in one sample of wav_file;
 *      0 if a read error occured
 */
unsigned short get_bytes_per_sample(FILE *wav_file);


/* bits_per_sample
 *  Return the number of bits in one sample of the supplied WAV file.
 *
 *  Arguments:
 *      wav_file - a pointer to a valid WAV file
 *
 *  Returns:
 *      the number of bits in one sample of wav_file;
 *      0 if a read error occured
 */
unsigned short get_bits_per_sample(FILE *wav_file);


/* get_data_length
 *  Return the number of bytes required to store all the samples of a WAV file.
 *
 *  Arguments:
 *      wav_file - a pointer to a valid WAV file
 *
 *  Returns:
 *      the number of bytes require to store all samples in wav_file;
 *      0 if a read error occured
 */
unsigned get_data_length(FILE *wav_file);

#endif//WAV_LIB_H
