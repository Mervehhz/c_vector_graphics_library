#include "epslib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FIG_NUM 6
#define LINE_NUM 11
#define EDGE_NUM 6

Figure * create_figure(Figure *);

void main(void){
    Figure figure[FIG_NUM];
    Point2D * poly_line = (Point2D*)malloc(sizeof(Point2D)),poly_line2[EDGE_NUM+1],* width_height = (Point2D*) malloc(sizeof(Point2D)),* centre = (Point2D*) malloc(sizeof(Point2D));;
    Point2D * start_roi = (Point2D*)malloc(sizeof(Point2D)),* end_roi = (Point2D*)malloc(sizeof(Point2D));
    start_roi->x = -50;     start_roi->y = 100;
    end_roi->x = 400;       end_roi->y = 150;

    /*starting figures*/
    for(int i=0;i<FIG_NUM;i++)
        figure[i] = *create_figure(&figure[i]);

    /*drawing poly line*/
     for(int i=0;i<LINE_NUM;i++){
        if((!(3-i) % 2) || !((i-3) % 2)){
            poly_line[i].x=(i+1)*50;
            poly_line[i].y=50;
        }
        else{
            poly_line[i].x=(i+1)*50;
            poly_line[i].y=0;
        }
        poly_line=(Point2D*)realloc(poly_line,sizeof(Point2D)*(i+2));
        poly_line[i].x -= 200;
        poly_line[i].y += 100;
    }
    draw_polyline(&figure[0],poly_line,LINE_NUM);
    resize_figure(&figure[0],start_roi,end_roi);
    
    /*drawing the fuction ex -> sin, cos etc*/
    draw_fx(&figure[1],cos,0,30,0.01);
    scale_figure(&figure[1],figure[1].resolution,figure[1].resolution);


    /*drawing hexagon*/
    poly_line2[0].x = 0; poly_line2[0].y = 250;
    poly_line2[1].x = 100;   poly_line2[1].y = 300;
    poly_line2[2].x = 250; poly_line2[2].y = 300;
    poly_line2[3].x = 350; poly_line2[3].y = 250;
    poly_line2[4].x = 250; poly_line2[4].y = 200;
    poly_line2[5].x = 100; poly_line2[5].y = 200;
    poly_line2[6].x = 0; poly_line2[6].y = 250;
    draw_polygon(&figure[2],poly_line2);


    /*drawing ellipse*/
    width_height->x = 8;
    width_height->y = 4;
    centre->x = 0;
    centre->y = 0;
    draw_ellipse(&figure[3],centre,width_height);
    scale_figure(&figure[3],figure[3].resolution,figure[3].resolution);


    /*backing up the figures we will append*/
    figure[4] = figure[2];
    figure[5] = figure[3];

    /*appending figures*/
    draw_ellipse(&figure[5],centre,width_height);
    scale_figure(&figure[5],figure[5].resolution,figure[5].resolution);
    append_figures(&figure[4],&figure[5]);

    /*converting to eps file*/
    export_eps(&figure[0],"poly_line.eps");
    export_eps(&figure[1],"draw_fx.eps");
    export_eps(&figure[2],"poly_gone.eps");
    export_eps(&figure[3],"ellipse.eps");
    export_eps(&figure[4],"appending.eps");
}

Figure * create_figure(Figure * figure){
    Color c;
    c.red = 0.0;  c.green = 0.0;  c.blue = 0.0;

    figure = start_figure(1000,1000);
    set_thickness_resolution(figure,0,30);
    set_color(figure,c);

    return figure;
}