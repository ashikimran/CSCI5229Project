
/*
 *  Flying dragon
 *
 *  Loads a dragon model and simulates the dragon's flight in different weathers and environments
 *  
 *  Key bindings:
 *  l/L        Toggle lighting on/off
 *  f/F        Toggle flight/static mode
 *  e/E        Toggle surrounding environment  
 *  g/G        Fireballs ON/OFF
 *  t/T        Dragon breathe fire ON/OFF
 *  h/H        Snow ON/OFF
 *  r/R        Rain ON/OFF 
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */


#include "CSCIx229.h"
#define	MAX_PARTICLES 10000     // Number Of Particles To Create

int    fly=0;    //  Animated flight
int    axes=0;   //  Display axes
int    move=0;   //  Light movement
int    light=0;  //  Lighting
int    th=0;     //  Azimuth of view angle
int    ph=0;     //  Elevation of view angle
int    zh=0;     //  Azimuth of light
double Yl=2;     //  Elevation of light
int    fov=55;   //  Field of view (for perspective)
double asp=1;    //  Aspect ratio
double dim=3.0;  //  Size of world
int    box=1;    //  Draw sky
int    sky[30];   //  Sky textures
double X  = 0;   //  Location
double Y  = 0;   //  Location
double Z  = 0;   //  Location
double Dx = 1;   //  Direction
double Dy = 0;   //  Direction
double Dz = 0;   //  Direction
double Sx = 1;   //  Sideways
double Sy = 0;   //  Sideways
double Sz = 0;   //  Sideways
double Ux = 1;   //  Up
double Uy = 0;   //  Up
double Uz = 0;   //  Up
double Ox = 0;   //  LookAt
double Oy = 0;   //  LookAt
double Oz = 0;   //  LookAt
double Ex = 1;   //  Eye
double Ey = 1;   //  Eye
double Ez = 1;   //  Eye
int obj;          //  Object display list
float RGBA[4] = {1,1,1,1};  //  Colors
double scale=1.0; //  Image scale
int env = 0;     // Skybox environment
int fball = 0;   // Fireballs
int snow = 0;    //  Snow simulation
int rain = 0;    // Rain simulation
int fbreathe = 0; // Fire-breathe effect
int c  = 1;
int environ = 0;


// Light values
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
unsigned int texture[7];
int true = 1;

float	zoom=-40.0f;	// Used To Zoom Out
int loop;		// Misc Loop Variable

// Fire breathe variables
float	slowdownfire=2.0f;	// Slow Down Particles
float xspeedfire;		// Base X Speed 
float yspeedfire;		// Base Y Speed 

// Snow variables
float slowdownsnow=0.01f;	// Slow Down Particles
float xspeedsnow;		// Base X Speed 
float yspeedsnow;		// Base Y Speed 

// Rain variables
float slowdownrain=0.01f;	// Slow Down Particles
float xspeedrain;		// Base X Speed 
float yspeedrain;		// Base Y Speed 




// Create structure of a single particle
typedef struct						
{
   float life;	 // Particle Life
   float fade;	 // Fade Speed
   float r;	 // Red Value
   float g;	 // Green Value
   float b;	 // Blue Value
   float x;	 // X Position
   float y;	 // Y Position
   float z;	 // Z Position
   float xi;	 // X Direction
   float yi;	 // Y Direction
   float zi;	 // Z Direction
   float xg;	 // X Gravity
   float yg;	 // Y Gravity
   float zg;	 // Z Gravity
}
particlesfire;	 // Particles Structure


// Create structure of a single particle
typedef struct						
{
   float life;	 // Particle Life
   float fade;	 // Fade Speed
   float r;	 // Red Value
   float g;	 // Green Value
   float b;	 // Blue Value
   float x;	 // X Position
   float y;	 // Y Position
   float z;	 // Z Position
   float xi;	 // X Direction
   float yi;	 // Y Direction
   float zi;	 // Z Direction
   float xg;	 // X Gravity
   float yg;	 // Y Gravity
   float zg;	 // Z Gravity
}
particlessnow;	         // Particles Structure



