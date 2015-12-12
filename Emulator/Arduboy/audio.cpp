#include "Arduboy.h"
#include "audio.h"

volatile byte *_tunes_timer1_pin_port;
volatile byte _tunes_timer1_pin_mask;
volatile int32_t timer1_toggle_count;
volatile byte *_tunes_timer3_pin_port;
volatile byte _tunes_timer3_pin_mask;
byte _tune_pins[AVAILABLE_TIMERS];
byte _tune_num_chans = 0;
volatile boolean tune_playing; // is the score still playing?
volatile unsigned wait_timer_frequency2;       /* its current frequency */
volatile unsigned wait_timer_old_frequency2;   /* its previous frequency */
volatile boolean wait_timer_playing = false;   /* is it currently playing a note? */
volatile boolean doing_delay = false;          /* are we using it for a tune_delay()? */
volatile boolean tonePlaying = false;
volatile unsigned long wait_toggle_count;      /* countdown score waits */
volatile unsigned long delay_toggle_count;     /* countdown tune_ delay() delays */


// pointers to your musical score and your position in said score
volatile const byte *score_start = 0;
volatile const byte *score_cursor = 0;

/* AUDIO */

void ArduboyAudio::on()
{
  audio_enabled = true;
}

bool ArduboyAudio::enabled()
{
  return audio_enabled;
}

void ArduboyAudio::off()
{
  audio_enabled = false;
}

void ArduboyAudio::save_on_off()
{
}

void ArduboyAudio::setup()
{
  tune_playing = false;
}

void ArduboyAudio::tone(uint8_t, unsigned int, unsigned long)
{
  // if (audio_enabled)
    // ::tone(channel, frequency, duration);
}


/* TUNES */

void ArduboyTunes::initChannel(byte)
{
}

void ArduboyTunes::playNote(byte, byte)
{
}

void ArduboyTunes::stopNote(byte)
{
}

void ArduboyTunes::playScore(const byte *)
{
}

void ArduboyTunes::stopScore (void)
{
}

bool ArduboyTunes::playing()
{
  return tune_playing;
}

/* Do score commands until a "wait" is found, or the score is stopped.
This is called initially from tune_playcore, but then is called
from the interrupt routine when waits expire.
*/
/* if CMD < 0x80, then the other 7 bits and the next byte are a 15-bit big-endian number of msec to wait */
void ArduboyTunes::step()
{
}

void ArduboyTunes::delay (unsigned)
{
}

void ArduboyTunes::closeChannels(void)
{
}

void ArduboyTunes::soundOutput()
{
}

void ArduboyTunes::tone(unsigned int, unsigned long)
{
}