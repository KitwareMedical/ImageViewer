//
// "Id"
//
// My3DView class implementation for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2002 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#include "My3DView.h"
#include <math.h>


#if HAVE_GL
My3DView::My3DView(int x,int y,int w,int h,const char *l)
            : Fl_Gl_Window(x,y,w,h,l)
#else
My3DView::My3DView(int x,int y,int w,int h,const char *l)
            : Fl_Box(x,y,w,h,l)
#endif /* HAVE_GL */
{
    vAng = 0.0;
    hAng=0.0;
    size=1.0;
    xshift=0.0;
    yshift=0.0;
   
#if !HAVE_GL
    label("OpenGL is required for this demo to operate.");
    align(FL_ALIGN_WRAP | FL_ALIGN_INSIDE);
#endif /* !HAVE_GL */
}

#if HAVE_GL
void My3DView::drawObject() {
    int  k = 0;
    long num;
    double xmax, ymax, zmax, xmin, ymin, zmin, xmid, ymid, zmid;
    meshType::PointType     node;
    const unsigned long     *tp;
    meshType::PointType*    node_pt;
    node_pt = &node;

    meshType::PointsContainerPointer    points;
    meshType::PointsContainerIterator   pointsit;
    meshType::CellsContainerPointer     cells;
    meshType::CellsContainerIterator    cellsit;

    points = m_mesh->GetPoints();
    pointsit = points->Begin();
    cells = m_mesh->GetCells();
    cellsit = cells->Begin();

    num = m_mesh->GetNumberOfPoints();
    xmax = ymax = zmax = 0.0;
    xmin = ymin = zmin = 10000.0;
    while ( k < num ) {
        node = pointsit.Value();
        if (node[0] > xmax) xmax = node[0];
        if (node[0] < xmin) xmin = node[0];
        if (node[1] > ymax) ymax = node[1];
        if (node[1] < ymin) ymin = node[1];
        if (node[2] > zmax) zmax = node[2];
        if (node[2] < zmin) zmin = node[2];
        k++;
        ++pointsit;
    }

    xmid = (xmax+xmin)*0.5;
    ymid = (ymax+ymin)*0.5;
    zmid = (zmax+zmin)*0.5;

    GLfloat light_ambient[]  = {1.0, 1.0, 1.0, 0.5};
    GLfloat light_diffuse[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_specular[]   = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[]  = {50};
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};

    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    k = 0;
    while( k < m_mesh->GetNumberOfCells()) {
        tp = cellsit.Value()->GetPointIds();
        glBegin(GL_POLYGON);
        m_meshnormals->GetPoint(tp[0], node_pt);
        glNormal3f(node[0], node[1], node[2]);
        m_mesh->GetPoint(tp[0], node_pt);
        glVertex3f(node[0]-xmid, node[1]-ymid, node[2]-zmid);
        m_meshnormals->GetPoint(tp[1], node_pt);
        glNormal3f(node[0], node[1], node[2]);
        m_mesh->GetPoint(tp[1], node_pt);
        glVertex3f(node[0]-xmid, node[1]-ymid, node[2]-zmid);
        m_meshnormals->GetPoint(tp[2], node_pt);
        glNormal3f(node[0], node[1], node[2]);
        m_mesh->GetPoint(tp[2], node_pt);
        glVertex3f(node[0]-xmid, node[1]-ymid, node[2]-zmid);
        glEnd();

        ++cellsit;
        k++;
    }
}//drawObject

void My3DView::setup() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glLineWidth(1.0f);
}

void My3DView::loadObject(meshPointer mesh) {
    m_mesh = mesh; 
}

void My3DView::loadNormals(meshPointer meshnormals) {
    m_meshnormals = meshnormals; 
}

void My3DView::draw() {
    if (!valid()) {
        glLoadIdentity();
        glViewport(0,0,w(),h());
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        gluLookAt(0.0, 0.0, 250.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity(); 
        gluPerspective(60.0, w()/(h()+0.0001), 5, 1000);
        glColor3f(1.0, 0.0, 0.0);
        glEnable(GL_DEPTH_TEST);
//        glOrtho(-10,10,-10,10,-20020,10000);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(xshift, yshift, 0);
    glRotatef(hAng,0,1,0); glRotatef(vAng,1,0,0);
    glScalef(float(size),float(size),float(size));

    if (m_mesh) drawObject();
    
    glPopMatrix();
}
#endif /* HAVE_GL */

//
// End of "Id".
//