// Create structure of a single particle
typedef struct						
{
   float life;	 // Particle Life
   float fade;	 // Fade Speed
   float r;	 // Red Value
   float g;	 // Green Value
   float b;	 // Blue Value
   float x;	 // X Position
   float y;	 // Y Position
   float z;	 // Z Position
   float xi;	 // X Direction
   float yi;	 // Y Direction
   float zi;	 // Z Direction
   float xg;	 // X Gravity
   float yg;	 // Y Gravity
   float zg;	 // Z Gravity
}
particlesrain;	         // Particles Structure


particlesfire particlefire[MAX_PARTICLES];	// Particle Array (Room For Particle Info)
particlessnow particlesnow[MAX_PARTICLES];	// Particle Array (Room For Particle Info)
particlesrain particlerain[MAX_PARTICLES];	// Particle Array (Room For Particle Info)


/*
 * Initialize properties of fire particles
 */
void initglfire()										
{	
   glShadeModel(GL_SMOOTH);				// Enable Smooth Shading
   glClearDepth(1.0f);					// Depth Buffer Setup
   glDisable(GL_DEPTH_TEST);				// Disable Depth Testing
   glEnable(GL_BLEND);					// Enable Blending
   glBlendFunc(GL_SRC_ALPHA,GL_ONE);			// Type Of Blending To Perform
   glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	// Really Nice Perspective Calculations
   glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);			// Really Nice Point Smoothing
   glEnable(GL_TEXTURE_2D);				// Enable Texture Mapping
   glBindTexture(GL_TEXTURE_2D,texture[0]);		// Select Our Texture

   for (loop=0;loop<MAX_PARTICLES;loop++)			
      {
         particlefire[loop].life=1.0f;				// Give All The Particles Full Life
	 particlefire[loop].fade=(rand()%100)/1000.0f+0.003f;	// Random Fade Speed
		
         // Set colour of the fire particles
         particlefire[loop].r= 1;	
	 particlefire[loop].g= 0.27;	
	 particlefire[loop].b= 0;	

         // Set pull in the direction of the 3 axes
         particlefire[loop].xg=0.7f;					
	 particlefire[loop].yg=0.7f;					
	 particlefire[loop].zg=0.7f;									
     }
     glDisable(GL_BLEND);
     glEnable(GL_DEPTH_TEST);
}


/*
 * Initialize properties of snow particles
 */

void initglsnow()										
{
	
   glShadeModel(GL_SMOOTH);				// Enable Smooth Shading
   glClearDepth(1.0f);					// Depth Buffer Setup
   glDisable(GL_DEPTH_TEST);				// Disable Depth Testing
   glEnable(GL_BLEND);					// Enable Blending
   glBlendFunc(GL_SRC_ALPHA,GL_ONE);			// Type Of Blending To Perform
   glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	// Really Nice Perspective Calculations
   glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);			// Really Nice Point Smoothing
   glEnable(GL_TEXTURE_2D);				// Enable Texture Mapping
   glBindTexture(GL_TEXTURE_2D,texture[0]);		// Select Our Texture

   for (loop=0;loop<MAX_PARTICLES;loop++)			
      {
         particlesnow[loop].life=1.0f;				// Give All The Particles Full Life
	 particlesnow[loop].fade=(rand()%100)/1000.0f+0.003f;	// Random Fade Speed
		
         // Set colour of the snow particles
         particlesnow[loop].r= 1;	
	 particlesnow[loop].g= 1;	
	 particlesnow[loop].b= 1;	

         // Set pull in the direction of the 3 axes
         particlesnow[loop].xg=0.8f;					
	 particlesnow[loop].yg=0.8f;					
	 particlesnow[loop].zg=0.8f;									
     }
     glDisable(GL_BLEND);
     glEnable(GL_DEPTH_TEST);
}




