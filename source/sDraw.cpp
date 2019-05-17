#include "sDraw.h"


/*
textStyle::textStyle(): isBold(false),
    isReversed(false),
    isLight(false),
    isUnderlined(false),
    isStrikethrough(false),
    textColor(BLACK) {}


scrText::scrText(mPoint pt = mPoint(), std::string txt = ""): textStyle(),
    pos(pt),
    text(txt) {}
*/

bool isOnLine(mPoint ptTest, mPoint pt1, mPoint pt2) {
    //there is many ways to get this to be 10000% faster //only to be used with the polygon function as it assumes vertical vertical is always true
    int minX = min(pt1.x, pt2.x);
    int maxX = max(pt1.x, pt2.x);
    int minY = min(pt1.y, pt2.y);
    int maxY = max(pt1.y, pt2.y);
    int compY = pt2.y;

    if(minX == pt1.x) {compY = pt1.y;}

    if((ptTest.x >= minX && ptTest.x <= maxX) &&
       (ptTest.y >= minY && ptTest.y <= maxY)) {

        if(pt1.x == pt2.x) { //vertical line === goint to be on the line
            return true;
        }

        // we can use slope now that we know that it is not vertical
        double m = (pt1.y - pt2.y + 0.0) / (0.0 + pt1.x - pt2.x);
        double fudgeFac = abs(m);//-0.01;
        //        if(fudgeFac<1){fudgeFac=1/fudgeFac;}

        if((max(double(ptTest.y), m * (ptTest.x - minX) + double(compY)) - min(double(ptTest.y),
                m * (ptTest.x - minX) + double(compY))) < fudgeFac) {
            return true;
        }

    }

    return false;
}

bool isLinePart(mPoint ptTest, mPoint pt1, mPoint pt2, int fud = -1) {
    //there is many ways to get this to be 10000% faster
    //    float minX = float(std::min(pt1.x,pt2.x));

    //    if( minX - pt1.x < 0.5){ compY = float(pt1.y);}
    /*
        if(pt1.x==pt2.x){//assumes that we are only checking for values inside of line segment bounding box dont actually need this as the draw line function will never ask this function to draw a vertical line
    	return true;
        }
        else{
    */
    // we can use slope now that we know that it is not vertical
    float m = (float(pt1.y) - float(pt2.y) + 0.0F) / (0.0F + float(pt1.x) - float(pt2.x)); // can be negative
    //slope ex 1
    float fudgeFac = std::abs(m);

    if(fudgeFac < 1) {fudgeFac = 1;}

    if(fud != -1) {fudgeFac = float(fud);}

    //    	if(   (max(float(ptTest.y),m*(ptTest.x-minX) + float(compY)) - min(float(ptTest.y),m*(ptTest.x-minX) + float(compY))) < fudgeFac ){

    if(std::abs(float(ptTest.y) - (m * (float(ptTest.x) - float(pt1.x)) + float(pt1.y)))  <= fudgeFac) {
        return true;
    }

    //    }
    return false;
}




