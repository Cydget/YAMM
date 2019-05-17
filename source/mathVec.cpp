#include "mathVec.h"

void swapLD(float& A, float& B) {
    float tmp = A;
    A = B;
    B = tmp;
}

u32 mPoint::length() {
    return std::sqrt(this->x * this->x + this->y * this->y);
}
float mVec::length() {
    return std::sqrt(this->x * this->x + this->y * this->y);
}
void mVec::normalize() {
    float tmpx = this->x / this->length();
    this->y = this->y / this->length();
    this->x = tmpx;
}
void mVec::addVec(mVec inVec) {
    this->x += inVec.x;
    this->y += inVec.y;
}
void mVec::subVec(mVec inVec) {
    this->x -= inVec.x;
    this->y -= inVec.y;
}
float mVec::dotVec(mVec inVec) {
    return (this->x * inVec.x) + (this->y * inVec.y);
}
void mVec::pCCW() {
    float tmp = this->x;
    this->x = this->y;
    this->y = tmp;
    this->x = this->x  * -1;
    //    swapLD(this->x,this->y);
    //    this->x*= -1;
}
void mVec::pCW() {
    float tmp = this->x;
    this->x = this->y;
    this->y = tmp;
    this->y = this->y  * -1;


    //swapLD(this->x,this->y);
    //this->y*= -1;
}
void mVec::set(mPoint pt) {
    this->x = (float)(pt.x);
    this->y = (float)(pt.y);
}
void mVec::set(float X, float Y) {
    this->x = X;
    this->y = Y;
}
void mVec::scale(float scalar) {
    this->x *= scalar;
    this->y *= scalar;
}


mVec::mVec(mPoint pt): x(pt.x), y(pt.y) {}
//mVec::mVec(int X, int Y): x(X), y(Y){}
mVec::mVec(float X = 0, float Y = 0): x(X), y(Y) {}
mVec::mVec(): mVec::mVec(0, 0) {}