/*
 * Initialize properties of rain particles
 */

void initglrain()										
{
	
   glShadeModel(GL_SMOOTH);				// Enable Smooth Shading
   glClearDepth(1.0f);					// Depth Buffer Setup
   glDisable(GL_DEPTH_TEST);				// Disable Depth Testing
   glEnable(GL_BLEND);					// Enable Blending
   glBlendFunc(GL_SRC_ALPHA,GL_ONE);			// Type Of Blending To Perform
   glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	// Really Nice Perspective Calculations
   glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);			// Really Nice Point Smoothing
   glEnable(GL_TEXTURE_2D);				// Enable Texture Mapping
   glBindTexture(GL_TEXTURE_2D,texture[0]);		// Select Our Texture

   for (loop=0;loop<MAX_PARTICLES;loop++)			
      {
         particlerain[loop].life=1.0f;				// Give All The Particles Full Life
	 particlerain[loop].fade=(rand()%100)/1000.0f+ 15.0f + 0.003f;	// Random Fade Speed
		
         // Set colour of the fire particles
         particlerain[loop].r= 1;	
	 particlerain[loop].g= 1;	
	 particlerain[loop].b= 1;	

         // Set pull in the direction of the 3 axes
         particlerain[loop].xg=0.0f;					
	 particlerain[loop].yg=-0.8f;					
	 particlerain[loop].zg=0.0f;									
     }
     glDisable(GL_BLEND);
     glEnable(GL_DEPTH_TEST);
}




void DrawFire()										
{
   for (loop=0;loop<MAX_PARTICLES;loop++)					
      {
         if (true == 1)							
	    {
		float x=particlefire[loop].x;						
		float y=particlefire[loop].y;						
		float z=particlefire[loop].z+zoom;					

		// Draw The Particle Using Our RGB Values and fade it based on life
		glColor4f(particlefire[loop].r,particlefire[loop].g,particlefire[loop].b,particlefire[loop].life);

		glBegin(GL_TRIANGLE_STRIP);						
		    glTexCoord2d(1,1); glVertex3f(x+0.5f,y+0.5f,z); 
		    glTexCoord2d(0,1); glVertex3f(x-0.5f,y+0.5f,z); 
		    glTexCoord2d(1,0); glVertex3f(x+0.5f,y-0.5f,z); 
	            glTexCoord2d(0,0); glVertex3f(x-0.5f,y-0.5f,z); 
		glEnd();										

		particlefire[loop].x+=particlefire[loop].xi/(slowdownfire*1000);// Move On The X Axis By X Speed
		particlefire[loop].y+=particlefire[loop].yi/(slowdownfire*1000);// Move On The Y Axis By Y Speed
		particlefire[loop].z+=particlefire[loop].zi/(slowdownfire*1000);// Move On The Z Axis By Z Speed

		particlefire[loop].xi+=particlefire[loop].xg;	            // Take Pull On X Axis Into Account
		particlefire[loop].yi+=particlefire[loop].yg;		    // Take Pull On Y Axis Into Account
		particlefire[loop].zi+=particlefire[loop].zg;		    // Take Pull On Z Axis Into Account
		particlefire[loop].life-=particlefire[loop].fade;	    // Reduce Particles Life By 'Fade'

		if (particlefire[loop].life<0.0f)					// If Particle Is Burned Out
		   {
		      particlefire[loop].life=1.0f;					// Give It New Life
		      particlefire[loop].fade=(rand()%100)/1000.0f+0.003f;	// Random Fade Value
                      
                      // Reset particle position
		      particlefire[loop].x=-7.0f;				
	              particlefire[loop].y=-10.0f;			        
	              particlefire[loop].z=7.0f;
			        
		      particlefire[loop].xi=xspeedfire+((rand()%60)-32.0f);	// X Axis Speed And Direction
		      particlefire[loop].yi=yspeedfire+((rand()%60)-30.0f);	// Y Axis Speed And Direction
		      particlefire[loop].zi=((rand()%60)-30.0f);	        // Z Axis Speed And Direction
		      
              
			  // Set colour of fire particlefire
              particlefire[loop].r= 1.0;			
		      particlefire[loop].g= 0.27;			
		      particlefire[loop].b= 0;			
		  }

			
	    }
    }
    
}




