
#include <SFML/Graphics.hpp>
#include "stick_figure.hpp"
#include "motion_capture.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cmath> // Para sin y cos en la demo


// Este mapa es CRUCIAL para el modo de captura de color.
// Asocia el ÍNDICE del objeto azul detectado (0, 1, 2...) con una JointID.
// El orden de detección de OpenCV (después de cualquier filtrado/ordenamiento que añadas)
// determinará qué objeto azul es el índice 0, cuál el 1, etc.
// DEBES colocar tus cintas azules en un orden que corresponda a este mapa.
// Ejemplo: si pones una cinta en tu cabeza, otra en tu mano izq, y otra en tu mano der,
// y esperas que se detecten en ESE orden, el mapa sería:
// {0, JointID::HEAD}, {1, JointID::L_WRIST}, {2, JointID::R_WRIST}
// ¡AJUSTA ESTO CUIDADOSAMENTE!
const std::map<int, JointID> COLOR_INDEX_TO_JOINT_MAP = {
    {0, JointID::HEAD},
    {1, JointID::L_WRIST},
    {2, JointID::R_WRIST},
    // Añade más si vas a usar más marcadores de cinta azul
    // Por ejemplo, si usas 5 marcadores:
    // {3, JointID::L_ELBOW},
    // {4, JointID::R_ELBOW}
    // {0, JointID::NECK}, // O quizás un marcador en el pecho para TORSO_TOP
    // {1, JointID::L_SHOULDER},
    // {2, JointID::R_SHOULDER},
    // {3, JointID::HIP_CENTER}
};

// Variables para el modo Demo
bool demoModeActive = true; // Empezar en modo demo para verificar esqueleto
float demoTime = 0.0f;
sf::Clock demoClock; // Para el tiempo de la demo

// Función para la animación de demostración
void animateDemoFigure(StickFigure& figure, float time, sf::Vector2u windowSize) {
    float centerX = static_cast<float>(windowSize.x) / 2.f;
    float centerY = static_cast<float>(windowSize.y) / 2.f;

    // Movimiento base simple (ej. torso y cabeza)
    figure.setJointPosition(JointID::HIP_CENTER, {centerX, centerY + 60.f + 10.f * std::sin(time * 2.f)});
    figure.setJointPosition(JointID::TORSO_TOP, {centerX, centerY - 60.f + 10.f * std::sin(time * 2.f)});
    figure.setJointPosition(JointID::NECK, {centerX, centerY - 70.f + 10.f * std::sin(time * 2.f)});
    figure.setJointPosition(JointID::HEAD, {centerX + 15.f * std::cos(time), centerY - 110.f + 10.f * std::sin(time * 2.f)});

    // Brazos saludando
    float armAngleL = -90.f + 45.f * std::sin(time * 3.f); // Ángulo en grados
    float armAngleR = -90.f - 45.f * std::sin(time * 3.f + 0.5f);

    sf::Vector2f neckPos = figure.getJointPosition(JointID::NECK); // Usar como referencia

    figure.setJointPosition(JointID::L_SHOULDER, {neckPos.x - 50.f, neckPos.y + 5.f});
    sf::Vector2f lShoulderPos = figure.getJointPosition(JointID::L_SHOULDER);
    figure.setJointPosition(JointID::L_ELBOW, {lShoulderPos.x + 40.f * std::cos(armAngleL * 3.14159f / 180.f),
                                               lShoulderPos.y + 40.f * std::sin(armAngleL * 3.14159f / 180.f)});
    sf::Vector2f lElbowPos = figure.getJointPosition(JointID::L_ELBOW);
    figure.setJointPosition(JointID::L_WRIST, {lElbowPos.x + 40.f * std::cos(armAngleL * 3.14159f / 180.f),
                                               lElbowPos.y + 40.f * std::sin(armAngleL * 3.14159f / 180.f)});


    figure.setJointPosition(JointID::R_SHOULDER, {neckPos.x + 50.f, neckPos.y + 5.f});
    sf::Vector2f rShoulderPos = figure.getJointPosition(JointID::R_SHOULDER);
    figure.setJointPosition(JointID::R_ELBOW, {rShoulderPos.x + 40.f * std::cos(armAngleR * 3.14159f / 180.f),
                                               rShoulderPos.y + 40.f * std::sin(armAngleR * 3.14159f / 180.f)});
    sf::Vector2f rElbowPos = figure.getJointPosition(JointID::R_ELBOW);
    figure.setJointPosition(JointID::R_WRIST, {rElbowPos.x + 40.f * std::cos(armAngleR * 3.14159f / 180.f),
                                               rElbowPos.y + 40.f * std::sin(armAngleR * 3.14159f / 180.f)});


    // Piernas (mantenerlas simples o con un ligero balanceo)
    figure.setJointPosition(JointID::L_HIP, {figure.getJointPosition(JointID::HIP_CENTER).x - 30.f, figure.getJointPosition(JointID::HIP_CENTER).y});
    figure.setJointPosition(JointID::L_KNEE, {figure.getJointPosition(JointID::L_HIP).x, figure.getJointPosition(JointID::L_HIP).y + 60.f + 5.f * std::sin(time)});
    figure.setJointPosition(JointID::L_ANKLE, {figure.getJointPosition(JointID::L_KNEE).x, figure.getJointPosition(JointID::L_KNEE).y + 60.f});

    figure.setJointPosition(JointID::R_HIP, {figure.getJointPosition(JointID::HIP_CENTER).x + 30.f, figure.getJointPosition(JointID::HIP_CENTER).y});
    figure.setJointPosition(JointID::R_KNEE, {figure.getJointPosition(JointID::R_HIP).x, figure.getJointPosition(JointID::R_HIP).y + 60.f - 5.f * std::sin(time)});
    figure.setJointPosition(JointID::R_ANKLE, {figure.getJointPosition(JointID::R_KNEE).x, figure.getJointPosition(JointID::R_KNEE).y + 60.f});
}


