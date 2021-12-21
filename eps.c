#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "epslib.h"

#define EDGE_NUM 6

double ellipse_func(float, float, float);

/*determination of framework size (bounding box)*/
Figure * start_figure(double width, double height){
    Figure *fig=(Figure*)malloc(sizeof(Figure));

	fig->urx=width/2.0;
	fig->ury=height/2.0;
	fig->llx=-1*(width/2.0); 
	fig->lly=-1*(height/2.0);

    return fig;
}

void set_thickness_resolution(Figure * fig, double thickness, double resolution){

    fig->thickness=thickness;
    fig->resolution=resolution;
}

void set_color(Figure * fig, Color c){
  
    fig->color.red=c.red;
    fig->color.green=c.green;
    fig->color.blue=c.blue;
}

/*in this algorithm, every step is summed with x step size and every new x is sent to f (x) function*/
void draw_fx(Figure * fig, double f(double x), double start_x, double end_x, double step_size){
    Point2D * iter=NULL;
    int counter=(end_x-start_x)/step_size; /*counter = number of steps*/
   
    while(counter+1){
        /*if the linklist is empty*/
        if(iter == NULL){
           iter=(Point2D*)malloc(sizeof(Point2D));
           iter->x=start_x;
           iter->y=f(start_x);
           iter->next=NULL;
           fig->point=iter;
        }
        else{
            while(iter->next != NULL) iter=iter->next;

                start_x += step_size;
                iter->next=(Point2D*)malloc(sizeof(Point2D));
                iter->next->x=start_x;
                iter->next->y=f(start_x);
                iter->next->next=NULL;
        }
        counter--;
    }
}

/*this function draws a lot of  line*/
void draw_polyline(Figure * fig, Point2D * poly_line, int n){
    Point2D * iter = NULL;
    
    for(int i=0;i<n;i++){

        if(iter == NULL){
            iter = (Point2D*)malloc(sizeof(Point2D));
            iter->x=poly_line[i].x;
            iter->y=poly_line[i].y;
            iter->next=NULL;
            fig->point=iter;
        }
        else{
            while(iter->next != NULL) iter=iter->next;

            iter->next = (Point2D*)malloc(sizeof(Point2D));
            iter->next->x=poly_line[i].x;
            iter->next->y=poly_line[i].y;
            iter->next->next=NULL;
        }
    }
}

void draw_polygon(Figure * fig, Point2D * poly_line){
    Point2D * iter = NULL;

    for(int i=0;i<EDGE_NUM+1;i++){

        if(iter == NULL){
            iter = (Point2D*)malloc(sizeof(Point2D));
            iter->x=poly_line[i].x;
            iter->y=poly_line[i].y;
            iter->next=NULL;
            fig->point=iter;
        }
        else{
            while(iter->next != NULL) iter=iter->next;

            iter->next = (Point2D*)malloc(sizeof(Point2D));
            iter->next->x=poly_line[i].x;
            iter->next->y=poly_line[i].y;
            iter->next->next=NULL;
        }
    }
}

void draw_ellipse(Figure * fig, Point2D * centre, Point2D * width_height){
    Point2D * iter=NULL;
    /*Since there are 4 regions in the coordinate system and the step size is 0.01, the number of steps is up to counter.*/
    int counter=(width_height->x*4)/0.01,temp = counter,x=width_height->x,y=width_height->y,flag=0;
   
    while(counter+1){
 
        if(iter == NULL){
           iter=(Point2D*)malloc(sizeof(Point2D));
           iter->x=width_height->x;
           iter->y=ellipse_func(x,y,width_height->x);
           iter->next=NULL;
           fig->point=iter;
        }
        else{
            while(iter->next != NULL) iter=iter->next;

                if(counter == temp/2)   flag = 1; /*if it has saved the coordinates of the half of the ellipse, the flag that controls the increase and decrease of the x coordinate*/
                if(!flag)                width_height->x -= 0.01;
                else                     width_height->x += 0.01;
                    
                iter->next=(Point2D*)malloc(sizeof(Point2D));
                iter->next->x=width_height->x;
                /*checking whether the y coordinate is positive or negative*/
                if(!flag)
                    iter->next->y=ellipse_func(x,y,width_height->x);
                else
                    iter->next->y=-1*(ellipse_func(x,y,width_height->x));
                iter->next->next=NULL;
        }
        counter--;
    }
}