void myScr::updateScreen() {
    //gfxFlushBuffers();
    //gfxSwapBuffers();
    //gfxWaitForVsync();
    framebufferEnd(&fb);//finishes frame then starts next one
    framebuf = (u32*) framebufferBegin(&fb, &stride);

}
//void myScr::drawPixel(mPoint pt, u32 color){
//        framebuf[pt.y*(stride>>2)+pt.x] = color;
//}
void myScr::drawUpdateProgress(double finished,double total,font* fnt){
    int padding = 8;
    //draw main box
    drawRoundedRect(mPoint(width/3,height/3), mPoint(2*width/3,2*height/3), 25, RGBA8(66,191,244, 255));//this might become gradually darker, but I want to find out
    
    //draw download bar
    drawRect(mPoint(width/3+2*padding,2*height/3-50-2*padding), mPoint(2*width/3-2*padding,2*height/3-2*padding), RGBA8(255,255,200,255));
    drawRectBorder(mPoint(width/3+2*padding,2*height/3-50-2*padding), mPoint(2*width/3-2*padding,2*height/3-2*padding), 2, RGBA8(50,50,50,255));
    
    //width max is equal to 2*width/3-2*padding - width/3-2*padding = width/3-4*padding 
    drawRect(mPoint(width/3+2*padding,2*height/3-50-2*padding), mPoint((width/3+2*padding)+finished*(width/3-4*padding)/total,2*height/3-2*padding), RGBA8(0,240,60,70));
    
    /*Text should look like
     * 
     * Downloading: 
     * [finished]/[total] bytes
     * 
     * I should make it different sizes or colors*/
    
    //draw text info
    mPoint centerTextPoint(width/2,height/3+padding);
    std::string txtVal = "Downloading\n" + std::to_string((unsigned int)(finished/1024)) + "/ " + std::to_string((unsigned int)(total/1024)) + "\nKiB";
    drawText(txtVal,centerTextPoint, fnt, width/3-padding,true, RGBA8(0,0,0,255));
    updateScreen();
}

void myScr::drawPixel(mPoint pt, u32 color) {
    if(pt.x >= 0 && pt.y >= 0 &&  pt.x < int(width) && pt.y < int(height) ) {
        //float alpha = (color & (0xff << 24))/0xff; // get alpha on a scale from 0 to 1
        //I should have made this globalish
        //also it is wasteful to recopy the framebuf into another oldC when it doesnt need to move in the first place
        int a = ((color & 0xff000000) >> 24);

        if(a < 250) {
            //todo Find out why I need to use bit shifting instead of pointers/unions
            framebuf[pt.y*(stride>>2)+pt.x] = RGBA8_MAXALPHA((color & 0x000000ff) * a / 0xff + (framebuf[pt.y * width + pt.x] & 0x000000ff) *
                                            (255 - a) / 0xff,
                                            ((color & 0x0000ff00) >> 8) * a / 0xff + ((framebuf[pt.y * width + pt.x] & 0x0000ff00) >> 8) * (255 - a) / 0xff,
                                            ((color & 0x00ff0000) >> 16) * a / 0xff + ((framebuf[pt.y * width + pt.x] & 0x00ff0000) >> 16) * (255 - a) / 0xff);




        }
        else {
            framebuf[pt.y*(stride>>2)+pt.x]  = color;
        }

    }
}

void myScr::drawPixel(mVec pt, u32 color) {
    mPoint mPt;
    mPt.x = round(pt.x);
    mPt.y = round(pt.y);
    myScr::drawPixel(mPt, color);
}
void myScr::drawLine(mPoint pt1, mPoint pt2, u32 color) {
    if(pt1.y == pt2.y) { //horizontal
        for(int x = min(pt1.x, pt2.x), endX = max(pt1.x, pt2.x); x <= endX; ++x) {
            drawPixel(mPoint(x, pt1.y), color);
        }
    }
    else if(pt1.x == pt2.x) { //vertical
        for(int y = min(pt1.y, pt2.y), endY = max(pt1.y, pt2.y); y <= endY; ++y) {
            drawPixel(mPoint(pt1.x, y), color);
        }
    }
    else { //diagnal line
        for(int y = min(pt1.y, pt2.y), yMax = max(pt1.y, pt2.y); y < yMax; ++y) {
            for(int x = min(pt1.x, pt2.x), xMax = max(pt1.x, pt2.x); x < xMax; ++x) {
                if(isLinePart(mPoint(x, y), pt1, pt2)) {
                    drawPixel(mPoint(x, y), color);
                }
            }
        }
    }



    //draw();//this draw might be a bit too much
}