void DrawSnow()										
{
   for (loop=0;loop<MAX_PARTICLES;loop++)					
      {
         if (true == 1)							
	    {
		float x=particlesnow[loop].x - 40;						
		float y=particlesnow[loop].y;						
		float z=particlesnow[loop].z+zoom;					

		// Draw The Particle Using Our RGB Values and fade it based on life
		glColor4f(particlesnow[loop].r,particlesnow[loop].g,particlesnow[loop].b,particlesnow[loop].life);

		// Draw particlesnows
		glColor3f(1.0, 1.0, 1.0);
		glPushMatrix();
		glTranslatef(x, y, z);
		glutSolidSphere(0.8, 3, 3);
		glPopMatrix();	
	
		particlesnow[loop].x+=particlesnow[loop].xi/(slowdownsnow*1000);   // Move On The X Axis By X Speed
		particlesnow[loop].y+=particlesnow[loop].yi/(slowdownsnow*1000);   // Move On The Y Axis By Y Speed
		particlesnow[loop].z+=particlesnow[loop].zi/(slowdownsnow*1000);   // Move On The Z Axis By Z Speed

		particlesnow[loop].xi+=particlesnow[loop].xg;	            // Take Pull On X Axis Into Account
		particlesnow[loop].yi+=particlesnow[loop].yg;		    // Take Pull On Y Axis Into Account
		particlesnow[loop].zi+=particlesnow[loop].zg;		    // Take Pull On Z Axis Into Account
		particlesnow[loop].life-=particlesnow[loop].fade;	    // Reduce Particles Life By 'Fade'

		if (particlesnow[loop].life<0.0f)					// If Particle Is Burned Out
		   {
		      particlesnow[loop].life=1.0f;					// Give It New Life
	              particlesnow[loop].fade=(rand()%1000)/10000.0f+0.003f;	// Random Fade Value

                      // Reset particle position
		      particlesnow[loop].x= (rand() % 100) * (-c);						
	              particlesnow[loop].y= (rand() % 100) * (-c);						
	              particlesnow[loop].z= (rand() % 100) * (-c) ;					
		      particlesnow[loop].xi=xspeedsnow+((rand()%60)-32.0f);	// X Axis Speed And Direction
		      particlesnow[loop].yi=yspeedsnow+((rand()%60)-30.0f);	// Y Axis Speed And Direction
		      particlesnow[loop].zi=((rand()%60)-30.0f);	        // Z Axis Speed And Direction
		      particlesnow[loop].r= 1;			
		      particlesnow[loop].g= 1;			
		      particlesnow[loop].b= 1;			
		  }

			
	    }
    }
    
}





