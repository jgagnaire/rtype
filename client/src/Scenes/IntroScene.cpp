#include "Scenes/IntroScene.hh"

IntroScene::IntroScene()
{
    _arbok.load("arbok.png", 87);
    _buffers.push_back(&_arbok);
}

IntroScene::~IntroScene()
{

}

void IntroScene::update()
{
    _arbok.update();
}
