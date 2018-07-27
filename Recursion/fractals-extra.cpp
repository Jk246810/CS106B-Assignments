
#include "fractals.h"
#include <iostream>
#include <cmath>
#include "simpio.h"
#include "gwindow.h"
using namespace std;

void checkUserOrder(int order){
    if (order<0) {
        throw "you cannot enter a negative order value";
    }
}

/**
 * @brief drawFractalSquare: repeates a square pattern to make interesting shapes
 * @param window: the window which the shape will be pasted on
 * @param x: the starting x-value to place the pattern
 * @param y: the starting y-value to place the pattern
 * @param size: the desired size of the pattern
 * @param order: how many times the pattern is repeated
 */
void drawFractalSquare(GWindow& window, double x, double y, double size, int order){
    int nsize = size/3;
    checkUserOrder(order);
    if(order == 1){
        window.drawRect(x,y,nsize,nsize);
        window.drawRect(x-nsize,y-nsize,nsize,nsize);
        window.drawRect(x+nsize,y-nsize,nsize,nsize);
        window.drawRect(x+nsize,y+nsize,nsize,nsize);
        window.drawRect(x-nsize,y+nsize,nsize,nsize);
        //draws the pattern, draws rectangles with touching vertices
    }else{
        drawFractalSquare(window, x, y, nsize, order-1);
        drawFractalSquare(window, x-nsize,y-nsize, nsize, order-1);
        drawFractalSquare(window, x+nsize,y-nsize, nsize, order-1);
        drawFractalSquare(window, x+nsize,y+nsize, nsize, order-1);
        drawFractalSquare(window, x-nsize,y+nsize, nsize, order-1);
        //recursively calls the function to nestle the squares inside their larger containers
        //call the function five times because the first order function has five squares
    }
}


/**
 * @brief drawSierpinskiTriangle: draws the sierpinski triangle pattern
 * @param window: the window which the shape will be pasted on
 * @param x: the starting x-value to place the pattern
 * @param y: the starting y-value to place the pattern
 * @param size: the desired size of the pattern
 * @param order:  how many times the pattern is repeated
 */
void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order) {
    checkUserOrder(order);
    int halfSize = size/2;
    int height = sqrt((size*size) - (halfSize*halfSize));
    if(order == 1){
        window.drawLine(x,y, x+size, y);
        window.drawLine(x+size, y, x+halfSize, y+height);
        window.drawLine(x+halfSize, y+height, x, y);
        //draws the triangles
    }else{
        drawSierpinskiTriangle(window, x, y, size/2, order-1);
        drawSierpinskiTriangle(window, x+size/2, y, size/2, order-1);
        drawSierpinskiTriangle(window, x+size/4, y+(height)/2, size/2, order-1);
        //recursively calls the function three times because the pattern has 3 triangles
        //calls it recursively to nestle the small triangles into the larger triangles.
    }

}


/**
 * @brief drawBranch: helper function for draw tree in order to draw the branches
 * @param gw: the window which the branches will be pasted on
 * @param pt: the starting point of the branch
 * @param size: the size of the branch
 * @param angle: the angle of the line which creates the branch
 * @param order: the number of times the pattern is repeated
 */
void drawBranch(GWindow& gw,GPoint pt,int size,int angle,int order){
    gw.setColor("#8b7765");
   /*this is here so that if the color is set to green it will set the color
    back to brown for the next function call*/
    if(order == 1){
        gw.setColor("#2e8b57");
        /*makes the last branch green*/
    }
    if(order>0){
        pt = gw.drawPolarLine(pt, size, angle);
        /*draws the line which creates the branch and records the point*/
        drawBranch(gw, pt, size/2, angle+45, order-1);
        drawBranch(gw, pt, size/2, angle+30, order-1);
        drawBranch(gw, pt, size/2, angle+15, order-1);
        drawBranch(gw, pt, size/2, angle, order-1);
        drawBranch(gw, pt, size/2, angle-15, order-1);
        drawBranch(gw, pt, size/2, angle-30, order-1);
        drawBranch(gw, pt, size/2, angle-45, order-1);
        /*draws the seven attached branches to the single drawn branch using the
         * recorded point for its starting point*/

    }
}


/**
 * @brief drawTree: creates the tree
 * @param gw: the window which the branches will be pasted on
 * @param x: the starting x-value to place the pattern
 * @param y: the starting y-value to place the pattern
 * @param size: the desired size of the pattern
 * @param order: how many times the pattern is repeated
 */
void drawTree(GWindow& gw, double x, double y, double size, int order) {
    checkUserOrder(order);
    //checks if the given order is valid
    GPoint pt(x+size/2,y+size);
    //gets the initial point from user input
   drawBranch(gw, pt, size/2, 90, order);
   /* must call a helper function in order to keep track of the point
    * which is a parameter that the program needs to know but not the user
    */
}