void DrawRain()										
{
   for (loop=0;loop<MAX_PARTICLES;loop++)					
      {
         if (true == 1)							
	    {
		float x=particlerain[loop].x - 40;						
		float y=particlerain[loop].y;						
		float z=particlerain[loop].z+zoom;					

		// Draw particlerains
		glColor3f(0.53, 0.81, 1);
		glBegin(GL_LINES);
		   glVertex3f(x, y, z);
		   glVertex3f(x, y+0.5, z);
		glEnd();
	
		particlerain[loop].x+=particlerain[loop].xi/(slowdownrain*1000);// Move On The X Axis By X Speed
		particlerain[loop].y+=particlerain[loop].yi/(slowdownrain*1000);// Move On The Y Axis By Y Speed
		particlerain[loop].z+=particlerain[loop].zi/(slowdownrain*1000);// Move On The Z Axis By Z Speed

		particlerain[loop].xi+=particlerain[loop].xg;	            // Take Pull On X Axis Into Account
		particlerain[loop].yi+=particlerain[loop].yg;		    // Take Pull On Y Axis Into Account
		particlerain[loop].zi+=particlerain[loop].zg;		    // Take Pull On Z Axis Into Account
		particlerain[loop].life-=particlerain[loop].fade;	    // Reduce Particles Life By 'Fade'

		if (particlerain[loop].life<0.0f)					// If Particle Is Burned Out
		   {
		      particlerain[loop].life=1.0f;					// Give It New Life
		      particlerain[loop].fade=(rand()%1000)/10000.0f+0.003f;	// Random Fade Value
                      
                      // Reset particle positions 
		      particlerain[loop].x= (rand() % 100);						
		      particlerain[loop].y= 60;						
	              particlerain[loop].z= (rand() % 100);					
		      particlerain[loop].yi=yspeedrain+((rand()%60)-30.0f);	// Y Axis Speed And Direction
		      particlerain[loop].r= 0.53;			
		      particlerain[loop].g= 0.81;			
		      particlerain[loop].b= 1;			
		  }

			
	    }
    }
    
}


/*
 *  Draw vertex in polar coordinates
 */
void Vertex(int th,int ph)
{
   double x = -Sin(th)*Cos(ph);
   double y =  Cos(th)*Cos(ph);
   double z =          Sin(ph);
   glNormal3d(x,y,z);
   glTexCoord2d(th/360.0,ph/180.0+0.5);
   glVertex3d(x,y,z);
}


/*
 *  Draw fireball
 */
void FireBall(double x,double y,double z,double r)
{
   int th,ph;
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);
   
   //  Set texture
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   //  Latitude bands
   glColor3f(1,0.84,0);
   for (ph=-90;ph<90;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=5)
      {
         Vertex(th,ph);
         Vertex(th,ph+5);
      }
      glEnd();
   }
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}



/*
 *  Fireball movement
 */
void DrawSphere(double x0,double y0,double z0,
                       double dx,double dy,double dz,
                       double ux,double uy,double uz)
{
   int i;
   //  Position of members
   double X[] = {-1.0};
   double Y[] = {-0.3};
   double Z[] = {+0.0};
   //  Unit vector in direction of flight
   double D0 = sqrt(dx*dx+dy*dy+dz*dz);
   double X0 = dx/D0;
   double Y0 = dy/D0;
   double Z0 = dz/D0;
   //  Unit vector in "up" direction
   double D1 = sqrt(ux*ux+uy*uy+uz*uz);
   double X1 = ux/D1;
   double Y1 = uy/D1;
   double Z1 = uz/D1;
   //  Cross product gives the third vector
   double X2 = Y0*Z1-Y1*Z0;
   double Y2 = Z0*X1-Z1*X0;
   double Z2 = X0*Y1-X1*Y0;
   //  Rotation matrix
   double M[16] = {X0,Y0,Z0,0 , X1,Y1,Z1,0 , X2,Y2,Z2,0 , 0,0,0,1};
   //  Save current transforms
   glPushMatrix();
   //  Offset and rotate
   glTranslated(x0,y0,z0);
   glMultMatrixd(M);
   for (i=0;i<1;i++)
      {
         glPushMatrix();
         glTranslated(X[i],Y[i],Z[i]);
         FireBall(0,0,0,1);  
         glPopMatrix();
      }
   //  Undo transformations
   glPopMatrix();
   
}



/*
 *  Draw the dragon
 */
