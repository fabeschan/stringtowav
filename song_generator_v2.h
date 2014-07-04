// chanfabi luosteve

#ifndef SONG_GENERATOR_V2_H
#define SONG_GENERATOR_V2_H

/* song_generator_v2.h
 * A very basic library for generating simple WAV (.wav) files.
 * Adds the ability to process multiple channels, compared with
 * song_generator_v1.
 *
 * Functions:
 *   generate_song - create a .wav file from a string of notes
 */


/* generate_song
 *   Create a .wav file from a string of notes note_string, which may contain
 *   different channels of notes.
 *   Precondition:
 *       note_string contains only a combination of letters "A" to "G",
 *       ">", "<", integers "1" to "9", "|", and ".".
 *
 *   Arguments:
 *       note_string - a note string with the above precondition.
 *       output_file - a string containing the name of the .wav file
 *                     to be created
 *
 *   Returns:
 *       -1 if an error occurred; 0 otherwise.
 */
int generate_song(const char *note_string, const char *output_file);


#endif//SONG_GENERATOR_V2_H