void myScr::drawLine(mVec pt1, mVec pt2, u32 color) {
    //only draws horizontal lines and vertical lines as that is quicker

    if(std::abs(pt1.y - pt2.y) < 0.5) {  //horizontal
        for(int x = min(pt1.x, pt2.x), endX = max(pt1.x, pt2.x); x <= endX; ++x) {
            drawPixel(mPoint(x, pt1.y), color);
        }
    }
    else if(std::abs(pt1.x - pt2.x) < 0.5) {  //vertical
        for(int y = std::min(pt1.y, pt2.y), endY = std::max(pt1.y, pt2.y); y <= endY; ++y) {
            drawPixel(mPoint(pt1.x, y), color);
        }
    }
    else { //diagnal line

        for(int y = round(std::min(pt1.y, pt2.y)), yMax = round(std::max(pt1.y, pt2.y));    y < yMax; ++y) {
            for(int x = round(std::min(pt1.x, pt2.x)), xMax = round(std::max(pt1.x, pt2.x)); x < xMax; ++x) {


                if(isLinePart(mPoint(x, y), mPoint(pt1.x, pt1.y), mPoint(pt2.x, pt2.y))) {
                    drawPixel(mPoint(x, y), color);


                }
            }
        }
    }



    //draw();//this draw might be a bit too much
}


void myScr::drawRect(mPoint pt1, mPoint pt2, u32 color) {
    for(int y = min(pt1.y, pt2.y), yEnd = max(pt1.y, pt2.y);  y <= yEnd ; ++y) {
        drawLine(mPoint(pt1.x, y), mPoint(pt2.x, y), color);
    }

    //draw();
}
void myScr::drawRectEmpty(mPoint pt1, mPoint pt2, u32 color) {
    drawLine(mPoint(pt1.x, pt1.y), mPoint(pt2.x, pt1.y), color);
    drawLine(mPoint(pt2.x, pt1.y), mPoint(pt2.x, pt2.y), color);
    drawLine(mPoint(pt2.x, pt2.y), mPoint(pt1.x, pt2.y), color);
    drawLine(mPoint(pt1.x, pt2.y), mPoint(pt1.x, pt1.y), color);
}
void myScr::drawRectBorder(mPoint pt1, mPoint pt2, u32 borderSize, u32 color) {
    //Is this clever or should i just draw 4 rectangles?
    for(u32 step = 0; step < borderSize; ++step) {
        drawRectEmpty(mPoint(pt1.x + step, pt1.y + step), mPoint(pt2.x - step, pt2.y - step), color);
    }
}


void myScr::background(u32 color) {
    mPoint p;

    for(p.y = 0; p.y < int(height); p.y++) { //Access the buffer linearly.
        for(p.x = 0; p.x < int(width); p.x++) {
            drawPixel(p, color);
        }
    }

}

void myScr::drawPoly(polygonShape* poly, u32 color) {

    //god my algorithm is terrible

    //var ySkiplist = new Array(height);
    //var ySkiplistCounter=0;
    for(int y = 0; y < int(height); y++) {
        for(int x = 0; x < int(width); x++) {

            for(unsigned int i = 0; i + 1 < poly->pointCount; ++i) {
                if(isOnLine(mPoint(x, y), poly->points[i], poly->points[i + 1])) {
                    int tmpX = x;

                    for(unsigned int k = 0; k + 1 < poly->pointCount; ++k) { //3 should be point len

                        for(int newX = tmpX + 1; newX <= int(width); ++newX) {
                            for(unsigned int j = 0; j + 1 < poly->pointCount; ++j) {
                                if(j != i  && ((newX - tmpX > 1)) && isOnLine(mPoint(newX, y), poly->points[j], poly->points[j + 1])) {
                                    if(!(k % 2)) {
                                        //                                                    stroke(millis() % 255,(millis()+20) % 255 + 100,(millis()+50) % 255);
                                        drawLine(mPoint(tmpX, y), mPoint(newX, y), color);
                                        //                                                line(tmpX,y,newX,y,color);
                                    }

                                    for(unsigned int n = j + 1; n + 1 < poly->pointCount; ++n) {
                                        if(isOnLine(mPoint(newX, y), poly->points[n], poly->points[n + 1])) {k++;}
                                    }

                                    tmpX = newX;
                                    newX = width;

                                    j = poly->pointCount;

                                    //ySkiplist[ySkiplistCounter]=y;
                                    //ySkiplistCounter++;
                                    //newX=width;
                                    //i=points.length;

                                }
                            }
                        }

                    }

                    x = width;
                    i = poly->pointCount;
                    // point(x,y);
                }


                //line(points[i].x,points[i].y,points[i+1].x,points[i+1].y);
            }



        }
    }


}


