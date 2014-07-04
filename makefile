all: out

out: main.c song_generator_v2.c sound_processing.c wav_lib.c
	gcc main.c song_generator_v2.c sound_processing.c wav_lib.c -o out

v1: main.c song_generator_v1.c sound_processing.c wav_lib.c
	gcc main.c song_generator_v1.c sound_processing.c wav_lib.c -o out

v2: out

clean:
	rm -rf *o out
