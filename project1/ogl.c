#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



// Declare global variables for round, winner, and score
int roundNumber = 1;
char winner[50] = "No one";
float score = 0;

void readValuesFromFile() {
    FILE *match_file = fopen("match.txt", "r");
        char match[64];
        char *token;


        if(match_file == NULL){
          perror("Error opening match file");
        }
        else{
            fgets(match,64,match_file);
            token = strtok(match,",");  
            roundNumber = atoi(token);
            token = strtok(NULL, ",");
            strcpy(winner,token);
            token = strtok(NULL, "\n");
            score = atof(token);
        }
        fclose(match_file);
}


void init()
{
    // Set the background color to white
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void drawCircle(float x, float y, float radius, float r, float g, float b)
{
    int i;
    int triangleAmount = 100; // # of triangles used to draw circle

    // Set color to the given RGB values
    glColor3f(r, g, b);

    // Draw the circle as a triangle fan
    glBegin(GL_TRIANGLE_FAN);
    // Center point
    glVertex2f(x, y);
    for (i = 0; i <= triangleAmount; i++)
    {
        glVertex2f(
            x + (radius * cos(i * 2.0f * 3.14159f / triangleAmount)),
            y + (radius * sin(i * 2.0f * 3.14159f / triangleAmount)));
    }
    glEnd();
}

void drawText(char *string, float x, float y)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}

void display()
{
    // Clear the buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the four balls
    drawCircle(-0.7, 0.0, 0.2, 1.0, 0.0, 0.0);  // red ball 1
    drawCircle(-0.35, 0.0, 0.2, 1.0, 0.0, 0.0); // red ball 2
    drawCircle(0.35, 0.0, 0.2, 0.0, 0.0, 1.0);  // blue ball 1
    drawCircle(0.7, 0.0, 0.2, 0.0, 0.0, 1.0);   // blue ball 2

    // Draw the round text box
    glColor3f(1.0, 1.0, 1.0);     // set color to white
    glRectf(-0.5, 0.8, 0.5, 0.6); // draw rectangle
    char roundText[30];
    char winnerIs[100];
    char scored[30];

    glColor3f(0.0, 0.0, 0.0); // set color to black
    glLineWidth(2.0);         // set the line width to 2 pixels
    glBegin(GL_LINE_LOOP);    // draw the rectangle border
    glVertex2f(-0.5, 0.8);
    glVertex2f(0.5, 0.8);
    glVertex2f(0.5, 0.6);
    glVertex2f(-0.5, 0.6);
    glEnd();
    glColor3f(0.0, 0.0, 0.0);         // set color to black
    sprintf(roundText, "Round %d", roundNumber);
    drawText(roundText, -0.15, 0.68); // draw text
// Draw the winner and score text
    glColor3f(1.0, 0.84, 0.0); // set color to golden
    snprintf(winnerIs, sizeof(winnerIs), "The winner is: %s", winner);
    drawText(winnerIs, -0.8, -0.4); // draw winner text
    glColor3f(0.0, 0.0, 0.0); // set color to black
    sprintf(scored, "Score: %f", score);
    drawText(scored, -0.8, -0.6); // draw score text
    // Flush the buffer
    glFlush();
    readValuesFromFile() ;
    // call glutPostRedisplay to mark the window as needing to be redisplayed
  glutPostRedisplay();
  
  // swap the buffers to display the current image
  glutSwapBuffers();
}



int main(int argc, char **argv)
{

     
    
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Four Balls with Text Box");

    // Call the initialization function
    init();

    // Set the display function
    glutDisplayFunc(display);

    // Enter the main loop
    glutMainLoop();
    
    return 0;
}