void myScr::drawCircle(mPoint pt1, int radius, u32 color) { //this is in centered mode

    for(float y = pt1.y - radius, limY = (pt1.y + radius); y < limY; ++y) {
        for(float x = pt1.x - radius, limX = (pt1.x + radius); x < limX; ++x) {

            if(((y - float(pt1.y)) * (y - float(pt1.y)) + (x - float(pt1.x)) * (x - float(pt1.x)))  - float(radius * radius)  < 0.8F) {
                drawPixel(mPoint(x, y), color);
            }

        }
    }


}

//from https://www.gamedev.net/forums/topic/295943-is-this-a-better-point-in-triangle-test-2d/
float side(mVec p1, mVec p2, mVec p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}
bool isInside(mPoint cP, mPoint tP1, mPoint tP2, mPoint tP3) {
    bool b2 = side(mVec(cP), mVec(tP1), mVec(tP2)) < 0;
    return ((b2 == (side(mVec(cP), mVec(tP2), mVec(tP3)) < 0)) && (b2 == (side(mVec(cP), mVec(tP3), mVec(tP1)) < 0)));
}







void myScr::drawTriangle(mPoint pt1, mPoint pt2, mPoint pt3, u32 color) {
    int maxX = std::max(pt1.x, std::max(pt2.x, pt3.x));
    int x =    std::min(pt1.x, std::min(pt2.x, pt3.x));
    int maxY = std::max(pt1.y, std::max(pt2.y, pt3.y));
    int y =    std::min(pt1.y, std::min(pt2.y, pt3.y));

    while(y <= maxY) {
        int xCpy = x;

        while(xCpy <= maxX) {
            if(isInside(mPoint(xCpy, y), pt1, pt2, pt3)) {
                drawPixel(mPoint(xCpy, y), color);
            }

            xCpy++;
        }

        y++;
    }
}

void myScr::drawTextLine(std::string txt, mPoint StartPos, font* fnt, u32 color) {
    //float centeredOffset = 0;
    //unsigned int lineNumber = 0;
    //unsigned int lineWidth = 0;
    mPoint cursorPos = StartPos;
    //this is very broken and Im not sure how it doesnt crash
    int a = ((color & 0xff000000) >> 24); //0xff << 6*4
    int r = ((color & 0x000000ff) >> 0) * a / 0xff; //0xff << 0*4
    int g = ((color & 0x0000ff00) >> 8) * a / 0xff; //0xff << 2*4
    int b = ((color & 0x00ff0000) >> 16) * a / 0xff; //0xff << 4*4

    for(size_t pos = 0 ; pos < txt.length();) {


        long unsigned int codePoint = getCodePoint(txt, pos, pos);//first pos is start point second pos is to incrament by byte count

        if(codePoint != '\n') { //comparison between long unsigned int and a signed int might not be the best idea

            //get glyf info
            int width, height, pngXStart, pngYStart, xOffset, yOffset, xAdvance;
            fnt->getLetterInfo(codePoint, width, height, pngXStart, pngYStart, xOffset, yOffset, xAdvance);

            //Draw glyf
            for(int y = pngYStart; y < (pngYStart + height); ++y) {
                for(int x = pngXStart; x < (pngXStart + width); ++x) {
                    int alpha = upng_get_buffer(fnt->fontPNG)[(y * fnt->pngWidth * fnt->pngDepth) + x * fnt->pngDepth + (fnt->pngDepth - 1)];

                    if(alpha) {
                        drawPixel(mPoint(cursorPos.x + (x - pngXStart + xOffset), cursorPos.y + (y - pngYStart + yOffset)), RGBA8(r, g, b,
                                  a * alpha / 0xff));
                    }
                }
            }

            //Move to the new x position

            if(codePoint == ' ') {cursorPos.x += xAdvance / 2;}
            else {cursorPos.x += width + xOffset;}


        }
        else {
            //Move pos to new line
            cursorPos.x = StartPos.x;
            cursorPos.y += fnt->charHeight * 4 / 5;
            pos++;
        }

    }
}

