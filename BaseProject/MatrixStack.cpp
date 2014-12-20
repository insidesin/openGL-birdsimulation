#include "include/MatrixStack.h"
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

MatrixStack::MatrixStack(unsigned int num):stack_(0),tos(0)
{

    stack_ = new GLdouble[16*num];
    loadIdentity();
}


MatrixStack::~MatrixStack()
{

    if (!stack_) delete [] stack_;

}

const GLfloat *MatrixStack::getMatrixf()
{
    unsigned char i;
    for(i=0; i<16; result_[i]= (GLfloat) stack_[tos+i], i++);
    return result_;
}

const GLdouble *MatrixStack::getMatrixd()
{
    return &(stack_[tos]);
}

void MatrixStack::transformf(const GLfloat *s, GLfloat *d)
{
    d[0] = (stack_[tos]*s[0]+stack_[tos+4]*s[1]+stack_[tos+8]*s[2]+stack_[tos+12]*s[3]);
    d[1] = (stack_[tos+1]*s[0]+stack_[tos+5]*s[1]+stack_[tos+9]*s[2]+stack_[tos+13]*s[3]);
    d[2] = (stack_[tos+2]*s[0]+stack_[tos+6]*s[1]+stack_[tos+10]*s[2]+stack_[tos+14]*s[3]);
    d[3] = (stack_[tos+3]*s[0]+stack_[tos+7]*s[1]+stack_[tos+11]*s[2]+stack_[tos+15]*s[3]);
}

void MatrixStack::transformd(const GLdouble *s, GLdouble *d)
{

    d[0] = stack_[tos]*s[0]+stack_[tos+4]*s[1]+stack_[tos+8]*s[2]+stack_[tos+12]*s[3];
    d[1] = stack_[tos+1]*s[0]+stack_[tos+5]*s[1]+stack_[tos+9]*s[2]+stack_[tos+13]*s[3];
    d[2] = stack_[tos+2]*s[0]+stack_[tos+6]*s[1]+stack_[tos+10]*s[2]+stack_[tos+14]*s[3];
    d[3] = stack_[tos+3]*s[0]+stack_[tos+7]*s[1]+stack_[tos+11]*s[2]+stack_[tos+15]*s[3];
}
/*
10,000,000 multiplications
real    0m1.005s
user    0m0.868s
sys     0m0.000s
*/
void MatrixStack::mult(void)
{
    GLdouble w[16];
    unsigned char i;
    w[0] = stack_[tos]*tmpMat_[0]+stack_[tos+4]*tmpMat_[1]+stack_[tos+8]*tmpMat_[2]+stack_[tos+12]*tmpMat_[3];
    w[4] = stack_[tos]*tmpMat_[4]+stack_[tos+4]*tmpMat_[5]+stack_[tos+8]*tmpMat_[6]+stack_[tos+12]*tmpMat_[7];
    w[8] = stack_[tos]*tmpMat_[8]+stack_[tos+4]*tmpMat_[9]+stack_[tos+8]*tmpMat_[10]+stack_[tos+12]*tmpMat_[11];
    w[12] = stack_[tos]*tmpMat_[12]+stack_[tos+4]*tmpMat_[13]+stack_[tos+8]*tmpMat_[14]+stack_[tos+12]*tmpMat_[15];

    w[1] = stack_[tos+1]*tmpMat_[0]+stack_[tos+5]*tmpMat_[1]+stack_[tos+9]*tmpMat_[2]+stack_[tos+13]*tmpMat_[3];
    w[5] = stack_[tos+1]*tmpMat_[4]+stack_[tos+5]*tmpMat_[5]+stack_[tos+9]*tmpMat_[6]+stack_[tos+13]*tmpMat_[7];
    w[9] = stack_[tos+1]*tmpMat_[8]+stack_[tos+5]*tmpMat_[9]+stack_[tos+9]*tmpMat_[10]+stack_[tos+13]*tmpMat_[11];
    w[13] = stack_[tos+1]*tmpMat_[12]+stack_[tos+5]*tmpMat_[13]+stack_[tos+9]*tmpMat_[14]+stack_[tos+13]*tmpMat_[15];

    w[2] = stack_[tos+2]*tmpMat_[0]+stack_[tos+6]*tmpMat_[1]+stack_[tos+10]*tmpMat_[2]+stack_[tos+14]*tmpMat_[3];
    w[6] = stack_[tos+2]*tmpMat_[4]+stack_[tos+6]*tmpMat_[5]+stack_[tos+10]*tmpMat_[6]+stack_[tos+14]*tmpMat_[7];
    w[10] = stack_[tos+2]*tmpMat_[8]+stack_[tos+6]*tmpMat_[9]+stack_[tos+10]*tmpMat_[10]+stack_[tos+14]*tmpMat_[11];
    w[14] = stack_[tos+2]*tmpMat_[12]+stack_[tos+6]*tmpMat_[13]+stack_[tos+10]*tmpMat_[14]+stack_[tos+14]*tmpMat_[15];

    w[3] = stack_[tos+3]*tmpMat_[0]+stack_[tos+7]*tmpMat_[1]+stack_[tos+11]*tmpMat_[2]+stack_[tos+15]*tmpMat_[3];
    w[7] = stack_[tos+3]*tmpMat_[4]+stack_[tos+7]*tmpMat_[5]+stack_[tos+11]*tmpMat_[6]+stack_[tos+15]*tmpMat_[7];
    w[11] = stack_[tos+3]*tmpMat_[8]+stack_[tos+7]*tmpMat_[9]+stack_[tos+11]*tmpMat_[10]+stack_[tos+15]*tmpMat_[11];
    w[15] = stack_[tos+3]*tmpMat_[12]+stack_[tos+7]*tmpMat_[13]+stack_[tos+11]*tmpMat_[14]+stack_[tos+15]*tmpMat_[15];
    for (i=0; i<16; stack_[tos+i]=w[i], i++);

}