void DrawDragon(double x0,double y0,double z0,
                       double dx,double dy,double dz,
                       double ux,double uy,double uz)
{
   int i,k;
   //  Position of members
   double X[] = {-1.0};
   double Y[] = {-0.3};
   double Z[] = {+0.0};
   //  Unit vector in direction of flght
   double D0 = sqrt(dx*dx+dy*dy+dz*dz);
   double X0 = dx/D0;
   double Y0 = dy/D0;
   double Z0 = dz/D0;
   //  Unit vector in "up" direction
   double D1 = sqrt(ux*ux+uy*uy+uz*uz);
   double X1 = ux/D1;
   double Y1 = uy/D1;
   double Z1 = uz/D1;
   //  Cross product gives the third vector
   double X2 = Y0*Z1-Y1*Z0;
   double Y2 = Z0*X1-Z1*X0;
   double Z2 = X0*Y1-X1*Y0;
   //  Rotation matrix
   double M[16] = {X0,Y0,Z0,0 , X1,Y1,Z1,0 , X2,Y2,Z2,0 , 0,0,0,1};
  
   //  Save current transforms
   glPushMatrix();
    
   //  Offset and rotate
   glTranslated(x0,y0,z0);
      
   glMultMatrixd(M);
  
   for (i=0;i<1;i++)
      {
         glPushMatrix();
         
         glTranslated(X[i],Y[i],Z[i]);
         glScaled(scale,scale,scale);
         glRotated(-90,0,1,0);
         glRotated(45,1,0,0);
         glRotated(25,0,0,1);
         glCallList(obj);
         for (k = 0;k < 8;k++)
            if (fbreathe == 1 && fly == 1)
               DrawFire();
         glPopMatrix();
      }

   //  Undo transformations   
   glPopMatrix();   
   
}



/* 
 *  Draw sky box
 */
void Sky(double D, int env)
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,sky[env]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0,1); glVertex3f(-D,+D,-D);
   glEnd();   

   glBindTexture(GL_TEXTURE_2D,sky[env + 1]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(1,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0,1); glVertex3f(+D,+D,-D);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,sky[env + 2]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(1,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0,1); glVertex3f(+D,+D,+D);
   glEnd();
   
   glBindTexture(GL_TEXTURE_2D,sky[env + 3]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0,1); glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   glBindTexture(GL_TEXTURE_2D,sky[env + 4]);
   glBegin(GL_QUADS);
   glTexCoord2f(1,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0,0); glVertex3f(-D,+D,+D);
   glTexCoord2f(1,0); glVertex3f(-D,+D,-D);
   glEnd();
   
     
   glBindTexture(GL_TEXTURE_2D,sky[env + 5]);
   glBegin(GL_QUADS);
   glTexCoord2f(1,1); glVertex3f(-D,-D,+D);
   glTexCoord2f(0,1); glVertex3f(+D,-D,+D);
   glTexCoord2f(0,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1,0); glVertex3f(-D,-D,-D);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}



/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r
 */
void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,0);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}




