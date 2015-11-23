#ifndef GAME_H__
#define GAME_H__

#ifdef __cplusplus
extern "C" {
#endif

void startGame();
void loopGame();
  
#ifdef EMULATOR
  
unsigned char* getDisplayBuffer(int* size);
int getDisplayWidth(void);
int getDisplayHeight(void);
  
#endif

#ifdef __cplusplus
}
#endif

#endif // GAME_H__
