#ifndef ANIMATION_H
#define ANIMATION_H

typedef const unsigned char* FrameData;

typedef struct _Animation {
  const FrameData* frames;
  int frameCount;
} Animation;

inline Animation CreateAnimation(const FrameData* frames, int frameCount)
{
  Animation animation;
  animation.frames = frames;
  animation.frameCount = frameCount;
  return animation;
}

#endif // ANIMATION_H
