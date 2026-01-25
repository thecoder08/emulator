#include "3dproc.h"
#include <xgfx/drawing.h>
#include <stdio.h>
#include <math.h>

unsigned char command = 0;
unsigned int byte = 0;
unsigned char activeObject = 0;
unsigned char faceIndex = 0;
unsigned char enabled3D = 1;

int width = 1024;
int height = 768;
int depthBuffer[1024*768];

Object objects[256];

void command3D(unsigned char newCommand) {
    command = newCommand;
    byte = 0;
    faceIndex = 0;
}

void data3D(unsigned char data) {
    switch(command) {
        case 0: // enable/disable 3DProc
        enabled3D = data;
        break;
        case 1: // update object data buffer
        if (byte == 0) {
            activeObject = data;
        }
        else if (byte % 19 == 1) {
            objects[activeObject].faces[faceIndex].positions[0][0] = data;
        }
        else if (byte % 19 == 2) {
            objects[activeObject].faces[faceIndex].positions[0][1] = data;
        }
        else if (byte % 19 == 3) {
            objects[activeObject].faces[faceIndex].positions[0][2] = data;
        }
        else if (byte % 19 == 4) {
            objects[activeObject].faces[faceIndex].positions[1][0] = data;
        }
        else if (byte % 19 == 5) {
            objects[activeObject].faces[faceIndex].positions[1][1] = data;
        }
        else if (byte % 19 == 6) {
            objects[activeObject].faces[faceIndex].positions[1][2] = data;
        }
        else if (byte % 19 == 7) {
            objects[activeObject].faces[faceIndex].positions[2][0] = data;
        }
        else if (byte % 19 == 8) {
            objects[activeObject].faces[faceIndex].positions[2][1] = data;
        }
        else if (byte % 19 == 9) {
            objects[activeObject].faces[faceIndex].positions[2][2] = data;
        }
        else if (byte % 19 == 10) {
            objects[activeObject].faces[faceIndex].normals[0][0] = data;
        }
        else if (byte % 19 == 11) {
            objects[activeObject].faces[faceIndex].normals[0][1] = data;
        }
        else if (byte % 19 == 12) {
            objects[activeObject].faces[faceIndex].normals[0][2] = data;
        }
        else if (byte % 19 == 13) {
            objects[activeObject].faces[faceIndex].normals[1][0] = data;
        }
        else if (byte % 19 == 14) {
            objects[activeObject].faces[faceIndex].normals[1][1] = data;
        }
        else if (byte % 19 == 15) {
            objects[activeObject].faces[faceIndex].normals[1][2] = data;
        }
        else if (byte % 19 == 16) {
            objects[activeObject].faces[faceIndex].normals[2][0] = data;
        }
        else if (byte % 19 == 17) {
            objects[activeObject].faces[faceIndex].normals[2][1] = data;
        }
        else if (byte % 19 == 18) {
            objects[activeObject].faces[faceIndex].normals[2][2] = data;
        }
        else if (byte % 19 == 0) {
            objects[activeObject].faces[faceIndex].color = data;
            faceIndex++;
            objects[activeObject].numFaces = faceIndex;
        }
        byte++;
        break;
        case 2: // update object translation
        if (byte == 0) {
            activeObject = data;
        }
        else if (byte % 3 == 1) {
            objects[activeObject].position[0] = data;
        }
        else if (byte % 3 == 2) {
            objects[activeObject].position[1] = data;
        }
        else if (byte % 3 == 0) {
            objects[activeObject].position[2] = data;
        }
        byte++;
        break;
        case 3: // update object rotation
        if (byte == 0) {
            activeObject = data;
        }
        else if (byte % 3 == 1) {
            objects[activeObject].rotation[0] = data;
        }
        else if (byte % 3 == 2) {
            objects[activeObject].rotation[1] = data;
        }
        else if (byte % 3 == 0) {
            objects[activeObject].rotation[2] = data;
        }
        byte++;
        case 4: // update camera translation
        if (byte % 3 == 0) 
        break;
        case 5:
        break;
        default:
        break;
    }
}

float max(char x, char y) { return (x>y) ? x : y; }
float min(char x, char y) { return (x<y) ? x : y; }

typedef float vec3f[3];

float edgeFunction(vec3f a, vec3f b, vec3f c) {
    return ((c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]));
}

void drawTriangle(vec3 v1, vec3 v2, vec3 v3, unsigned char color, int depth) {
    unsigned char red = color & 0b11000000;
    unsigned char green = (color & 0b00111000) << 2;
    unsigned char blue = (color & 0b00000111) << 5;
    int finalColor = (red << 16) | (green << 8) | blue;
    vec3f v1f;
    v1f[0] = v1[0];
    v1f[1] = v1[1];
    v1f[2] = v1[2];
    vec3f v2f;
    v2f[0] = v2[0];
    v2f[1] = v2[1];
    v2f[2] = v2[2];
    vec3f v3f;
    v3f[0] = v3[0];
    v3f[1] = v3[1];
    v3f[2] = v3[2];
    for (int i = (int)max(max(v1[1], v2[1]), v3[1])*-4 + (height/2); i < (int)min(min(v1[1], v2[1]), v3[1])*-4 + (height/2); i++) {
        for (int j = (int)min(min(v1[0], v2[0]), v3[0])*4 + (width/2); j < (int)max(max(v1[0], v2[0]), v3[0])*4 + (width/2); j++) {
            //plot(j, i, 0x00808080);
            if (i < 0 || i >= height || j < 0 || j >= width) {
                continue;
            }
            if (depth >= depthBuffer[i * width + j]) {
                continue;
            }
            vec3f sample;
            sample[0] = (j-(width/2))/4.f;
            sample[1] = (i-(height/2))/-4.f;
            if (((edgeFunction(v1f, v2f, sample) >= 0) && (edgeFunction(v2f, v3f, sample) >= 0) && (edgeFunction(v3f, v1f, sample) >= 0)) || ((edgeFunction(v1f, v2f, sample) <= 0) && (edgeFunction(v2f, v3f, sample) <= 0) && (edgeFunction(v3f, v1f, sample) <= 0))) {
                
                depthBuffer[i * width + j] = depth;
                plot(j, i, finalColor);
            }
        }
    }
}

