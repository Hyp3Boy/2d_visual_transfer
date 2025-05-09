#include "stick_figure.hpp"
#include <iostream>
#include <cmath> // Para std::sqrt, std::atan2

StickFigure::StickFigure() {
    defineBones();
}

void StickFigure::defineBones() {
    bones.emplace_back(JointID::HEAD, JointID::NECK, sf::Color::Black, 3.f);
    bones.emplace_back(JointID::NECK, JointID::TORSO_TOP);
    bones.emplace_back(JointID::TORSO_TOP, JointID::HIP_CENTER, sf::Color(100,100,100), 5.f);

    bones.emplace_back(JointID::NECK, JointID::L_SHOULDER);
    bones.emplace_back(JointID::L_SHOULDER, JointID::L_ELBOW);
    bones.emplace_back(JointID::L_ELBOW, JointID::L_WRIST);

    bones.emplace_back(JointID::NECK, JointID::R_SHOULDER);
    bones.emplace_back(JointID::R_SHOULDER, JointID::R_ELBOW);
    bones.emplace_back(JointID::R_ELBOW, JointID::R_WRIST);

    bones.emplace_back(JointID::HIP_CENTER, JointID::L_HIP);
    bones.emplace_back(JointID::L_HIP, JointID::L_KNEE);
    bones.emplace_back(JointID::L_KNEE, JointID::L_ANKLE);

    bones.emplace_back(JointID::HIP_CENTER, JointID::R_HIP);
    bones.emplace_back(JointID::R_HIP, JointID::R_KNEE);
    bones.emplace_back(JointID::R_KNEE, JointID::R_ANKLE);
}

void StickFigure::initialize(sf::Vector2u windowSize) {
    float centerX = static_cast<float>(windowSize.x) / 2.f;
    float centerY = static_cast<float>(windowSize.y) / 2.f;

    currentJoints[JointID::HIP_CENTER] = {centerX, centerY + 60.f};
    currentJoints[JointID::TORSO_TOP] = {centerX, centerY - 60.f}; // Cerca del cuello
    currentJoints[JointID::NECK] = {centerX, centerY - 70.f};
    currentJoints[JointID::HEAD] = {centerX, centerY - 110.f};

    currentJoints[JointID::L_SHOULDER] = {centerX - 50.f, currentJoints[JointID::NECK].y + 5.f};
    currentJoints[JointID::L_ELBOW] = {centerX - 90.f, currentJoints[JointID::L_SHOULDER].y};
    currentJoints[JointID::L_WRIST] = {centerX - 130.f, currentJoints[JointID::L_SHOULDER].y};

    currentJoints[JointID::R_SHOULDER] = {centerX + 50.f, currentJoints[JointID::NECK].y + 5.f};
    currentJoints[JointID::R_ELBOW] = {centerX + 90.f, currentJoints[JointID::R_SHOULDER].y};
    currentJoints[JointID::R_WRIST] = {centerX + 130.f, currentJoints[JointID::R_SHOULDER].y};

    currentJoints[JointID::L_HIP] = {centerX - 30.f, currentJoints[JointID::HIP_CENTER].y};
    currentJoints[JointID::L_KNEE] = {centerX - 30.f, centerY + 120.f};
    currentJoints[JointID::L_ANKLE] = {centerX - 30.f, centerY + 180.f};

    currentJoints[JointID::R_HIP] = {centerX + 30.f, currentJoints[JointID::HIP_CENTER].y};
    currentJoints[JointID::R_KNEE] = {centerX + 30.f, centerY + 120.f};
    currentJoints[JointID::R_ANKLE] = {centerX + 30.f, centerY + 180.f};

    std::cout << "Personaje de palitos inicializado." << std::endl;
}

void StickFigure::updatePose(const std::map<int, sf::Vector2f>& trackedPoints,
                             const std::map<int, JointID>& pointIndexToJointMap) {
    for (const auto& pair : trackedPoints) {
        int pointIndex = pair.first; // Clave ahora es el índice del punto de color detectado
        sf::Vector2f screenPos = pair.second;

        auto it = pointIndexToJointMap.find(pointIndex);
        if (it != pointIndexToJointMap.end()) {
            JointID joint = it->second;
            currentJoints[joint] = screenPos;
        }
    }
}

void StickFigure::setJointPosition(JointID joint, sf::Vector2f position) {
    currentJoints[joint] = position;
}

sf::Vector2f StickFigure::getJointPosition(JointID joint) const {
    auto it = currentJoints.find(joint);
    if (it != currentJoints.end()) {
        return it->second;
    }
    std::cerr << "Advertencia: Se intentó obtener la posición de una articulación (" << static_cast<int>(joint) << ") no existente." << std::endl;
    return sf::Vector2f(0,0);
}

void StickFigure::draw(sf::RenderWindow& window) {
    for (const auto& bone : bones) {
        if (currentJoints.count(bone.joint1) && currentJoints.count(bone.joint2)) {
            sf::Vector2f p1 = currentJoints.at(bone.joint1);
            sf::Vector2f p2 = currentJoints.at(bone.joint2);
            sf::Vector2f direction = p2 - p1;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (length > 0) {
                sf::RectangleShape boneShape(sf::Vector2f(length, bone.thickness));
                boneShape.setFillColor(bone.color);
                boneShape.setOrigin(sf::Vector2f(0.f, bone.thickness / 2.f));
                boneShape.setPosition(p1);
                float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
                boneShape.setRotation(sf::Angle(sf::degrees(angle)));
                window.draw(boneShape);
            }
        }
    }

    float jointRadius = 4.f;
    for (const auto& joint_pair : currentJoints) {
        sf::CircleShape joint_circle(jointRadius);
        joint_circle.setFillColor(sf::Color::Red);
        joint_circle.setOrigin(sf::Vector2f(jointRadius, jointRadius));
        joint_circle.setPosition(joint_pair.second);
        window.draw(joint_circle);
    }
}