/*ellipse formul is (x^2/a^2 + y^2/b^2 = 1)*/
double ellipse_func(float x, float y, float i){
    double a = y*y*(1-i*i/(x*x));

    if(a < 0)   a *= -1;
    return sqrt(a);
}

/*this function allows the graphic to be enlarged or reduced*/
void scale_figure(Figure * figure, double scale_x, double scale_y){
    Point2D * temp = figure->point, * iter;

    for(int i=0;temp != NULL;i++,temp = temp->next){
        temp->x *= scale_x;
        temp->y *= scale_y;
        if(i == 0)  iter = temp;
    }
    figure->point = iter;
}

/*this function is used to crop or expand graphics*/
void resize_figure(Figure * figure, Point2D *start_roi, Point2D *end_roi){
    Point2D * iter = (Point2D*)malloc(sizeof(Point2D)), *root, *temp = figure->point;
    
    for(;temp->x < start_roi->x;temp=temp->next);/*to jump smaller than start_roi->x*/
    
    /*if smallest point is start_roi*/
    if(temp->x != start_roi->x){
        iter->x = start_roi->x;
        iter->y = start_roi->y;
    }
    /*if start_roi points are equal to figure->point's first point*/
    else{
        iter->x = temp->x;
        iter->y = temp->y;
        temp = temp->next;
    }
    root = iter; /*in order not to lose root*/
    while(figure->point->next != NULL)    figure->point = figure->point->next; /*go to the end of the link list*/

    /*if end_roi is bigger than the end point of the figure->point, that is, it wants to expand the graphic*/
    if(figure->point->x < end_roi->x){
        for(;temp != NULL;temp = temp->next){
            iter->next = (Point2D*) malloc(sizeof(Point2D));
            iter = iter->next;
            iter->x = temp->x;
            iter->y = temp->y;
        }
        while(iter->next != NULL)   iter = iter->next;
        iter->next = (Point2D*) malloc(sizeof(Point2D));
        iter->next->x = end_roi->x;
        iter->next->y = end_roi->y;
    }/*if end_roi is smaller than the end point of the figure->point, that is, it wants to expand the graphic*/
    else{
        for(;(temp->x < end_roi->x || temp->x == end_roi->x) && temp != NULL;temp = temp->next){
            iter->next = (Point2D*) malloc(sizeof(Point2D));
            iter = iter->next;
            iter->x = temp->x;
            iter->y = temp->y;
        }
    }
    figure->point = root;
}

/*this function is used to merge two graphs.*/
void append_figures(Figure * fig1, Figure * fig2){
    Point2D * temp = fig2->point,*root = temp;
    
    while(temp->next != NULL)    temp = temp->next;
    
    temp->next = (Point2D*) malloc(sizeof(Point2D));
    temp->next = fig1->point; /*to add second graphic*/
    temp->next->flag = 1; /*flag to use the moveto command that switches between 2 graphics*/
    
    fig1->point = root;
}
 
void export_eps(Figure * fig, char * file_name){
    Point2D * temp;
    FILE * fp=fopen(file_name,"w");

    fprintf(fp,"%%!PS-Adobe-3.0 EPSF-3.0");
    fprintf(fp,"\n%%%%BoundingBox: %d %d %d %d",(int)fig->llx,(int)fig->lly,(int)fig->urx,(int)fig->ury);
    fprintf(fp,"\n%d setlinewidth",(int)fig->thickness);
    fprintf(fp,"\n%.1f %.1f %.1f setrgbcolor",fig->color.red,fig->color.green,fig->color.blue);
    fprintf(fp,"\n%.1f %.1f moveto",fig->point->x,fig->point->y);

    for(temp = fig->point->next;temp != NULL;temp=temp->next){
        fprintf(fp,"\n%lf %lf ",temp->x,temp->y);
        if(temp->flag == 1)  fprintf(fp,"moveto");
        else                 fprintf(fp,"lineto");
    }

    while(fig->point->next != NULL) fig->point = fig->point->next;
    fprintf(fp,"\n%lf %lf lineto",fig->point->x,fig->point->y);
    
    fprintf(fp,"\nstroke\n");
    fclose(fp);
}