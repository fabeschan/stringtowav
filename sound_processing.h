// chanfabi luosteve

#ifndef SOUND_PROCESSING_H
#define SOUND_PROCESSING_H

/* sound_processing.h
 * A very basic library for processing WAV (.wav) files.
 *
 * Functions:
 *   reverse - Reverse the samples in a .wav file and save them to a new file
 *   mix - Read the samples from two .wav files and create a new .wav file with
 *         samples obtained by summing the corresponding samples from each input.
 *   change_volume - Read the samples from a .wav file and write a new file
 *                   whose volume has been scaled by a constant factor.
 */


/* reverse
 *   Reverse the samples in a .wav file and save them to a new .wav file.
 *
 *   Arguments:
 *       input - a string containing the name of the file to be reversed
 *       output - a string containing the name of the file in which to write
 *                the reversed samples
 *
 *   Returns:
 *       -1 if an error occurred; 0 otherwise
 */
int reverse(const char *input, const char *output);


/* mix
 *   Read the samples from two .wav files and write a new .wav file
 *   obtained by summing the corresponding samples from each input.
 *
 *   Arguments:
 *       input1 - a string containing the name of the first .wav file to mix
 *       input2 - a string containing the name of the second .wav file to mix
 *       output - a string containing the name of the file in which to write
 *                the mixed samples
 *
 *   Returns:
 *       -1 if an error occurred; 0 otherwise
 */
int mix(const char *input1, const char *input2, const char *output);


/* change_volume
 *   Read the samples from a .wav file and write a new .wav file
 *   whose volume has been scaled by a constant factor.
 *
 *   Arguments:
 *       input - a string containing the name of the .wav file to be read
 *       factor - the factor by which the volume will be multiplied
 *       output - a string containing the name of the file in which to write
 *                the modified samples
 *
 *   Returns:
 *       -1 if an error occurred; 0 otherwise
 */
int change_volume(const char *input, double factor, const char *output);

#endif//SOUND_PROCESSING_H