/*
10,000,000 multiplications
real    0m2.318s
user    0m1.781s
sys     0m0.001s
*/
void MatrixStack::mult2(void)
{
    GLdouble w[16];
    unsigned char t;
    unsigned char u;
    unsigned char v;
    for(t=0; t<4; t++)
        for(u=0; u<4; u++)
            {
                w[4*u+t]=0.0;
                for(v=0; v<4; v++)
                    w[4*u+t] += stack_[tos+4*v+t]*tmpMat_[4*u+v];
            }
    for (t=0; t<16; stack_[tos+t]=w[t], t++);
}


void MatrixStack::multMatrixd(const GLdouble *m)
{
    unsigned char t;
    for (t=0; t<16; tmpMat_[t]= m[t], t++);
    mult();
}

void MatrixStack::multMatrixf(const GLfloat *m)
{
    unsigned char t;
    for (t=0; t<16; tmpMat_[t]=(GLdouble) m[t], t++);
    mult();
}


void MatrixStack::printMatrix(void)
{
    unsigned char i;
    for (i=0; i<16;  i++)
        {
            if (i%4==0) printf("\n");
            printf("%g ", stack_[tos+i]);
        }
    printf("\n");
}

void MatrixStack::loadIdentity(void)
{
    unsigned char i;
    for ( i=0; i<16; stack_[tos+i]=0, i++);
    stack_[tos]=stack_[tos+5]=stack_[tos+10]=stack_[tos+15]=1.0;
}

void MatrixStack::loadMatrixd(const GLdouble *m)
{
    unsigned char i;
    for (i=0; i<16; stack_[tos+i]=m[i], i++);
}

void MatrixStack::loadMatrixf(const GLfloat *m)
{
    unsigned char i;
    for (i=0; i<16; stack_[tos+i]= (GLdouble) m[i], i++);
}

void MatrixStack::pushMatrix(void)
{
    unsigned char i;
    for ( i=0; i<16; stack_[tos+16+i] = stack_[tos+i], i++);
    tos+=16;
}

void MatrixStack::popMatrix(void)
{
    if (tos>15) tos -= 16;
}

void MatrixStack::ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear,GLdouble zFar)
{
    GLdouble rl = right - left;
    GLdouble tb = top - bottom;
    GLdouble fn = zFar-zNear;
    unsigned char i;
    for (i=0; i<16; tmpMat_[i++]=0);
    tmpMat_[0] = 2.0/rl;
    tmpMat_[12] = -(right+left)/rl;
    tmpMat_[5] = 2.0/tb;
    tmpMat_[13] = -(top+bottom)/tb;
    tmpMat_[10] = -2.0/fn;
    tmpMat_[14] = -(zFar+zNear)/fn;
    tmpMat_[15] = 1.0;
    mult();
}

void MatrixStack::frustum(GLdouble	 left, GLdouble	right, GLdouble	bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
    GLdouble rl = right - left;
    GLdouble tb = top - bottom;
    GLdouble fn = zFar-zNear;
    unsigned char i;
    for (i=0; i<16; tmpMat_[i++]=0);
    tmpMat_[0] = 2.0*zNear/rl;
    tmpMat_[8] = (right+left)/rl;
    tmpMat_[5] = 2.0*zNear/tb;
    tmpMat_[9] = (top+bottom)/tb;
    tmpMat_[10] = -(zFar+zNear)/fn;
    tmpMat_[14] = -2.0*zFar*zNear/fn;
    tmpMat_[11] = -1.0;
    mult();
}
void MatrixStack::perspective(GLdouble fov, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble nf = zNear-zFar;
    GLdouble f = 1.0/tan(M_PI*fov/360.0);
    for (unsigned char i=0; i<16; tmpMat_[i++]=0);

    tmpMat_[0] = f/aspect;
    tmpMat_[5] = f;
    tmpMat_[10] = (zNear+zFar)/nf;
    tmpMat_[11] = -1.0;
    tmpMat_[14] = 2.0*zNear*zFar/nf;
    mult();
}

