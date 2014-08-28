//This example shows how to load and play Locus SoundEffects.

#include "Locus/Audio/SoundState.h"
#include "Locus/Audio/SoundEffect.h"

#include "Locus/Common/Exception.h"

#include <iostream>

int main(int argc, char** argv)
{
   try
   {
      //An OpenAL context is created here and made current
      Locus::SoundState soundState;

      //This sound effect becomes implicitly attached to the OpenAL context created above
      Locus::SoundEffect soundEffect;

      //Can also load ogg files
      if (!soundEffect.Load("path_to/my_audio_file.wav"))
      {
         std::cerr << "There was a problem reading the audio file." << std::endl;
         return 1;
      }

      soundEffect.Play();

      while (soundEffect.IsPlaying())
      {
         //wait for the sound to stop playing
      }
   } //The SoundEffect is destroyed first (while the SoundState's OpenAL context is current). The SoundState is then destroyed along with its  OpenAL context
   catch (Locus::Exception& locusException)
   {
      std::cerr << locusException.Message() << std::endl;
      return 1;
   }
   catch (...)
   {
      std::cerr << "Something unexpected happened." << std::endl;
      return 1;
   }

   return 0;
}