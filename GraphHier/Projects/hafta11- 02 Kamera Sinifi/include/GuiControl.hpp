#ifndef GuiControl_hpp
#define GuiControl_hpp
#include <glm/vec3.hpp>
#include <functional>
class GLFWwindow;

enum SHAPETYPE
{
    SHAPE_CUBE=0,
    SHAPE_SQUARE=1,
    SHAPE_CIRCLE=2,
    SHAPE_HEXAGON=3,
    SHAPE_TRIANGLE=4,
    SHAPE_PYRAMID=5,
    SHAPE_CYLINDER=6
      
};

typedef std::function<void(SHAPETYPE)>  SekilOlusturFonksiyonu;
class GuiControl
{
public:
    GuiControl();
    void init(GLFWwindow* window);
    void createFrame();
    void createFrameItems();
    void draw();
    void setPos(glm::vec3* pos);
    void setAngle(glm::vec3* angles);
    SekilOlusturFonksiyonu tiklaSekilOlustur;
private:
    int secilenSekil;
    glm::vec3* vecPos;
    glm::vec3* vecAngles;

};

#endif