/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClearColor(0,0.3,0.7,0);
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   gluLookAt(Ex,Ey,Ez , Ox,Oy,Oz , Ux,Uy,Uz);

   //  Draw sky
   if (box) Sky(3.5*dim, env);

   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float F = (light==2) ? 1 : 0.3;
      float Ambient[]   = {0.3*F,0.3*F,0.3*F,1};
      float Diffuse[]   = {0.5*F,0.5*F,0.5*F,1};
      float Specular[]  = {1.0*F,1.0*F,1.0*F,1};
      float white[]     = {1,1,1,1};
      //  Light direction
      float Position[]  = {5*Cos(zh),Yl,5*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      ball(Position[0],Position[1],Position[2] , 0.1);
      //  Enable lighting with normalization
      glEnable(GL_LIGHTING);
      glEnable(GL_NORMALIZE);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
      glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,32.0f);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   }
   else
      glDisable(GL_LIGHTING);

   //  Draw flight of Dragon
   DrawDragon(X,Y,Z , Dx,Dy,Dz , Ux,Uy,Uz);
   
   if (snow == 1)
      DrawSnow();

   if (rain == 1 && (environ == 2 || environ == 3 || environ == 4))
      DrawRain();
   
   if (env == 0 && fball == 1 && fly == 1)
   {
      DrawSphere(-X,-Y,-Z, Dx,Dy,Dz , Ux,Uy,Uz);
      DrawSphere(-X + 2,-Y + 1,-Z + 3, Dx,Dy,Dz , Ux,Uy,Uz);
      DrawSphere(-X,Y,-Z, Dx,Dy,Dz , Ux,Uy,Uz);
      DrawSphere(-X,-Y,Z * 2, Dx,Dy,Dz , Ux,Uy,Uz);
      DrawSphere(-X,Y,-Z, Dx,Dy,Dz , Ux,Uy,Uz);
      DrawSphere(X + 2.5,-Y + 3.5,Z - 2.5, Dx,Dy,Dz , Ux,Uy,Uz);
      DrawSphere(X + 2.5,Y,-Z, Dx,Dy,Dz , Ux,Uy,Uz);
      DrawSphere(X + 3,Y + 2,Z + 3, Dx,Dy,Dz , Ux,Uy,Uz);
      DrawSphere(X - 3,Y - 2,Z + 1, Dx,Dy,Dz , Ux,Uy,Uz);
   }
     
      
   //  Draw axes
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Environment = %d",th,ph,dim,environ + 1);
   if (light) Print(" Light=%d,%.1f",zh,Yl);
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}




/*
 *  GLUT calls this routine every 50ms
 */
void timer(int toggle)
{
   //  Toggle movement
   if (toggle>0)
      move = !move;
   //  Increment light position
   else
      zh = (zh+5)%360;
   //  Animate flight using Lorenz transform
   if (fly)
   {
     
      //  Lorenz integration parameters
      double dt = 0.003;
      double s = -1.7;
      double b = 2.66;
      double r = 50;
      //  Old vectors
      double D;
      double Dx0 = Dx;
      double Dy0 = Dy;
      double Dz0 = Dz;
      //  Fix degenerate case
      if (X==0 && Y==0 && Z==0) Y = Z = 40;
      //  Update position
      Dx = s*(Y-X);
      Dy = X*(r-Z)-Y;
      Dz = X*Y - b*Z;
      X += dt*Dx;
      Y += dt*Dy;
      Z += dt*Dz;
      //  Normalize DX
      D = sqrt(Dx*Dx+Dy*Dy+Dz*Dz);
      Dx /= D;
      Dy /= D;
      Dz /= D;
      //  Calculate sideways
      Sx  = Dy0*Dz-Dz0*Dy;
      Sy  = Dz0*Dx-Dx0*Dz;
      Sz  = Dx0*Dy-Dy0*Dx;
      //  Calculate Up
      Ux  = Dz*Sy - Dy*Sz;
      Uy  = Dx*Sz - Dz*Sx;
      Uz  = Dy*Sx - Dx*Sy;
      //  Normalize Up
      D = sqrt(Ux*Ux+Uy*Uy+Uz*Uz);
      Ux /= D;
      Uy /= D;
      Uz /= D;
      //  Eye and lookat position
      Ex = X-7*Dx;
      Ey = Y-7*Dy;
      Ez = Z-7*Dz;
      Ox = X;
      Oy = Y;
      Oz = Z;
            
   }
   //  Static View
   else
   {      
      Ex = -2*dim*Sin(th)*Cos(ph);
      Ey = +2*dim        *Sin(ph);
      Ez = +2*dim*Cos(th)*Cos(ph);
      Ox = Oy = Oz = 0;
      X = Y = Z = 0;
      Dx = 1; Dy = 0; Dz = 0;
      Ux = 0; Uy = 1; Uz = 0;
   }
   
   //  Set timer to go again
   if (move && toggle>=0) glutTimerFunc(50,timer,0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}



/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(fov,asp,dim);
   //  Update state
   timer(-1);
}