typedef vec3f mat3f[3];

void mul_vec3_mat3(vec3f vector, mat3f matrix, vec3f dest) {
    dest[0] = (matrix[0][0] * vector[0]) + (matrix[0][1] * vector[1]) + (matrix[0][2] * vector[2]);
    dest[1] = (matrix[1][0] * vector[0]) + (matrix[1][1] * vector[1]) + (matrix[1][2] * vector[2]);
    dest[2] = (matrix[2][0] * vector[0]) + (matrix[2][1] * vector[1]) + (matrix[2][2] * vector[2]);
}

void rotationMatrixXYZ(float angleX, float angleY, float angleZ, mat3f rotationMatrix) {
    rotationMatrix[0][0] = cos(angleY)*cos(angleZ); rotationMatrix[0][1] = sin(angleX)*sin(angleY)*cos(angleZ)-cos(angleX)*sin(angleZ); rotationMatrix[0][2] = cos(angleX)*sin(angleY)*cos(angleZ)+sin(angleX)*sin(angleZ);
    rotationMatrix[1][0] = cos(angleY)*sin(angleZ); rotationMatrix[1][1] = sin(angleX)*sin(angleY)*sin(angleZ)+cos(angleX)*cos(angleZ); rotationMatrix[1][2] = cos(angleX)*sin(angleY)*sin(angleZ)-sin(angleX)*cos(angleZ);
    rotationMatrix[2][0] = -sin(angleY); rotationMatrix[2][1] = sin(angleX)*cos(angleY); rotationMatrix[2][2] = cos(angleX)*cos(angleY);
}

void update3D() {
    if (enabled3D == 0) {
        return;
    }
    for (int i=0; i<width*height; ++i) depthBuffer[i] = 0x7fffffff;
    for (int i = 0; i < 256; i++) {
        mat3f rotationMatrix;
        objects[i].rotation[0]++;
        objects[i].rotation[1]++;
        rotationMatrixXYZ((float)objects[i].rotation[0]*3.14159/128, (float)objects[i].rotation[1]*3.14159/128, (float)objects[i].rotation[2]*3.14159/128, rotationMatrix);
        for (int j = 0; j < objects[i].numFaces; j++) { // if numFaces is 0, object simply won't be drawn
            // rotate vertices
            vec3f v1 = {objects[i].faces[j].positions[0][0], objects[i].faces[j].positions[0][1], objects[i].faces[j].positions[0][2]};
            vec3f v2 = {objects[i].faces[j].positions[1][0], objects[i].faces[j].positions[1][1], objects[i].faces[j].positions[1][2]};
            vec3f v3 = {objects[i].faces[j].positions[2][0], objects[i].faces[j].positions[2][1], objects[i].faces[j].positions[2][2]};
            vec3f v1f;
            vec3f v2f;
            vec3f v3f;
            mul_vec3_mat3(v1, rotationMatrix, v1f);
            mul_vec3_mat3(v2, rotationMatrix, v2f);
            mul_vec3_mat3(v3, rotationMatrix, v3f);
            vec3 v1rotated;
            v1rotated[0] = v1f[0]; v1rotated[1] = v1f[1]; v1rotated[2] = v1f[2];
            vec3 v2rotated;
            v2rotated[0] = v2f[0]; v2rotated[1] = v2f[1]; v2rotated[2] = v2f[2];
            vec3 v3rotated;
            v3rotated[0] = v3f[0]; v3rotated[1] = v3f[1]; v3rotated[2] = v3f[2];
            // translate vertices
            v1rotated[0] += objects[i].position[0];
            v1rotated[1] += objects[i].position[1];
            v1rotated[2] += objects[i].position[2];
            drawTriangle(v1rotated, v2rotated, v3rotated, objects[i].faces[j].color, objects[i].faces[j].positions[0][2]);
        }
    }
}

void init3D() {
    objects[0].faces[0].positions[0][0] = 10;
    objects[0].faces[0].positions[0][1] = 10;
    objects[0].faces[0].positions[0][2] = 0;
    objects[0].faces[0].positions[1][0] = -10;
    objects[0].faces[0].positions[1][1] = 10;
    objects[0].faces[0].positions[1][2] = 0;
    objects[0].faces[0].positions[2][0] = 0;
    objects[0].faces[0].positions[2][1] = -10;
    objects[0].faces[0].positions[2][2] = 0;
    objects[0].faces[0].color = 200; // red
    objects[0].numFaces = 1;
}