int main() {
    unsigned int windowWidth = 1024;
    unsigned int windowHeight = 768;
    sf::RenderWindow window(sf::VideoMode(sf::Vector2<unsigned int>(windowWidth, windowHeight)),
                            "Stick Figure Animation with Motion Capture",
                            sf::Style::Default);
    window.setFramerateLimit(60);

    StickFigure playerFigure;
    playerFigure.initialize(window.getSize()); // Inicializa en T-Pose

    MotionCapture motionTracker;
    // No inicializar cámara si empezamos en modo demo sin cámara
    // if (!motionTracker.initializeCamera(0)) {
    //     std::cerr << "Fallo al inicializar la cámara. Saliendo." << std::endl;
    //     // return -1; // Quizás quieras permitir que el modo demo funcione sin cámara
    // }

    std::cout << "Controles:" << std::endl;
    std::cout << "  Q: Salir" << std::endl;
    std::cout << "  M: Cambiar Modo (Demo / Captura en Vivo)" << std::endl;
    std::cout << "  D: Mostrar/Ocultar vista de cámara OpenCV (debug, solo en modo Captura)" << std::endl;
    std::cout << "  S: Mostrar/Ocultar máscara de color en vista OpenCV (cuando D está activa)" << std::endl;
    // bool showDebugCVView = false;
    // bool showColorMaskInDebug = false;

    demoClock.restart(); // Inicia el reloj para la demo

    // while (window.isOpen()) {
    //     sf::Event event;
    //     while (window.pollEvent(event)) {
    //         if (event.type == sf::Event::Closed) {
    //             window.close();
    //         }
    //         if (event.type == sf::Event::KeyPressed) {
    //             if (event.key.code == sf::Keyboard::Q) {
    //                 window.close();
    //             }
    //             if (event.key.code == sf::Keyboard::M) {
    //                 demoModeActive = !demoModeActive;
    //                 if (demoModeActive) {
    //                     std::cout << "Modo Demo ACTIVADO." << std::endl;
    //                     playerFigure.initialize(window.getSize()); // Reiniciar a T-pose para la demo
    //                     demoClock.restart(); // Reiniciar tiempo de la demo
    //                     if (motionTracker.isCameraOpen()) motionTracker.releaseCamera(); // Apagar cámara si estaba encendida
    //                 } else {
    //                     std::cout << "Modo Captura en Vivo ACTIVADO." << std::endl;
    //                     if (!motionTracker.isCameraOpen()){
    //                          if(!motionTracker.initializeCamera(0)){
    //                             std::cerr << "Fallo al inicializar cámara para modo en vivo. Volviendo a modo Demo." << std::endl;
    //                             demoModeActive = true;
    //                          }
    //                     }
    //                     playerFigure.initialize(window.getSize()); // Reiniciar a T-pose
    //                 }
    //             }
    //             if (event.key.code == sf::Keyboard::D) {
    //                 if (!demoModeActive) { // El debug view solo tiene sentido en modo captura
    //                     showDebugCVView = !showDebugCVView;
    //                     if (!showDebugCVView) {
    //                         cv::destroyWindow("OpenCV Debug View (Original with Detections)");
    //                         cv::destroyWindow("OpenCV Debug View (Masked)");
    //                         // cv::destroyWindow("Color Mask");
    //                     }
    //                 } else {
    //                     std::cout << "La vista de debug de OpenCV solo está disponible en modo Captura (presiona M)." << std::endl;
    //                 }
    //             }
    //              if (event.key.code == sf::Keyboard::S) {
    //                 if (showDebugCVView && !demoModeActive) {
    //                     showColorMaskInDebug = !showColorMaskInDebug;
    //                     std::cout << "Mostrar máscara de color en debug: " << (showColorMaskInDebug ? "Sí" : "No") << std::endl;
    //                     // Forzar el cierre y reapertura de la ventana de debug para que refleje el cambio
    //                     cv::destroyWindow("OpenCV Debug View (Original with Detections)");
    //                     cv::destroyWindow("OpenCV Debug View (Masked)");
    //                 }
    //             }
    //         }
    //     }

    //     if (demoModeActive) {
    //         demoTime = demoClock.getElapsedTime().asSeconds();
    //         animateDemoFigure(playerFigure, demoTime, window.getSize());
    //     } else { // Modo Captura en Vivo
    //         if (motionTracker.isCameraOpen()) {
    //             std::vector<sf::Vector2f> detectedScreenPoints = motionTracker.getBlueObjectPositions();
    //             std::map<int, sf::Vector2f> mappedPoints;
    //             for (size_t i = 0; i < detectedScreenPoints.size(); ++i) {
    //                 mappedPoints[static_cast<int>(i)] = detectedScreenPoints[i];
    //             }
    //             playerFigure.updatePose(mappedPoints, COLOR_INDEX_TO_JOINT_MAP);

    //             if (showDebugCVView) {
    //                 motionTracker.displayDebugCameraView(showColorMaskInDebug);
    //             }
    //         } else {
    //             // Podrías mostrar un mensaje o simplemente el personaje estático si la cámara no está
    //              playerFigure.initialize(window.getSize()); // Mantener en T-pose
    //         }
    //     }

    //     window.clear(sf::Color(220, 220, 220));
    //     playerFigure.draw(window);
    //     window.display();

    //     if (!demoModeActive && showDebugCVView && motionTracker.isCameraOpen()) {
    //         if (cv::waitKey(1) == 27) { /* No hacer nada aquí, Q cierra todo */ }
    //     } else if (demoModeActive) {
    //         // En modo demo, no necesitamos cv::waitKey si no hay ventana de OpenCV
    //     }
    // }

    // if (motionTracker.isCameraOpen()) {
    //     motionTracker.releaseCamera();
    // }
    // cv::destroyAllWindows(); // Cierra cualquier ventana de OpenCV restante
    // std::cout << "Aplicación cerrada limpiamente." << std::endl;
    // return 0;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                break; // Salir del bucle de sondeo de eventos si la ventana se cierra
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Q)
                {
                    window.close();
                }
            }
        }

        demoTime = demoClock.getElapsedTime().asSeconds();
        animateDemoFigure(playerFigure, demoTime, window.getSize());

        window.clear(sf::Color(220, 220, 220));
        playerFigure.draw(window);
        window.display();
    }

    std::cout << "Aplicación cerrada limpiamente." << std::endl;
    return 0;
}