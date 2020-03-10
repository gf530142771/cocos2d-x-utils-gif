#include "GifSprite.h"
GifSprite* GifSprite::create(const std::string &filename)
{
    GifSprite *sprite = new (std::nothrow) GifSprite();
    if (sprite)
    {
        sprite->initMovieData(filename);
        if(sprite->initWithTexture(nullptr)){
            sprite->runMovie();
            sprite->autorelease();
            return sprite;
        }
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool GifSprite::initWithTexture(Texture2D *texture){
    if(mMovieData){
        return Sprite::initWithTexture(mMovieData->GetTexture(0));
    }
    return false;
}

GifSprite::GifSprite()
:mMovieData(nullptr)
,mIsLoop(true)
,mMovieAnination(nullptr)
,mAnimateEndCallback(nullptr)
{
    
}

GifSprite::~GifSprite(){
    if(mMovieData){
        delete mMovieData;
    }
    
    if(mMovieAnination){
        delete mMovieAnination;
    }
}

void GifSprite::setIsLoop(bool isLoop){
    if(mIsLoop == isLoop)
        return;
    mIsLoop = isLoop;
    runAnimate();
}

void GifSprite::setEndCallback(const AnimateEndCallback& _AnimateEndCallback){
    mAnimateEndCallback = _AnimateEndCallback;
}

void GifSprite::initMovieData(const std::string& filename){
    if(mMovieData){
        delete mMovieData;
    }
    mMovieData = new GIFMovieData;
    mMovieData->init(filename.c_str());
}

void GifSprite::runMovie(){
    int count = mMovieData->getGifCount();
    int time = mMovieData->duration();
    Vector<SpriteFrame*> vec;
    for(int i = 0; i < count; i++){
        auto texture = mMovieData->GetTexture(i);
        if(texture){
            Rect rect = Rect::ZERO;
            rect.size = texture->getContentSize();
            auto spriteFrame = SpriteFrame::createWithTexture(texture, rect);
            vec.pushBack(spriteFrame);
        }
    }
    
    if(mMovieAnination){
        delete mMovieAnination;
    }
    mMovieAnination = new Animation();
    mMovieAnination->initWithSpriteFrames(vec);
    //设置动画播放时间
    mMovieAnination->setDelayPerUnit(time * 1.0f/1000/count);
    this->runAnimate();
    
}

void GifSprite::runAnimate(){
    this->stopAllActions();
    this->setTexture(mMovieData->GetTexture(0));
    auto animate = Animate::create(mMovieAnination);
    if(mIsLoop){
        this->runAction(RepeatForever::create(
                                              Sequence::create(
                                                               animate,
                                                               CallFunc::create([&](){
                                                  CCLOG("播放完成");
                                                  if(mAnimateEndCallback){
                                                      mAnimateEndCallback();
                                                  }
                                              }),
                                                               NULL)));
    }else{
        this->runAction(Sequence::create(
                                         animate,
                                         CallFunc::create([&](){
            CCLOG("播放完成");
            if(mAnimateEndCallback){
                mAnimateEndCallback();
            }
        }),
                                         NULL));
    }
}
