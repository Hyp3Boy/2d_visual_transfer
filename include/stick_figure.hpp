#ifndef STICK_FIGURE_HPP
#define STICK_FIGURE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>

// Enum para identificar las articulaciones/puntos clave
enum class JointID {
    HEAD, NECK,
    L_SHOULDER, L_ELBOW, L_WRIST,
    R_SHOULDER, R_ELBOW, R_WRIST,
    TORSO_TOP, HIP_CENTER,
    L_HIP, L_KNEE, L_ANKLE,
    R_HIP, R_KNEE, R_ANKLE,
    JOINT_COUNT
};

// Estructura para representar un hueso, conectando dos articulaciones
struct Bone {
    JointID joint1;
    JointID joint2;
    sf::Color color;
    float thickness;

    Bone(JointID j1, JointID j2, sf::Color c = sf::Color::Black, float t = 2.f)
        : joint1(j1), joint2(j2), color(c), thickness(t) {}
};

class StickFigure {
public:
    StickFigure();

    void initialize(sf::Vector2u windowSize);
    // El int key en trackedPoints ahora se referirá al índice del objeto de color detectado
    void updatePose(const std::map<int, sf::Vector2f>& trackedPoints,
                    const std::map<int, JointID>& pointIndexToJointMap);
    void draw(sf::RenderWindow& window);

    void setJointPosition(JointID joint, sf::Vector2f position);
    sf::Vector2f getJointPosition(JointID joint) const;


private:
    std::map<JointID, sf::Vector2f> currentJoints;
    std::vector<Bone> bones;

    void defineBones();
};

#endif // STICK_FIGURE_HPP