
#define STB_IMAGE_IMPLEMENTATION
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include <filesystem>
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <vector>
#include <string>
#include <stb/stb_image.h>
#include "Timer.hpp"
#include "FileLoader.hpp"
#include "ShaderProgram.hpp"
#include "GuiControl.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VertexTypes.hpp"
#include "UtilityFuncitons.hpp"
#include "MeshManager.hpp"
#include "Mesh3D.hpp"
#include "Mesh2D.hpp"
#include "Camera.hpp"

#include <unistd.h>
using namespace std;


ShaderProgram programTexture;
ShaderProgram programColor;
GuiControl imguiWindow;
unsigned int textureId;
Timer timer;



MeshManager meshManager;
Mesh2D* circle;
Mesh3D * cube;
Mesh3D* grid;
Camera* camera;
std::vector<glm::vec3> positions;
std::vector<int> circleIndices;
std::vector<int> cubeIndices;
glm::mat4 mtxTransform;


glm::vec3 vecMove;
glm::vec3 angles;

std::string get_current_dir() {
   char buff[FILENAME_MAX]; //create string buffer to hold path
   GetCurrentDir( buff, FILENAME_MAX );
   string current_working_dir(buff);
   return current_working_dir;
}
void createPositions()
{
    /*for(int i=0;i<20;i++)
    {*/
        int x = rand()%10;
        int y = rand()%10;
        int z = rand()%10;
        positions.emplace_back(x,y,z);
       /* x = rand()%10;
        y = rand()%10;
        z = rand()%10;
        cubePositions.emplace_back(x,-y,-z);
        x = rand()%10;
        y = rand()%10;
        z = rand()%10;
        cubePositions.emplace_back(-x,y,-z);   
    }*/
}
void sekilOlustur(SHAPETYPE type)
{
    if(type==SHAPE_CIRCLE)
    {
        if(!positions.empty())
        {
            positions.back()=vecMove;
        }
        circleIndices.emplace_back(positions.size());
        createPositions();
        vecMove=glm::vec3(0,0,0);
        std::cout<<"SHAPE_CIRCLE"<<std::endl;
    }
    if(type==SHAPE_SQUARE)
    {
        if(!positions.empty())
        {
            positions.back()=vecMove;
        }
        cubeIndices.emplace_back(positions.size());
        createPositions();
        vecMove=glm::vec3(0,0,0);
        std::cout<<"SHAPE_SQUARE"<<std::endl;
    }
        if(type==SHAPE_TRIANGLE)
    {
        std::cout<<"SHAPE_TRIANGLE"<<std::endl;
    }    
}
unsigned int kareOlustur()
{
    unsigned int vertexArrayObjectId;
    int vertexCount = 4;
    
    TexturedVertexList vertices(vertexCount);
    IndexList   indices(6);
    //indexCount = 6;

    vertices[0].pos = glm::vec3(-0.5,-0.5,1.0f);
    vertices[1].pos = glm::vec3( 0.5,-0.5,1.0f);
    vertices[2].pos = glm::vec3( 0.5, 0.5,1.0f);
    vertices[3].pos = glm::vec3(-0.5, 0.5,1.0f);

    vertices[0].tex = glm::vec2( 0.0f, 0.0f);
    vertices[1].tex = glm::vec2( 1.0f, 0.0f);
    vertices[2].tex = glm::vec2( 1.0f, 1.0f);
    vertices[3].tex = glm::vec2(0.0f, 1.0f);

    indices[0]= 0;  indices[1]= 1; indices[2]= 2;
    indices[3]= 0;  indices[4]= 2; indices[5]= 3;

   // vertexArrayObjectId =  createTextureBuffer(vertices,indices);

    return vertexArrayObjectId;

}

