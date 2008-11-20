// main.cpp - material application

// V-ART example: Applying a texture to a surface

// This application shows how to add a texture to a surface.

// Changelog
// Mar 05, 2007- Leonardo Garcia Fischer
// - Application created.


#include <vart/contrib/viewerglutogl.h>
#include <vart/contrib/mousecontrol.h>
#include <vart/scene.h>
#include <vart/meshobject.h>
#include <vart/sphere.h>
#include <vart/texture.h>
#include <vart/transform.h>

using namespace VART;

int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv);
    static VART::ViewerGlutOGL viewer;
    static VART::Scene scene;
    VART::Camera camera;
    
    VART::Material material;
    VART::Texture texture;
    VART::Sphere sphere;
    VART::Transform sphereTransform;
    
    std::list<VART::MeshObject*> tableData;
    std::list<VART::MeshObject*>::iterator it;
    VART::Transform tableTransform;
    
    //Here, we loads a texture from a image file, aply it to a material, and then, aply the
    //material to a sphere.
    if( texture.LoadFromFile( "texture/texture.png" ) )
    {
        material.SetTexture( texture );
        sphere.SetMaterial( material );
        
        //Other sphere configs.
        sphere.Show();
        sphere.SetDescription("cylinder");
        sphereTransform.MakeScale(10, 10, 10);
        sphereTransform.AddChild( sphere );
    }
    else
    {
        std::cout << "Could not open texture file. Check if the image format is supported, " 
            << "if it is in the correct path, and if V-Art was compiled with OpenGL "
            << "support and an image reader library." << std::endl;
    }

    //Here, we loads a object from a ".obj" file. Note that the object refers to a material 
    //library, that refers to two other textures.
    if( VART::MeshObject::ReadFromOBJ( "table/table2_2.obj", &tableData ) )
    {
        //Here, we adds the object data to a transform, and make some setup to the data.
        //and the transform.
        for( it=tableData.begin(); it!=tableData.end(); it++ )
        {
            (*it)->autoDelete = true;
            tableTransform.AddChild( **it );
        }
        tableTransform.MakeTranslation( VART::Point4D(20,5,0) );
    }
    else
        std::cout << "Could not read 'table/table2_2.obj' for table data." << std::endl;

    //Adds the sphere and the object to a scene.
    scene.AddObject( &sphereTransform );
    scene.AddObject( &tableTransform );

    //Adds a camera to a scene and configure it.
    camera.SetDescription( "camera" );
    scene.AddCamera( &camera );
    scene.SetCamera( "camera" );
    scene.ComputeBoundingBox();
    scene.MakeCameraViewAll();

    //Add a light to the scene.
    scene.AddLight( Light::BRIGHT_AMBIENT() );
    scene.DrawLightsOGL();

    //Setup the viewer.
    viewer.SetTitle("V-ART material example");
    viewer.SetScene(scene);

    //Enable texture in OpenGL
    glEnable( GL_TEXTURE_2D );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    //Enter the main loop
    VART::ViewerGlutOGL::MainLoop();
    return 0;
}