void MatrixStack::lookAt(GLdouble ex, GLdouble ey, GLdouble ez, GLdouble ax, GLdouble ay, GLdouble az, GLdouble ux, GLdouble uy, GLdouble uz)

{
    //normalise vector n = (eye-at)
    tmpMat_[2] = (ex-ax);
    tmpMat_[6] = (ey-ay);
    tmpMat_[10] = (ez-az);
    tmpMat_[14] = 0.0;
    GLdouble length = sqrt(tmpMat_[2]*tmpMat_[2]+
                           tmpMat_[6]*tmpMat_[6]+
                           tmpMat_[10]*tmpMat_[10]);
    tmpMat_[2] /= length;
    tmpMat_[6] /= length;
    tmpMat_[10] /= length;

    //normalise vector u = (up cross n)
    tmpMat_[0] = uy*tmpMat_[10] - uz*tmpMat_[6];
    tmpMat_[4] = uz*tmpMat_[2] - ux*tmpMat_[10];
    tmpMat_[8] = ux*tmpMat_[6] - uy*tmpMat_[2];
    tmpMat_[12] = 0.0;

    length = sqrt(tmpMat_[0]*tmpMat_[0]+
                  tmpMat_[4]*tmpMat_[4]+
                  tmpMat_[8]*tmpMat_[8]);

    tmpMat_[0] /= length;
    tmpMat_[4] /= length;
    tmpMat_[8] /= length;

    //normalise vector v = (n cross u)
    tmpMat_[1] = tmpMat_[6]*tmpMat_[8] - tmpMat_[10]*tmpMat_[4];
    tmpMat_[5] = tmpMat_[10]*tmpMat_[0] - tmpMat_[2]*tmpMat_[8];
    tmpMat_[9] = tmpMat_[2]*tmpMat_[4] - tmpMat_[6]*tmpMat_[0];
    tmpMat_[13] = 0.0;

    length = sqrt(tmpMat_[1]*tmpMat_[1]+
                  tmpMat_[5]*tmpMat_[5]+
                  tmpMat_[9]*tmpMat_[9]);

    tmpMat_[1] /= length;
    tmpMat_[5] /= length;
    tmpMat_[9] /= length;

    tmpMat_[3] = tmpMat_[7]= tmpMat_[11] = 0.0;
    tmpMat_[15] = 1.0;

    mult();
    translated(-ex,-ey,-ez);

}

void MatrixStack::rotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
    angle = M_PI*angle/180.0;
    GLdouble s = sin(angle);
    GLdouble c = cos(angle);
    GLdouble d = 1.0-c;
    GLdouble nm = sqrt(x*x+y*y+z*z);
    x=x/nm;
    y=y/nm;
    z=z/nm;
    tmpMat_[0] = x*x*d+c;
    tmpMat_[4] = x*y*d-z*s;
    tmpMat_[8] = x*z*d+y*s;
    tmpMat_[12] = tmpMat_[13] = tmpMat_[14] = 0.0;
    tmpMat_[1] = y*x*d+z*s;
    tmpMat_[5] = y*y*d+c;
    tmpMat_[9] = y*z*d-x*s;
    tmpMat_[2] = z*x*d-y*s;
    tmpMat_[6] = z*y*d+x*s;
    tmpMat_[10] = z*z*d+c;
    tmpMat_[3] = tmpMat_[7] = tmpMat_[11] = 0.0;
    tmpMat_[15] = 1.0;
    mult();
}

void MatrixStack::rotatef(GLfloat angle, GLfloat	x, GLfloat	y, GLfloat	z)
{
    rotated((GLdouble) angle, (GLdouble)x, (GLdouble)y, (GLdouble)z);
}

void MatrixStack::scaled(GLdouble x, GLdouble y, GLdouble z)
{
    unsigned char i;
    for (i=0; i<16; tmpMat_[i++]=0);
    tmpMat_[0] = x;
    tmpMat_[5] = y;
    tmpMat_[10] = z;
    tmpMat_[15] = 1.0;
    mult();
}

void MatrixStack::scalef(GLfloat	 x, GLfloat	y, GLfloat z)
{
    scaled((GLdouble)x, (GLdouble)y, (GLdouble)z);
}

void MatrixStack::translated(GLdouble x, GLdouble y, GLdouble z)
{
    unsigned char i;
    for (i=0; i<16; tmpMat_[i++]=0);
    tmpMat_[12] = x;
    tmpMat_[13] = y;
    tmpMat_[14] = z;
    tmpMat_[0] = tmpMat_[5] = tmpMat_[10] = tmpMat_[15] = 1.0;
    mult();
}

void MatrixStack::translatef(GLfloat x, GLfloat y, GLfloat z)
{
    translated((GLdouble)x, (GLdouble) y, (GLdouble) z);
}