unsigned int loadTexture(std::string fileName)
{
    unsigned int id;
    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D,id);
    int width,height,nrChannels;
    auto data = stbi_load(fileName.c_str(),&width,&height,&nrChannels,0);
  
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    return id;
}
void keyboard_callback(GLFWwindow* window,int key,int scanCode,int action,int mods)
{
    if(key==GLFW_KEY_ESCAPE)
        glfwTerminate();
    if(key==GLFW_KEY_W)
        camera->moveForward();
    if(key==GLFW_KEY_S)
        camera->moveBackWard();        
    if(key==GLFW_KEY_A)
        camera->moveLeft();
    if(key==GLFW_KEY_D)
        camera->moveRight();           
    if(key==GLFW_KEY_LEFT)
        camera->rotateLeft();
    if(key==GLFW_KEY_RIGHT)
        camera->rotateRight();
    if(key==GLFW_KEY_UP)
        camera->rotateUp();
    if(key==GLFW_KEY_DOWN)
        camera->rotateDown();
        
}
void initScene(GLFWwindow* window)
{
    cout << get_current_dir() << endl;
    programTexture.attachShader("../Projects/hafta11- 02 Kamera Sinifi/src/vertex.glsl",GL_VERTEX_SHADER);
    programTexture.attachShader("../Projects/hafta11- 02 Kamera Sinifi/src/fragment.glsl",GL_FRAGMENT_SHADER);
    programTexture.link();
    programTexture.addUniform("uMtxTransform");
    programTexture.addUniform("uMtxView");
    programTexture.addUniform("uMtxProj");


    programColor.attachShader("../Projects/hafta11- 02 Kamera Sinifi/src/colorVertex.glsl",GL_VERTEX_SHADER);
    programColor.attachShader("../Projects/hafta11- 02 Kamera Sinifi/src/colorFragment.glsl",GL_FRAGMENT_SHADER);
    programColor.link();
    programColor.addUniform("uMtxTransform");
    programColor.addUniform("uMtxView");
    programColor.addUniform("uMtxProj");
    programColor.addUniform("uColor");

   
    imguiWindow.setPos(&vecMove);
    imguiWindow.setAngle(&angles);
    imguiWindow.tiklaSekilOlustur= sekilOlustur;
    cube = meshManager.createCube();
    circle = meshManager.createCircle();
    grid = meshManager.createGrid();
    grid->setColorActive(true);
    grid->setColor({1.0,0.0,0.0,1.0});
    //createCubePositions();
    camera = new Camera();
    textureId =loadTexture("C:/Users/senil/Desktop/GraphHier/Projects/hafta11- 02 Kamera Sinifi/images/brick.jpg");

   
    imguiWindow.init(window);

    glEnable(GL_DEPTH_TEST);
    timer.start();
      
}
void draw()
{
    double elapsedTime = timer.elapsedSeconds();
    if(elapsedTime<0.016)
    {
        unsigned int miliseconds = (0.016-elapsedTime)*1000;

        timer.sleep(miliseconds);
    }
    glClearColor(0.3,0.4,0.3,1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    programTexture.use();   
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureId);  
    camera->setCamera(programTexture);
   
    
    for(int i=0;i<positions.size();i++)
    {
        if(i==(positions.size()-1))
        {
            if(!cubeIndices.empty())
            {
                for(int j=0;j<cubeIndices.size();j++)
                {
                    if(cubeIndices[j]==i)
                    {
                        cube->setPosition(vecMove);
                        cube->draw(programTexture);
                    }
                }
            }
            if(!circleIndices.empty())
            {
                for(int a=0;a<circleIndices.size();a++)
                {
                    if(circleIndices[a]==i)
                    {
                        circle->setPosition(glm::vec2(vecMove));
                        circle->draw(programTexture);
                    }
                }
            }
            
            
        }
        else
        {
            if(!cubeIndices.empty())
            {
                for(int d=0;d<cubeIndices.size();d++)
                {
                    if(cubeIndices[d]==i)
                    {
                        cube->setPosition(positions[i]);
                        cube->draw(programTexture);
                    }
                }
            }
            if(!circleIndices.empty())
            {
                for(int h=0;h<circleIndices.size();h++)
                {
                    if(circleIndices[h]==i)
                    {
                        circle->setPosition(glm::vec2(positions[i]));
                        circle->draw(programTexture);
                    }
                }
            }
        }
        
    }
    
    programColor.use();
    camera->setCamera(programColor);
    grid->draw(programColor);
}

int main()
{
    if(!glfwInit())    return -1;

    GLFWwindow* window = glfwCreateWindow(1000,900,"ilk program",0,0);
    if(window==nullptr)
    {
        std::cout<<"pencere olusmadi"<<std::endl;
        glfwTerminate();
        return -1;        
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window,(GLFWkeyfun)keyboard_callback);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<"glad yukleme hatasi"<<std::endl;
    }

    initScene(window); 
   
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        draw();
        imguiWindow.createFrame();
        imguiWindow.createFrameItems();
        imguiWindow.draw();
        glfwSwapBuffers(window);
    }




    
}