void myScr::drawText(std::string txt, mPoint StartPos, font* fnt, u32 maxWidth, bool isCentered = false,
                     u32 color = RGBA8_MAXALPHA(0xff, 0xff,
                                 0xff)) {

    std::string line = "";
    size_t pos = 0;
    mPoint cursorPos = StartPos;
    //drawTextLine("test",cursorPos, fnt,color);

    while(pos < txt.length()) {

        line = getFirstLine(txt, pos);
        u32 lineWidth = fnt->getTextLength(line);

        while(lineWidth >= maxWidth) {
            //A faster way to do this would be to do a binary search like thing where it checks half the size,
            //and if it is smaller check half again
            //but if its bigger check half plus 1/4 etc until the 1/(2*n) * length <= 1
            //but this would only be usefull if there happens to be many many lines that over flow, but as that is not the case starting from the left end is probably better
            line.pop_back(); // c++11
            lineWidth = fnt->getTextLength(line);
        }

        if(line == "") {pos += 1;}
        else {pos += line.length();}

        drawTextLine(line, (isCentered) ? mPoint(cursorPos.x - lineWidth / 2, cursorPos.y) : cursorPos, fnt, color);

        cursorPos.y += fnt->charHeight * 4 / 5; //setup for next line


    }


}
int getTextHeight(std::string txt, font* fnt, u32 maxWidth, bool isCentered = false) {

    std::string line = "";
    size_t pos = 0;
    mPoint cursorPos(0,0);// = StartPos;
    //drawTextLine("test",cursorPos, fnt,color);

    while(pos < txt.length()) {

        line = getFirstLine(txt, pos);
        u32 lineWidth = fnt->getTextLength(line);

        while(lineWidth >= maxWidth) {
            //A faster way to do this would be to do a binary search like thing where it checks half the size,
            //and if it is smaller check half again
            //but if its bigger check half plus 1/4 etc until the 1/(2*n) * length <= 1
            //but this would only be usefull if there happens to be many many lines that over flow, but as that is not the case starting from the left end is probably better
            line.pop_back(); // c++11
            lineWidth = fnt->getTextLength(line);
        }

        if(line == "") {pos += 1;}
        else {pos += line.length();}

        //drawTextLine(line, (isCentered) ? mPoint(cursorPos.x - lineWidth / 2, cursorPos.y) : cursorPos, fnt, color);

        cursorPos.y += fnt->charHeight * 4 / 5; //setup for next line


    }

    return cursorPos.y;
}











void myScr::drawRGB8(mPoint pt, unsigned char* buf, unsigned int imgBufWid, unsigned int imgBufHei, unsigned int newWid,
                     unsigned int newHei) {

    struct RGBBYTES {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };
    RGBBYTES* pixelPtr = reinterpret_cast<RGBBYTES*>(buf);

    float xScale = float(newWid) / float(imgBufWid);
    float yScale = float(newHei) / float(imgBufHei);
    mVec cursor(pt);

    for(unsigned int y = 0 ; y < imgBufWid * imgBufHei; y += imgBufWid) {
        for(unsigned int x = 0; x < imgBufWid; x++) {
            //           drawPixel(mPoint(cursor.x,cursor.y),RGBA8_MAXALPHA(pixelPtr[x+y].r,pixelPtr[x+y].g,pixelPtr[x+y].b));
            drawRect(mPoint(cursor.x, cursor.y), mPoint(cursor.x + xScale, cursor.y + yScale), RGBA8_MAXALPHA(pixelPtr[x + y].r,
                     pixelPtr[x + y].g, pixelPtr[x + y].b));
            cursor.x += xScale;
        }

        cursor.x = pt.x;
        cursor.y += yScale;
    }




}