/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Fly
   else if (ch == 'f' || ch == 'F')
      dim = (fly = !fly) ? 30 : 3;
   //  Toggle skybox
   else if (ch == 'b' || ch == 'B')
      box = 1-box;
   //  Cycle light
   else if (ch == 'l')
      light = (light+1)%3;
   else if (ch == 'L')
      light = (light+2)%3;
   //  Toggle light movement
   else if (ch == 's' || ch == 'S')
      timer(1);
   //  Light azimuth
   else if (ch=='[')
      zh -= 1;
   else if (ch==']')
      zh += 1;
   //  Light elevation
   else if (ch=='-')
      Yl -= 0.1;
   else if (ch=='+')
      Yl += 0.1;
   else if (ch == 'e' || ch == 'E')
     {
      env = (env + 6) % 24;
      environ = (environ + 1) % 4;
     }
   else if (ch == 'g' || ch == 'G')
      fball = 1 - fball;
   else if (ch == 't' || ch == 'T')
      fbreathe = 1 - fbreathe;
   else if (ch == 'h' || ch == 'H')
      snow = 1 - snow;
   else if (ch == 'r' || ch == 'R')
      rain = 1 - rain;
   //  Reproject
   Project(fov,asp,dim);
   //  Update state
   timer(-1);
}


/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(fov,asp,dim);
}




/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   if (argc!=2 && argc!=3 && argc!=6) Fatal("Usage: %s <obj> [scale [R G B]]\n",argv[0]);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Ashik Imran Akbar Basha");
   //  Set callbacks
   glutDisplayFunc(display);   
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   initglfire();  
   initglsnow();  
   initglrain();   

   //  Set scale
   if (argc>=3) scale = strtod(argv[2],NULL);
   if (scale<=0) scale = 1;
   //  Set color
   if (argc>=6) RGBA[0] = strtod(argv[3],NULL);
   if (argc>=6) RGBA[1] = strtod(argv[4],NULL);
   if (argc>=6) RGBA[2] = strtod(argv[5],NULL);
   //  Load object
   obj = LoadOBJ(argv[1]);
   texture[0] = LoadTexBMP("fire.bmp");
   //  Load skybox texture
   sky[0] = LoadTexBMP("mercury_ft.bmp");
   sky[1] = LoadTexBMP("mercury_lf.bmp");
   sky[2] = LoadTexBMP("mercury_bk.bmp");
   sky[3] = LoadTexBMP("mercury_rt.bmp");
   sky[4] = LoadTexBMP("mercury_up.bmp");
   sky[5] = LoadTexBMP("mercury_dn.bmp");
   sky[6] = LoadTexBMP("snowy_ft.bmp");
   sky[7] = LoadTexBMP("snowy_lf.bmp");
   sky[8] = LoadTexBMP("snowy_bk.bmp");
   sky[9] = LoadTexBMP("snowy_rt.bmp");
   sky[10] = LoadTexBMP("snowy_up.bmp");
   sky[11] = LoadTexBMP("snowy_dn.bmp");
   sky[12] = LoadTexBMP("blizzard_ft.bmp");
   sky[13] = LoadTexBMP("blizzard_lf.bmp");
   sky[14] = LoadTexBMP("blizzard_bk.bmp");
   sky[15] = LoadTexBMP("blizzard_rt.bmp");
   sky[16] = LoadTexBMP("blizzard_up.bmp");
   sky[17] = LoadTexBMP("blizzard_dn.bmp");
   sky[18] = LoadTexBMP("beach_ft.bmp");
   sky[19] = LoadTexBMP("beach_lf.bmp");
   sky[20] = LoadTexBMP("beach_bk.bmp");
   sky[21] = LoadTexBMP("beach_rt.bmp");
   sky[22] = LoadTexBMP("beach_up.bmp");
   sky[23] = LoadTexBMP("beach_dn.bmp");      
   

   //  Set timer
   timer(1);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}




