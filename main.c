#include "wav_lib.h"

extern int reverse(char *input, char *output);
extern int mix(char *input1, char *input2, char *output);
extern int change_volume(char *input, double factor, char *output);
extern int generate_song(char *note_string, char *output_filename);
extern int memory(const char *note_string);
//extern int split_note_string(const char *note_string, channel_t *first);



/* Some basic tests for the rough P2 solution
 */
int main(void)
{
	// uncomment these functions as you implement them for some basic tests
	reverse("cow.wav", "cow_reversed.wav");
	reverse("cde.wav", "cde_reversed.wav");
	reverse("welcome.wav", "welcome_reversed.wav");

	reverse( "cow.wav", "cow_reversed.wav");

	mix("cde.wav",
		"door.wav",
		"cde_door_mix.wav"
		);

	mix("welcome.wav",
		"door.wav",
		"welcome_door_mix.wav"
		);

	change_volume("welcome.wav", 2.0,
				  "welcome_loud.wav");

	change_volume("welcome.wav", 0.25,
				  "welcome_quiet.wav");

	generate_song("CDEFGAB", "scale.wav");

	// notes, lengths, octaves
	generate_song("4E3GG4C2.2D2E2F2G2A6D", "anthem.wav");
	generate_song(">4C>4C<2BGA2B>2C", "mystery_song.wav");


	// channels
	/*
	generate_song("8C|8F|8A|>4C", "chord.wav");

	generate_song(">CCGGAA2GFFEEDD2CGGFFEE2DGGFFEE2DCCGGAA2GFFEEDD2C|"
				  "CGEGCA2EBFCGBF2CEGFAEG<2B>EGFAEG<2B>CGEGCA2EBFCGBF2C",
				  "twinkle.wav");

    //generate_song(">C<G>CG.F.ED<3BGG2GBGB>E.D.<B>C3E<GG2G>C<G>CG.F.ED<3BGG2GBGB>E.D.<B>4CC<B>CD4E4GFGFE4D<4B>4FEFED4C4E4GFEFG4A2GFE4FEFED3C<B|<>C<7..GABBBABB7..GAB>C<BAB>C7..<GABBBABB7.>C<BAB>C<B>CDED2CDEFG.E2DDEFGFE2D<B>CDF5C<GFG>C<B2AGAB>C2.<2BAB>CD<BA2GFGAF4.>E<GFG", "hiihihi.wav");

	//everything
	generate_song("2.8C8C<7A8A>7C8C8D|2.8E8E7D8C7E8F8G|2.8G8G7F8E7G8A8B|>CC2CC2C2C2C4.<2B>C2DC2C<2B>C<2A>4.C2CC2C2C2F2E2C2D8D",
				  "collins.wav"); */


	return 0;
}