void myScr::drawRoundedRect(mPoint pt1, mPoint pt2, int radius, u32 color) {
    mPoint topLeft(min(pt1.x, pt2.x), min(pt1.y, pt2.y));
    mPoint bottomRight(max(pt1.x, pt2.x), max(pt1.y, pt2.y));


    drawRect(mPoint(topLeft.x + radius, topLeft.y), mPoint(bottomRight.x - radius, bottomRight.y), color);

    drawRect(mPoint(topLeft.x, topLeft.y + radius), mPoint(topLeft.x + radius, bottomRight.y - radius), color);
    drawRect(mPoint(bottomRight.x - radius, topLeft.y + radius), mPoint(bottomRight.x, bottomRight.y - radius), color);

    drawCircle(mPoint(topLeft.x + radius, topLeft.y + radius), radius, color);
    drawCircle(mPoint(topLeft.x + radius, bottomRight.y - radius), radius, color);
    drawCircle(mPoint(bottomRight.x - radius, topLeft.y + radius), radius, color);
    drawCircle(mPoint(bottomRight.x - radius, bottomRight.y - radius), radius, color);

    //drawCircle(topLeft,radius,color);
    //drawCircle(bottomRight,radius,color);
}




/*Backup before I start fucking with it
 * void myScr::drawText(std::string txt, mPoint StartPos, font* fnt, bool isCentered = false, u32 color = RGBA8_MAXALPHA(0xff, 0xff,
                     0xff)) {
    //float centeredOffset = 0;
    //unsigned int lineNumber = 0;
    //unsigned int lineWidth = 0;
    mPoint cursorPos = StartPos;
    //this is very broken and Im not sure how it doesnt crash
    int a = ((color & 0xff000000) >> 24); //0xff << 6*4
    int r = ((color & 0x000000ff) >> 0) * a / 0xff; //0xff << 0*4
    int g = ((color & 0x0000ff00) >> 8) * a / 0xff; //0xff << 2*4
    int b = ((color & 0x00ff0000) >> 16) * a / 0xff; //0xff << 4*4

    for(size_t pos = 0 ; pos < txt.length();) {
        if(txt.at(pos) != '\n') {
            int width, height, pngXStart, pngYStart, xOffset, yOffset, xAdvance;
            fnt->getLetterInfo(getCodePoint(txt, pos, pos), width, height, pngXStart, pngYStart, xOffset, yOffset, xAdvance);
            for(int y = pngYStart; y < (pngYStart + height); ++y) {
                for(int x = pngXStart; x < (pngXStart + width); ++x) {
                    int alpha = upng_get_buffer(fnt->fontPNG)[(y * fnt->pngWidth * fnt->pngDepth) + x * fnt->pngDepth + (fnt->pngDepth - 1)];
                    if(alpha) {
                        drawPixel(mPoint(cursorPos.x + (x - pngXStart + xOffset), cursorPos.y + (y - pngYStart + yOffset)), RGBA8(r, g, b,
                                  a * alpha / 0xff));
                    }
                }

            }
            cursorPos.x += width + xOffset;
            if(pos > 0) {
                if(txt.at(pos -1 ) == ' ') {cursorPos.x += xAdvance/2;}
            }

        }
        else {
            cursorPos.x = StartPos.x;
            cursorPos.y += fnt->charHeight * 4/5;
            pos++;
        }

    }



}



*/
