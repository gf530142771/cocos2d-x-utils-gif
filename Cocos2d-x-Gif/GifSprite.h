//
//  GifSprite.h
//  HelloWorld
//
//  Created by gf on 2019/6/6.
//

#ifndef GifSprite_h
#define GifSprite_h
#include "cocos2d.h"
#include "Gif/GIFMovieData.h"
using namespace cocos2d;
typedef std::function<void()> AnimateEndCallback;
class GifSprite : public Sprite{
public:
    static GifSprite* create(const std::string& filename);
public:
    GifSprite();
    ~GifSprite();
    virtual bool initWithTexture(Texture2D *texture);
    void setIsLoop(bool isLoop);
    void setEndCallback(const AnimateEndCallback& _AnimateEndCallback);
private:
    void initMovieData(const std::string& filename);
    //绘制动画
    void runMovie();
    void runAnimate();
private:
    //gif数据
    GIFMovieData* mMovieData;
    //动画
    Animation* mMovieAnination;
    
    //是否循环
    bool mIsLoop;
    AnimateEndCallback mAnimateEndCallback;
};
#endif /* GifSprite_h */
