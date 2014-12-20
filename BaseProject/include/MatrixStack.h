/*
A matrix stack.
Only pushing and popping alter the size of the stack
Continual popping from the stack will reach its bottom, but go no further
Continual pushing onto the stack will eventually segfault (STACKMAX times without pop)
Initally the TOS is the identity matrix
There is no debugging help here, no info about hitting the bottom or how many matrices are on the stack

*/
#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include<GL/glew.h>

class MatrixStack
{

    GLdouble tmpMat_[16];
    GLdouble *stack_;
    GLfloat result_[16];

    GLuint tos;
    void mult(void);
    void mult2(void);
    void mult1(void);
public:

    MatrixStack(unsigned int);
    ~MatrixStack(void);
    const GLfloat *getMatrixf();
    const GLdouble *getMatrixd();
    /* apply the TOS matrix to a point, source and destination
    are arrays of 4 elements */
    void transformf(const GLfloat *s, GLfloat *d);
    void transformd(const GLdouble *s, GLdouble *d);

    /* Emergency help */
    void printMatrix(void);

    /* replace the TOS matrix */
    void loadIdentity();
    void loadMatrixd(const GLdouble *m);
    void loadMatrixf(const GLfloat *m);

    /* multiply the TOS by m */
    void multMatrixd(const GLdouble *m);
    void multMatrixf(const GLfloat *m);

    /* dup the TOS with push, pop the TOS with pop */
    void pushMatrix(void);
    void popMatrix(void);

    /* the remaining construct a matrix and multiply the TOS for a new TOS, TOS=TOS*NEW */
    void ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
    void frustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
    void perspective(GLdouble fov, GLdouble aspect, GLdouble zNear, GLdouble zFar);
    void lookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble atx, GLdouble aty, GLdouble atz, GLdouble upx, GLdouble upy, GLdouble upz);
    void rotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
    void rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
    void scaled(GLdouble x, GLdouble y, GLdouble z);
    void scalef(GLfloat x, GLfloat y, GLfloat z);
    void translated(GLdouble x, GLdouble y, GLdouble z);
    void translatef(GLfloat x, GLfloat y, GLfloat z);
};

#endif //MATRIXSTACK_H
