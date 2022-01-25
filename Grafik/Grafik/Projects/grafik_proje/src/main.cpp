
#define STB_IMAGE_IMPLEMENTATION
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
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

using namespace std;


ShaderProgram programTexture;
ShaderProgram programColor;
GuiControl imguiWindow;
unsigned int textureId;
Timer timer;



MeshManager meshManager;
Mesh2D* square;
Mesh2D* circle;
Mesh2D* hexagon;
Mesh2D* triangle;
Mesh3D * cube;
Mesh3D * cylinder;
Mesh3D * pyramid;
Mesh3D* grid;
Camera* camera;
std::vector<glm::vec3> positions;
std::vector<glm::vec3> anglesList;
std::vector<glm::vec3> scaleList;
std::vector<int> circleIndices;
std::vector<int> squareIndices;
std::vector<int> triangleIndices;
std::vector<int> cubeIndices;
std::vector<int> cylinderIndices;
std::vector<int> hexIndices;
std::vector<int> pyramidIndices;
glm::mat4 mtxTransform;


glm::vec3 vecMove;
glm::vec3 angles;
glm::vec3 scale;

std::string get_current_dir() {
   char buff[FILENAME_MAX];
   string current_working_dir(buff);
   return current_working_dir;
}
void createPositions()
{
    
        int x = rand()%20-10;
        int y = rand()%20-10;
        int z = rand()%20-10;
        positions.emplace_back(x,y,z);
        anglesList.emplace_back(0,0,0);
        scaleList.emplace_back(1,1,1);
}
void sekilOlustur(SHAPETYPE type)
{
    if(type==SHAPE_CIRCLE)
    {
        if(!positions.empty())
        {
            positions.back()=vecMove;
        }
        if(!anglesList.empty())
        {
            anglesList.back()=angles;
        }
        if(!scaleList.empty())
        {
            scaleList.back()=scale;
        }
        circleIndices.emplace_back(positions.size());
        createPositions();
        vecMove=positions[positions.size()-1];
        angles=glm::vec3(0,0,0);
        scale=glm::vec3(1,1,1);
        std::cout<<"SHAPE_CIRCLE"<<std::endl;
    }
    if(type==SHAPE_SQUARE)
    {
        if(!positions.empty())
        {
            positions.back()=vecMove;
        }
        if(!anglesList.empty())
        {
            anglesList.back()=angles;
        }
        if(!scaleList.empty())
        {
            scaleList.back()=scale;
        }
        squareIndices.emplace_back(positions.size());
        createPositions();
        vecMove=positions[positions.size()-1];
        angles=glm::vec3(0,0,0);
        scale=glm::vec3(1,1,1);
        std::cout<<"SHAPE_SQUARE"<<std::endl;
    }
    if(type==SHAPE_TRIANGLE)
    {
        if(!positions.empty())
        {
            positions.back()=vecMove;
        }
        if(!anglesList.empty())
        {
            anglesList.back()=angles;
        }
        if(!scaleList.empty())
        {
            scaleList.back()=scale;
        }
        triangleIndices.emplace_back(positions.size());
        createPositions();
        vecMove=positions[positions.size()-1];
        angles=glm::vec3(0,0,0);
        scale=glm::vec3(1,1,1);
        std::cout<<"SHAPE_TRIANGLE"<<std::endl;
    }
    if(type==SHAPE_CUBE)
    {
        if(!positions.empty())
        {
            positions.back()=vecMove;
        }
        if(!anglesList.empty())
        {
            anglesList.back()=angles;
        }
        if(!scaleList.empty())
        {
            scaleList.back()=scale;
        }
        cubeIndices.emplace_back(positions.size());
        createPositions();
        vecMove=positions[positions.size()-1];
        angles=glm::vec3(0,0,0);
        scale=glm::vec3(1,1,1);
        std::cout<<"SHAPE_CUBE"<<std::endl;
    }
    if(type==SHAPE_CYLINDER)
    {
        if(!positions.empty())
        {
            positions.back()=vecMove;
        }
        if(!anglesList.empty())
        {
            anglesList.back()=angles;
        }
        if(!scaleList.empty())
        {
            scaleList.back()=scale;
        }
        cylinderIndices.emplace_back(positions.size());
        createPositions();
        vecMove=positions[positions.size()-1];
        angles=glm::vec3(0,0,0);
        scale=glm::vec3(1,1,1);
        std::cout<<"SHAPE_CYLINDER"<<std::endl;
    }
    if(type==SHAPE_PYRAMID)
    {
        if(!positions.empty())
        {
            positions.back()=vecMove;
        }
        if(!anglesList.empty())
        {
            anglesList.back()=angles;
        }
        if(!scaleList.empty())
        {
            scaleList.back()=scale;
        }
        pyramidIndices.emplace_back(positions.size());
        createPositions();
        vecMove=positions[positions.size()-1];
        angles=glm::vec3(0,0,0);
        scale=glm::vec3(1,1,1);
        std::cout<<"SHAPE_PYRAMID"<<std::endl;
    }
    if(type==SHAPE_HEXAGON)
    {
        if(!positions.empty())
        {
            positions.back()=vecMove;
        }
        if(!anglesList.empty())
        {
            anglesList.back()=angles;
        }
        if(!scaleList.empty())
        {
            scaleList.back()=scale;
        }
        hexIndices.emplace_back(positions.size());
        createPositions();
        vecMove=positions[positions.size()-1];
        angles=glm::vec3(0,0,0);
        scale=glm::vec3(1,1,1);
        std::cout<<"SHAPE_HEXAGON"<<std::endl;
    }
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
    programTexture.attachShader("../Projects/grafik_proje/src/vertex.glsl",GL_VERTEX_SHADER);
    programTexture.attachShader("../Projects/grafik_proje/src/fragment.glsl",GL_FRAGMENT_SHADER);
    programTexture.link();
    programTexture.addUniform("uMtxTransform");
    programTexture.addUniform("uMtxView");
    programTexture.addUniform("uMtxProj");


    programColor.attachShader("../Projects/grafik_proje/src/colorVertex.glsl",GL_VERTEX_SHADER);
    programColor.attachShader("../Projects/grafik_proje/src/colorFragment.glsl",GL_FRAGMENT_SHADER);
    programColor.link();
    programColor.addUniform("uMtxTransform");
    programColor.addUniform("uMtxView");
    programColor.addUniform("uMtxProj");
    programColor.addUniform("uColor");

   
    imguiWindow.setPos(&vecMove);
    imguiWindow.setAngle(&angles);
    imguiWindow.setScale(&scale);
    imguiWindow.tiklaSekilOlustur= sekilOlustur;
    square = meshManager.createSquare();
    cube = meshManager.createCube();
    circle = meshManager.createCircle();
    triangle = meshManager.createTriangle();
    cylinder = meshManager.createCylinder(2,1);
    hexagon = meshManager.createHex();
    pyramid = meshManager.createPyramid();

    grid = meshManager.createGrid();
    grid->setColorActive(true);
    grid->setColor({1.0,0.0,0.0,1.0});
    camera = new Camera();
    textureId =loadTexture("../Projects/grafik_proje/images/brick.jpg");

   
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
            if(!squareIndices.empty())
            {
                for(int x=0;x<squareIndices.size();x++)
                {
                    if(squareIndices[x]==i)
                    {
                        square->setPosition(glm::vec2(vecMove));
                        square->setAngle(angles.x);
                        square->setScale(glm::vec2(scale));
                        square->draw(programTexture);
                    }
                }
            }
            if(!cubeIndices.empty())
            {
                for(int j=0;j<cubeIndices.size();j++)
                {
                    if(cubeIndices[j]==i)
                    {
                        cube->setPosition(vecMove);
                        cube->setAngles(angles);
                        cube->setScale(scale);
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
                        circle->setAngle(angles.x);
                        circle->setScale(glm::vec2(scale));
                        circle->draw(programTexture);
                    }
                }
            }
            if(!triangleIndices.empty())
            {
                for(int a=0;a<triangleIndices.size();a++)
                {
                    if(triangleIndices[a]==i)
                    {
                        triangle->setPosition(glm::vec2(vecMove));
                        triangle->setAngle(angles.x);
                        triangle->setScale(glm::vec2(scale));
                        triangle->draw(programTexture);
                    }
                }
            }
            if(!cylinderIndices.empty())
            {
                for(int j=0;j<cylinderIndices.size();j++)
                {
                    if(cylinderIndices[j]==i)
                    {
                        cylinder->setPosition(vecMove);
                        cylinder->setAngles(angles);
                        cylinder->setScale(scale);
                        cylinder->draw(programColor);
                    }
                }
            }
            if(!pyramidIndices.empty())
            {
                for(int p=0;p<pyramidIndices.size();p++)
                {
                    if(pyramidIndices[p]==i)
                    {
                        pyramid->setPosition(vecMove);
                        pyramid->setAngles(angles);
                        pyramid->setScale(scale);
                        pyramid->draw(programTexture);
                    }
                }
            }
            if(!hexIndices.empty())
            {
                for(int l=0;l<hexIndices.size();l++)
                {
                    if(hexIndices[l]==i)
                    {
                        hexagon->setPosition(glm::vec2(vecMove));
                        hexagon->setAngle(angles.x);
                        hexagon->setScale(glm::vec2(scale));
                        hexagon->draw(programTexture);
                    }
                }
            }
            
            
        }
        else
        {
            if(!squareIndices.empty())
            {
                for(int x=0;x<squareIndices.size();x++)
                {
                    if(squareIndices[x]==i)
                    {
                        square->setPosition(glm::vec2(positions[i]));
                        square->setAngle(anglesList[i].x);
                        square->setScale(glm::vec2(scaleList[i]));
                        square->draw(programTexture);
                    }
                }
            }
            if(!cubeIndices.empty())
            {
                for(int d=0;d<cubeIndices.size();d++)
                {
                    if(cubeIndices[d]==i)
                    {
                        cube->setPosition(positions[i]);
                        cube->setAngles(anglesList[i]);
                        cube->setScale(scaleList[i]);
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
                        circle->setAngle(anglesList[i].x);
                        circle->setScale(glm::vec2(scaleList[i]));
                        circle->draw(programTexture);
                    }
                }
            }
            if(!triangleIndices.empty())
            {
                for(int a=0;a<triangleIndices.size();a++)
                {
                    if(triangleIndices[a]==i)
                    {
                        triangle->setPosition(glm::vec2(positions[i]));
                        triangle->setAngle(anglesList[i].x);
                        triangle->setScale(glm::vec2(scaleList[i]));
                        triangle->draw(programTexture);
                    }
                }
            }
            if(!cylinderIndices.empty())
            {
                for(int j=0;j<cylinderIndices.size();j++)
                {
                    if(cylinderIndices[j]==i)
                    {
                        cylinder->setPosition(positions[i]);
                        cylinder->setAngles(anglesList[i]);
                        cylinder->setScale(scaleList[i]);
                        cylinder->draw(programColor);
                    }
                }
            }
            if(!hexIndices.empty())
            {
                for(int o=0;o<hexIndices.size();o++)
                {
                    if(hexIndices[o]==i)
                    {
                        hexagon->setPosition(glm::vec2(positions[i]));
                        hexagon->setAngle(anglesList[i].x);
                        hexagon->setScale(glm::vec2(scaleList[i]));
                        hexagon->draw(programTexture);
                    }
                }
            }
            if(!pyramidIndices.empty())
            {
                for(int m=0;m<pyramidIndices.size();m++)
                {
                    if(pyramidIndices[m]==i)
                    {
                        pyramid->setPosition(positions[i]);
                        pyramid->setAngles(anglesList[i]);
                        pyramid->setScale(scaleList[i]);
                        pyramid->draw(programTexture);
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
